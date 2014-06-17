/************************************************************************************/
/**
 * \file    hw_mock.hpp
 * \brief   Hardware abstraction to provide mock functions  to transceiver access
 * \author  thomas.ruschival
 *  		Created on: Jun 17, 2014
 */
/************************************************************************************/

#include <map>
#include <cstdint>

namespace hw_mock{

	const std::size_t PAGESIZE = 256;

	enum {
		a0 = 0x50,
		dd = 0x51,
	} sfp_dev_addr_t;

	class MockSFP{
		public:
			MockSFP(std::uint8_t* a1, *std::uint8_t* dd);
			void set_data(sfp_dev_addr_t page, std::uint8_t* newval);
			void manip_data(sfp_dev_addr_t page, std::size_t byteoffset, std::uint8_t newval);

			int read(sfp_dev_addr_t device, std::uint8_t regaddr, std::uint8_t * data, std::size_t size) const;
			int write(sfp_dev_addr_t device, std::uint8_t regaddr, const std::uint8_t * data, std::size_t size);
		private:
			std::uint8_t* a0;
			std::uint8_t* dd;
	};

	class Hardware{

		public:
			Hardware(int ntcvs);

			const MockSFP& get_sfp(int index);

		private:
			std::map<int, MockSFP> tcvs;
	};

	int i2c_write(int index, uint8_t dev_addr, uint8_t reg_addr, uint8_t* data, uint8_t len);

	int i2c_read(int index, uint8_t dev_addr, uint8_t reg_addr, uint8_t* data, uint8_t len);


};
