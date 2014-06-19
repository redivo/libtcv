/*
 * TestDut.cpp
 *
 *  Created on: Jul 3, 2013
 *      Author: ruschi
 */

#include <memory>

extern "C"{
#include "libtcv/tcv.h"
}
#include "gtest/gtest.h"
#include "fake_hw.hpp"
#include "fake_tcv.hpp"

using namespace std;
using namespace TestDoubles;

class TestFixtureClass: public ::testing::Test {
public:
	TestFixtureClass(){
		add_tcv(1, make_shared<FakeSFP>());
		add_tcv(3, make_shared<FakeSFP>());
	};
    ~TestFixtureClass(){
    	clear_tcvs();
    }
};


TEST_F(TestFixtureClass, setupworks) {
	EXPECT_NE(get_tcv(1), nullptr);
	EXPECT_NE(get_tcv(3), nullptr);
	EXPECT_EQ(get_tcv(2), nullptr);
}

TEST_F(TestFixtureClass, readBeyond)
{
	auto mtcv = get_tcv(1);
	uint8_t buf[512];
	int ret;

	ret = mtcv->read(a0,0,buf,128);
	EXPECT_EQ(ret, 128);
	ret = mtcv->read(a0,120,buf,128);
	EXPECT_EQ(ret, 128);
	ret = mtcv->read(a0,150,buf,128);
	EXPECT_EQ(ret, 106);

}

TEST_F(TestFixtureClass, identSFP)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	int ret = tcv_init(tcv, 1, i2c_read, i2c_write);
	EXPECT_EQ(ret, 0);
	EXPECT_EQ(tcv->fun->get_itendifier(tcv), TCV_TYPE_SFP);
}


