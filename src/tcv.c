/*
 * NOTE
 * This file is based on SFF-8472 rev11.3 and INF-8077i rev4.5.
 * It's an MSA translation.
 */

#include <stdlib.h>
#include "tcv.h"
#include "sfp.h"

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

int tcv_get_itendifier(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_itendifier(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_itendifier(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_ext_itendifier(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_ext_itendifier(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_ext_itendifier(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_connector(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_connector(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_connector(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_10g_complience_codes(tcv_t *tcv, tcv_10g_eth_compliance_codes_t *codes)
{
	if (tcv == NULL || codes == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_10g_complience_codes(tcv, codes);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_10g_complience_codes(tcv, codes);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_infiniband_complience_codes(tcv_t *tcv, tcv_infiniband_compliance_codes_t *codes)
{
	if (tcv == NULL || codes == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_infiniband_complience_codes(tcv, codes);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_infiniband_complience_codes(tcv, codes);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_escon_complience_codes(tcv_t *tcv, tcv_escon_compliance_codes_t *codes)
{
	if (tcv == NULL || codes == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_escon_complience_codes(tcv, codes);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_escon_complience_codes(tcv, codes);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_sonet_complience_codes(tcv_t *tcv, tcv_sonet_compliance_codes_t *codes)
{
	if (tcv == NULL || codes == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_sonet_complience_codes(tcv, codes);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_sonet_complience_codes(tcv, codes);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_sonet_compliences(tcv_t *tcv, tcv_sonet_compliances_t *compliances)
{
	if (tcv == NULL || compliances == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_sonet_compliences(tcv, compliances);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_sonet_compliences(tcv, compliances);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_eth_complience_codes(tcv_t *tcv, tcv_eth_compliance_codes_t *codes)
{
	if (tcv == NULL || codes == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_eth_complience_codes(tcv, codes);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_eth_complience_codes(tcv, codes);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_fibre_channel_link_length(tcv_t *tcv, tcv_fibre_channel_link_length_t *lengths)
{
	if (tcv == NULL || lengths == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_fibre_channel_link_length(tcv, lengths);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_fibre_channel_link_length(tcv, lengths);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_fibre_channel_technology(tcv_t *tcv, tcv_fibre_channel_technology_t *technology)
{
	if (tcv == NULL || technology == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_fibre_channel_technology(tcv, technology);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_fibre_channel_technology(tcv, technology);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_sfp_plus_cable_technology(tcv_t *tcv, sfp_plus_cable_technology_t *technology)
{
	if (tcv == NULL || technology == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_sfp_plus_cable_technology(tcv, technology);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_sfp_plus_cable_technology(tcv, technology);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_fibre_channel_media(tcv_t *tcv, tcv_fibre_channel_media_t *media)
{
	if (tcv == NULL || media == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_fibre_channel_media(tcv, media);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_fibre_channel_media(tcv, media);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_fibre_channel_speed(tcv_t *tcv, fibre_channel_speed_t *speed)
{
	if (tcv == NULL || speed == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_fibre_channel_speed(tcv, speed);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_fibre_channel_speed(tcv, speed);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_encoding(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_encoding(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_encoding(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_nominal_bit_rate(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_nominal_bit_rate(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_nominal_bit_rate(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_rate_identifier(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_rate_identifier(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_rate_identifier(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_sm_length(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_sm_length(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_sm_length(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_om2_length(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_om2_length(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_om2_length(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_om1_length(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_om1_length(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_om1_length(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_om4_length_copper_length(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_om4_length_copper_length(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_om4_length_copper_length(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_om3_length(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_om3_length(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_om3_length(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_vendor_name(tcv_t *tcv, char vendor_name[TCV_VENDOR_NAME_SIZE + 1])
{
	if (tcv == NULL || vendor_name == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_vendor_name(tcv, vendor_name);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_vendor_name(tcv, vendor_name);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_vendor_oui(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_vendor_oui(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_vendor_oui(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_vendor_revision(tcv_t *tcv, char pn[TCV_VENDOR_PN_SIZE + 1])
{
	if (tcv == NULL || pn == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_vendor_revision(tcv, pn);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_vendor_revision(tcv, pn);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_vendor_part_number(tcv_t *tcv, char rev[TCV_VENDOR_REV_SIZE + 1])
{
	if (tcv == NULL || rev == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_vendor_part_number(tcv, rev[TCV_VENDOR_REV_SIZE + 1]);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_vendor_part_number(tcv, rev[TCV_VENDOR_REV_SIZE + 1]);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_wavelength(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_wavelength(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_wavelength(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_passive_cable_compliance(tcv_t *tcv, passive_cable_compliance_t *compliance)
{
	if (tcv == NULL || compliance == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_passive_cable_compliance(tcv, compliance);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_passive_cable_compliance(tcv, compliance);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_active_cable_compliance(tcv_t *tcv, active_cable_compliance_t *compliance)
{
	if (tcv == NULL || compliance == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_active_cable_compliance(tcv, compliance);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_active_cable_compliance(tcv, compliance);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_cc_base(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_cc_base(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_cc_base(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_calculate_cc_base(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_calculate_cc_base(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_calculate_cc_base(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_implemented_options(tcv_t *tcv, tcv_implemented_options_t *options)
{
	if (tcv == NULL || options == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_implemented_options(tcv, options);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_implemented_options(tcv, options);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_max_bit_rate(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_max_bit_rate(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_max_bit_rate(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_min_bit_rate(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_min_bit_rate(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_min_bit_rate(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_vendor_sn(tcv_t *tcv, char vendor_sn[TCV_VENDOR_SN_SIZE + 1])
{
	if (tcv == NULL || vendor_sn == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_vendor_sn(tcv, vendor_sn);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_vendor_sn(tcv, vendor_sn);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_vendor_date_code(tcv_t *tcv, tcv_date_code_t *date_code)
{
	if (tcv == NULL || date_code == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_vendor_date_code(tcv, date_code);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_vendor_date_code(tcv, date_code);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_diagnostic_type(tcv_t *tcv, tcv_diagnostic_type_t *diag_type)
{
	if (tcv == NULL || diag_type == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_diagnostic_type(tcv, diag_type);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_diagnostic_type(tcv, diag_type);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_enhance_options(tcv_t *tcv, tcv_enhanced_options_type_t *options)
{
	if (tcv == NULL || options == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_enhance_options(tcv, options);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_enhance_options(tcv, options);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_get_cc_ext(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_get_cc_ext(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_get_cc_ext(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

int tcv_calculate_cc_ext(tcv_t *tcv)
{
	if (tcv == NULL)
		return TCV_ERR_INVALID_ARG;

	switch (((sfp_data_t*)tcv->data)->type) {
		case TCV_TYPE_SFP:
			return sfp_calculate_cc_ext(tcv);

//		TODO
//		case TCV_TYPE_XFP:
//			return xfp_calculate_cc_ext(tcv);

		default:
			return TCV_ERR_GENERIC;
	}

	return 0;
}

/******************************************************************************/

