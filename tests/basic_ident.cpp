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

/* Test update vendor oui */
TEST_F(TestFixtureClass, getVendorOUI)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	vector<uint8_t> vid = {0x01, 0x02, 0x03};
	/*write 3 bytes to eeprom */
	EXPECT_EQ(mtcv->manip_eeprom(37, vid), 3);
	// Not initialized, should fail
	EXPECT_NE(tcv->fun->get_vendor_oui(tcv), 0x010203);

	int ret = tcv_init(tcv, 1, i2c_read, i2c_write);
	EXPECT_EQ(ret, 0);
	EXPECT_EQ(tcv->fun->get_vendor_oui(tcv), 0x010203);
}


/* Test vendor name */
TEST_F(TestFixtureClass, getVendorName)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	string name = "Fritz & Frieda  "; //16 chars
	char buf[128];
	/*write the name to eeprom */
	EXPECT_EQ(mtcv->manip_eeprom(20, name), name.length());
	tcv_init(tcv, 1, i2c_read, i2c_write);
	EXPECT_EQ(tcv->fun->get_vendor_name(tcv,buf),0);
	EXPECT_STREQ(buf, name.c_str());

	name = "Sadia";
	EXPECT_EQ(mtcv->manip_eeprom(20, name), name.length());
	tcv_init(tcv, 1, i2c_read, i2c_write);
	EXPECT_EQ(tcv->fun->get_vendor_name(tcv,buf),0);
	// Expect it to fail - eeprom should contain junk from last test
	EXPECT_STRNE(buf, name.c_str());
}

/* Check Vendor Serial Number */
TEST_F(TestFixtureClass, getVendorSN)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	string pn = "Tabajara-SFP    "; //16 chars
	char buf[128];
	/*write the name to eeprom */
	EXPECT_EQ(mtcv->manip_eeprom(40, pn), pn.length());
	tcv_init(tcv, 1, i2c_read, i2c_write);
	EXPECT_EQ(tcv->fun->get_vendor_part_number(tcv,buf),0);
	EXPECT_STREQ(buf, pn.c_str());

	/* Set invalid PN */
	vector<uint8_t> invalid_pn(6,0x00);
	EXPECT_EQ(mtcv->manip_eeprom(40, invalid_pn), invalid_pn.size());
	tcv_init(tcv, 1, i2c_read, i2c_write);
	EXPECT_EQ(tcv->fun->get_vendor_part_number(tcv,buf),0);
	EXPECT_STRNE(buf, 0);
}

/* Test extended Identifiers */
TEST_F(TestFixtureClass, VendorRev)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	string rev = "ZXY9"; //4 chars
	char buf[128];
	/*write the name to eeprom */
	EXPECT_EQ(mtcv->manip_eeprom(56, rev), rev.length());
	tcv_init(tcv, 1, i2c_read, i2c_write);
	int ret = tcv->fun->get_vendor_revision(tcv,buf);
	EXPECT_EQ(ret,0);
	EXPECT_STREQ(buf, rev.c_str());
}

/* Test extended Identifiers */
TEST_F(TestFixtureClass, extendedIdent)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	 /*
	  * [...]The field should be  set to 04h for all SFP modules indicating
	  * two-wire interface ID module definition (SFF-8972)
	  */
	EXPECT_EQ( mtcv->manip_eeprom(1, vector<uint8_t>(1, 0x4)), 1);
	tcv_init(tcv, 1, i2c_read, i2c_write);
	EXPECT_EQ(tcv->fun->get_ext_itendifier(tcv),0x04);
}
