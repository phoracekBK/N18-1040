#ifndef _IO_CARD_H_
#define _IO_CARD_H_

#include "io_mapping.h"



#include <c_freq.h>
#include <comedilib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/time.h>
#include <math.h>

struct _io_card_;
typedef struct _io_card_ io_card;


io_card * io_card_new();
void io_card_sync_inputs(io_card * this);
void io_card_sync_outputs(io_card * this);
uint8_t io_card_get_input(io_card * this, int card, int addr);
uint8_t io_card_get_output(io_card * this, int card, int addr);
void io_card_set_output(io_card * this, int card, int addr, uint8_t val);
uint8_t io_card_get_bit_value(io_card * this, int card, int bit1, int bit2, int bit3);
const char ** io_card_get_a1_in_labels();
const char ** io_card_get_a1_out_labels();
const char ** io_card_get_a2_in_labels();
const char ** io_card_get_a2_out_labels();
void io_card_finalize(io_card * this);




#endif
