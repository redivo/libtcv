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
 * NOTE
 * This file is based on SFF-8472 rev11.3 and INF-8077i rev4.5.
 * It's an MSA translation.
 */

#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>

#include "libtcv/tcv_internal.h"
#include "libtcv/sfp.h"
#include "libtcv/xfp.h"

/**
 * Check if transceiver was properly created by tcv_create()
 * @param tcv
 * @return
 */
static bool tcv_is_valid(tcv_t *tcv)
{
	/* Check if parametes is valid/was created successfully by tcv_create */
	if (tcv && tcv->created)
		return true;

	return false;
}

/******************************************************************************/

/**
 * Check if transceiver was properly initialized
 * @param tcv
 * @return
 */
static bool tcv_is_initialized(tcv_t *tcv)
{
	if (!tcv_is_valid(tcv))
		return false;

	return tcv->initialized;
}
/******************************************************************************/

/******************************************************************************/
/**
 * Check if tcv is exists/is valid and lock for exclusive access
 * @param tcv
 * @return
 */
static bool tcv_check_and_lock_ok(tcv_t* tcv)
{
	if (!tcv_is_valid(tcv))
		return false;

	if (pthread_mutex_lock(&tcv->lock))
		return false;

	return true;
}

/******************************************************************************/
/**
 * Unlock mutex wrapper to use if tcv_check_and_lock() uses mutex
 * @param tcv
 * @return 0 for success, error code oderwise
 */
static inline int tcv_unlock(tcv_t* tcv)
{
	return pthread_mutex_unlock(&tcv->lock);
}

/******************************************************************************/
tcv_t * tcv_create(int index, i2c_read_cb_t read, i2c_write_cb_t write)
{
	tcv_t * tcv;
	/* Check parameters */
	if (read == NULL || write == NULL)
		return NULL ;

	tcv = (tcv_t*) malloc(sizeof(tcv_t));
	if (!tcv)
		return NULL ;

	/* initialize mutex */
	if (pthread_mutex_init(&tcv->lock, NULL) < 0) {
		free(tcv);
		return NULL ;
	}

	/* Initialize data */
	tcv->index = index;
	/* Provide basic read/write functions */
	tcv->read = read;
	tcv->write = write;
	tcv->created = true;
	/* initialize to be able to check in tcv_is_initialized() */
	tcv->data = NULL;
	tcv->initialized = false;
	return tcv;
}

/******************************************************************************/
static const uint8_t TCV_DEVADDR_A0 = 0x50;
static const uint8_t TCV_IDENTIFIER = 0x00;

int tcv_init(tcv_t *tcv)
{
	uint8_t identifier;
	int ret = 0;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	ret = tcv->read(tcv->index, TCV_DEVADDR_A0, TCV_IDENTIFIER, &identifier, 1);
	if (ret < 0) {
		tcv_unlock(tcv);
		return ret;
	}

	/* if someone calls init on a transceiver with alloc'ed data - clear it first */
	if (tcv->data) {
		free(tcv->data);
		tcv->data = NULL;
	}

	switch (identifier) {
		case TCV_TYPE_SFP:
			ret = sfp_init(tcv);
			break;

//		TODO
//		case TCV_TYPE_XFP:
//			((sfp_data_t*)tcv->data) = malloc(sizeof(xfp_data_t));
//			((xfp_data_t*)((sfp_data_t*)tcv->data))->type = TCV_TYPE_XFP;
//			break;

		default:
			return TCV_ERR_GENERIC;
	}
	if (tcv_unlock(tcv))
		return TCV_ERR_GENERIC;

	return ret;
}

/******************************************************************************/

int tcv_destroy(tcv_t *tcv)
{
	int ret = 0;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv->data) {
		free(tcv->data);
		tcv->data = NULL;
		tcv->created = false;
	}
	tcv_unlock(tcv);
	pthread_mutex_destroy(&tcv->lock);
	free(tcv);

	return ret;
}

/******************************************************************************/

int tcv_get_identifier(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_identifier(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_ext_identifier(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_ext_identifier(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_connector(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_connector(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_10g_compliance_codes(tcv_t *tcv,
                                 tcv_10g_eth_compliance_codes_t *codes)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !codes)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_10g_compliance_codes(tcv, codes);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_infiniband_compliance_codes(
        tcv_t *tcv, tcv_infiniband_compliance_codes_t *codes)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !codes)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_infiniband_compliance_codes(tcv, codes);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_escon_compliance_codes(tcv_t *tcv,
                                   tcv_escon_compliance_codes_t *codes)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !codes)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_escon_compliance_codes(tcv, codes);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_sonet_compliance_codes(tcv_t *tcv,
                                   tcv_sonet_compliance_codes_t *codes)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !codes)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_sonet_compliance_codes(tcv, codes);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_sonet_compliances(tcv_t *tcv, tcv_sonet_compliances_t *compliances)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n",
	        __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_eth_compliance_codes(tcv_t *tcv, tcv_eth_compliance_codes_t *codes)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !codes)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_eth_compliance_codes(tcv, codes);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_fibre_channel_link_length(tcv_t *tcv,
                                      tcv_fibre_channel_link_length_t *lengths)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !lengths)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_fibre_channel_link_length(tcv, lengths);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_fibre_channel_technology(tcv_t *tcv,
                                     tcv_fibre_channel_technology_t *technology)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n",
	        __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_sfp_plus_cable_technology(tcv_t *tcv,
                                      sfp_plus_cable_technology_t *technology)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !technology)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_sfp_plus_cable_technology(tcv, technology);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_fibre_channel_media(tcv_t *tcv, tcv_fibre_channel_media_t *media)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !media)
		return TCV_ERR_INVALID_ARG;

	ret = tcv_is_initialized(tcv);
	if (ret) {
		ret = tcv->fun->get_fibre_channel_media(tcv, media);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_fibre_channel_speed(tcv_t *tcv, fibre_channel_speed_t *speed)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !speed)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_fibre_channel_speed(tcv, speed);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_encoding(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	ret = tcv_is_initialized(tcv);
	if (ret) {
		ret = tcv->fun->get_encoding(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_nominal_bit_rate(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	ret = tcv_is_initialized(tcv);
	if (ret) {
		ret = tcv->fun->get_nominal_bit_rate(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_rate_identifier(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_rate_identifier(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_sm_length(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_sm_length(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_om2_length(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_om2_length(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_om1_length(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_om1_length(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_om4_copper_length(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_om4_copper_length(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_om3_length(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_om3_length(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_vendor_name(tcv_t *tcv, char* vendor_name)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !vendor_name)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_vendor_name(tcv, vendor_name);
	}
	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_vendor_oui(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_vendor_oui(tcv);
	}
	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_vendor_revision(tcv_t *tcv, char* rev)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !rev)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_vendor_revision(tcv, rev);
	}
	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_vendor_part_number(tcv_t *tcv, char* pn)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !pn)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_vendor_part_number(tcv, pn);
	}
	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_wavelength(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_wave_len(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_passive_cable_compliance(tcv_t *tcv,
                                     passive_cable_compliance_t *compliance)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n",
	        __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_active_cable_compliance(tcv_t *tcv,
                                    active_cable_compliance_t *compliance)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n",
	        __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_cc_base(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n",
	        __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_calculate_cc_base(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n",
	        __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_implemented_options(tcv_t *tcv, tcv_implemented_options_t *options)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !options)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_implemented_options(tcv, options);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_max_bit_rate(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_max_bit_rate(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_min_bit_rate(tcv_t *tcv)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv))
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_min_bit_rate(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_vendor_sn(tcv_t *tcv, char* vendor_sn)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !vendor_sn)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_vendor_serial_number(tcv, vendor_sn);
	}
	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_vendor_date_code(tcv_t *tcv, tcv_date_code_t *date_code)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !date_code)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_vendor_date_code(tcv, date_code);
	}
	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_diagnostic_type(tcv_t *tcv, tcv_diagnostic_type_t *diag_type)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !diag_type)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_diagnostic_type(tcv, diag_type);
	}
	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/
int tcv_get_enhanced_options(tcv_t *tcv, tcv_enhanced_options_type_t *options)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !options)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_enhanced_options(tcv, options);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_get_cc_ext(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n",
	        __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_calculate_cc_ext(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n",
	        __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

const uint8_t* tcv_get_vendor_rom(tcv_t *tcv)
{
	uint8_t* ret = NULL;

	if (!tcv_check_and_lock_ok(tcv))
		return NULL ;

	if (tcv_is_initialized(tcv)) {
		/*cast away const to supress warning, we add const through return */
		ret = (uint8_t*)tcv->fun->get_vendor_rom(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

size_t tcv_get_vendor_rom_size(tcv_t *tcv)
{
	int ret = 0;

	if (!tcv_check_and_lock_ok(tcv))
		return 0;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->get_vendor_rom_size(tcv);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

const uint8_t* tcv_get_8079_rom(tcv_t *tcv)
{
	uint8_t* ret = NULL;

	if (!tcv_check_and_lock_ok(tcv))
		return NULL ;

	if (tcv_is_initialized(tcv)) {
		/*cast away const to supress warning, we add const through return */
		ret = (uint8_t*) tcv->fun->get_8079_rom(tcv);
	}
	tcv_unlock(tcv);
	return ret;

}

/******************************************************************************/

int tcv_read(tcv_t *tcv, uint8_t devaddr, uint8_t regaddr, uint8_t* data, size_t len)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !data)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->raw_read(tcv, devaddr, regaddr, data, len);
	}

	tcv_unlock(tcv);
	return ret;
}

/******************************************************************************/

int tcv_write(tcv_t *tcv, uint8_t devaddr, uint8_t regaddr, uint8_t* data, size_t len)
{
	int ret = TCV_ERR_NOT_INITIALIZED;

	if (!tcv_check_and_lock_ok(tcv) || !data)
		return TCV_ERR_INVALID_ARG;

	if (tcv_is_initialized(tcv)) {
		ret = tcv->fun->raw_write(tcv, devaddr, regaddr, data, len);
	}

	tcv_unlock(tcv);
	return ret;
}

