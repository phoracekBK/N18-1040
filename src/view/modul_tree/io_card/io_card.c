#include "io_card.h"
#include "io_card_const.h"

struct _io_card_
{
	comedi_t *ref_a1;
	comedi_t *ref_a2;

	uint8_t *** io_ref;

	unsigned int A1_in;
	unsigned int A1_out;
	unsigned int A2_in;
	unsigned int A2_out;

#if IO_CARD_SIMAULATION_EN == TRUE
	pthread_t simulation_thread;
#endif
	int64_t timer;
};



const char * a1_in_labels[16] = {"MBR0", 				/* IN 0 */
				"MBR1",					/* IN 1 */
				"RESERVE",				/* IN 2 */
				"Non active",				/* IN 3 */
				"Non acvite",				/* IN 4 */
				"FN2",					/* IN 5 */
				"SN0",					/* IN 6 */
				"SN1",					/* IN 7*/
				"SN2",					/* IN 8 */
				"FN_jam",				/* IN 9 */
				"TA_BF",				/* IN 10 */
				"FN0",					/* IN 11 */
				"FN1",		   			/* IN 12 */
				"RESERVE",				/* IN 13 */
				"RESERVE",				/* IN 14 */
				"RESERVE"};				/* IN 15 */

const char * a1_out_labels[16] = {"MB0",				/* OUT 0 */
				"MB1",					/* OUT 1 */
				"RESERVE",				/* OUT 2 */
				"PRN_rdy",				/* OUT 3 */
				"RESERVE",				/* OUT 4 */
				"PRN_active",				/* OUT 5 */
				"RESERVE",				/* OUT 6 */
				"XBF",					/* OUT 7 */
				"RESERVE",				/* OUT 8 */
				"TA_cyc",				/* OUT 9 */
				"ENA",					/* OUT 10 */
				"RESERVE",				/* OUT 11 */
				"TI_ins",				/* OUT 12 */
				"RESERVE",				/* OUT 13 */
				"INHIBIT_NEXT_SHEET",			/* OUT 14 */
				"RESERVE"};				/* OUT 15 */

const char * a2_in_labels[16] = {"TI_incyc",				/* IN 0 */
				"RJ_full",				/* IN 1 */
				"SN_full",				/* IN 2 */
				"RJ_cnt", 				/* IN 3 */
				"RJ_jam",				/* IN 4 */
				"camera_trigger",			/* IN 5 */
				"LNA_E_STOP",				/* IN 6 */
				"RESERVE",				/* IN 7 */
				"RESERVE",				/* IN 8 */
				"RESERVE",				/* IN 9 */
				"RESERVE",				/* IN 10 */
				"RESERVE",				/* IN 11 */
				"RESERVE",				/* IN 12 */
				"RESERVE",				/* IN 13 */
				"RESERVE",				/* IN 14 */
				"RESERVE"};				/* IN 15 */


const char * a2_out_labels[16] = {"RESERVE",				/* OUT 0 */
				"RESERVE",				/* OUT 1 */
				"RESERVE",				/* OUT 2 */
				"RESERVE",				/* OUT 3 */
				"RESERVE",				/* OUT 4 */
				"RESERVE",				/* OUT 5 */
				"RESERVE",				/* OUT 6 */
				"RESERVE",				/* OUT 7 */
				"RESERVE",				/* OUT 8 */
				"RESERVE",				/* OUT 9 */
				"RESERVE",				/* OUT 10 */
				"RESERVE",				/* OUT 11 */
				"RESERVE",				/* OUT 12 */
				"RESERVE",				/* OUT 13 */
				"RESERVE",				/* OUT 14 */
				"RESERVE"};				/* OUT 15 */





io_card * io_card_new()
{
	io_card * this = (io_card*) malloc(sizeof(io_card));
	
	this->timer = c_freq_millis();

	/* connect cards */
	this->ref_a1 = comedi_open(IO_CART_A1_REF);
	this->ref_a2 = comedi_open(IO_CART_A2_REF);

	if(((this->ref_a1 != NULL) && (this->ref_a2 != NULL)))
	{
		this->A1_in = 0;
		this->A1_out = 0;
		this->A2_in = 0;
		this->A2_out = 0;	

		return this;
	}
	else
	{
		fputs("Can't connect IO cards\n", stderr);
		free(this);
		return NULL;
	}
}	


const char ** io_card_get_a1_in_labels()
{
	return a1_in_labels;
}

const char ** io_card_get_a1_out_labels()
{
	return a1_out_labels;
}

const char ** io_card_get_a2_in_labels()
{
	return a2_in_labels;
}

const char ** io_card_get_a2_out_labels()
{
	return a2_out_labels;
}


void io_card_sync_inputs(io_card * this)
{
	if(comedi_dio_bitfield2(this->ref_a1, IO_CARD_CHANNEL_INPUT, 0x0, &this->A1_in, 0) < 0)
	{
		fputs("Can't read inputs from card A1!\n", stderr);
	}

	if(comedi_dio_bitfield2(this->ref_a2, IO_CARD_CHANNEL_INPUT, 0x0, &this->A2_in, 0) < 0)
	{
		fputs("Can't read inputs from card A2!\n", stderr);
	} 
}

void io_card_sync_outputs(io_card * this)
{
	if(comedi_dio_bitfield2(this->ref_a1, IO_CARD_CHANNEL_OUTPUT, 0xFFFF, &this->A1_out, 0) < 0)
	{
		fputs("Can't write utputs to card A1!\n", stderr);
	}

	if(comedi_dio_bitfield2(this->ref_a2, IO_CARD_CHANNEL_OUTPUT, 0xFFFF, &this->A2_out, 0) < 0)
	{
		fputs("Can't write utputs to card A2!\n", stderr);
	}
}

uint8_t io_card_get_output(io_card * this, int card, int addr)
{
	if(card == IO_CARD_A1)
	{
		return ((this->A1_out & SO(addr)) > 0 ? 1 : 0);
	}
	else
	{
		return ((this->A2_out & SO(addr)) > 0 ? 1 : 0);
	}
}


uint8_t io_card_get_input(io_card * this, int card, int addr)
{
	/* reverse logic 0 = 1 */
	if(card == IO_CARD_A1)
	{
		return ((this->A1_in & SO(addr)) > 0 ? 0 : 1);
	}
	else
	{
		return ((this->A2_in & SO(addr)) > 0 ? 0 : 1);
	}
}

void io_card_set_output(io_card * this, int card, int addr, uint8_t val)
{
	if(card == IO_CARD_A1)
	{
		if(val > 0)
			this->A1_out |= SO(addr);
		else
			this->A1_out &= SZ(addr);
	}
	else
	{
		if(val > 0)
			this->A2_out |= SO(addr);
		else
			this->A2_out &= SZ(addr);
	}
}


uint8_t io_card_get_bit_value(io_card * this, int card, int bit1, int bit2, int bit3)
{
	int bit_val = 0;

	if(io_card_get_input(this, card, bit1) > 0)
		bit_val += 1;

	if(io_card_get_input(this, card, bit2) > 0)
		bit_val += 2;

	if(io_card_get_input(this, card, bit3) > 0)
		bit_val += 4;

	return bit_val;
}

void io_card_finalize(io_card * this)
{
	this->A1_out = 0;
	this->A2_out = 0;

	comedi_dio_bitfield2(this->ref_a1, IO_CARD_CHANNEL_OUTPUT, 0xFFFF, &this->A1_out, 0);
	comedi_dio_bitfield2(this->ref_a2, IO_CARD_CHANNEL_OUTPUT, 0xFFFF, &this->A2_out, 0);

	comedi_close(this->ref_a1);
	comedi_close(this->ref_a2);

	free(this);
}

