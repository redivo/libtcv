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
