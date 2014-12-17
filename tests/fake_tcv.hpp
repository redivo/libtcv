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
#include <cstddef>
#include <iostream>
#include <vector>


#include <libtcv/tcv.h>
#include "fake_hw_i2c.hpp"

namespace TestDoubles {

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
		FakeTCV(int index, i2c_read_cb_t read, i2c_write_cb_t write, std::size_t pagesize = 256)
			: eeprom(pagesize,0xFF) , diagnostics(pagesize, 0xFF)
		{
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

		tcv_t* get_ctcv() const
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
		virtual int manip_eeprom(std::size_t index,
		        const std::vector<std::uint8_t>& data);

		/**
		 * Manipulate eeprom content for test
		 * @param index offset where to start
		 * @param data new data to write
		 * @return number of bytes copied, <0 for error
		 */
		virtual  int manip_eeprom(std::size_t index, const std::string& data);

		/**
		 * Manipulate a single byte in eeprom
		 * @param index offset where to start
		 * @param data new data to write
		 * @return number of bytes copied, <0 for error
		 */
		virtual int manip_eeprom(std::size_t index, const std::uint8_t data);

		/**
		 * Manipulate digital diagnostics
		 * @param index offset where to start
		 * @param data new data to write
		 * @return number of bytes copied, <0 for error
		 */
		virtual int manip_dd(std::size_t index, std::uint8_t data);

		/**
		 * Manipulate digital diagnostics
		 * @param index offset where to start
		 * @param data written to eeprom with MSB at lower address
		 * @return number of bytes copied, <0 for error
		 */
		virtual int manip_dd(std::size_t index, std::uint16_t data);

		/**
		 * Manipulate digital diagnostics values (wrap uint16_t)
		 * @param index offset where to start
		 * @param data written to eeprom with MSB at lower address
		 * @return number of bytes copied, <0 for error
		 */
		virtual int manip_dd(std::size_t index, std::int16_t data);

		/**
		 * Convert 4-Byte word to big-endian and store in DD calibration
		 * @param index eeprom address offset
		 * @param data Host-Order data
		 * @return number of bytes copied, <0 for error
		 */
		virtual int manip_dd(std::size_t index, std::uint32_t data);

		/**
		 * Manipulate digital diagnosticvcs
		 * @param index offset where to start
		 * @param data new data to write
		 * @return number of bytes copied, <0 for error
		 */
		virtual int manip_dd(std::size_t index, const std::string& data);

		/**
		 * Manipulate digital diagnostics calibration data
		 * @param index offset where to start
		 * @param data written to dd calibration with MSB at lower address
		 * @return number of bytes copied, <0 for error
		 */
		virtual int manip_dd(std::size_t index, float data);

	protected:
		std::vector<std::uint8_t> eeprom;
		std::vector<uint8_t> diagnostics;
		tcv_t* tcv;

		/**
		 * Checks if up to length bytes can be accessed in containter
		 * @param container to operate on
		 * @param offset index starting read/write
		 * @param length number of requested elements
		 * @return legth if it fits, maximum available elements otherwise
		 */
		template<typename T>
		size_t cap_length(const std::vector<T> & container, size_t offset, size_t length) const;
};

/**
 * Simple SFP implementation
 */
class FakeSFP: public FakeTCV
{
	public:
		FakeSFP(int index, i2c_read_cb_t read, i2c_write_cb_t write)
				: FakeTCV(index,read,write)
		{
			eeprom[0] = TCV_TYPE_SFP;
			/* Digital diagnostics, internally calibrated, rx_pwr=avg */
			eeprom[92] = (1<<6) | (1<<5) |(1<<3);
		}
		;
		/* overwrite read for Digital Diagnostics */
		virtual int read(tcv_dev_addr_t device, std::uint8_t regaddr,
		        std::uint8_t * data, std::size_t len) const;
};

/**
 * Simple XFP implementation
 */
class FakeXFP: public FakeTCV
{
	public:
		FakeXFP(int index, i2c_read_cb_t read, i2c_write_cb_t write)
				: FakeTCV(index, read, write, 128)
		{
			eeprom[0] = TCV_TYPE_XFP;
		}
		;

};
}

#endif /* FAKE_TCV_HPP_ */
