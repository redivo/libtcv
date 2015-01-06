/*
 * Test Digital Diagnostics
 *
 *  Created on: Jul 3, 2014
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
#include <cmath>

using namespace std;
using namespace TestDoubles;

class TestDiagnosticSetup : public ::testing::Test {
	public:
	TestDiagnosticSetup()
	{
		add_tcv(1, make_shared<FakeSFP>(1, i2c_read, i2c_write));
		add_tcv(3, make_shared<FakeSFP>(3, i2c_read, i2c_write));
	}

	~TestDiagnosticSetup()
	{
		clear_tcvs();
	}
};

/* Test for diagnositcs type */
TEST_F(TestDiagnosticSetup, DDisInitialized)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	tcv_diagnostic_type_t diags;
	tcv_init(tcv);
	EXPECT_EQ(0, tcv_get_diagnostic_type(tcv, &diags));
	/* Bit should be set */
	EXPECT_EQ(diags.bits.address_change_required, 0);
	EXPECT_EQ(diags.bits.pwr_measurement_type, 1);
	EXPECT_EQ(diags.bits.externally_calibrated, 0);
	EXPECT_EQ(diags.bits.internally_calibrated, 1);
	EXPECT_EQ(diags.bits.dd_implemented, 1);
}

/* Test for diagnositcs type */
TEST_F(TestDiagnosticSetup, diagnosticsTypeValid)
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
	EXPECT_EQ(diags.bits.externally_calibrated, 0);
	EXPECT_EQ(diags.bits.internally_calibrated, 1);
	EXPECT_EQ(diags.bits.dd_implemented, 1);

}

/* Test for diagnositcs type */
TEST_F(TestDiagnosticSetup, diagnosticsTypeInValid)
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



/* Test temperature using sane value */
TEST_F(TestDiagnosticSetup, tempInternalCalib)
{
	int16_t t_read;
	float temp = 48.5; 		// 3*16+1/2
	int16_t temp_f = (int16_t) (temp * 256.0);

	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	mtcv->manip_eeprom(92,0x60); // Internally calibrated
	tcv_init(tcv);

	mtcv->manip_dd(96, uint16_t(temp_f));

	EXPECT_EQ(0, tcv_get_temperature(tcv, &t_read));
	EXPECT_FLOAT_EQ(temp, ((float )t_read) / 256.0);

	temp = -1.5;
	temp_f = (int16_t) (temp * 256.0);
	mtcv->manip_dd(96, uint16_t(temp_f));

	EXPECT_EQ(0, tcv_get_temperature(tcv, &t_read));
	EXPECT_FLOAT_EQ(temp, ((float )t_read) / 256.0);
}


/* Test temperature using sane value */
TEST_F(TestDiagnosticSetup, tempExternalCalib)
{
	int16_t t_read;
	float temp = 48.5; 		// 3*16+1/2
	float offset = 0.125;  // 1/8
	float slope = 1.0625;   // 1+1/16
	int16_t slope_f = (int16_t) (slope * 256.0);
	int16_t temp_f = (int16_t) (temp * 256.0);
	int16_t offset_f = (int16_t) (offset * 256.0);

	auto mtcv = get_tcv(1);
	tcv_t *tcv = mtcv->get_ctcv();
	mtcv->manip_eeprom(92,0x50); // Externally calibrated

	tcv_init(tcv);
	mtcv->manip_dd(84, slope_f);
	mtcv->manip_dd(86, offset_f);
	mtcv->manip_dd(96, temp_f);

	float expected = (slope * temp) + offset;

	EXPECT_EQ(0, tcv_get_temperature(tcv, &t_read));
	EXPECT_FLOAT_EQ(expected, ((float )t_read) / 256.0);

	temp = 0.5;
	temp_f = (int16_t) (temp * 256.0);
	mtcv->manip_dd(96, temp_f);
	expected = (slope * temp) + offset;

	EXPECT_EQ(0, tcv_get_temperature(tcv, &t_read));
	EXPECT_FLOAT_EQ(expected, ((float )t_read) / 256.0);

	temp = -1.5;
	temp_f = (int16_t) (temp * 256.0);
	mtcv->manip_dd(96, temp_f);
	expected = (slope * temp) + offset;

	EXPECT_EQ(0, tcv_get_temperature(tcv, &t_read));
	EXPECT_FLOAT_EQ(expected, ((float )t_read) / 256.0);
}


/* Test Voltage Diagnostics */
TEST_F(TestDiagnosticSetup, voltageExternalCalib)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = NULL;

	uint16_t vcc_read;
	int16_t vcc = 3300; 	 // uV
	int16_t offset = 2125;   // uV
	int16_t slope = 0x0401;
	/*test invalid param */
	EXPECT_NE(0, tcv_get_voltage(tcv, &vcc_read));
	tcv = mtcv->get_ctcv();

	mtcv->manip_dd(88, slope);
	mtcv->manip_dd(90, offset);
	mtcv->manip_dd(98, vcc);
	mtcv->manip_eeprom(92,0x58); // Externally calibrated
	tcv_init(tcv);

	int expected = (slope * vcc) / 256 + offset;

	EXPECT_EQ(0, tcv_get_voltage(tcv, &vcc_read));
	EXPECT_EQ(expected, vcc_read);
}


/* Test Bias Current Diagnostics */
TEST_F(TestDiagnosticSetup, txbiasExternalCalib)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = NULL;
	uint16_t cur_read;
	int16_t curr_ad = 3210; 	 // uA
	int16_t offset = 1000;   // uA
	int16_t slope = 0x0108;

	/*test invalid param */
	EXPECT_NE(0, tcv_get_tx_cur(tcv, &cur_read));

	tcv = mtcv->get_ctcv();
	tcv_init(tcv);
	mtcv->manip_dd(76, slope);
	mtcv->manip_dd(78, offset);
	mtcv->manip_dd(100, curr_ad);
	mtcv->manip_eeprom(92,0x58); // Externally calibrated
	tcv_init(tcv);

	int expected = (slope * curr_ad) / 256 + offset;

	EXPECT_EQ(0, tcv_get_tx_cur(tcv, &cur_read));
	EXPECT_EQ(expected, cur_read);
}

/* Test Current Diagnostics internally Calibrated */
TEST_F(TestDiagnosticSetup, txbiasInternalCalib)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = NULL;
	uint16_t cur_read;
	int16_t curr_ad = 32100; 	 // uA

	/*test invalid param */
	EXPECT_NE(0, tcv_get_tx_cur(tcv, &cur_read));

	tcv = mtcv->get_ctcv();
	mtcv->manip_eeprom(92, 0x60); // Internally calibrated
	tcv_init(tcv);

	mtcv->manip_dd(100, curr_ad);

	EXPECT_EQ(0, tcv_get_tx_cur(tcv, &cur_read));
	EXPECT_EQ(curr_ad, cur_read);
}


/* Test Tx-Power calculation */
TEST_F(TestDiagnosticSetup, txPwrExternalCalib)
{
    auto mtcv = get_tcv(1);
    tcv_t* tcv = NULL;
    uint16_t pwr_read;
    int16_t adval = 17543; // uW
    int16_t offset = -235; // uW
    int16_t slope = 0x0102;

    /*test invalid param */
    EXPECT_NE(0, tcv_get_tx_pwr(tcv, &pwr_read));

    tcv = mtcv->get_ctcv();
    mtcv->manip_eeprom(92, 0x50); // Externally calibrated
    tcv_init(tcv);

    mtcv->manip_dd(80, slope);
    mtcv->manip_dd(82, offset);
    mtcv->manip_dd(102, adval);

    int expected = (slope * adval) / 256 + offset;

    EXPECT_EQ(0, tcv_get_tx_pwr(tcv, &pwr_read));
    EXPECT_EQ(expected, pwr_read);
}

/* Test Tx-Power calculation (internally calibrated) */
TEST_F(TestDiagnosticSetup, txPwrInternalCalib)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = NULL;
	uint16_t pwr_read;
	int16_t adval = 17543; // uW

	/*test invalid param */
	EXPECT_NE(0, tcv_get_tx_pwr(tcv, &pwr_read));

	tcv = mtcv->get_ctcv();
	mtcv->manip_eeprom(92, 0x60); // Internally calibrated
	tcv_init(tcv);

	mtcv->manip_dd(102, adval);

	EXPECT_EQ(0, tcv_get_tx_pwr(tcv, &pwr_read));
	EXPECT_EQ(adval, pwr_read);
}


/* Test RX-Power */
TEST_F(TestDiagnosticSetup, rxPwrExternalCalib)
{
	auto mtcv = get_tcv(1);
	tcv_t *tcv = NULL;
	uint16_t pwr_read;
	int expected = 0;
	uint16_t adval = 16224; // 0x3F60 -4.4dBm

	/* Values taken from Apac Opto LS38-C3S-TC-N-DD */
	float pwr_fac[5] = {
		-3.787173,
		0.222775,
		0.0,
		0.000,
		0.0000
	};

	/*test invalid param */
	EXPECT_NE(0, tcv_get_rx_pwr(tcv, &pwr_read));

	tcv = mtcv->get_ctcv();
	mtcv->manip_eeprom(92, 0x50); // Externally calibrated
	tcv_init(tcv);

	/* reverse order */
	mtcv->manip_dd(56, pwr_fac[4]);
	mtcv->manip_dd(60, pwr_fac[3]);
	mtcv->manip_dd(64, pwr_fac[2]);
	mtcv->manip_dd(68, pwr_fac[1]);
	mtcv->manip_dd(72, pwr_fac[0]);

	mtcv->manip_dd(104, adval);

	expected = (uint16_t)(pwr_fac[4] * pow(adval,4) +
			pwr_fac[3] * pow(adval,3) +
			pwr_fac[2] * pow(adval,2) +
			pwr_fac[1] * pow(adval,1) +
			pwr_fac[0]);

	EXPECT_EQ(0, tcv_get_rx_pwr(tcv, &pwr_read));
	EXPECT_EQ(expected, pwr_read);
}
