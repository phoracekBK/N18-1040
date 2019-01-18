#ifndef _IO_MAPPING_H_
#define _IO_MAPPING_H_

#define IO_CART_A1_REF "/dev/comedi4"
#define IO_CART_A2_REF "/dev/comedi5"

#define SO(index)(1<<index)
#define SZ(index)(~(1<<index))



enum _io_card_channel_mapping_
{
	IO_CARD_CHANNEL_INPUT = 0,
	IO_CARD_CHANNEL_OUTPUT = 1,
	IO_CARD_CHANNEL_SIZE
};





/* mapping of input on card A1 */
enum _mapping_a1_in_
{
	A1_IN_0_MBR0 = 0,
	A1_IN_1_MBR1,
	A1_IN_2_RESERVE,
	A1_IN_3_NOT_WORKING,
	A1_IN_4_NOT_WORKING,
	A1_IN_5_FN2,
	A1_IN_6_SN0,
	A1_IN_7_SN1,
	A1_IN_8_SN2,
	A1_IN_9_FN_jam,
	A1_IN_10_TA_BF,
	A1_IN_11_FN0,
	A1_IN_12_FN1,
	A1_IN_13_RESERVE,
	A1_IN_14_RESERVE,
	A1_IN_15_RESERVE,
	A1_IN_SIZE
};


/* mapping of outputs on card A1 */
enum _mapping_a1_out_
{
	A1_OUT_0_BM0 = 0,
	A1_OUT_1_BM1,
	A1_OUT_2_RESERVE,
	A1_OUT_3_PRN_rdy,
	A1_OUT_4_RESERVE,
	A1_OUT_5_PRN_active,
	A1_OUT_6_RESERVE,
	A1_OUT_7_XBF,
	A1_OUT_8_RESERVE,
	A1_OUT_9_TA_cyc,
	A1_OUT_10_ENA,
	A1_OUT_11_RESERVE,
	A1_OUT_12_TI_ins,
	A1_OUT_13_RESERVE,
	A1_OUT_14_INHIBIT_NEXT_SHEET,
	A1_OUT_15_RESERVE,
	A1_OUT_SIZE
};



/* mapping of input on card A2 */
enum _mapping_a2_in_
{
	A2_IN_0_TI_incyc = 0,
	A2_IN_1_RJ_full,
	A2_IN_2_SN_full,
	A2_IN_3_RJ_cnt,
	A2_IN_4_RJ_jam,
	A2_IN_5_camera_trigger,
	A2_IN_6_LNA_E_stop,
	A2_IN_7_RESERVE,
	A2_IN_8_RESERVE,
	A2_IN_9_RESERVE,
	A2_IN_10_RESERVE,
	A2_IN_11_RESERVE,
	A2_IN_12_RESERVE,
	A2_IN_13_RESERVE,
	A2_IN_14_RESERVE,
	A2_IN_15_RESERVE,
	A2_IN_SIZE
};



/* mapping of output on card A2 */
enum _mapping_a2_out_
{
	A2_OUT_0_RESERVE = 0,
	A2_OUT_1_RESERVE,
	A2_OUT_2_RESERVE,
	A2_OUT_3_RESERVE,
	A2_OUT_4_RESERVE,
	A2_OUT_5_RESERVE,
	A2_OUT_6_RESERVE,
	A2_OUT_7_RESERVE,
	A2_OUT_8_RESERVE,
	A2_OUT_9_RESERVE,
	A2_OUT_10_RESERVE,
	A2_OUT_11_RESERVE,
	A2_OUT_12_RESERVE,
	A2_OUT_13_RESERVE,
	A2_OUT_14_RESERVE,
	A2_OUT_15_RESERVE,
	A2_OUT_SIZE
};


#endif
