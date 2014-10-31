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

/************************************************************************************
 * \file   fake_hw_i2c.hpp
 * \brief  
 * \author  ruschi
 *  		Copyright (c) - 2014 thomas ruschival <thomas@ruschival.de>
 * Created on: Jun 29, 2014
 ************************************************************************************/

#ifndef FAKE_HW_I2C_H_
#define FAKE_HW_I2C_H_
#include <cstdint>

extern "C" int i2c_write(int index, std::uint8_t dev_addr,
		std::uint8_t reg_addr, const std::uint8_t* data, std::size_t len);


extern "C" int i2c_read(int index, std::uint8_t dev_addr, std::uint8_t reg_addr,
		std::uint8_t* data, std::size_t len);



#endif /* FAKE_HW_I2C_H_ */
