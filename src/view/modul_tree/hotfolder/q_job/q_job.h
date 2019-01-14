#ifndef _Q_JOB_H_
#define _Q_JOB_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#include <c_string.h>
#include <array_list.h>


struct _q_job_;
typedef struct _q_job_ q_job;



q_job * q_job_new(char* name, uint32_t order, char flag, char * date_time);
q_job * q_job_copy(q_job * job);
uint8_t q_job_compare(q_job * job_1, q_job * job_2);
void q_job_finalize(void* job);
char * q_job_get_job_name(q_job * job);
char * q_job_get_pdf_name(q_job * job);
char * q_job_get_camera_csv_name(q_job * job);
char * q_job_get_bkcore_csv_name(q_job * job);
char q_job_get_flag(q_job * job);
int q_job_get_job_order(q_job * job);
char * q_job_get_date_time(q_job * job);
int q_job_get_sheet_number(q_job * job);
int q_job_get_stamp_number(q_job * job);
void q_job_set_stamp_number(q_job * job, int stamp_number);
void q_job_set_sheet_number(q_job * job, int sheet_number);
void q_job_set_pdf_name(q_job * job, char * pdf_name);
void q_job_set_camera_csv_name(q_job * job, char * camera_csv_name);
void q_job_set_bkcore_csv_name(q_job * job, char * bkcore_csv_name);

#endif
