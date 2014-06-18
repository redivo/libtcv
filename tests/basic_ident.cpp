/*
 * TestDut.cpp
 *
 *  Created on: Jul 3, 2013
 *      Author: ruschi
 */

#include "libtcv/tcv.h"
#include "gtest/gtest.h"
#include "mock_hw.hpp"
#include "mock_tcv.hpp"

using namespace std;
using namespace mock;

class TestFixtureClass: public ::testing::Test {
public:
	TestFixtureClass(){
		mtcv1 =   new MockTCV;
		mtcv2 =   new MockTCV;
		add_tcv(1,mtcv1 );
		add_tcv(3,mtcv2);
	};
    tcv_t tcv1;
    tcv_t tcv3;

    MockTCV* mtcv1;
    MockTCV* mtcv2;

    ~TestFixtureClass(){

    }
};


TEST(BasicTest,GtestTest){
	EXPECT_EQ(1,1);
}


TEST_F(TestFixtureClass, setupworks) {
	MockTCV tcv = get_tcv(1);
	EXPECT_NE(tcv,NULL);
	tcv = get_tcv(2);
	EXPECT_EQ(tcv,NULL);
}

