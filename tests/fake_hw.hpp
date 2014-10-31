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
 * \file    fake_hw.hpp
 * \brief   Hardware abstraction to provide mock functions  to transceiver access
 * \author  thomas.ruschival
 *  		Created on: Jun 17, 2014
 */
/************************************************************************************/

#ifndef FAKE_HW_HPP_
#define FAKE_HW_HPP_


#include <cstdint>
#include <memory>
#include "libtcv/tcv.h"
#include "fake_tcv.hpp"

namespace TestDoubles {

/** Access Fake Transceivers */
std::shared_ptr<FakeTCV> get_tcv(int index);

void clear_tcvs();

int add_tcv(int index, std::shared_ptr<FakeTCV> tcv);

int remove_tcv(int index);

}
;
#endif /* FAKE_HW_HPP_ */
