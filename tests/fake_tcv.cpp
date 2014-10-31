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

} /*namespace */
