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


#include <algorithm>    // std::copy
#include <vector>
#include <memory>
#include "fake_hw.hpp"


using namespace std;

namespace TestDoubles {

static map<int, shared_ptr<FakeTCV> > tcvs;

void clear_tcvs(){
	tcvs.clear();
}


std::shared_ptr<FakeTCV> get_tcv(int index){
	auto it = tcvs.find(index);
	if(it != tcvs.end())
		return it->second;

	return nullptr;
}

int add_tcv(int index, shared_ptr<FakeTCV> tcv){
	auto it = tcvs.find(index);
	if (it != tcvs.end())
		return -1;

	tcvs[index]=tcv;
	return 0;
}

int remove_tcv(int index){
	auto it = tcvs.find(index);
	if(it != tcvs.end()){
		tcvs.erase(it);
		return 0;
	}
	return -1;
}

extern "C" int i2c_write(int index, uint8_t dev_addr, uint8_t reg_addr, const uint8_t* data, size_t len)
{
	auto tcv = get_tcv(index);
	if (tcv != nullptr) {
		return tcv->write(static_cast<tcv_dev_addr_t>(dev_addr), reg_addr, data, len);
	}
	return -1;
}

extern "C" int i2c_read(int index, uint8_t dev_addr, uint8_t reg_addr, uint8_t* data, size_t len)
{
	auto tcv = get_tcv(index);
	if (tcv != nullptr ) {
		return tcv->read(static_cast<tcv_dev_addr_t>(dev_addr), reg_addr, data, len);
	}
	return -1;
}

}

