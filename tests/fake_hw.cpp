
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

