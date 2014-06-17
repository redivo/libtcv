#include "libtcv/xfp.h"
#include "libtcv/tcv.h"

/**
 * XFP specific member fuctions,
 * nothing implemented, default to tcv_xxx prototypes
 */
const struct tcv_functions xfp_funcs = {
	.get_itendifier = tcv_get_identifier,
	.get_vendor_name = tcv_get_vendor_name,
	.get_vendor_part_number = tcv_get_vendor_part_number,
	.get_vendor_serial_number = tcv_get_vendor_sn,
	.get_vendor_date_code = tcv_get_vendor_date_code,
};
