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

extern "C"{
#include <arpa/inet.h> /* nthol, htons etc */
}

using namespace std;

namespace TestDoubles{


int FakeTCV::write(tcv_dev_addr_t device, std::uint8_t regaddr,
		const std::uint8_t * data, std::size_t size)
{
	return 0;
}

int FakeTCV::read(tcv_dev_addr_t device, std::uint8_t regaddr,
        std::uint8_t * data, std::size_t len) const
{
	int elements;
	switch (device) {
		case a0:
			elements = cap_length(eeprom, regaddr, len);
			if(elements > 0)
				copy_n(eeprom.begin()+regaddr, elements, data);
			break;
		default:
			return -1;
	}
	return elements;
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

template<typename T>
size_t FakeTCV::cap_length(const std::vector<T> & container, size_t offset, size_t length) const
{
	if(container.size() <= offset)
		return 0;

	return (length+offset) > container.size() ? container.size() - offset : length;
}

int FakeSFP::read(tcv_dev_addr_t device, std::uint8_t regaddr,
        std::uint8_t * data, std::size_t len) const
{
	int elements;
	switch (device) {
		case a0:
			elements = cap_length(eeprom, regaddr, len);
			if(elements > 0)
				copy_n(eeprom.begin()+regaddr, elements, data);
			break;
		case dd:
			elements = cap_length(diagnostics, regaddr, len);
			if(elements > 0)
				copy_n(diagnostics.begin()+regaddr, elements, data);
			break;
		default:
			return -1;
	}

	return elements;
}


int FakeTCV::manip_dd(std::size_t index, std::uint8_t data)
{
	if(index >= diagnostics.size())
		return 0;

	diagnostics[index] = data;
	return 1;
}

int FakeTCV::manip_dd(std::size_t index, std::uint16_t data)
{
	/** need 2 bytes */
	if (index + 1 >= diagnostics.size())
		return 0;

	uint16_t tmp = htons(data);

	diagnostics[index] = static_cast<uint8_t>(tmp & 0x00FF);
	diagnostics[index + 1] = static_cast<uint8_t>(tmp >> 8);

	return 2;
}

int FakeTCV::manip_dd(std::size_t index, std::int16_t data)
{
	return manip_dd(index, static_cast<std::uint16_t>(data));
}



int FakeTCV::manip_dd(std::size_t index, const std::string& data)
{
	size_t space = diagnostics.size() - index;
	/* only copy what fits in vector*/
	int elements = (data.size() > space) ? data.size() - space : data.length();
	std::copy_n(data.begin(), elements, diagnostics.begin() + index);

	return elements;
}

int FakeTCV::manip_dd(std::size_t index, uint32_t data)
{
	uint32_t bytes = htonl(data);
	diagnostics[index] = (bytes & 0xFF);
	diagnostics[index + 1] =((bytes >> 8) & 0xFF);
	diagnostics[index + 2] =((bytes >> 16) & 0xFF);
	diagnostics[index + 3] = ((bytes >> 24) & 0xFF);
	return 4;
}

int FakeTCV::manip_dd(std::size_t index, float data)
{
	union {
		uint32_t bytes;
		float flt;
	} converter;
	converter.flt = data;

	manip_dd(index,  converter.bytes);
	return 1;
}

} /*namespace */
