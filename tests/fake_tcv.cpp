/************************************************************************************/
/**
 * \file   fake_tcv.cpp
 * \brief  
 * \author  thomas.ruschival
 *  		Created on: Jun 18, 2014
 */
/************************************************************************************/

#include <algorithm>    // std::copy
#include <vector>
#include "fake_tcv.hpp"

using namespace std;

namespace TestDoubles{


bool FakeTCV::range_is_valid(tcv_dev_addr_t device, std::size_t offset,
        std::size_t len) const
{
	std::cout << " Eeprom " << eeprom.size() << " : " << eeprom[8] << std::endl;
	switch (device) {
		case a0:
			return (offset + len <= eeprom.size() );
		default:
			return false;
	}
}

int FakeTCV::read(tcv_dev_addr_t device, std::uint8_t regaddr,
        std::uint8_t * data, std::size_t len) const
{
	switch (device) {
		case a0:
			std::copy(eeprom.begin() + regaddr,
				        eeprom.begin() + regaddr + len, data);
			return (regaddr + len <= eeprom.size()) ?  len : eeprom.size()-regaddr;
			break;
		case dd: /* dd not implemented */
		default:
			return -1;
	}

	return 0;
}


int FakeTCV::write(tcv_dev_addr_t device, std::uint8_t regaddr,
		const std::uint8_t * data, std::size_t size)
{
	if(!range_is_valid(device, regaddr, size))
		return -1;

	switch(device){
		case a0:

			std::copy(data, data+size, eeprom.begin()+regaddr);

		break;
		case dd: /* no write to DD */
		default:
			return -1;
	}

	return 0;
}

}
