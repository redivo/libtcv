/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 George Redivo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

/************************************************************************************/
/**
 * \file   fake_tcv.hpp
 * \brief  
 * \author  thomas.ruschival
 *  		Created on: Jun 18, 2014
 */
/************************************************************************************/

#ifndef FAKE_TCV_HPP_
#define FAKE_TCV_HPP_

#include <cstdint>
#include <iostream>
#include <vector>


#include <libtcv/tcv.h>
#include "fake_hw_i2c.hpp"

namespace TestDoubles {

const std::size_t PAGESIZE = 256;

/**
 * Possible device addresses
 */
enum tcv_dev_addr_t
{
	a0 = 0x50, dd = 0x51,
};

class FakeTCV
{
	public:
		FakeTCV(int index, i2c_read_cb_t read, i2c_write_cb_t write)
				: eeprom(PAGESIZE, 0xFF)
		{
			eeprom[0] = TCV_TYPE_UNKNOWN;
			tcv = tcv_create(index, i2c_read, i2c_write);
		}
		;

		virtual int read(tcv_dev_addr_t device, std::uint8_t regaddr,
		        std::uint8_t * data, std::size_t len) const;

		virtual int write(tcv_dev_addr_t device, std::uint8_t regaddr,
		        const std::uint8_t * data, std::size_t len);

		virtual ~FakeTCV()
		{
			tcv_destroy(tcv);
		}


		virtual bool range_is_valid(tcv_dev_addr_t device, std::size_t offset,
		        std::size_t size) const;

		tcv_t* get_ctcv()
		{
			return tcv;
		}
		;

		/**********************
		 * Instrumentation functions
		 **********************/

		/**
		 * Manipulate eeprom content for test
		 * @param index offset where to start
		 * @param data new data to write
		 * @return number of bytes copied, <0 for error
		 */
		int manip_eeprom(std::size_t index,
		        const std::vector<std::uint8_t>& data);

		/**
		 * Manipulate eeprom content for test
		 * @param index offset where to start
		 * @param data new data to write
		 * @return number of bytes copied, <0 for error
		 */
		int manip_eeprom(std::size_t index, const std::string& data);

		/**
		 * Manipulate a single byte in eeprom
		 * @param index offset where to start
		 * @param data new data to write
		 * @return number of bytes copied, <0 for error
		 */
		int manip_eeprom(std::size_t index, const std::uint8_t data);

	protected:
		std::vector<std::uint8_t> eeprom;
		tcv_t* tcv;
};

/**
 * Simple SFP implementation
 */
class FakeSFP: public FakeTCV
{
	public:
		FakeSFP(int index, i2c_read_cb_t read, i2c_write_cb_t write)
				: FakeTCV(index,read,write), diagnostics_size(256)
		{
			eeprom[0] = TCV_TYPE_SFP;
			/* Digital diagnostics, internally calibrated, rx_pwr=avg */
			eeprom[92] = (1<<6) | (1<<5) |(1<<3);
		}
		;
	private:
		const std::size_t diagnostics_size;
};

/**
 * Simple XFP implementation
 */
class FakeXFP: public FakeTCV
{
	public:
		FakeXFP(int index, i2c_read_cb_t read, i2c_write_cb_t write): FakeTCV(index,read,write)
		{
			eeprom[0] = TCV_TYPE_XFP;
		}
		;

};
}

#endif /* FAKE_TCV_HPP_ */
