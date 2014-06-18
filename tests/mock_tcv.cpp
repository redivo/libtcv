/************************************************************************************/
/**
 * \file   mock_tcv.cpp
 * \brief  
 * \author  thomas.ruschival
 *  		Created on: Jun 18, 2014
 */
/************************************************************************************/

#include <algorithm>    // std::copy
#include <vector>
#include "mock_tcv.hpp"

namespace mock{

MockTCV::MockTCV() :eeprom(PAGESIZE,0xFF), diagnostics_size(0){
	eeprom[0] = 0x03;
}



bool MockTCV::range_is_valid(tcv_dev_addr_t device, std::size_t offset, std::size_t size){

	switch(device){
		case a0:
			return (offset+size >= eeprom.size());
		case dd:
			return (offset+size >= diagnostics_size);
		default:
			return false;
	}
}


int MockTCV::read(tcv_dev_addr_t device, std::uint8_t regaddr, std::uint8_t * data,
		std::size_t size) const
{
	switch(device){
		case a0:
			std::copy(eeprom.begin()+regaddr, eeprom.begin()+regaddr+size, data);
		break;
		case dd: /* dd not implemented */
		default:
			return -1;
	}

	return 0;
}


int MockTCV::write(tcv_dev_addr_t device, std::uint8_t regaddr,
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
