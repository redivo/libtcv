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

extern "C" {
#include <libtcv/tcv.h>
}

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
		FakeTCV()
				: eeprom(PAGESIZE, 0xFF)
		{
			eeprom[0] = TCV_TYPE_UNKNOWN;
		}
		;

		virtual int read(tcv_dev_addr_t device, std::uint8_t regaddr,
		        std::uint8_t * data, std::size_t len) const;

		virtual int write(tcv_dev_addr_t device, std::uint8_t regaddr,
		        const std::uint8_t * data, std::size_t len);

		virtual ~FakeTCV()
		{
		}
		;

		virtual bool range_is_valid(tcv_dev_addr_t device, std::size_t offset,
		        std::size_t size) const;

		tcv_t* get_ctcv()
		{
			return &tcv;
		}
		;

		/**********************
		 * Instrumentation functions
		 **********************/

		/**
		 * Manipulate eeprom content for test
		 * @param index offset where to start
		 * @param data new data to write
		 */
		void manip_eeprom(std::size_t index,
		        const std::vector<std::uint8_t>& data);

	protected:
		std::vector<std::uint8_t> eeprom;
		tcv_t tcv;
};

/**
 * Simple SFP implementation
 */
class FakeSFP: public FakeTCV
{
	public:
		FakeSFP()
				: diagnostics_size(256)
		{
			eeprom[0] = TCV_TYPE_SFP;
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
		FakeXFP()
		{
			eeprom[0] = TCV_TYPE_XFP;
		}
		;

};
}

#endif /* FAKE_TCV_HPP_ */
