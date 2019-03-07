#ifndef _MACHINE_STATISTIC_H_
#define _MACHINE_STATISTIC_H_


#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>


struct _statistic_day_;
typedef struct _statistic_day_ statistic_day;

struct _machine_statistic_;
typedef struct _machine_statistic_ machine_statistic;


machine_statistic * machine_statistic_new();
void machine_statistic_increment_feeded_sheets(machine_statistic * this);
void machine_statistic_increment_stacked_sheets(machine_statistic * this);
void machine_statistic_increment_rejected_sheets(machine_statistic * this);

uint64_t machine_statistic_get_feeded_sheets(machine_statistic * this, int8_t day);
uint64_t machine_statistic_get_stacked_sheets(machine_statistic * this, int8_t day);
uint64_t machine_statistic_get_rejected_sheets(machine_statistic * this, int8_t day);
double machine_statistic_get_error_rate(machine_statistic * this, int8_t day);

void machine_statistic_clear_day(machine_statistic * this, int8_t day);

void machine_statistic_finalize(machine_statistic ** this);

#endif
