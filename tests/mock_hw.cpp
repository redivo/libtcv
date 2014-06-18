
#include <algorithm>    // std::copy
#include <vector>
#include "mock_hw.hpp"


using namespace std;

namespace mock {

static map<int, MockTCV> tcvs;

void clear_tcvs(){
	tcvs.clear();
}


MockTCV& get_tcv(int index){
	map<int, MockTCV>::iterator it = tcvs.find(index);
	return it->second;
}

int add_tcv(int index, MockTCV* tcv){
	/** Check if it already exists in Map -> make sure we don't loose track of MockTCV ptr */
	map<int, MockTCV>::iterator it = tcvs.find(index);
	if (it != tcvs.end())
		return -1;

	tcvs[index]=tcv;
	return 0;
}

int remove_tcv(int index){
	map<int, MockTCV>::iterator it = tcvs.find(index);
	if(it != tcvs.end()){
		tcvs.erase(it);
		return 0;
	}
	return -1;
}

int i2c_write(int index, uint8_t dev_addr, uint8_t reg_addr, uint8_t* data, uint8_t len)
{
	MockTCV& tcv = get_tcv(index);
	if (tcv != NULL) {
		return tcv->write(static_cast<tcv_dev_addr_t>(dev_addr), reg_addr, data, len);
	}
	return -1;
}

int i2c_read(int index, uint8_t dev_addr, uint8_t reg_addr, uint8_t* data, uint8_t len)
{
	MockTCV* tcv = get_tcv(index);
	if (tcv != NULL) {
		return tcv->read(static_cast<tcv_dev_addr_t>(dev_addr), reg_addr, data, len);
	}
	return -1;
}

}

