#ifndef _INFO_STRUCT_H_
#define _INFO_STRUCT_H_


#include <c_string.h>
#include <array_list.h>
#include <c_unit_lib.h>

#include <features.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/time.h>
#include <math.h>


struct _job_info_;
typedef struct _job_info_ job_info;


struct _info_struct_;
typedef struct _info_struct_ info_struct;




struct _info_struct_
{
	char * sheet_order;
	char * result;
};


job_info * job_info_new(char * csv_address);
int job_info_get_job_index(job_info * this);
int job_info_get_printed_sheet_number(job_info * this);
void job_info_set_order_name(job_info * this, char* order_name);
void job_info_clear(job_info * this);
void info_struct_finalize(void * this);
void job_info_add_job_record(job_info * this);
void job_info_generate_missing_sheet_records(job_info * this);
int8_t job_info_add_sheet_record(job_info * this, char * sheet_order);
void job_info_set_sheet_record_result(job_info * this, char * result, int index);
c_string * job_info_generate_csv(job_info * this, char * time_date_st);
int8_t job_info_generate_csv_name(job_info * this);
c_string * job_info_get_csv_name(job_info * this);
void job_info_finalize();
void job_info_set_total_sheet_number(job_info * this, int total_sheet_number);
void job_info_set_total_stamp_number(job_info * this, int total_stamp_number);
int job_info_get_total_sheet_number(job_info * this);
int job_info_get_total_stamp_number(job_info * this);
int job_info_get_job_sheet_number(job_info * this, int job_index);
void job_info_set_end_status(job_info * this, bool status);
info_struct * job_info_get_sheet_info(job_info * this, int job_index, int sheet_index);

uint8_t job_info_constructor_suite_case();


#endif
