/************************************************************************************/
/**
 * \file    mock_hw.hpp
 * \brief   Hardware abstraction to provide mock functions  to transceiver access
 * \author  thomas.ruschival
 *  		Created on: Jun 17, 2014
 */
/************************************************************************************/

#include <map>
#include <cstdint>
#include "mock_tcv.hpp"

namespace mock {

MockTCV* get_tcv(int index);

int add_tcv(int index, MockTCV* tcv);

int remove_tcv(int index);

int i2c_write(int index, std::uint8_t dev_addr, std::uint8_t reg_addr, std::uint8_t* data,
		std::uint8_t len);

int i2c_read(int index, std::uint8_t dev_addr, std::uint8_t reg_addr, std::uint8_t* data,
		std::uint8_t len);

}
;
