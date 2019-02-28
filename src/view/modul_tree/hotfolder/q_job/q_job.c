#include "q_job.h"


struct _q_job_
{
	char * job_name;
	char * bkcore_name;
	char * camera_name;
	char * pdf_name;
	char flag;
	int order;

	char * nominal;
	char * order_name;
	char * stamp_type;

	int sheet_number;
	int stamp_number;
	char * date_time;
};


q_job * q_job_new(char* name, uint32_t order, char flag, char * date_time)
{
	q_job * job = (q_job*) malloc(sizeof(q_job));
	job->job_name = name;
	job->camera_name = NULL;
	job->pdf_name = NULL;
	job->bkcore_name = NULL;
	job->order = order;
	job->flag = flag;


	job->nominal = NULL;
	job->order_name = NULL;
	job->stamp_type = NULL;


	job->sheet_number = 0;
	job->stamp_number = 0;
	job->date_time = date_time;

	return job;
}


char * q_job_get_job_name(q_job * job)
{
	return job->job_name;
}

char * q_job_get_pdf_name(q_job * job)
{
	return job->pdf_name;
}

char * q_job_get_camera_csv_name(q_job * job)
{
	return job->camera_name;
}

char * q_job_get_bkcore_csv_name(q_job * job)
{
	return job->bkcore_name;
}

char q_job_get_flag(q_job * job)
{
	return job->flag;
}

int q_job_get_job_order(q_job * job)
{
	return job->order;
}

char * q_job_get_date_time(q_job * job)
{
	return job->date_time;
}

int q_job_get_sheet_number(q_job * job)
{
	return job->sheet_number;
}	

int q_job_get_stamp_number(q_job * job)
{
	return job->stamp_number;
}

void q_job_set_stamp_number(q_job * job, int stamp_number)
{
	job->stamp_number = stamp_number;
}

void q_job_set_sheet_number(q_job * job, int sheet_number)
{
	job->sheet_number = sheet_number;
}


char * q_job_get_nominal(q_job * job)
{
	return job->nominal;
}

char * q_job_get_stamp_type(q_job * job)
{
	return job->stamp_type;
}

char * q_job_get_order_name(q_job * job)
{
	return job->order_name;
}


void q_job_set_nominal(q_job * job, char * nominal)
{
	if(job->nominal != NULL)
		free(job->nominal);

	job->nominal = (char *) malloc(sizeof(char) * (strlen(nominal)+1));
	strcpy(job->nominal, nominal);
}

void q_job_set_order_name(q_job * job, char * order_name)
{
	if(job->order_name != NULL)
		free(job->order_name);

	job->order_name = (char*) malloc(sizeof(char*) * (strlen(order_name)+1));
	strcpy(job->order_name, order_name);
}

void q_job_set_stamp_type(q_job * job, char * stamp_type)
{
	if(job->stamp_type != NULL)
		free(job->stamp_type);

	job->stamp_type = (char*) malloc(sizeof(char) * (strlen(stamp_type)+1));
	strcpy(job->stamp_type, stamp_type);
}

void q_job_set_pdf_name(q_job * job, char * pdf_name)
{
	if(job->pdf_name != NULL)
		free(job->pdf_name);

	job->pdf_name = malloc(sizeof(char)* (strlen(pdf_name)+1));
	strcpy(job->pdf_name, pdf_name);
}

void q_job_set_camera_csv_name(q_job * job, char * camera_csv_name)
{
	if(job->camera_name != NULL)
		free(job->camera_name);

	job->camera_name = malloc(sizeof(char)*(strlen(camera_csv_name)+1));
	strcpy(job->camera_name, camera_csv_name);
}

void q_job_set_bkcore_csv_name(q_job * job, char * bkcore_csv_name)
{
	if(job->bkcore_name != NULL)
		free(job->bkcore_name);
	
	job->bkcore_name = malloc(sizeof(char)*(strlen(bkcore_csv_name)+1));
	strcpy(job->bkcore_name, bkcore_csv_name);
}



uint8_t q_job_compare(q_job * job_1, q_job * job_2)
{
	if((job_1 != NULL) && (job_2 != NULL))
	{
		if(strcmp(job_1->job_name, job_2->job_name) == 0)
		{
			if(job_1->order == job_2->order && job_1->flag == job_2->flag)
			{
				return 1;
			}
		}
	}

	return 0;
}

q_job * q_job_copy(q_job * job)
{
	uint8_t str_len = strlen(job->job_name)+1;

	char * job_name  = (char*) malloc(sizeof(char)*str_len);
	memcpy(job_name, job->job_name, str_len);

	char * date_time = NULL;

	if(job->date_time != NULL)
	{
		str_len = strlen(job->date_time);
		date_time = malloc(sizeof(char)*(str_len+1));
		memcpy(date_time, job->date_time, str_len);
	}
	
	q_job * new_job = q_job_new(job_name, job->order, job->flag, date_time);

	if(job->camera_name != NULL)
	{
		str_len = strlen(job->camera_name)+1;
		new_job->camera_name = (char*) malloc(sizeof(char) * str_len);
		memcpy(new_job->camera_name, job->camera_name, str_len);
	}

	if(job->pdf_name != NULL)
	{
		str_len = strlen(job->pdf_name)+1;
		new_job->pdf_name = (char*) malloc(sizeof(char) * str_len);
		memcpy(new_job->pdf_name, job->pdf_name, str_len);
	}

	if(job->bkcore_name != NULL)
	{
		str_len = strlen(job->bkcore_name)+1;
		new_job->bkcore_name = (char*) malloc(sizeof(char)*str_len);
		memcpy(new_job->bkcore_name, job->bkcore_name, str_len);
	}

	if(job->nominal != NULL)
	{
		str_len = strlen(job->nominal)+1;
		new_job->nominal = (char*) malloc(sizeof(char)*str_len);
		memcpy(new_job->nominal, job->nominal, str_len);
	}

	if(job->order_name != NULL)
	{
		str_len = strlen(job->order_name);
		new_job->order_name = (char*) malloc(sizeof(char)*str_len);
		memcpy(new_job->order_name, job->order_name, str_len);
	}

	if(job->stamp_type != NULL)
	{
		str_len = strlen(job->stamp_type)+1;
		new_job->stamp_type  = (char*) malloc(sizeof(char)*str_len);
		memcpy(new_job->stamp_type, job->stamp_type, str_len);
	}

	new_job->sheet_number = job->sheet_number;
	new_job->stamp_number = job->stamp_number;

	return new_job;
}


void q_job_finalize(void * job)
{	
	q_job * job_ref = (q_job*) job;

	if(job_ref->job_name != NULL)
	{
		free(job_ref->job_name);
		job_ref->job_name = NULL;
	}	
	
	if(job_ref->camera_name != NULL)
	{
		free(job_ref->camera_name);
		job_ref->camera_name = NULL;
	}

	if(job_ref->pdf_name != NULL)
	{
		free(job_ref->pdf_name);
		job_ref->pdf_name = NULL;
	}
	
	if(job_ref->bkcore_name != NULL)
	{
		free(job_ref->bkcore_name);
		job_ref->bkcore_name = NULL;
	}

	if(job_ref->date_time != NULL)
	{	
		free(job_ref->date_time);
		job_ref->date_time = NULL;
	}

	if(job_ref->nominal != NULL)
	{
		free(job_ref->nominal);
		job_ref->nominal = NULL;
	}

	if(job_ref->order_name != NULL)
	{
		free(job_ref->order_name);
		job_ref->order_name = NULL;
	}

	if(job_ref->stamp_type != NULL)
	{
		free(job_ref->stamp_type);
		job_ref->stamp_type = NULL;
	}

	free(job);
}
