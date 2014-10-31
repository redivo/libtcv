/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 George Redivo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

/*
 * TestDut.cpp
 *
 *  Created on: Jul 3, 2013
 *      Author: ruschi
 */

#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include <iostream>

extern "C"{
#include "libtcv/tcv.h"
}
#include "gtest/gtest.h"
#include "fake_hw.hpp"
#include "fake_tcv.hpp"

using namespace std;
using namespace TestDoubles;

class TestFixtureClass : public ::testing::Test {
	public:
	TestFixtureClass()
	{
		add_tcv(1, make_shared<FakeSFP>(1, i2c_read, i2c_write));
		add_tcv(3, make_shared<FakeSFP>(3, i2c_read, i2c_write));
	}

	~TestFixtureClass()
	{
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

TEST_F(TestFixtureClass, doubleInit)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	int ret = tcv_init(tcv);
	ASSERT_EQ(0, ret);
	ret = tcv_init(tcv);
	ASSERT_EQ(0, ret);
}


TEST_F(TestFixtureClass, identSFP)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	EXPECT_EQ(0, tcv_init(tcv));
	EXPECT_EQ(TCV_TYPE_SFP, tcv_get_identifier(tcv));
}

/* Test update vendor oui */
TEST_F(TestFixtureClass, getVendorOUI)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	vector<uint8_t> vid = {0x01, 0x02, 0x03};
	/*write 3 bytes to eeprom */
	mtcv->manip_eeprom(37, vid);
	// Not initialized, should fail
	EXPECT_NE(tcv_get_vendor_oui(tcv), 0x010203);

	tcv_init(tcv);

	EXPECT_EQ(0x010203,tcv_get_vendor_oui(tcv));
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
	tcv_init(tcv);
	EXPECT_EQ(0,tcv_get_vendor_name(tcv,buf));
	EXPECT_STREQ(buf, name.c_str());

	name = "Sadia";
	EXPECT_EQ(mtcv->manip_eeprom(20, name), name.length());
	tcv_init(tcv);
	EXPECT_EQ(0,tcv_get_vendor_name(tcv,buf));
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
	tcv_init(tcv);
	EXPECT_EQ(tcv_get_vendor_part_number(tcv,buf),0);
	EXPECT_STREQ(buf, pn.c_str());

	/* Set invalid PN */
	vector<uint8_t> invalid_pn(6,0x00);
	EXPECT_EQ(mtcv->manip_eeprom(40, invalid_pn), invalid_pn.size());
	tcv_init(tcv);
	EXPECT_EQ(tcv_get_vendor_part_number(tcv,buf),0);
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
	tcv_init(tcv);
	int ret = tcv_get_vendor_revision(tcv,buf);
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
	mtcv->manip_eeprom(1, vector<uint8_t>(1, 0x4));
	tcv_init(tcv);
	EXPECT_EQ(0x04,tcv_get_ext_identifier(tcv));
}


/* Test for diagnositcs type */
TEST_F(TestFixtureClass, diagnosticsTypeValid)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	uint8_t diag_caps = 0x60;
	tcv_diagnostic_type_t diags;
	mtcv->manip_eeprom(92, diag_caps);
	tcv_init(tcv);
	EXPECT_EQ(0, tcv_get_diagnostic_type(tcv, &diags));

	EXPECT_EQ(diags.bits.address_change_required, 0);
	EXPECT_EQ(diags.bits.pwr_measurement_type, 0);
	EXPECT_EQ(diags.bits.internally_calibrated, 1);
	EXPECT_EQ(diags.bits.externally_calibrated, 0);
	EXPECT_EQ(diags.bits.dd_implemented, 1);

}

/* Test for diagnositcs type */
TEST_F(TestFixtureClass, diagnosticsTypeInValid)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	uint8_t diag_caps = 0xFF;
	tcv_diagnostic_type_t diags;
	mtcv->manip_eeprom(92,  diag_caps);
	tcv_init(tcv);
	EXPECT_EQ(0, tcv_get_diagnostic_type(tcv, &diags));
	/* Function should truncate invalid crap */
	EXPECT_EQ(diags.bmp, 0x1F);
}


/* Test for diagnositcs type */
TEST_F(TestFixtureClass, enhancedOptionsValid)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	uint8_t raw = 0x5A;
	tcv_enhanced_options_type_t eopts;
	mtcv->manip_eeprom(93,  raw);
	tcv_init(tcv);
	EXPECT_EQ(0, tcv_get_enhanced_options(tcv, &eopts));
	EXPECT_EQ(0x2D,eopts.bmp);
}


/* Test for diagnositcs type */
TEST_F(TestFixtureClass, enhancedOptionsInValid)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	uint8_t raw = 0xFF;
	tcv_enhanced_options_type_t eopts;
	mtcv->manip_eeprom(93,  raw);
	tcv_init(tcv);
	EXPECT_EQ(0, tcv_get_enhanced_options(tcv, &eopts));
	EXPECT_EQ(0x7F,eopts.bmp);
}


/* Test access to vendor rom section */
TEST_F(TestFixtureClass, getVendorRom)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();

	string vrom = "A.C.M.E";

	mtcv->manip_eeprom(96, vrom);

	tcv_init(tcv);
	const uint8_t* buff = tcv_get_vendor_rom(tcv);
	string res(reinterpret_cast<const char*>(buff),vrom.size());
	EXPECT_STREQ(vrom.c_str(),res.c_str());
}


/* Test access to extended rom section */
TEST_F(TestFixtureClass, get8979Rom)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();

	string rom = "Hallo Welt!";
	mtcv->manip_eeprom(128, rom);

	tcv_init(tcv);
	const uint8_t* buff = tcv_get_8079_rom(tcv);
	string res(reinterpret_cast<const char*>(buff),128);

	EXPECT_TRUE(std::equal(rom.begin(),rom.end(),res.begin()));
}


/* Test get_vendor_rom_size() */
TEST_F(TestFixtureClass, getVendorRomSize)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	tcv_init(tcv);

	EXPECT_EQ(static_cast<size_t>(32), tcv_get_vendor_rom_size(tcv));
}


/* Test for diagnositcs type */
TEST_F(TestFixtureClass, getEthStandardCode)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	tcv_eth_compliance_codes_t code;

	mtcv->manip_eeprom(6, (1<<3));
	tcv_init(tcv);
	EXPECT_EQ(0, tcv_get_eth_compliance_codes(tcv, &code));
	EXPECT_NE(0,code.bits.eth_1000_base_t);

	mtcv->manip_eeprom(6, (1<<4));
	tcv_init(tcv);
	EXPECT_EQ(0, tcv_get_eth_compliance_codes(tcv, &code));
	EXPECT_NE(0, code.bits.eth_100base_lx_lx10);
	EXPECT_EQ(0, code.bits.eth_1000_base_t);

	mtcv->manip_eeprom(6, 1);
	tcv_init(tcv);
	EXPECT_EQ(0, tcv_get_eth_compliance_codes(tcv, &code));
	EXPECT_NE(0, code.bits.eth_1000_base_sx);
	EXPECT_EQ(0, code.bits.eth_base_px);
}


/* Test for diagnositcs type */
TEST_F(TestFixtureClass, get10GStandardCode)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	tcv_10g_eth_compliance_codes_t code;
	mtcv->manip_eeprom(3, (0xF)); //Infiniband - should not show up in 10GbE
	tcv_init(tcv);
	EXPECT_EQ(0, tcv_get_10g_compliance_codes(tcv, &code));
	EXPECT_EQ(0, code.bmp);

	mtcv->manip_eeprom(3, (1<<4));
	tcv_init(tcv);
	EXPECT_EQ(0, tcv_get_10g_compliance_codes(tcv, &code));
	EXPECT_NE(0, code.bits.eth10g_base_sr);

	mtcv->manip_eeprom(3, (1<<7));
	tcv_init(tcv);
	EXPECT_EQ(0, tcv_get_10g_compliance_codes(tcv, &code));
	EXPECT_NE(0, code.bits.eth10g_base_er);
}





/**
 * Put this test at the end since asan will bail out
 */
TEST_F(TestFixtureClass, doubleDestroy)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	int ret = tcv_init(tcv);
	ASSERT_EQ(0, ret);
	ret = tcv_destroy(tcv);
	ASSERT_EQ(0, ret);
	/* should be invalid */
	ret = tcv_destroy(tcv); //really bad use after free()!
	ASSERT_EQ(TCV_ERR_INVALID_ARG, ret);
}

