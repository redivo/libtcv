/*
 * NOTE
 * This file is based on SFF-8472 rev11.3.
 * It's an MSA translation.
 */

#ifndef __LIBTCV_SFP_H__
#define __LIBTCV_SFP_H__

#include "tcv.h"

/******************************************************************************/
/**
 * \brief	SFP structure.
 *
 * It's an SFP representation, it's valid for both SFP and SFP+.
 */
typedef struct {
	uint8_t type;	//! Transceiver type
	uint8_t a0[256];	//! Internal device 0xA0 (Basic info)
	uint8_t a2[256];	//! Internal device 0xA2 (Digital Diagnostic)
	uint8_t ac[256];	//! Internal device 0xAc (Internal PHY)
} sfp_data_t;

/******************************************************************************/
/**
 * \brief	Inform the transceiver type identifier.
 * \param	tcv	Pointer to transceiver structure
 * \return	Transceiver type if ok; code error otherwise
 */
int sfp_get_itendifier(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the extended transceiver type identifier.
 * \param	tcv	Pointer to transceiver structure
 * \return	Extended transceiver type if ok; code error otherwise
 */
int sfp_get_ext_itendifier(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the transceiver's connector
 * \param	tcv	Pointer to transceiver structure
 * \return	Transceiver's connector if ok; code error otherwise
 */
int sfp_get_connector(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the 10G Ethernet compliance codes
 * \param	tcv		Pointer to transceiver structure
 * \param	codes	Pointer to variable to be filled with the 10G compliance codes bitmap
 * \return	0 if OK, error code otherwise
 */
int sfp_get_10g_compliance_codes(tcv_t *tcv, tcv_10g_eth_compliance_codes_t *codes);

/******************************************************************************/
/**
 * \brief	Inform the Infiniband compliance codes
 * \param	tcv		Pointer to transceiver structure
 * \param	codes	Pointer to variable to be filled with the infiniband compliance codes bitmap
 * \return	0 if OK, error code otherwise
 */
int sfp_get_infiniband_compliance_codes(tcv_t *tcv, tcv_infiniband_compliance_codes_t *codes);

/******************************************************************************/
/**
 * \brief	Inform ESCON compliance codes
 * \param	tcv		Pointer to transceiver structure
 * \param	codes	Pointer to variable to be filled with the infiniband compliance codes bitmap
 * \return	0 if OK, error code otherwise
 */
int sfp_get_escon_compliance_codes(tcv_t *tcv, tcv_escon_compliance_codes_t *codes);

/******************************************************************************/
/**
 * \brief	Inform SONET compliance codes
 * \param	tcv		Pointer to transceiver structure
 * \param	codes	Pointer to variable to be filled with the SONET compliance codes bitmap
 * \return	0 if OK, error code otherwise
 */
int sfp_get_sonet_compliance_codes(tcv_t *tcv, tcv_sonet_compliance_codes_t *codes);

/******************************************************************************/
/**
 * \brief	Inform SONET compliance
 * \param	tcv			Pointer to transceiver structure
 * \param	compliances	Pointer to variable to be filled with the SONET compliances
 * \return	0 if OK, error code otherwise
 */
int sfp_get_sonet_compliances(tcv_t *tcv, tcv_sonet_compliances_t *compliances);

/******************************************************************************/
/**
 * \brief	Inform Ethernet compliance code
 * \param	tcv		Pointer to transceiver structure
 * \param	codes	Pointer to variable to be filled with the Ethernet compliance code
 * \return	0 if OK, error code otherwise
 */
int sfp_get_eth_compliance_codes(tcv_t *tcv, tcv_eth_compliance_codes_t *codes);

/******************************************************************************/
/**
 * \brief	Inform fibre channel link length
 * \param	tcv		Pointer to transceiver structure
 * \param	codes	Pointer to variable to be filled with the link lengths
 * \return	0 if OK, error code otherwise
 */
int sfp_get_fibre_channel_link_length(tcv_t *tcv, tcv_fibre_channel_link_length_t *lengths);

/******************************************************************************/
/**
 * \brief	Inform fibre channel technology
 * \param	tcv			Pointer to transceiver structure
 * \param	technology	Pointer to variable to be filled with the fibre channel technology
 * \return	0 if OK, error code otherwise
 */
int sfp_get_fibre_channel_technology(tcv_t *tcv, tcv_fibre_channel_technology_t *technology);

/******************************************************************************/
/**
 * \brief	Inform SFP+' cable technology
 * \param	tcv			Pointer to transceiver structure
 * \param	technology	Pointer to variable to be filled with the cable technology
 * \return	0 if OK, error code otherwise
 */
int sfp_get_sfp_plus_cable_technology(tcv_t *tcv, sfp_plus_cable_technology_t *technology);

/******************************************************************************/
/**
 * \brief	Inform the fibre channel transition media
 * \param	tcv		Pointer to transceiver structure
 * \param	media	Pointer to variable to be filled with media
 * \return	0 if OK, error code otherwise
 */
int sfp_get_fibre_channel_media(tcv_t *tcv, tcv_fibre_channel_media_t *media);

/******************************************************************************/
/**
 * \brief	Inform the fibre channel speed bitmap
 * \param	tcv		Pointer to transceiver structure
 * \param	speed	Pointer to variable to be filled with speed bitmap
 * \return	0 if OK, error code otherwise
 */
int sfp_get_fibre_channel_speed(tcv_t *tcv, fibre_channel_speed_t *speed);

/******************************************************************************/
/**
 * \brief	Inform the extended transceiver serial encoding
 * \param	tcv	Pointer to transceiver structure
 * \return	Transceiver encoding if ok; code error otherwise
 */
int sfp_get_encoding(tcv_t *tcv);

/******************************************************************************/

/**
 * \brief	Inform the nominal bit rate in units of MBytes
 * \param	tcv	Pointer to transceiver structure
 * \return	The nominal bit rate if ok; code error otherwise
 */
int sfp_get_nominal_bit_rate(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the rate identifier
 * \param	tcv	Pointer to transceiver structure
 * \return	Rate identifier if ok; code error otherwise
 */
int sfp_get_rate_identifier(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the single mode supported length in units of meters.
 * \param	tcv	Pointer to transceiver structure
 * \return	SM length if ok; code error otherwise. If the length is greater
 *		than 254000m, return TCV_SM_LENGTH_GREATER_THAN_254000M
 */
int sfp_get_sm_length(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the OM2 supported length in units of meters.
 * \param	tcv	Pointer to transceiver structure
 * \return	OM2 length if ok; code error otherwise. If the length is greater
 *		than 2540m, return TCV_OM2_LENGTH_GREATER_THAN_2540M
 */
int sfp_get_om2_length(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the OM1 supported length in units of meters.
 * \param	tcv	Pointer to transceiver structure
 * \return	OM1 length if ok; code error otherwise. If the length is greater than 2540m, return TCV_OM1_LENGTH_GREATER_THAN_2540M
 */
int sfp_get_om1_length(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the OM4 supported length or the copper supported length, if it's a copper SFP, in units of meters.
 * \param	tcv	Pointer to transceiver structure
 * \return	OM4 length if ok; code error otherwise. If the length is greater than 2540m, return TCV_OM4_LENGTH_GREATER_THAN_2540M
 */
int sfp_get_om4_length_copper_length(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the OM3 supported length in units of meters.
 * \param	tcv	Pointer to transceiver structure
 * \return	OM3 length if ok; code error otherwise. If the length is greater than 2540m, return TCV_OM3_LENGTH_GREATER_THAN_2540M
 */
int sfp_get_om3_length(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the SFP's vendor name.
 * \param	tcv			Pointer to transceiver structure
 * \param	vendor_name	Pointer to char that will be filled with the vendor name. It must be already allocated.
 * \return	0 if ok; code error otherwise.
 */
int sfp_get_vendor_name(tcv_t *tcv, char vendor_name[TCV_VENDOR_NAME_SIZE + 1]);

/******************************************************************************/
/**
 * \brief	Inform the SFP's vendor OUI.
 * \param	tcv			Pointer to transceiver structure
 * \return	Vendor OUI if ok; code error otherwise.
 */
int sfp_get_vendor_oui(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the vendor part number.
 * \param	tcv			Pointer to transceiver structure
 * \param	part_number	Pointer to be filled with the vendor part number. It must be already allocated.
 * \return	0 if ok; code error otherwise.
 */
int sfp_get_vendor_part_number(tcv_t *tcv, char pn[TCV_VENDOR_PN_SIZE + 1]);

/******************************************************************************/
/**
 * \brief	Inform the vendor revision.
 * \param	tcv	Pointer to transceiver structure
 * \param	rev	Pointer to be filled with the vendor revision. It must be already allocated.
 * \return	0 if ok; code error otherwise.
 */
int sfp_get_vendor_revision(tcv_t *tcv, char rev[TCV_VENDOR_REV_SIZE + 1]);

/******************************************************************************/
/**
 * \brief	Inform the wavelength in nanometers.
 * \param	tcv	Pointer to transceiver structure
 * \return	Wavelength if ok; code error otherwise.
 */
int sfp_get_wavelength(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the passive cable compliance bitmap
 * \param	tcv			Pointer to transceiver structure
 * \param	compliance	Pointer to variable to be filled with compliance bitmap
 * \return	0 if OK, error code otherwise
 */
int sfp_get_passive_cable_compliance(tcv_t *tcv, passive_cable_compliance_t *compliance);

/******************************************************************************/
/**
 * \brief	Inform the active cable compliance bitmap
 * \param	tcv			Pointer to transceiver structure
 * \param	compliance	Pointer to variable to be filled with compliance bitmap
 * \return	0 if OK, error code otherwise
 */
int sfp_get_active_cable_compliance(tcv_t *tcv, active_cable_compliance_t *compliance);

/******************************************************************************/
/**
 * \brief	Inform the CC Base read in SFP
 * \param	tcv			Pointer to transceiver structure
 * \return	Read CC Base if OK, error code otherwise
 */
int sfp_get_cc_base(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the calculated CC Base of SFP
 * \param	tcv			Pointer to transceiver structure
 * \return	Calculated CC Base if OK, error code otherwise
 */
int sfp_calculate_cc_base(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the implemented options
 * \param	tcv		Pointer to transceiver structure
 * \param	options	Pointer to variable to be filled with the implemented options
 * \return	0 if OK, error code otherwise
 */
int sfp_get_implemented_options(tcv_t *tcv, tcv_implemented_options_t *options);

/******************************************************************************/
/**
 * \brief	Inform the max bit rate in units of 1% above the nominal rate
 * \param	tcv	Pointer to transceiver structure
 * \return	The max bit rate if ok; code error otherwise
 */
int sfp_get_max_bit_rate(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the min bit rate in units of 1% below the nominal rate
 * \param	tcv	Pointer to transceiver structure
 * \return	The min bit rate if ok; code error otherwise
 */
int sfp_get_min_bit_rate(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the SFP's vendor Serial Number.
 * \param	tcv			Pointer to transceiver structure
 * \param	vendor_sn	Pointer to char that will be filled with the vendor serial number. It must be already allocated.
 * \return	0 if ok; code error otherwise.
 */
int sfp_get_vendor_sn(tcv_t *tcv, char vendor_sn[TCV_VENDOR_SN_SIZE + 1]);

/******************************************************************************/
/**
 * \brief	Inform the SFP's vendor's date code.
 * \param	tcv			Pointer to transceiver structure
 * \param	date_code	Pointer to variable that will be filled with the vendor's date code.
 * \return	0 if ok; code error otherwise.
 */
int sfp_get_vendor_date_code(tcv_t *tcv, tcv_date_code_t *date_code);

/******************************************************************************/
/**
 * \brief	Inform the SFP's diagnostics monitoring type
 * \param	tcv			Pointer to transceiver structure
 * \param	diag_type	Pointer to variable that will be filled with the SFP's monitoring type.
 * \return	0 if ok; code error otherwise.
 */
int sfp_get_diagnostic_type(tcv_t *tcv, tcv_diagnostic_type_t *diag_type);

/******************************************************************************/
/**
 * \brief	Inform the SFP's diagnostics monitoring type
 * \param	tcv		Pointer to transceiver structure
 * \param	options	Pointer to variable that will be filled with the SFP's monitoring type.
 * \return	0 if ok; code error otherwise.
 */
int sfp_get_enhance_options(tcv_t *tcv, tcv_enhanced_options_type_t *options);

/******************************************************************************/
/**
 * \brief	Inform the CC Extended read in SFP
 * \param	tcv			Pointer to transceiver structure
 * \return	Read CC Extended if OK, error code otherwise
 */
int sfp_get_cc_ext(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the calculated CC Extended of SFP
 * \param	tcv			Pointer to transceiver structure
 * \return	Calculated CC Extended if OK, error code otherwise
 */
int sfp_calculate_cc_ext(tcv_t *tcv);

/******************************************************************************/

/**
 * SFP member function mapping
 */
extern const struct tcv_functions sfp_funcs;

#endif /* __LIBTCV_SFP_H__ */
