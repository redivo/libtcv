/*
 * TestDut.cpp
 *
 *  Created on: Jul 3, 2013
 *      Author: ruschi
 */

#include "libtcv/tcv.h"
#include "gtest/gtest.h"

using namespace std;


class TestFixtureClass: public ::testing::Test {
public:
	TestFixtureClass(){};
    //	tcv_t tcv;
};


TEST(BasicTest,initial){
    EXPECT_EQ(1,1);
    //tcv_t tcv;
	//sfft_init(&tcv);
}


TEST_F(TestFixtureClass, willfail) {
       //EXPECT_EQ(add(tmg, 12,-4), 10);
}

