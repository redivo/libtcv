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
	int elements;
	switch (device) {
		case a0:
			if(regaddr >= eeprom.size())
				return -1;

			elements = (len+regaddr) > eeprom.size() ? eeprom.size() - regaddr : len;

			std::copy_n(eeprom.begin() + regaddr, elements, data);
			return elements;

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

int FakeTCV::manip_eeprom(std::size_t index,
                          const std::vector<std::uint8_t>& data)
{
	size_t space = eeprom.size() - index;
	/* only copy what fits in vector*/
	int elements = (data.size() > space) ? data.size() - space : data.size();

	std::copy_n(data.begin(), elements, eeprom.begin() + index);

	return elements;
}


int FakeTCV::manip_eeprom(std::size_t index, const std::string& data)
{
	size_t space = eeprom.size() - index;
	/* only copy what fits in vector*/
	int elements = (data.size() > space) ? data.size() - space : data.length();
	std::copy_n(data.begin(),elements,eeprom.begin()+index);

	return elements;
}


int FakeTCV::manip_eeprom(std::size_t index, const std::uint8_t data){
	if(index >= eeprom.size())
		return 0;

	eeprom[index] = data;
	return 1;
}

} /*namespace */
