/*
 * NOTE
 * This file is based on SFF-8472 rev11.3.
 * It's an MSA translation.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "libtcv/tcv.h"
#include "libtcv/sfp.h"

/******************************************************************************/

/*
 +---------------------------------------------------------+
 |                          0xA0                           |
 +---------+------+--------------------------------+-------+
 | Reg     | Size | Name                           | Table |
 +---------+------+--------------------------------+-------+
 | 0       | 1    | Identifier                     | 3.2   |
 | 1       | 1    | Ext. Identifier                | 3.3   |
 | 2       | 1    | Connector                      | 3.4   |
 | 3-10    | 8    | Transceiver                    | 3.5   |
 | 11      | 1    | Encoding                       | 3.6   |
 | 12      | 1    | BR, Nominal                    | --    |
 | 13      | 1    | Rate Identifier                | 3.6a  |
 | 14      | 1    | Length (SMF, km)               | --    |
 | 15      | 1    | Length (SMF)                   | --    |
 | 16      | 1    | Length (50um)                  | --    |
 | 17      | 1    | Length (62.5um)                | --    |
 | 18      | 1    | Length (OM4 or copper capable) | --    |
 | 19      | 1    | Length (OM3)                   | --    |
 | 20-35   | 16   | Vendor name                    | --    |
 | 36      | 1    | Transceiver                    | 3.5   |
 | 37-39   | 3    | Vendor OUI                     | --    |
 | 40-55   | 16   | Vendor PN                      | --    |
 | 56-59   | 4    | Vendor rev                     | --    |
 | 60-61   | 2    | Wavelength                     | --    |
 | 62      | 1    | Reserved                       | --    |
 | 63      | 1    | CC_BASE                        | --    |
 | 64-65   | 2    | Options                        | 3.7   |
 | 66      | 1    | BR, mas                        | --    |
 | 67      | 1    | BR, min                        | --    |
 | 68-83   | 16   | Vendor SN                      | --    |
 | 84-91   | 8    | Date code                      | 3.8   |
 | 92      | 1    | Diagnostic Monitoring Type     | 3.9   |
 | 93      | 1    | Enhanced Options               | 3.10  |
 | 94      | 1    | SFF-8472 Compliancce           | 3.12  |
 | 95      | 1    | CC_EXT                         | --    |
 | 96-127  | 32   | Vendos Specific                | --    |
 | 128-255 | 128  | Reserved                       | --    |
 +---------+------+--------------------------------+-------+
*/

/******************************************************************************/

#define BASIC_INFO_REG_IDENTIFIER						0
#define BASIC_INFO_REG_IDENTIFIER_SIZE					1

#define BASIC_INFO_REG_EXT_IDENTIFIER					1
#define BASIC_INFO_REG_EXT_IDENTIFIER_SIZE				1

#define BASIC_INFO_REG_CONNECTOR						2
#define BASIC_INFO_REG_CONNECTOR_SIZE					1

#define BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1		3
#define BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1_SIZE	8

#define BASIC_INFO_REG_ENCODING							11
#define BASIC_INFO_REG_ENCODING_SIZE					1

#define BASIC_INFO_REG_NOMINAL_BIT_RATE					12
#define BASIC_INFO_REG_NOMINAL_BIT_RATE_SIZE			1

#define BASIC_INFO_REG_RATE_IDENTIFIER					13
#define BASIC_INFO_REG_RATE_IDENTIFIER_SIZE				1

#define BASIC_INFO_REG_LENGTH_SMF_KM					14
#define BASIC_INFO_REG_LENGTH_SMF_KM_SIZE				1

#define BASIC_INFO_REG_LENGTH_SMF_100M					15
#define BASIC_INFO_REG_LENGTH_SMF_100M_SIZE				1

#define BASIC_INFO_REG_LENGTH_OM2_10M					16
#define BASIC_INFO_REG_LENGTH_OM2_10M_SIZE				1

#define BASIC_INFO_REG_LENGTH_OM1_10M					17
#define BASIC_INFO_REG_LENGTH_OM1_10M_SIZE				1

#define BASIC_INFO_REG_LENGTH_OM4_10M_COPPER_1M			18
#define BASIC_INFO_REG_LENGTH_OM4_10M_COPPER_1M_SIZE	1

#define BASIC_INFO_REG_LENGTH_OM3_10M					19
#define BASIC_INFO_REG_LENGTH_OM3_10M_SIZE				1

#define BASIC_INFO_REG_VENDOR_NAME						20
#define BASIC_INFO_REG_VENDOR_NAME_SIZE					16

#define BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_2		36
#define BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_2_SIZE	1

#define BASIC_INFO_REG_VENDOR_OUI						37
#define BASIC_INFO_REG_VENDOR_OUI_SIZE_SIZE				3

#define BASIC_INFO_REG_VENDOR_PN						40
#define BASIC_INFO_REG_VENDOR_PN_SIZE					16

#define BASIC_INFO_REG_VENDOR_REV						56
#define BASIC_INFO_REG_VENDOR_REV_SIZE					4

#define BASIC_INFO_REG_WAVELENGTH						60
#define BASIC_INFO_REG_WAVELENGTH_SIZE					2

#define BASIC_INFO_REG_UNALLOCATED						62
#define BASIC_INFO_REG_UNALLOCATED_SIZE					1

#define BASIC_INFO_REG_CC_BASE							63
#define BASIC_INFO_REG_CC_BASE_SIZE						1

#define BASIC_INFO_REG_OPTIONS							64
#define BASIC_INFO_REG_OPTIONS_SIZE						2

#define BASIC_INFO_REG_BIT_RATE_MAX						66
#define BASIC_INFO_REG_BIT_RATE_MAX_SIZE				1

#define BASIC_INFO_REG_BIT_RATE_MIN						67
#define BASIC_INFO_REG_BIT_RATE_MIN_SIZE				1

#define BASIC_INFO_REG_VENDOR_SN						68
#define BASIC_INFO_REG_VENDOR_SN_SIZE					16

#define BASIC_INFO_REG_VENDOR_DATE_CODE					84
#define BASIC_INFO_REG_VENDOR_DATE_CODE_SIZE			8

#define BASIC_INFO_REG_DIAG_MONITORING_TYPE				92
#define BASIC_INFO_REG_DIAG_MONITORING_TYPE_SIZE		1

#define BASIC_INFO_REG_ENHANCED_OPTIONS					93
#define BASIC_INFO_REG_ENHANCED_OPTIONS_SIZE			1

#define BASIC_INFO_REG_SFF_COMPLIANCE					94
#define BASIC_INFO_REG_SFF_COMPLIANCE_SIZE				1

#define BASIC_INFO_REG_CC_EXT							95
#define BASIC_INFO_REG_CC_EXT_SIZE						1

#define BASIC_INFO_REG_VENDORS_SPECIFIC					96
#define BASIC_INFO_REG_VENDORS_SPECIFIC_SIZE			32

#define BASIC_INFO_REG_RESERVED							128
#define BASIC_INFO_REG_RESERVED_SIZE					128

/******************************************************************************/

int sfp_get_itendifier(tcv_t *tcv)
{
	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	return ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_IDENTIFIER];
}

/******************************************************************************/

int sfp_get_ext_itendifier(tcv_t *tcv)
{
	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	return ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_EXT_IDENTIFIER];
}

/******************************************************************************/

int sfp_get_connector(tcv_t *tcv)
{
	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	return ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_CONNECTOR];
}

/******************************************************************************/

/* Table 3.5b
 +------+-----+--------------+
 | Reg  | Bit | Description  |
 +------+-----+--------------+
 | 0x24 | 0   | Unallocated  |
 | 0x03 | 7   | 10G Base-ER  |
 | 0x03 | 6   | 10G Base-LRM |
 | 0x03 | 5   | 10G Base-LR  |
 | 0x03 | 4   | 10G Base SR  |
 +------+-----+--------------+
*/

#define SFP_10G_ETH_COMPLIANCE_REG	BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1 + 0
#define SFP_10G_ETH_COMPLIANCE_MASK	0xF0

int sfp_get_10g_compliance_codes(tcv_t *tcv, tcv_10g_eth_compliance_codes_t *codes)
{
	if (tcv == NULL || codes == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Clear bitmap */
	codes->bmp = 0;

	/* Fill bitmap */
	codes->bmp = 0;
	codes->bmp = (((sfp_data_t*)tcv->data)->a0[SFP_10G_ETH_COMPLIANCE_REG] & SFP_10G_ETH_COMPLIANCE_MASK) >> 4;

	return 0;
}

/******************************************************************************/

/* Table 3.5c
 +------+-----+-------------------+
 | Reg  | Bit | Description       |
 +------+-----+-------------------+
 | 0x03 | 3   | 1X SX             |
 | 0x03 | 2   | 1X LX             |
 | 0x03 | 1   | 1X Copper Active  |
 | 0x03 | 0   | 1C Copper Passive |
 +------+-----+-------------------+
*/

#define INFINIBAND_COMPLIANCE_REG	BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1 + 0
#define INFINIBAND_MASK 0x0F

int sfp_get_infiniband_compliance_codes(tcv_t *tcv, tcv_infiniband_compliance_codes_t *codes)
{
	if (tcv == NULL || codes == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Clear bitmap */
	codes->bmp = 0;

	codes->bmp = ((sfp_data_t*)tcv->data)->a0[INFINIBAND_COMPLIANCE_REG] & INFINIBAND_MASK;

	return 0;
}

/******************************************************************************/

/* Table 3.5d
 +------+-----+-------------------------+
 | Reg  | Bit | Description             |
 +------+-----+-------------------------+
 | 0x04 | 7   | ESCON MMF, 1310nm LED   |
 | 0x04 | 6   | ESCON SMF, 1310nm Laser |
 +------+-----+-------------------------+
*/

#define ESCON_COMPLIANCE_REG	BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1 + 1
#define ESCON_MASK 				0xC0

int sfp_get_escon_compliance_codes(tcv_t *tcv, tcv_escon_compliance_codes_t *codes)
{
	if (tcv == NULL || codes == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Clear bitmap */
	codes->bmp = 0;

	codes->bmp = (((sfp_data_t*)tcv->data)->a0[ESCON_COMPLIANCE_REG] & ESCON_MASK) >> 6;

	return 0;
}

/******************************************************************************/

/* Table 3.5e
 +------+-----+----------------------------------+
 | Reg  | Bit | Description                      |
 +------+-----+----------------------------------+
 | 0x04 | 5   | OC-192 short reach               |
 | 0x04 | 4   | SONET reach specifier bit 1      |
 | 0x04 | 3   | SONET reach specifier bit 2      |
 | 0x04 | 2   | OC-48, long reach                |
 | 0x04 | 1   | OC-48, intermediate reach        |
 | 0x04 | 0   | OC-48, short reach               |
 | 0x05 | 7   | Unallocated                      |
 | 0x05 | 6   | OC-12, single mode, long reach   |
 | 0x05 | 5   | OC-12, single mode, inter. reach |
 | 0x05 | 4   | OC-12, short reach               |
 | 0x05 | 3   | Unallocated                      |
 | 0x05 | 2   | OC-3, single mode, long reach    |
 | 0x05 | 1   | OC-3, single mode, inter. reach  |
 | 0x05 | 0   | OC-3 short reach                 |
 +------+-----+----------------------------------+
*/

#define SONET_OC192_SR		(1 << 5)
#define SONET_OC48_LR		(1 << 2)
#define SONET_OC48_IR		(1 << 1)
#define SONET_OC48_SR		(1 << 0)
#define SONET_OC12_SM_LR	(1 << 6)
#define SONET_OC12_SM_IR	(1 << 5)
#define SONET_OC12_SR		(1 << 4)
#define SONET_OC3_SM_LR		(1 << 2)
#define SONET_OC3_SM_IR		(1 << 1)
#define SONET_OC3_SR		(1 << 0)

#define SONET_COMPLIANCE_REG_1	BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1 + 1
#define SONET_COMPLIANCE_REG_2	BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1 + 2

int sfp_get_sonet_compliance_codes(tcv_t *tcv, tcv_sonet_compliance_codes_t *codes)
{
	if (tcv == NULL || codes == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Clear bitmap */
	codes->bmp = 0;

	codes->bits.oc_192_sr = !! (((sfp_data_t*)tcv->data)->a0[SONET_COMPLIANCE_REG_1] & SONET_OC192_SR);
	codes->bits.oc_48_lr = !! (((sfp_data_t*)tcv->data)->a0[SONET_COMPLIANCE_REG_1] & SONET_OC48_LR);
	codes->bits.oc_48_ir = !! (((sfp_data_t*)tcv->data)->a0[SONET_COMPLIANCE_REG_1] & SONET_OC48_IR);
	codes->bits.oc_48_sr = !! (((sfp_data_t*)tcv->data)->a0[SONET_COMPLIANCE_REG_1] & SONET_OC48_SR);
	codes->bits.oc_12_sm_lr = !! (((sfp_data_t*)tcv->data)->a0[SONET_COMPLIANCE_REG_2] & SONET_OC12_SM_LR);
	codes->bits.oc_12_sm_ir = !! (((sfp_data_t*)tcv->data)->a0[SONET_COMPLIANCE_REG_2] & SONET_OC12_SM_IR);
	codes->bits.oc_12_sr = !! (((sfp_data_t*)tcv->data)->a0[SONET_COMPLIANCE_REG_2] & SONET_OC12_SR);
	codes->bits.oc_3_sm_lr = !! (((sfp_data_t*)tcv->data)->a0[SONET_COMPLIANCE_REG_2] & SONET_OC3_SM_LR);
	codes->bits.oc_3_sm_ir = !! (((sfp_data_t*)tcv->data)->a0[SONET_COMPLIANCE_REG_2] & SONET_OC3_SM_IR);
	codes->bits.oc_3_sr = !! (((sfp_data_t*)tcv->data)->a0[SONET_COMPLIANCE_REG_2] & SONET_OC3_SR);

	return 0;
}

/******************************************************************************/

/* Table 3.5a
 +------------------+--------------+-----------+------------+----------------------+
 |       Speed      |     Reach    |Spec bit 1 | Spec bit 2 |      Description     |
 +------------------+--------------+-----------+------------+----------------------+
 | OC [3/12/48/192] |     Short    |     0     |     0      |  SONET SR compliant  |
 | OC [3/12/48/192] |     Short    |     1     |     0      | SONET SR-1 compliant |
 | OC [3/12/48/192] | Intermediate |     1     |     0      | SONET IR-1 compliant |
 |   OC [3/12/48]   | Intermediate |     0     |     1      | SONET IR-2 compliant |
 |   OC [3/12/48]   |     Long     |     1     |     0      | SONET LR-1 compliant |
 |   OC [3/12/48]   |     Long     |     0     |     1      | SONET LR-2 compliant |
 |   OC [3/12/48]   |     Long     |     1     |     1      | SONET LR-3 compliant |
 +------------------+--------------+-----------+------------+----------------------+
*/

#define SONET_REACH_SPECIFIER_1	(1 << 4)
#define SONET_REACH_SPECIFIER_2	(1 << 3)

int sfp_get_sonet_compliances(tcv_t *tcv, tcv_sonet_compliances_t *compliances)
{
	tcv_sonet_compliance_codes_t codes;
	bool spec_bit_1, spec_bit_2;
	int ret;

	if (tcv == NULL || compliances == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Clear bitmap */
	compliances->bmp = 0;

	/* Get set compliance codes */
	ret = tcv_get_sonet_compliance_codes(tcv, &codes);
	if (ret < 0)
		return ret;

	/* Get the specifiers bit */
	spec_bit_1 = !! (((sfp_data_t*)tcv->data)->a0[SONET_COMPLIANCE_REG_1] & SONET_REACH_SPECIFIER_1);
	spec_bit_2 = !! (((sfp_data_t*)tcv->data)->a0[SONET_COMPLIANCE_REG_1] & SONET_REACH_SPECIFIER_2);

	/* Verifie short reach */
	if (codes.bits.oc_192_sr || codes.bits.oc_48_sr || codes.bits.oc_12_sr || codes.bits.oc_3_sr) {
		if (!spec_bit_1 && !spec_bit_2)
			compliances->bits.sr_compliant = 1;
		else if (spec_bit_1 && !spec_bit_2)
			compliances->bits.sr_1_compliant = 1;
	}

	/* Verifie intermediate reach */
	if (codes.bits.oc_48_ir || codes.bits.oc_12_sm_ir || codes.bits.oc_3_sm_ir) {
		if (spec_bit_1 && !spec_bit_2)
			compliances->bits.ir_1_compliant = 1;
		else if(!spec_bit_1 && spec_bit_2)
			compliances->bits.ir_2_compliant = 1;
	}

	/* Verifie long reach */
	if (codes.bits.oc_48_ir || codes.bits.oc_12_sm_ir || codes.bits.oc_3_sm_ir) {
		if (spec_bit_1 && !spec_bit_2)
			compliances->bits.lr_1_compliant = 1;
		else if(!spec_bit_1 && spec_bit_2)
			compliances->bits.lr_2_compliant = 1;
		else if(spec_bit_1 && spec_bit_2)
			compliances->bits.lr_3_compliant = 1;
	}

	return 0;
}

/******************************************************************************/

/* Table 3.5f
 +------+-----+-----------------+
 | Reg  | Bit | Description     |
 +------+-----+-----------------+
 | 0x06 | 7   | BASE-PX         |
 | 0x06 | 6   | BASE-BX10       |
 | 0x06 | 5   | 100BASE-FX      |
 | 0x06 | 4   | 100BASE-LX/LX10 |
 | 0x06 | 3   | 1000BASE-T      |
 | 0x06 | 2   | 1000BASE-CX     |
 | 0x06 | 1   | 1000BASE-LX     |
 | 0x06 | 0   | 1000BASE-SX     |
 +------+-----+-----------------+
*/

#define ETH_COMPLIANCE_REG_1	BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1 + 3

int sfp_get_eth_compliance_codes(tcv_t *tcv, tcv_eth_compliance_codes_t *codes)
{
	if (tcv == NULL || codes == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Fill bitmap */
	codes->bmp = ((sfp_data_t*)tcv->data)->a0[ETH_COMPLIANCE_REG_1];

	return 0;
}

/******************************************************************************/

/* Table 3.5g
 +------+-----+------------------------+
 | Reg  | Bit | Description            |
 +------+-----+------------------------+
 | 0x07 | 7   | Very long distance (V) |
 | 0x07 | 6   | Short distance (S)     |
 | 0x07 | 5   | intermediate distance  |
 | 0x07 | 4   | Long distance (L)      |
 | 0x07 | 3   | Medium distance (M)    |
 +------+-----+------------------------+
*/

#define LINK_LENGTH_REG		BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1 + 4
#define LINK_LENGTH_MASK	0xF8

int sfp_get_fibre_channel_link_length(tcv_t *tcv, tcv_fibre_channel_link_length_t *lengths)
{
	if (tcv == NULL || lengths == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Clear bitmap */
	lengths->bmp = 0;

	/* Fill bitmap */
	lengths->bmp = (((sfp_data_t*)tcv->data)->a0[LINK_LENGTH_REG] & LINK_LENGTH_MASK) >> 3;

	return 0;
}

/******************************************************************************/

/* Table 3.5g
 +------+-----+---------------------------------+
 | Reg  | Bit | Description                     |
 +------+-----+---------------------------------+
 | 0x07 | 2   | Shortwave laser, linear (SA)    |
 | 0x07 | 1   | Longwave laser (LC)             |
 | 0x07 | 0   | Electrical inter-enclosure (EL) |
 | 0x08 | 7   | Electrical intra-enclosure (EL) |
 | 0x08 | 6   | Shortwave laser w/o OFC (SN)    |
 | 0x08 | 5   | Shortwave laser with OFC (SL)   |
 | 0x08 | 4   | Longwave laser (LL)             |
 +------+-----+---------------------------------+
*/

#define FIBRE_CHANNEL_TECH_REG_1	BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1 + 4
#define FIBRE_CHANNEL_TECH_MASK_1	0x07
#define FIBRE_CHANNEL_TECH_REG_2	BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1 + 5
#define FIBRE_CHANNEL_TECH_MASK_2	0xF0

int sfp_get_fibre_channel_technology(tcv_t *tcv, tcv_fibre_channel_technology_t *technology)
{
	if (tcv == NULL || technology == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Clear bitmap */
	technology->bmp = 0;

	/* Fill bitmap */
	technology->bmp |= (((sfp_data_t*)tcv->data)->a0[FIBRE_CHANNEL_TECH_REG_1] & FIBRE_CHANNEL_TECH_MASK_1) << 4;
	technology->bmp |= (((sfp_data_t*)tcv->data)->a0[FIBRE_CHANNEL_TECH_REG_2] & FIBRE_CHANNEL_TECH_MASK_2) >> 4;

	return 0;
}

/******************************************************************************/

/* Table 3.5h
 +------+-----+---------------+
 | Reg  | Bit | Description   |
 +------+-----+---------------+
 | 0x08 | 3   | Active cable  |
 | 0x08 | 2   | Passive cable |
 +------+-----+---------------+
*/

#define SFP_PLUS_TECH_REG	BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1 + 5
#define SFP_PLUS_TECH_MASK	0x0C

int sfp_get_sfp_plus_cable_technology(tcv_t *tcv, sfp_plus_cable_technology_t *technology)
{
	if (tcv == NULL || technology == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Clear bitmap */
	technology->bmp = 0;

	/* Fill bitmap */
	technology->bmp = (((sfp_data_t*)tcv->data)->a0[SFP_PLUS_TECH_REG] & SFP_PLUS_TECH_MASK) >> 2;

	return 0;
}

/******************************************************************************/

/* Table 3.5i
 +------+-----+--------------------------+
 | Reg  | Bit | Description              |
 +------+-----+--------------------------+
 | 0x09 | 7   | Twin axial pair (TW)     |
 | 0x09 | 6   | Twisted pair (TP)        |
 | 0x09 | 5   | Miniature coax (MI)      |
 | 0x09 | 4   | Video coax (TV)          |
 | 0x09 | 3   | Multimode 62.5nm (M6)    |
 | 0x09 | 2   | Multimode 50nm (M5, M5E) |
 | 0x09 | 0   | Single Mode (SM)         |
 +------+-----+--------------------------+
*/

#define MEDIA_REG		BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1 + 6
#define MEDIA_MASK_1	0xFC
#define MEDIA_MASK_2	0x01

int sfp_get_fibre_channel_media(tcv_t *tcv, tcv_fibre_channel_media_t *media)
{
	if (tcv == NULL || media == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Clear bitmap */
	media->bmp = 0;

	/* Fill bitmap */
	media->bmp |= (((sfp_data_t*)tcv->data)->a0[MEDIA_REG] & MEDIA_MASK_1) >> 1;
	media->bmp |= ((sfp_data_t*)tcv->data)->a0[MEDIA_REG] & MEDIA_MASK_2;
	return 0;
}

/******************************************************************************/

/* Table 3.5j
 +------+-----+-------------+
 | Reg  | Bit | Description |
 +------+-----+-------------+
 | 0x0A | 7   | 1200 MB/s   |
 | 0x0A | 6   | 800 MB/s    |
 | 0x0A | 5   | 1600 MB/s   |
 | 0x0A | 4   | 400 MB/s    |
 | 0x0A | 3   | 3200 MB/s   |
 | 0x0A | 2   | 200 MB/s    |
 | 0x0A | 0   | 100 MB/s    |
 +------+-----+-------------+
*/

#define FIBRE_CHANNEL_SPEED_REG		BASIC_INFO_REG_ELETRONIC_COMPATIBILITIE_1 + 7
#define FIBRE_CHANNEL_SPEED_MASK_1	0xFC
#define FIBRE_CHANNEL_SPEED_MASK_2	0x01

int sfp_get_fibre_channel_speed(tcv_t *tcv, fibre_channel_speed_t *speed)
{
	if (tcv == NULL || speed == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Clear bitmap */
	speed->bmp = 0;

	/* Fill bitmap */
	speed->bmp |= (((sfp_data_t*)tcv->data)->a0[MEDIA_REG] & MEDIA_MASK_1) >> 1;
	speed->bmp |= ((sfp_data_t*)tcv->data)->a0[MEDIA_REG] & MEDIA_MASK_2;

	return 0;
}

/******************************************************************************/

int sfp_get_encoding(tcv_t *tcv)
{
	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	return ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_ENCODING];
}

/******************************************************************************/

int sfp_get_nominal_bit_rate(tcv_t *tcv)
{
	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	if (((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_NOMINAL_BIT_RATE] != 0xFF)
		/* In register the bit rate is in units of 100MBytes */
		return ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_NOMINAL_BIT_RATE] * 100;

	/* To 0xFF value, the bit rate is at BASIC_INFO_REG_BIT_RATE_MAX, in units of 250MBytes */
	return ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_BIT_RATE_MAX] * 250;
}

/******************************************************************************/

int sfp_get_rate_identifier(tcv_t *tcv)
{
	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	return ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_RATE_IDENTIFIER];
}

/******************************************************************************/

int sfp_get_sm_length(tcv_t *tcv)
{
	unsigned char length;

	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Normalize length to meters */
	length = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_LENGTH_SMF_KM] * 1000;

	/* If Km length is 0, try to read from meters unit register */
	if (length == 0)
		length = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_LENGTH_SMF_100M] * 100;


	return length == 0 ? TCV_ERR_SM_LENGTH_NOT_DEFINED : length;
}

/******************************************************************************/

int sfp_get_om2_length(tcv_t *tcv)
{
	unsigned char length;

	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Normalize length to meters */
	length = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_LENGTH_OM2_10M] * 10;

	return length == 0 ? TCV_ERR_OM2_LENGTH_NOT_DEFINED : length;
}

/******************************************************************************/

int sfp_get_om1_length(tcv_t *tcv)
{
	unsigned char length;

	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Normalize length to meters */
	length = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_LENGTH_OM1_10M] * 10;

	return length == 0 ? TCV_ERR_OM1_LENGTH_NOT_DEFINED : length;
}

/******************************************************************************/
static bool sfp_is_optical(tcv_t *tcv)
{
	// TODO
	if (tcv)
		return 1;

	return 1;
}

int sfp_get_om4_length_copper_length(tcv_t *tcv)
{
	unsigned char length;

	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Normalize length to meters. Optical link is measured in units of 10 meters
	 * and copper link is measured in units of 1 meter. */
	if (sfp_is_optical(tcv))
		length = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_LENGTH_OM4_10M_COPPER_1M] * 10;
	else
		length = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_LENGTH_OM4_10M_COPPER_1M];

	return length == 0 ? TCV_ERR_OM4_LENGTH_NOT_DEFINED : length;
}

/******************************************************************************/

int sfp_get_om3_length(tcv_t *tcv)
{
	unsigned char length;

	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Normalize length to meters */
	length = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_LENGTH_OM3_10M] * 10;

	return length == 0 ? TCV_ERR_OM3_LENGTH_NOT_DEFINED : length;
}

/******************************************************************************/

int sfp_get_vendor_name(tcv_t *tcv, char vendor_name[BASIC_INFO_REG_VENDOR_NAME_SIZE + 1])
{
	if (tcv == NULL || vendor_name == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Indicates end of string */
	vendor_name[BASIC_INFO_REG_VENDOR_NAME_SIZE] = '\0';

	memcpy(vendor_name, &((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_VENDOR_NAME], BASIC_INFO_REG_VENDOR_NAME_SIZE);

	return 0;
}

/******************************************************************************/

int sfp_get_vendor_oui(tcv_t *tcv)
{
	uint32_t oui = 0;
	int i;

	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Concatenate the 3 bytes in just one variable */
	for (i = 0; i < BASIC_INFO_REG_VENDOR_OUI_SIZE_SIZE; i++){
		/* bytes are stored in eeprom in big endian order */
		oui = (oui<< 8);
		oui |= ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_VENDOR_OUI+i];
	}

	return oui;
}

/******************************************************************************/

int sfp_get_vendor_part_number(tcv_t *tcv, char pn[BASIC_INFO_REG_VENDOR_PN_SIZE + 1])
{
	if (tcv == NULL || pn == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Indicates end of string */
	pn[BASIC_INFO_REG_VENDOR_PN_SIZE] = '\0';

	memcpy(pn, &((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_VENDOR_PN], BASIC_INFO_REG_VENDOR_PN_SIZE);

	return 0;
}

/******************************************************************************/

int sfp_get_vendor_revision(tcv_t *tcv, char rev[BASIC_INFO_REG_VENDOR_REV_SIZE + 1])
{
	if (tcv == NULL || rev == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Indicates end of string */
	rev[BASIC_INFO_REG_VENDOR_REV_SIZE] = '\0';

	memcpy(rev, &((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_VENDOR_REV], BASIC_INFO_REG_VENDOR_REV_SIZE);

	return 0;
}

/******************************************************************************/

int sfp_get_wavelength(tcv_t *tcv)
{
	unsigned int length = 0;
	int ret;
	sfp_plus_cable_technology_t tech;

	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	ret = tcv_get_sfp_plus_cable_technology(tcv, &tech);
	if (ret < 0)
		return ret;

	/* Wavelength is valid only when SFP+ cable technology is zero */
	if (tech.bmp != 0)
		return TCV_ERR_FEATURE_NOT_AVAILABLE;

	/* The first address is the high part of the 16 bit wavelength and the next
	 * address is the low part */
	length |= ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_WAVELENGTH] << 8;
	length |= ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_WAVELENGTH + 1];

	return length;
}

/******************************************************************************/

/* Table 3.6b
 +------+-----+---------------------------------+
 | Reg  | Bit | Description                     |
 +------+-----+---------------------------------+
 | 0x3C | 7-6 | Unallocated                     |
 | 0x3C | 5-2 | Reserved for SFF-8461           |
 | 0x3C | 1   | Compliant to FC-PI-4 Apendix H  |
 | 0x3C | 0   | Compliant to SFF-8431 Apendix E |
 | 0x3D | 7-7 | Unallocated                     |
 +------+-----+---------------------------------+
*/

#define FC_PI_4_APENDIX_H_COMPLIANT		(1 << 1)
#define SFF_8431_APENDIX_E_COMPLIANT	(1 << 0)

int sfp_get_passive_cable_compliance(tcv_t *tcv, passive_cable_compliance_t *compliance)
{
	int ret;
	sfp_plus_cable_technology_t tech;

	if (tcv == NULL || compliance == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	ret = tcv_get_sfp_plus_cable_technology(tcv, &tech);
	if (ret < 0)
		return ret;

	/* Verify if the transceiver is passive cable */
	if (!tech.bits.passive_cable)
		return TCV_ERR_FEATURE_NOT_AVAILABLE;

	/* Clear bitmap */
	compliance->bmp = 0;

	compliance->bits.fc_pi_4_apndx_h_compliant =
		!! (((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_WAVELENGTH] & FC_PI_4_APENDIX_H_COMPLIANT);

	compliance->bits.sff_8431_apndx_e_compliant =
		!! (((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_WAVELENGTH] & SFF_8431_APENDIX_E_COMPLIANT);

	return 0;
}

/******************************************************************************/

/* Table 3.6c
 +------+-----+---------------------------------+
 | Reg  | Bit | Description                     |
 +------+-----+---------------------------------+
 | 0x3C | 7-4 | Unallocated                     |
 | 0x3C | 3   | Compliant to FC-PI-4 Limiting   |
 | 0x3C | 2   | Compliant to SFF-8431 Limiting  |
 | 0x3C | 1   | Compliant to FC-PI-4 Apendix H  |
 | 0x3C | 0   | Compliant to SFF-8431 Apendix E |
 | 0x3D | 7-0 | Unallocated                     |
 +------+-----+---------------------------------+
*/

#define FC_PI_4_LIMITING_COMPLIANT	(1 << 3)
#define SFF_8431_LIMITING_COMPLIANT	(1 << 2)

int sfp_get_active_cable_compliance(tcv_t *tcv, active_cable_compliance_t *compliance)
{
	int ret;
	sfp_plus_cable_technology_t tech;

	if (tcv == NULL || compliance == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	ret = tcv_get_sfp_plus_cable_technology(tcv, &tech);
	if (ret < 0)
		return ret;

	/* Verify if the transceiver is passive cable */
	if (!tech.bits.active_cable)
		return TCV_ERR_FEATURE_NOT_AVAILABLE;

	/* Clear bitmap */
	compliance->bmp = 0;

	compliance->bits.fc_pi_4_limiting_compliant =
		!! (((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_WAVELENGTH] & FC_PI_4_LIMITING_COMPLIANT);

	compliance->bits.sff_8431_limiting_compliant =
		!! (((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_WAVELENGTH] & SFF_8431_LIMITING_COMPLIANT);

	compliance->bits.fc_pi_4_apndx_h_compliant =
		!! (((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_WAVELENGTH] & FC_PI_4_APENDIX_H_COMPLIANT);

	compliance->bits.sff_8431_apndx_e_compliant =
		!! (((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_WAVELENGTH] & SFF_8431_APENDIX_E_COMPLIANT);

	return 0;
}

/******************************************************************************/

int sfp_get_cc_base(tcv_t *tcv)
{
	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	return ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_CC_BASE];
}

/******************************************************************************/

#define FIRST_CC_BASE_ADDR	0x00
#define LAST_CC_BASE_ADDR	0x3E

int sfp_calculate_cc_base(tcv_t *tcv)
{
	int i;
	int sum = 0;

	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* The CC Base is the low order 8 bits of the sum of the contents of all
	 * bytes from 0x00 to 0x62 */
	for (i = FIRST_CC_BASE_ADDR; i <= LAST_CC_BASE_ADDR; i++)
		sum += ((sfp_data_t*)tcv->data)->a0[i];

	/* Return low order 8 bits only */
	return (sum & 0xff);
}

/******************************************************************************/

/* Table 3.7
 +------+-----+----------------------------------------------------------+
 | Reg  | Bit | Description                                              |
 +------+-----+----------------------------------------------------------+
 | 0x40 | 7-3 | Unallocated                                              |
 | 0x40 | 2   | Cooled transceiver implementation                        |
 | 0x40 | 1   | Value 0 means Power Lever 1, value 1 means Power Level 2 |
 | 0x40 | 0   | Linear receiver output implemented                       |
 | 0x41 | 7-6 | Unallocated                                              |
 | 0x41 | 5   | Rate select implemented                                  |
 | 0x41 | 4   | TX disable implemented                                   |
 | 0x41 | 3   | TX fault implemented                                     |
 | 0x41 | 2   | Signal detect implemented (LOS opposite)                 |
 | 0x41 | 1   | LOS implemented                                          |
 | 0x41 | 0   | Unallocated                                              |
 +------+-----+----------------------------------------------------------+
*/

#define OPTION_COOLED_LASER		(1 << 2) /* BASIC_INFO_REG_OPTIONS */
#define OPTION_POWER_LEVEL		(1 << 1) /* BASIC_INFO_REG_OPTIONS */
#define OPTION_LINEAR_RX_OUT	(1 << 0) /* BASIC_INFO_REG_OPTIONS */
#define OPTION_RATE_SEL			(1 << 5) /* BASIC_INFO_REG_OPTIONS + 1 */
#define OPTION_TX_DIS			(1 << 4) /* BASIC_INFO_REG_OPTIONS + 1 */
#define OPTION_TX_FAULT			(1 << 3) /* BASIC_INFO_REG_OPTIONS + 1 */
#define OPTION_SIG_DETECT		(1 << 2) /* BASIC_INFO_REG_OPTIONS + 1 */
#define OPTION_LOS				(1 << 1) /* BASIC_INFO_REG_OPTIONS + 1 */

int sfp_get_implemented_options(tcv_t *tcv, tcv_implemented_options_t *options)
{
	if (tcv == NULL || options == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Clear options */
	options->bmp = 0;

	/* Information stored in BASIC_INFO_REG_OPTIONS */
	options->bits.cooled_laser_transmitted = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_OPTIONS];
	options->bits.power_lever_2 = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_OPTIONS];
	options->bits.linear_receiver_out = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_OPTIONS];

	/* Information stored in BASIC_INFO_REG_OPTIONS + 1 */
	options->bits.rate_select = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_OPTIONS + 1];
	options->bits.tx_disable = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_OPTIONS + 1];
	options->bits.tx_fault = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_OPTIONS + 1];
	options->bits.signal_detect = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_OPTIONS + 1];
	options->bits.los = ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_OPTIONS + 1];

	return 0;
}

/******************************************************************************/

int sfp_get_max_bit_rate(tcv_t *tcv)
{
	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* If nominal rate is set to 0xFF, BASIC_INFO_REG_BIT_RATE_MAX register has
	 * the nominal bit rate information and BASIC_INFO_REG_BIT_RATE_MIN has the
	 * max and min bit rate (in this case max and min BR are symmetrical. */
	if (((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_NOMINAL_BIT_RATE] == 0xFF)
		return ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_BIT_RATE_MIN];

	return ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_BIT_RATE_MAX];
}

/******************************************************************************/

int sfp_get_min_bit_rate(tcv_t *tcv)
{
	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	return ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_BIT_RATE_MIN];
}

/******************************************************************************/

int sfp_get_vendor_sn(tcv_t *tcv, char vendor_sn[TCV_VENDOR_SN_SIZE + 1])
{
	if (tcv == NULL || vendor_sn == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Indicates end of string */
	vendor_sn[TCV_VENDOR_SN_SIZE] = '\0';

	memcpy(vendor_sn, &((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_VENDOR_SN], TCV_VENDOR_SN_SIZE);

	return 0;
}

/******************************************************************************/

/* Table 3.8
 +-------------+------------------------------------------------------+
 | Reg         | Description                                          |
 +-------------+------------------------------------------------------+
 | 0x54 - 0x55 | ASCII code, two low order digits of year (00 = 2000) |
 | 0x56 - 0x57 | ASCII code, digits of month                          |
 | 0x58 - 0x59 | ASCII code, day of month                             |
 | 0x5A - 0x5B | ASCII code, vendor specific lot code                 |
 +-------------+------------------------------------------------------+
*/

#define DATE_CODE_YEAR_1	BASIC_INFO_REG_VENDOR_DATE_CODE + 0
#define DATE_CODE_YEAR_2	BASIC_INFO_REG_VENDOR_DATE_CODE + 1
#define DATE_CODE_MONTH_1	BASIC_INFO_REG_VENDOR_DATE_CODE + 2
#define DATE_CODE_MONTH_2	BASIC_INFO_REG_VENDOR_DATE_CODE + 3
#define DATE_CODE_DAY_1		BASIC_INFO_REG_VENDOR_DATE_CODE + 4
#define DATE_CODE_DAY_2		BASIC_INFO_REG_VENDOR_DATE_CODE + 5
#define DATE_CODE_LOT		BASIC_INFO_REG_VENDOR_DATE_CODE + 6
#define DATE_CODE_LOT_SIZE	2

int sfp_get_vendor_date_code(tcv_t *tcv, tcv_date_code_t *date_code)
{
	char tmp[3];

	if (tcv == NULL || date_code == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* Initialize tmp char with \0 */
	tmp[2] = '\0';

	/* Get year */
	tmp[0] = ((sfp_data_t*)tcv->data)->a0[DATE_CODE_YEAR_1];
	tmp[1] = ((sfp_data_t*)tcv->data)->a0[DATE_CODE_YEAR_2];
	date_code->year = atoi(tmp);

	/* Get month */
	tmp[0] = ((sfp_data_t*)tcv->data)->a0[DATE_CODE_MONTH_1];
	tmp[1] = ((sfp_data_t*)tcv->data)->a0[DATE_CODE_MONTH_2];
	date_code->month = atoi(tmp);

	/* Get day */
	tmp[0] = ((sfp_data_t*)tcv->data)->a0[DATE_CODE_DAY_1];
	tmp[1] = ((sfp_data_t*)tcv->data)->a0[DATE_CODE_DAY_2];
	date_code->day = atoi(tmp);

	/* Get lot code */
	memcpy(date_code->vendor_lot_code, &((sfp_data_t*)tcv->data)->a0[DATE_CODE_LOT], DATE_CODE_LOT_SIZE);

	return 0;
}

/******************************************************************************/

/* Table 3.9
 +------+-----+----------------------------------+
 | Reg  | Bit | Description                      |
 +------+-----+----------------------------------+
 | 0x5C | 7   | Reserved                         |
 | 0x5C | 6   | DD implemented                   |
 | 0x5C | 5   | Internally calibrated            |
 | 0x5C | 4   | Externally calibrated            |
 | 0x5C | 3   | Received power measurement type  |
 | 0x5C | 2   | Address change required          |
 | 0x5C | 1-0 | Unallocated                      |
 +------+-----+----------------------------------+
*/

#define DIAG_TYPE_MASK 0x7C

int sfp_get_diagnostic_type(tcv_t *tcv, tcv_diagnostic_type_t *diag_type)
{
	if (tcv == NULL || diag_type == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	diag_type->bmp = (((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_DIAG_MONITORING_TYPE] & DIAG_TYPE_MASK) >> 2;

	return 0;
}

/******************************************************************************/

/* Table 3.10
 +------+-----+--------------------------------------------------------------+
 | Reg  | Bit | Description                                                  |
 +------+-----+--------------------------------------------------------------+
 | 0x5D | 7   | Alarm/warning flags implemented for all monitored quantities |
 | 0x5D | 6   | Soft TX disable control, and monitoring implemented          |
 | 0x5D | 5   | Soft TX Fault monitoring implemented                         |
 | 0x5D | 4   | Soft RX LOS monitoring implemented                           |
 | 0x5D | 3   | Soft rate select control and monitoring implemented          |
 | 0x5D | 2   | Application select control implemented                       |
 | 0x5D | 1   | Soft rate select control implemented                         |
 | 0x5D | 0   | Unallocated                                                  |
 +------+-----+--------------------------------------------------------------+
*/

#define ENHANCED_OPTIONS_MASK 0xFE

int sfp_get_enhance_options(tcv_t *tcv, tcv_enhanced_options_type_t *options)
{
	if (tcv == NULL || options == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	options->bmp = (((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_ENHANCED_OPTIONS] & ENHANCED_OPTIONS_MASK) >> 1;

	return 0;
}

/******************************************************************************/

int sfp_get_cc_ext(tcv_t *tcv)
{
	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	return ((sfp_data_t*)tcv->data)->a0[BASIC_INFO_REG_CC_EXT];
}

/******************************************************************************/

#define FIRST_CC_EXT_ADDR	0x40
#define LAST_CC_EXT_ADDR	0x5E

int sfp_calculate_cc_ext(tcv_t *tcv)
{
	int i;
	int sum = 0;

	if (tcv == NULL || tcv->data == NULL)
		return TCV_ERR_INVALID_ARG;

	/* The CC Base is the low order 8 bits of the sum of the contents of all
	 * bytes from 0x00 to 0x62 */
	for (i = FIRST_CC_EXT_ADDR; i <= LAST_CC_EXT_ADDR; i++)
		sum += ((sfp_data_t*)tcv->data)->a0[i];

	/* Return low order 8 bits only */
	return (sum & 0xff);
}


/******************************************************************************/

const uint8_t* sfp_get_vendor_rom(tcv_t *tcv)
{
	const size_t VENDOR_ROM_OFFSET = 96;

	if (tcv == NULL || tcv->data == NULL)
		return NULL;

	return &((sfp_data_t*) tcv->data)->a0[VENDOR_ROM_OFFSET];
}

/******************************************************************************/

size_t sfp_get_vendor_rom_size(tcv_t *tcv)
{
	if (tcv == NULL || tcv->data == NULL)
		return 0;

	return 32;
}

/******************************************************************************/

const uint8_t* sfp_get_8079_rom(tcv_t *tcv)
{

	const size_t SFF_8079_ROM_OFFSET = 128;

	if (tcv == NULL || tcv->data == NULL)
		return NULL;

	return &((sfp_data_t*) tcv->data)->a0[SFF_8079_ROM_OFFSET];
}


/******************************************************************************/

/**
 * Member functions for sfp modules
 */
const struct tcv_functions sfp_funcs = {
	.get_itendifier = sfp_get_itendifier,
	.get_ext_itendifier = sfp_get_ext_itendifier,
	.get_connector = sfp_get_connector,
	.get_vendor_name = sfp_get_vendor_name,
	.get_vendor_oui = sfp_get_vendor_oui,
	.get_vendor_revision = sfp_get_vendor_revision,
	.get_vendor_part_number = sfp_get_vendor_part_number,
	.get_vendor_serial_number = sfp_get_vendor_sn,
	.get_vendor_date_code = sfp_get_vendor_date_code,
	.get_10g_compliance_codes = sfp_get_10g_compliance_codes,
	.get_infiniband_compliance_codes = sfp_get_infiniband_compliance_codes,
	.get_escon_compliance_codes = sfp_get_escon_compliance_codes,
	.get_sonet_compliance_codes = sfp_get_sonet_compliance_codes,
	.get_eth_compliance_codes = sfp_get_eth_compliance_codes,
	.get_fibre_channel_link_length = sfp_get_fibre_channel_link_length,
	.get_fibre_channel_technology  =  sfp_get_fibre_channel_technology,
	.get_sfp_plus_cable_technology = sfp_get_sfp_plus_cable_technology,
	.get_fibre_channel_media = sfp_get_fibre_channel_media,
	.get_fibre_channel_speed = sfp_get_fibre_channel_speed,
	.get_encoding =  sfp_get_encoding,
	.get_nominal_bit_rate = sfp_get_nominal_bit_rate,
	.get_rate_identifier =  sfp_get_rate_identifier,
	.get_sm_length =  sfp_get_sm_length,
	.get_om1_length = sfp_get_om1_length,
	.get_om2_length = sfp_get_om2_length,
	.get_om3_length = sfp_get_om3_length,
	.get_om4_copper_length = sfp_get_om4_length_copper_length,
	.get_wave_len =  sfp_get_wavelength,
	.get_passive_cable_compliance = sfp_get_passive_cable_compliance,
	.get_active_cable_compliance = sfp_get_active_cable_compliance,
	.get_cc_base = sfp_get_cc_base,
	.calculate_cc_base = sfp_calculate_cc_base,
	.get_implemented_options = sfp_get_implemented_options,
	.get_max_bit_rate =  sfp_get_max_bit_rate,
	.get_min_bit_rate = sfp_get_min_bit_rate,
	.get_diagnostic_type =  sfp_get_diagnostic_type,
	.get_enhanced_options = sfp_get_enhance_options,
	.get_cc_ext = sfp_get_cc_ext,
	.calculate_cc_ext = sfp_calculate_cc_ext,
	.get_vendor_rom = sfp_get_vendor_rom,
	.get_vendor_rom_size = sfp_get_vendor_rom_size,
	.get_8079_rom = sfp_get_8079_rom,
};
