/************************************************************************************/
/**
 * \file   mock_tcv.hpp
 * \brief  
 * \author  thomas.ruschival
 *  		Created on: Jun 18, 2014
 */
/************************************************************************************/
#ifndef MOCK_TCV_HPP_
#define MOCK_TCV_HPP_


#include <cstdint>

namespace mock {

	const std::size_t PAGESIZE = 256;

	enum tcv_dev_addr_t{
		a0 = 0x50,
		dd = 0x51,
	};

	class MockTCV {
		public:
			MockTCV();

			virtual int read(tcv_dev_addr_t device, std::uint8_t regaddr, std::uint8_t * data,
					std::size_t size) const;

			virtual int write(tcv_dev_addr_t device, std::uint8_t regaddr,
					const std::uint8_t * data, std::size_t size);

			virtual ~MockTCV(){};

			bool range_is_valid(tcv_dev_addr_t device, std::size_t offset, std::size_t size);

		private:
			std::vector<std::uint8_t> eeprom;
			const std::size_t diagnostics_size;
	};
}

#endif /* MOCK_TCV_HPP_ */
