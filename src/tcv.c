/*
 * NOTE
 * This file is based on SFF-8472 rev11.3 and INF-8077i rev4.5.
 * It's an MSA translation.
 */

#include <stdlib.h>
#include <stdio.h>
#include "libtcv/tcv.h"
#include "libtcv/sfp.h"
#include "libtcv/xfp.h"

/******************************************************************************/
#define TCV_DEVADDR_A0	0x50
#define TCV_IDENTIFIER	0x00

int tcv_init(tcv_t *tcv, int index, i2c_read_cb_t read, i2c_write_cb_t write)
{
	uint8_t identifier;
	int ret;

	/* Check parameters */
	if (tcv == NULL || read == NULL || write == NULL)
		return TCV_ERR_INVALID_ARG;

	tcv->index = index;
	tcv->read = read;
	tcv->write = write;

	ret = tcv->read(index, TCV_DEVADDR_A0, TCV_IDENTIFIER, &identifier, 1);
	if (ret < 0)
		return ret;

	switch (identifier) {
		case TCV_TYPE_SFP:
			tcv->data = malloc(sizeof(sfp_data_t));
			((sfp_data_t*)tcv->data)->type = TCV_TYPE_SFP;
			tcv->fun = &sfp_funcs;
			break;

//		TODO
//		case TCV_TYPE_XFP:
//			((sfp_data_t*)tcv->data) = malloc(sizeof(xfp_data_t));
//			((xfp_data_t*)((sfp_data_t*)tcv->data))->type = TCV_TYPE_XFP;
//			break;

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_identifier(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_ext_itendifier(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_connector(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_10g_compliance_codes(tcv_t *tcv, tcv_10g_eth_compliance_codes_t *codes)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_infiniband_compliance_codes(tcv_t *tcv, tcv_infiniband_compliance_codes_t *codes)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_escon_compliance_codes(tcv_t *tcv, tcv_escon_compliance_codes_t *codes)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_sonet_compliance_codes(tcv_t *tcv, tcv_sonet_compliance_codes_t *codes)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_sonet_compliances(tcv_t *tcv, tcv_sonet_compliances_t *compliances)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_eth_compliance_codes(tcv_t *tcv, tcv_eth_compliance_codes_t *codes)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_fibre_channel_link_length(tcv_t *tcv, tcv_fibre_channel_link_length_t *lengths)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_fibre_channel_technology(tcv_t *tcv, tcv_fibre_channel_technology_t *technology)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_sfp_plus_cable_technology(tcv_t *tcv, sfp_plus_cable_technology_t *technology)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_fibre_channel_media(tcv_t *tcv, tcv_fibre_channel_media_t *media)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_fibre_channel_speed(tcv_t *tcv, fibre_channel_speed_t *speed)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_encoding(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_nominal_bit_rate(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_rate_identifier(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_sm_length(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_om2_length(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_om1_length(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_om4_length_copper_length(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_om3_length(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_vendor_name(tcv_t *tcv, char vendor_name[TCV_VENDOR_NAME_SIZE + 1])
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_vendor_oui(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_vendor_revision(tcv_t *tcv, char pn[TCV_VENDOR_PN_SIZE + 1])
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_vendor_part_number(tcv_t *tcv, char rev[TCV_VENDOR_REV_SIZE + 1])
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_wavelength(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_passive_cable_compliance(tcv_t *tcv, passive_cable_compliance_t *compliance)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_active_cable_compliance(tcv_t *tcv, active_cable_compliance_t *compliance)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_cc_base(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_calculate_cc_base(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_implemented_options(tcv_t *tcv, tcv_implemented_options_t *options)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_max_bit_rate(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_min_bit_rate(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_vendor_sn(tcv_t *tcv, char vendor_sn[TCV_VENDOR_SN_SIZE + 1])
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_vendor_date_code(tcv_t *tcv, tcv_date_code_t *date_code)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_diagnostic_type(tcv_t *tcv, tcv_diagnostic_type_t *diag_type)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_enhance_options(tcv_t *tcv, tcv_enhanced_options_type_t *options)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_get_cc_ext(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/

int tcv_calculate_cc_ext(tcv_t *tcv)
{
	fprintf(stderr, "> %s - Error: generic function not implemented\n", __func__);
	return TCV_ERR_FEATURE_NOT_AVAILABLE;
}

/******************************************************************************/
