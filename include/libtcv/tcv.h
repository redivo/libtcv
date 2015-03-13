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
 * Public Header to include in Client programs
 *
 * NOTE
 * This file is based on SFF-8472 rev11.3 and INF-8077i rev4.5.
 * It's an MSA translation.
 */

#ifndef __LIBTCV_TCV_H__
#define __LIBTCV_TCV_H__

#include <stdint.h>
#include <stddef.h>
#include <endian.h>
#ifdef __cplusplus
extern "C"{
#endif

/******************************************************************************/
/* Transceiver identifier types */
#define TCV_TYPE_UNKNOWN			0x00
#define TCV_TYPE_GBIC				0x01
#define TCV_TYPE_SOLDERED_ON_MOBO	0x02
#define TCV_TYPE_SFP				0x03
#define TCV_TYPE_300_PIN_XBI		0x04
#define TCV_TYPE_XENPAK				0x05
#define TCV_TYPE_XFP				0x06
#define TCV_TYPE_XFF				0x07
#define TCV_TYPE_XFP_E				0x08
#define TCV_TYPE_XPAK				0x09
#define TCV_TYPE_X2					0x0A
#define TCV_TYPE_DWDM_SFP			0x0B
#define TCV_TYPE_QSFP				0x0C

/******************************************************************************/
/* Error defines */
#define TCV_ERR_GENERIC							-1
#define TCV_ERR_INVALID_ARG						-4
#define TCV_ERR_BASIC_INFO_NOT_PRESENT			-5
#define TCV_ERR_DIAGNOSTICS_INFO_NOT_PRESENT	-6
#define TCV_ERR_SM_LENGTH_NOT_DEFINED			-7
#define TCV_ERR_OM2_LENGTH_NOT_DEFINED			-8
#define TCV_ERR_OM1_LENGTH_NOT_DEFINED			-9
#define TCV_ERR_OM4_LENGTH_NOT_DEFINED			-10
#define TCV_ERR_OM3_LENGTH_NOT_DEFINED			-11
#define TCV_ERR_FEATURE_NOT_AVAILABLE			-12
/* Function called on not initialized handle */
#define TCV_ERR_NOT_INITIALIZED					-13

/**
 * transceiver reference in client code
 * Must be allocated by tcv_create() and deallocated with tcv_destroy()
 * A typical calling sequence would be:
 * tcv_t* t = tcv_create(...)
 * tcv_init(t)
 * tcv_get_identifier(t)
 * tcv_destroy(t)
 */
typedef struct tcv_t tcv_t;


/******************************************************************************/
/* I2C read and write callback typedefs */

/**
 * \brief	I2C read callback.
 * \param	index	Port index to be read.
 * \param	devaddr	Device address to be read.
 * \param	regaddr	First register address to be read.
 * \param	data	(out) register content read
 * \param	size	Size in bytes to be read.
 * \return	0 if ok, error code otherwise.
 */
typedef int (*i2c_read_cb_t)(int, uint8_t, uint8_t, uint8_t*, size_t);

/**
 * \brief	I2C write callback.
 * \param	index	Port index to be written.
 * \param	devaddr	Device address to be written.
 * \param	regaddr	First register address to be written.
 * \param	data	(in) data to write
 * \param	size	Size in bytes to be written.
 * \return	0 if ok, error code otherwise.
 */
typedef int (*i2c_write_cb_t)(int, uint8_t, uint8_t, const uint8_t*, size_t);

/******************************************************************************/

/**
 * \brief Create a Transceiver handle, allocates struct tcv_t
 * 		  initalizes mutex and calls tcv_init()
 * 		  return value must be deallocated with tcv_destroy()
 * @param index - identifier passed to read() and write()
 * @param read function to read data from transceiver
 * @param write function to write to transceiver
 * @return allocated tcv_t or NULL
 */
tcv_t* tcv_create(int index, i2c_read_cb_t read, i2c_write_cb_t write);

/******************************************************************************/
/**
 * \brief	Transceiver structure initialization.
 *          Called directly by client
 * \param	tcv		Pointer to TCV's structure to be initialized.
 * \return	0 if ok, error code otherwise.
 */
int tcv_init(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Deallocate resources for given transceiver
 * 			May block
 * \param	tcv		Pointer to TCV's structure to be destroyed
 * \return	0 if ok, error code otherwise.
 */
int tcv_destroy(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Inform the transceiver type identifier.
 * \param	tcv	Pointer to transceiver structure
 * \return	Transceiver type if ok; code error otherwise
 */
int tcv_get_identifier(tcv_t *tcv);

/******************************************************************************/

/* Table 3.3 */
#define TCV_EXT_TYPE_UNKNOWN				0x00
#define TCV_EXT_TYPE_GBIC_MOD_DEF_1			0x01
#define TCV_EXT_TYPE_GBIC_MOD_DEF_2			0x02
#define TCV_EXT_TYPE_GBIC_MOD_DEF_3			0x03
#define TCV_EXT_TYPE_GBIC_SFP_2WIRE_ONLY	0x04
#define TCV_EXT_TYPE_GBIC_MOD_DEF_5			0x05
#define TCV_EXT_TYPE_GBIC_MOD_DEF_6			0x06
#define TCV_EXT_TYPE_GBIC_MOD_DEF_7			0x07

/**
 * \brief	Inform the extended transceiver type identifier.
 * \param	tcv	Pointer to transceiver structure
 * \return	Extended transceiver type if ok; code error otherwise
 */
int tcv_get_ext_identifier(tcv_t *tcv);

/******************************************************************************/

/* Table 3.4 */
#define TCV_CONN_UNKNOWN			0x00
#define TCV_CONN_SC					0x01
#define TCV_CONN_STYLE_1_COPPER		0x02
#define TCV_CONN_STYLE_2_COPPER		0x03
#define TCV_CONN_BNC_TNC			0x04
#define TCV_CONN_COAXIAL_HEADERS	0x05
#define TCV_CONN_FIBER_JACK			0x06
#define TCV_CONN_LC					0x07
#define TCV_CONN_MT_RJ				0x08
#define TCV_CONN_MU					0x09
#define TCV_CONN_SG					0x0A
#define TCV_CONN_OPTICAL_PIGTAIL	0x0B
#define TCV_CONN_MPO_PARALLEL_OPTIC	0x0C
#define TCV_CONN_HSSDC_II			0x20
#define TCV_CONN_COPPER_PIGTAIL	0x21
#define TCV_CONN_RJ45				0x22

/**
 * \brief	Inform the transceiver's connector
 * \param	tcv	Pointer to transceiver structure
 * \return	Transceiver's connector if ok; code error otherwise
 */
int tcv_get_connector(tcv_t *tcv);

/******************************************************************************/
/**
 * \union tcv_10g_eth_compliance_codes_t
 * \brief Maps the transceiver's 10G Ethernet compliance bitmap
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t reserved :4;
		uint8_t eth10g_base_er :1;
		uint8_t eth10g_base_lrm :1;
		uint8_t eth10g_base_lr :1;
		uint8_t eth10g_base_sr :1;
#else
		uint8_t eth10g_base_sr :1;
		uint8_t eth10g_base_lr :1;
		uint8_t eth10g_base_lrm :1;
		uint8_t eth10g_base_er :1;
		uint8_t reserved :4;
#endif
	} bits;
	uint8_t bmp;
} tcv_10g_eth_compliance_codes_t;

/**
 * \brief	Inform the 10G Ethernet compliance codes
 * \param	tcv		Pointer to transceiver structure
 * \param	codes	Pointer to variable to be filled with the 10G compliance codes bitmap
 * \return	0 if OK, error code otherwise
 */
int tcv_get_10g_compliance_codes(tcv_t *tcv,
                                 tcv_10g_eth_compliance_codes_t *codes);

/******************************************************************************/

/**
 * \union tcv_infiniband_compliance_codes_t
 * \brief Maps the transceiver's infiniband compliance bitmap
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t reserved :4;
		uint8_t ib_1x_copper_passive :1;
		uint8_t ib_1x_copper_active :1;
		uint8_t ib_1x_lx :1;
		uint8_t ib_1x_sx :1;
#else
		uint8_t ib_1x_sx :1;
		uint8_t ib_1x_lx :1;
		uint8_t ib_1x_copper_active :1;
		uint8_t ib_1x_copper_passive :1;
		uint8_t reserved :4;
#endif
	} bits;
	uint8_t bmp;
} tcv_infiniband_compliance_codes_t;

/**
 * \brief	Inform the Infiniband compliance codes
 * \param	tcv		Pointer to transceiver structure
 * \param	codes	Pointer to variable to be filled with the infiniband compliance codes bitmap
 * \return	0 if OK, error code otherwise
 */
int tcv_get_infiniband_compliance_codes(
        tcv_t *tcv, tcv_infiniband_compliance_codes_t *codes);

/******************************************************************************/

/**
 * \union tcv_escon_compliance_codes_t
 * \brief Maps the transceiver's ESCON compliance bitmap
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t reserved :6;
		uint8_t smf_1310nm_laser :1;
		uint8_t mmf_1310nm_led :1;
#else
		uint8_t mmf_1310nm_led :1;
		uint8_t smf_1310nm_laser :1;
		uint8_t reserved :6;
#endif
	} bits;
	uint8_t bmp;
} tcv_escon_compliance_codes_t;

/**
 * \brief	Inform ESCON compliance codes
 * \param	tcv		Pointer to transceiver structure
 * \param	codes	Pointer to variable to be filled with the infiniband compliance codes bitmap
 * \return	0 if OK, error code otherwise
 */
int tcv_get_escon_compliance_codes(tcv_t *tcv,
                                   tcv_escon_compliance_codes_t *codes);

/******************************************************************************/

/**
 * \union tcv_sonet_compliance_codes_t
 * \brief Maps the transceiver's SONET compliance bitmap
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint16_t reserved :6;
		uint16_t oc_3_sr :1;
		uint16_t oc_3_sm_ir :1;
		uint16_t oc_3_sm_lr :1;
		uint16_t oc_12_sr :1;
		uint16_t oc_12_sm_ir :1;
		uint16_t oc_12_sm_lr :1;
		uint16_t oc_48_sr :1;
		uint16_t oc_48_ir :1;
		uint16_t oc_48_lr :1;
		uint16_t oc_192_sr :1;
#else
		uint16_t oc_192_sr :1;
		uint16_t oc_48_lr :1;
		uint16_t oc_48_ir :1;
		uint16_t oc_48_sr :1;
		uint16_t oc_12_sm_lr :1;
		uint16_t oc_12_sm_ir :1;
		uint16_t oc_12_sr :1;
		uint16_t oc_3_sm_lr :1;
		uint16_t oc_3_sm_ir :1;
		uint16_t oc_3_sr :1;
		uint16_t reserved :6;
#endif
	} bits;
	uint16_t bmp;
} tcv_sonet_compliance_codes_t;

/**
 * \brief	Inform SONET compliance codes
 * \param	tcv		Pointer to transceiver structure
 * \param	codes	Pointer to variable to be filled with the SONET compliance codes bitmap
 * \return	0 if OK, error code otherwise
 */
int tcv_get_sonet_compliance_codes(tcv_t *tcv,
                                   tcv_sonet_compliance_codes_t *codes);

/******************************************************************************/

/**
 * \union tcv_sonet_compliances_t
 * \brief Maps the transceiver's SONET compliances
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t reserved :1;
		uint8_t lr_3_compliant :1;
		uint8_t lr_2_compliant :1;
		uint8_t lr_1_compliant :1;
		uint8_t ir_2_compliant :1;
		uint8_t ir_1_compliant :1;
		uint8_t sr_1_compliant :1;
		uint8_t sr_compliant :1;
#else
		uint8_t sr_compliant :1;
		uint8_t sr_1_compliant :1;
		uint8_t ir_1_compliant :1;
		uint8_t ir_2_compliant :1;
		uint8_t lr_1_compliant :1;
		uint8_t lr_2_compliant :1;
		uint8_t lr_3_compliant :1;
		uint8_t reserved :1;
#endif
	} bits;
	uint8_t bmp;
} tcv_sonet_compliances_t;

/**
 * \brief	Inform SONET compliance
 * \param	tcv			Pointer to transceiver structure
 * \param	compliances	Pointer to variable to be filled with the SONET compliances
 * \return	0 if OK, error code otherwise
 */
int tcv_get_sonet_compliances(tcv_t *tcv, tcv_sonet_compliances_t *compliances);

/******************************************************************************/

/**
 * \union tcv_eth_compliance_codes_t
 * \brief Maps the transceiver's Ethernet compliance codes
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t eth_base_px :1;
		uint8_t eth_base_bx10 :1;
		uint8_t eth_100_base_fx :1;
		uint8_t eth_100base_lx_lx10 :1;
		uint8_t eth_1000_base_t :1;
		uint8_t eth_1000_base_cx :1;
		uint8_t eth_1000_base_lx :1;
		uint8_t eth_1000_base_sx :1;
#else
		uint8_t eth_1000_base_sx :1;
		uint8_t eth_1000_base_lx :1;
		uint8_t eth_1000_base_cx :1;
		uint8_t eth_1000_base_t :1;
		uint8_t eth_100base_lx_lx10 :1;
		uint8_t eth_100_base_fx :1;
		uint8_t eth_base_bx10 :1;
		uint8_t eth_base_px :1;
#endif
	} bits;
	uint8_t bmp;
} tcv_eth_compliance_codes_t;

/**
 * \brief	Inform Ethernet compliance code
 * \param	tcv		Pointer to transceiver structure
 * \param	codes	Pointer to variable to be filled with the Ethernet compliance code
 * \return	0 if OK, error code otherwise
 */
int tcv_get_eth_compliance_codes(tcv_t *tcv, tcv_eth_compliance_codes_t *codes);

/******************************************************************************/

/**
 * \union tcv_fibre_channel_link_length_t
 * \brief Maps the transceiver's Fibre Channel Link Length bitmap
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t reserved :4;
		uint8_t medium_dist :1;
		uint8_t long_dist :1;
		uint8_t intermediate_dist :1;
		uint8_t short_dist :1;
		uint8_t very_long_dist :1;
#else
		uint8_t very_long_dist :1;
		uint8_t short_dist :1;
		uint8_t intermediate_dist :1;
		uint8_t long_dist :1;
		uint8_t medium_dist :1;
		uint8_t reserved :4;
#endif
	} bits;
	uint8_t bmp;
} tcv_fibre_channel_link_length_t;

/**
 * \brief	Inform fibre channel link length
 * \param	tcv initialized transceiver @see{tcv_init}
 * \param	lengths	(out) link lengths
 * \return	0 if OK, error code otherwise
 */
int tcv_get_fibre_channel_link_length(tcv_t *tcv,
		tcv_fibre_channel_link_length_t *lengths);

/******************************************************************************/

/**
 * \union tcv_fibre_channel_technology_t
 * \brief Maps the transceiver's Fibre Channel technology
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t reserved :1;
		uint8_t ll :1;
		uint8_t sl :1;
		uint8_t sn :1;
		uint8_t el_intra_enclosure :1;
		uint8_t el_intER_enclosure :1;
		uint8_t lc :1;
		uint8_t sa :1;
#else
		uint8_t sa :1;
		uint8_t lc :1;
		uint8_t el_intER_enclosure :1;
		uint8_t el_intra_enclosure :1;
		uint8_t sn :1;
		uint8_t sl :1;
		uint8_t ll :1;
		uint8_t reserved :1;
#endif
	} bits;
	uint8_t bmp;
} tcv_fibre_channel_technology_t;

/**
 * \brief	Inform fibre channel technology
 * \param	tcv			Pointer to transceiver structure
 * \param	technology	Pointer to variable to be filled with the fibre channel technology
 * \return	0 if OK, error code otherwise
 */
int tcv_get_fibre_channel_technology(
		tcv_t *tcv, tcv_fibre_channel_technology_t *technology);

/******************************************************************************/

/**
 * \union sfp_plus_cable_technology_t
 * \brief Maps the SFP+' cable technology
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t reserved :6;
		uint8_t passive_cable :1;
		uint8_t active_cable :1;
#else
		uint8_t active_cable :1;
		uint8_t passive_cable :1;
		uint8_t reserved :6;
#endif
	} bits;
	uint8_t bmp;
} sfp_plus_cable_technology_t;

/**
 * \brief	Inform SFP+' cable technology
 * \param	tcv			Pointer to transceiver structure
 * \param	technology	Pointer to variable to be filled with the cable technology
 * \return	0 if OK, error code otherwise
 */
int tcv_get_sfp_plus_cable_technology(tcv_t *tcv,
                                      sfp_plus_cable_technology_t *technology);

/******************************************************************************/

/**
 * \union tcv_fibre_channel_media_t
 * \brief Maps the fibre channel transmission media
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t twin_axial_pair :1;
		uint8_t twisted_pair :1;
		uint8_t miniature_coax :1;
		uint8_t video_coax :1;
		uint8_t multimode_62_5nm :1;
		uint8_t multimode_50nm :1;
		uint8_t reserved :1;
		uint8_t single_mode :1;
#else
		uint8_t single_mode :1;
		uint8_t reserved :1;
		uint8_t multimode_50nm :1;
		uint8_t multimode_62_5nm :1;
		uint8_t video_coax :1;
		uint8_t miniature_coax :1;
		uint8_t twisted_pair :1;
		uint8_t twin_axial_pair :1;
#endif
	} bits;
	uint8_t bmp;
} tcv_fibre_channel_media_t;

/**
 * \brief	Inform the fibre channel transition media
 * \param	tcv		Pointer to transceiver structure
 * \param	media	Pointer to variable to be filled with media
 * \return	0 if OK, error code otherwise
 */
int tcv_get_fibre_channel_media(tcv_t *tcv, tcv_fibre_channel_media_t *media);

/******************************************************************************/

/**
 * \union fibre_channel_speed_t
 * \brief Maps the fibre channel speed
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t reserved :1;
		uint8_t sp_100_mbytes_s :1;
		uint8_t sp_200_mbytes_s :1;
		uint8_t sp_3200_mbytes_s :1;
		uint8_t sp_400_mbytes_s :1;
		uint8_t sp_1600_mbytes_s :1;
		uint8_t sp_800_mbytes_s :1;
		uint8_t sp_1200_mbytes_s :1;
#else
		uint8_t sp_1200_mbytes_s :1;
		uint8_t sp_800_mbytes_s :1;
		uint8_t sp_1600_mbytes_s :1;
		uint8_t sp_400_mbytes_s :1;
		uint8_t sp_3200_mbytes_s :1;
		uint8_t sp_200_mbytes_s :1;
		uint8_t sp_100_mbytes_s :1;
		uint8_t reserved :1;
#endif
	} bits;
	uint8_t bmp;
} fibre_channel_speed_t;

/**
 * \brief	Inform the fibre channel speed bitmap
 * \param	tcv		Pointer to transceiver structure
 * \param	speed	Pointer to variable to be filled with speed bitmap
 * \return	0 if OK, error code otherwise
 */
int tcv_get_fibre_channel_speed(tcv_t *tcv, fibre_channel_speed_t *speed);

/******************************************************************************/

/* Table 3.6 */
#define TCV_ENCODING_UNSPECIFIED		0x00
#define TCV_ENCODING_8B_10B				0x01
#define TCV_ENCODING_4B_5B				0x02
#define TCV_ENCODING_NRZ				0x03
#define TCV_ENCODING_MANCHESTER			0x04
#define TCV_ENCODING_SONET_SCRAMBLED	0x05
#define TCV_ENCODING_64B_66B			0x06

/**
 * \brief	Inform the extended transceiver serial encoding
 * \param	tcv	Pointer to transceiver structure
 * \return	Transceiver encoding if ok; code error otherwise
 */
int tcv_get_encoding(tcv_t *tcv);

/******************************************************************************/

/**
 * \brief	Inform the nominal bit rate in units of MBytes
 * \param	tcv	Pointer to transceiver structure
 * \return	The nominal bit rate if ok; code error otherwise
 */
int tcv_get_nominal_bit_rate(tcv_t *tcv);

/******************************************************************************/

/* Table 3.6a */
#define TCV_RATE_IDENT_UNSPECIFIED										0x00
#define TCV_RATE_IDENT_SFF_8079_4_2_1G_RATE_SEL_AS0_AS1					0x01
#define TCV_RATE_IDENT_SFF_8431_8_4_2G_RX_RATE_SEL_ONLY					0x02
#define TCV_RATE_IDENT_SFF_8431_8_4_2G_TX_RATE_SEL_ONLY					0x04
#define TCV_RATE_IDENT_SFF_8431_8_4_2G_INDEPENDENT_TX_RX_RATE_SEL		0x06
#define TCV_RATE_IDENT_SFF_FC_PI5_16_8_4G_RX_RATE_SEL_ONLY				0x08
#define TCV_RATE_IDENT_SFF_FC_PI5_16_8_4G_INDEPENDENT_RX_TX_RATE_SEL	0x0A
#define TCV_RATE_IDENT_SFF_FC_PI6_32_16_8_INDEPENDENT_RX_TX_RATE_SEL	0x0C

/**
 * \brief	Inform the rate identifier
 * \param	tcv	Pointer to transceiver structure
 * \return	Rate identifier if ok; code error otherwise
 */
int tcv_get_rate_identifier(tcv_t *tcv);

/******************************************************************************/

#define TCV_SM_LENGTH_GREATER_THAN_254000M	255000

/**
 * \brief	Inform the single mode supported length in units of meters.
 * \param	tcv	Pointer to transceiver structure
 * \return	SM length if ok; code error otherwise. If the length is greater than 254000m, return TCV_SM_LENGTH_GREATER_THAN_254000M
 */
int tcv_get_sm_length(tcv_t *tcv);

/******************************************************************************/

#define TCV_OM2_LENGTH_GREATER_THAN_2540M	2550

/**
 * \brief	Inform the OM2 supported length in units of meters.
 * \param	tcv	Pointer to transceiver structure
 * \return	OM2 length if ok; code error otherwise. If the length is greater than 2540m, return TCV_OM2_LENGTH_GREATER_THAN_2540M
 */
int tcv_get_om2_length(tcv_t *tcv);

/******************************************************************************/

#define TCV_OM1_LENGTH_GREATER_THAN_2540M	2550

/**
 * \brief	Inform the OM1 supported length in units of meters.
 * \param	tcv	Pointer to transceiver structure
 * \return	OM1 length if ok; code error otherwise. If the length is greater than 2540m, return TCV_OM1_LENGTH_GREATER_THAN_2540M
 */
int tcv_get_om1_length(tcv_t *tcv);

/******************************************************************************/

#define TCV_OM4_LENGTH_GREATER_THAN_2540M	2550

/**
 * \brief	Inform the OM4 supported length or the copper supported length, if it's a copper SFP, in units of meters.
 * \param	tcv	Pointer to transceiver structure
 * \return	OM4 length if ok; code error otherwise. If the length is greater than 2540m, return TCV_OM4_LENGTH_GREATER_THAN_2540M
 */
int tcv_get_om4_copper_length(tcv_t *tcv);

/******************************************************************************/

#define TCV_OM3_LENGTH_GREATER_THAN_2540M	2550

/**
 * \brief	Inform the OM3 supported length in units of meters.
 * \param	tcv	Pointer to transceiver structure
 * \return	OM3 length if ok; code error otherwise. If the length is greater than 2540m, return TCV_OM3_LENGTH_GREATER_THAN_2540M
 */
int tcv_get_om3_length(tcv_t *tcv);

/******************************************************************************/

#define TCV_VENDOR_NAME_SIZE 16

/**
 * \brief	Inform the SFP's vendor name.
 * \param	tcv			Pointer to transceiver structure
 * \param	vendor_name	Pointer to char that will be filled with the vendor name. It must be already allocated.
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_vendor_name(tcv_t *tcv, char vendor_name[TCV_VENDOR_NAME_SIZE + 1]);

/******************************************************************************/

/**
 * \brief	Inform the SFP's vendor OUI.
 * \param	tcv			Pointer to transceiver structure
 * \return	Vendor OUI if ok; code error otherwise.
 */
int tcv_get_vendor_oui(tcv_t *tcv);

/******************************************************************************/

#define TCV_VENDOR_PN_SIZE	16

/**
 * \brief	Inform the vendor part number.
 * \param	tcv initialized transceiver @see{tcv_init}
 * \param	pn	(out) char[17] containing part number
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_vendor_part_number(tcv_t *tcv, char pn[TCV_VENDOR_PN_SIZE + 1]);

/******************************************************************************/

#define TCV_VENDOR_REV_SIZE	4

/**
 * \brief	Inform the vendor revision.
 * \param	tcv	Pointer to transceiver structure
 * \param	rev	Pointer to be filled with the vendor revision. It must be already allocated.
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_vendor_revision(tcv_t *tcv, char rev[TCV_VENDOR_REV_SIZE + 1]);

/******************************************************************************/

/**
 * \brief	Inform the wavelength in nanometers.
 * \param	tcv	Pointer to transceiver structure
 * \return	Wavelength if ok; code error otherwise.
 */
int tcv_get_wavelength(tcv_t *tcv);

/******************************************************************************/

/**
 * \union passive_cable_compliance_t
 * \brief Maps the passive cable compliances
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t reserved :6;
		uint8_t sff_8431_apndx_e_compliant :1;
		uint8_t fc_pi_4_apndx_h_compliant :1;
#else
		uint8_t fc_pi_4_apndx_h_compliant :1;
		uint8_t sff_8431_apndx_e_compliant :1;
		uint8_t reserved :6;
#endif
	} bits;
	uint8_t bmp;
} passive_cable_compliance_t;

/**
 * \brief	Inform the passive cable compliance bitmap
 * \param	tcv			Pointer to transceiver structure
 * \param	compliance	Pointer to variable to be filled with compliance bitmap
 * \return	0 if OK, error code otherwise
 */
int tcv_get_passive_cable_compliance(tcv_t *tcv,
                                     passive_cable_compliance_t *compliance);

/******************************************************************************/

/**
 * \union active_cable_compliance_t
 * \brief Maps the active cable compliances
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t reserved :4;
		uint8_t sff_8431_apndx_e_compliant :1;
		uint8_t fc_pi_4_apndx_h_compliant :1;
		uint8_t sff_8431_limiting_compliant :1;
		uint8_t fc_pi_4_limiting_compliant :1;
#else
		uint8_t fc_pi_4_limiting_compliant :1;
		uint8_t sff_8431_limiting_compliant :1;
		uint8_t fc_pi_4_apndx_h_compliant :1;
		uint8_t sff_8431_apndx_e_compliant :1;
		uint8_t reserved :4;
#endif
	} bits;
	uint8_t bmp;
} active_cable_compliance_t;

/**
 * \brief	Inform the active cable compliance bitmap
 * \param	tcv			Pointer to transceiver structure
 * \param	compliance	Pointer to variable to be filled with compliance bitmap
 * \return	0 if OK, error code otherwise
 */
int tcv_get_active_cable_compliance(tcv_t *tcv,
                                    active_cable_compliance_t *compliance);

/******************************************************************************/

/**
 * \brief	Inform the CC Base read in SFP
 * \param	tcv			Pointer to transceiver structure
 * \return	Read CC Base if OK, error code otherwise
 */
int tcv_get_cc_base(tcv_t *tcv);

/******************************************************************************/

/**
 * \brief	Inform the calculated CC Base of SFP
 * \param	tcv			Pointer to transceiver structure
 * \return	Calculated CC Base if OK, error code otherwise
 */
int tcv_calculate_cc_base(tcv_t *tcv);

/******************************************************************************/

/**
 * \union tcv_implemented_options_t
 * \brief Shows the implemented options
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t los :1;
		uint8_t signal_detect :1;
		uint8_t tx_fault :1;
		uint8_t tx_disable :1;
		uint8_t rate_select :1;
		uint8_t linear_receiver_out :1;
		uint8_t power_lever_2 :1;
		uint8_t cooled_laser_transmitted :1;
#else
		uint8_t cooled_laser_transmitted :1;
		uint8_t power_lever_2 :1;
		uint8_t linear_receiver_out :1;
		uint8_t rate_select :1;
		uint8_t tx_disable :1;
		uint8_t tx_fault :1;
		uint8_t signal_detect :1;
		uint8_t los :1;
#endif
	} bits;
	uint8_t bmp;
} tcv_implemented_options_t;

/**
 * \brief	Inform the implemented options
 * \param	tcv		Pointer to transceiver structure
 * \param	options	(out) implemented options
 * \return	0 if OK, error code otherwise
 */
int tcv_get_implemented_options(tcv_t *tcv, tcv_implemented_options_t *options);

/******************************************************************************/

/**
 * \brief	Inform the max bit rate in units of 1% above the nominal rate
 * \param	tcv	Pointer to transceiver structure
 * \return	The max bit rate if ok; code error otherwise
 */
int tcv_get_max_bit_rate(tcv_t *tcv);

/******************************************************************************/

/**
 * \brief	Inform the min bit rate in units of 1% below the nominal rate
 * \param	tcv	Pointer to transceiver structure
 * \return	The min bit rate if ok; code error otherwise
 */
int tcv_get_min_bit_rate(tcv_t *tcv);

/******************************************************************************/

#define TCV_VENDOR_SN_SIZE	16

/**
 * \brief	Inform the SFP's vendor Serial Number.
 * \param	tcv			Pointer to transceiver structure
 * \param	vendor_sn	(out) serial number char array
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_vendor_sn(tcv_t *tcv, char vendor_sn[TCV_VENDOR_SN_SIZE + 1]);

/******************************************************************************/

#define TCV_VENDOR_LOT_CODE_SIZE 2

/**
 * \struct tcv_date_code_t
 * \brief  Maps the vendor's date code
 */
typedef struct {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	char vendor_lot_code[TCV_VENDOR_LOT_CODE_SIZE + 1];
} tcv_date_code_t;

/**
 * \brief	Inform the SFP's vendor's date code.
 * \param	tcv			Pointer to transceiver structure
 * \param	date_code	(out) date code
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_vendor_date_code(tcv_t *tcv, tcv_date_code_t *date_code);

/******************************************************************************/

/**
 * \union tcv_diagnostic_type_t
 * \brief Maps the diagnostic monitoring type
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t unallocated :3;
		uint8_t dd_implemented :1;
		uint8_t internally_calibrated :1;
		uint8_t externally_calibrated :1;
		uint8_t pwr_measurement_type :1;
		uint8_t address_change_required :1;
#else
		uint8_t address_change_required :1;
		uint8_t pwr_measurement_type :1;
		uint8_t externally_calibrated :1;
		uint8_t internally_calibrated :1;
		uint8_t dd_implemented :1;
		uint8_t unallocated :3;
#endif
	} bits;
	uint8_t bmp;
} tcv_diagnostic_type_t;

/**
 * \brief	Inform the SFP's diagnostics monitoring type
 * \param	tcv			Pointer to transceiver structure
 * \param	diag_type	Pointer to variable that will be filled with the SFP's monitoring type.
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_diagnostic_type(tcv_t *tcv, tcv_diagnostic_type_t *diag_type);

/******************************************************************************/

/**
 * \union tcv_enhanced_options_type_t
 * \brief Maps enhanced options
 */
typedef union {
	struct {
#if defined(__BIG_ENDIAN__)
		uint8_t reserved :1;
		uint8_t soft_rate_sel_ctrl_implemented :1;
		uint8_t app_select_ctrl_implemented :1;
		uint8_t soft_rate_sel_ctrl_mon_implemented :1;
		uint8_t soft_rx_los_mon_implemented :1;
		uint8_t soft_tx_fault_mon_implemented :1;
		uint8_t soft_tx_disable_ctrl_mon_implemented :1;
		uint8_t alarm_implemented :1;
#else
		uint8_t alarm_implemented :1;
		uint8_t soft_tx_disable_ctrl_mon_implemented :1;
		uint8_t soft_tx_fault_mon_implemented :1;
		uint8_t soft_rx_los_mon_implemented :1;
		uint8_t soft_rate_sel_ctrl_mon_implemented :1;
		uint8_t app_select_ctrl_implemented :1;
		uint8_t soft_rate_sel_ctrl_implemented :1;
		uint8_t reserved :1;
#endif
	} bits;
	uint8_t bmp;
} tcv_enhanced_options_type_t;

/**
 * \brief	Inform the SFP's diagnostics monitoring type
 * \param	tcv		Pointer to transceiver structure
 * \param	options	Pointer to variable that will be filled with the SFP's monitoring type.
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_enhanced_options(tcv_t *tcv, tcv_enhanced_options_type_t *options);

/******************************************************************************/

/**
 * \brief	Inform the CC Extended read in SFP
 * \param	tcv			Pointer to transceiver structure
 * \return	Read CC Extended if OK, error code otherwise
 */
int tcv_get_cc_ext(tcv_t *tcv);

/******************************************************************************/

/**
 * \brief	Inform the calculated CC Extended of SFP
 * \param	tcv			Pointer to transceiver structure
 * \return	Calculated CC Extended if OK, error code otherwise
 */
int tcv_calculate_cc_ext(tcv_t *tcv);

/******************************************************************************/

/**
 * \brief	Allow direct access to non-specified vendor area of page a0
 * \param	tcv Pointer to transceiver structure
 * \return	memory at byte 96-127
 */
const uint8_t* tcv_get_vendor_rom(tcv_t *tcv);

/******************************************************************************/

/**
 * \brief	Inform the size of vendor specific data
 * \param	tcv Pointer to transceiver structure
 * \return	32 for SFP and XFP, 0 if invalid/non-existent
 */
size_t tcv_get_vendor_rom_size(tcv_t *tcv);

/******************************************************************************/

/**
 * \brief	Allow direct access to user writable eeprom area of page a2
 * \param	tcv Pointer to transceiver structure
 * \return	memory at byte 128-247 of page a2
 */
const uint8_t* tcv_get_user_writable_eeprom(tcv_t *tcv);

/******************************************************************************/

/**
 * \brief	Inform the size of user writable eeprom area of page a2
 * \param	tcv Pointer to transceiver structure
 * \return	size of writable eeprom area of page a2
 */
size_t tcv_get_user_writable_eeprom_size(tcv_t *tcv);

/******************************************************************************/
/**
 * \brief	Allow access to SFF-8079 Application selection area
 *              This function must be obsoleted when SFF-8079 is implemented
 * \param	tcv Pointer to transceiver structure
 * \return	memory at byte 128-255 or NULL if not available
 */
const uint8_t* tcv_get_8079_rom(tcv_t *tcv);

/******************************************************************************/

/**
 * \brief	Read raw data from transceiver
 * 			use to access non-standardized registers and devices
 * \param	tcv	Pointer to transceiver structure
 * @param devaddr device address (0xA0, 0xA2, 0xAC...)
 * @param regaddr register offset0
 * @param data  (out) data read
 * @param len size of data
 * @return length read or errorcode
 */
int tcv_read(tcv_t *tcv, uint8_t devaddr, uint8_t regaddr, uint8_t* data, size_t len);

/**
 * \brief	Write raw data from transceiver
 * 			use to access non-standardized registers and devices
 * \param	tcv	Pointer to transceiver structure
 * @param devaddr device address (0xA0, 0xA2, 0xAC...)
 * @param regaddr register offset0
 * @param data  (in) data write
 * @param len size of data
 * @return length read or errorcode
 */
int tcv_write(tcv_t *tcv, uint8_t devaddr, uint8_t regaddr, const uint8_t* data, size_t len);

/**
 * Get the temperature of the transceiver as 16-Bit signed (8.8 fixed-point) integer
 * @param tcv
 * @param temp (out) current temperature
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_temperature(tcv_t* tcv, int16_t* temp);

/**
 * Transceiver supply voltage as 16-Bit signed (8.8 fixed-point) integer
 * @param tcv initialized transceiver @see{tcv_init}
 * @param vcc (out) current voltage
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_voltage(tcv_t* tcv, uint16_t* vcc);

/**
 * Transmit-Current as 16-Bit signed (8.8 fixed-point) integer
 * \param tcv initialized transceiver @see{tcv_init}
 * \param cur (out) tx-current (0.2 uA)
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_tx_cur(tcv_t* tcv, uint16_t* cur);

/**
 * Receive Power as 16-Bit signed (8.8 fixed-point) integer
 * \param tcv initialized transceiver @see{tcv_init}
 * \param pwr (out) rx-power (0.1 uW)
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_rx_pwr(tcv_t* tcv, uint16_t* pwr);

/**
 * Laser transmit power as 16-Bit signed (8.8 fixed-point) integer
 * \param tcv initialized transceiver @see{tcv_init}
 * \param pwr (out) tx-power (0.1 uW)
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_tx_pwr(tcv_t* tcv, uint16_t* pwr);

/**
 * Manufacturer defined max-temperture 16-Bit signed (8.8 fixed-point) integer
 * @param tcv initialized transceiver @see{tcv_init}
 * @param threshold (out) temperature warning threshold
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_temp_warning(tcv_t* tcv, uint16_t* threshold);

/**
 * Manufacturer defined max-rx power as 16-Bit signed (8.8 fixed-point) integer
 * @param tcv initialized transceiver @see{tcv_init}
 * @param threshold (out) power warning threshold
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_rx_pwr_warning(tcv_t* tcv, uint16_t* threshold);

/**
 * Manufacturer defined max-tx power as 16-Bit signed (8.8 fixed-point) integer
 * @param tcv initialized transceiver @see{tcv_init}
 * @param threshold (out) power warning threshold
 * \return	0 if ok; code error otherwise.
 */
int tcv_get_tx_pwr_warning(tcv_t* tcv, uint16_t* threshold);


#ifdef __cplusplus
} /*extern "C" */
#endif
#endif /* __LIBTCV_TCV_H__ */
