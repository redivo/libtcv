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
