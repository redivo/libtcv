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


#include <map>
#include <cstdint>
#include <memory>
#include "libtcv/tcv.h"
#include "fake_tcv.hpp"

namespace TestDoubles {

std::shared_ptr<FakeTCV> get_tcv(int index);

void clear_tcvs();

int add_tcv(int index, std::shared_ptr<FakeTCV> tcv);

int remove_tcv(int index);

extern "C" int i2c_write(int index, std::uint8_t dev_addr,
		std::uint8_t reg_addr, const std::uint8_t* data, std::size_t len);


extern "C" int i2c_read(int index, std::uint8_t dev_addr, std::uint8_t reg_addr,
		std::uint8_t* data, std::size_t len);

}
;
#endif /* FAKE_HW_HPP_ */
