#include "info_struct.h"


struct _info_struct_;
typedef struct _info_struct_ info_struct;




struct _job_info_
{
	char * csv_addr;
	c_string * order_name;
	c_string* csv_content;
	c_string * csv_name;

	uint32_t total_sheet_number;
	uint32_t total_stamps_number;

	uint32_t printed_sheet_number;
	uint32_t wrong_sheet_number;

	uint32_t feeded_sheet_number;
	uint32_t stacked_sheet_number;
	uint32_t rejected_sheet_number;

	bool end_status;

	/* 
	** contents list of array_lists contents list of array_list variables contains arrays of info_struct variables
	*/
	array_list * job_list;
};




job_info * job_info_new(char * csv_address)
{
	job_info * this = (job_info*) malloc(sizeof(job_info));

	this->csv_addr = (char *) malloc(sizeof(char) * (strlen(csv_address)+1));
	strcpy(this->csv_addr, csv_address);

	this->order_name = c_string_new();
	this->csv_content = c_string_new();
	this->job_list = array_list_new();
	this->csv_name = c_string_new();


	this->total_sheet_number = 0;
	this->total_stamps_number = 0;
	this->printed_sheet_number = 0;
	this->wrong_sheet_number = 0;

	
	this->feeded_sheet_number = 0;
	this->stacked_sheet_number = 0;
	this->rejected_sheet_number = 0;

	this->end_status = true;

	return this;
}


void job_info_set_end_status(job_info * this, bool status)
{
	this->end_status = status;
}

int job_info_get_job_sheet_number(job_info * this, int job_index)
{
	if(job_index < array_list_size(this->job_list))
	{
		array_list * job = array_list_get(this->job_list, job_index);

		if(job != NULL)
			return array_list_size(job);
		else

		return -1;
	}

	return -2;
}


int job_info_get_printed_sheet_number(job_info * this)
{
	return this->printed_sheet_number;
}

/* 
** job index counted from zero 
*/
int job_info_get_job_index(job_info * this)
{
	return (array_list_size(this->job_list) - 1);
}

void job_info_set_order_name(job_info * this, char* order_name)
{
	if((this != NULL) && (order_name != NULL))
		c_string_set_string(this->order_name, order_name);
}

void job_info_clear(job_info * this)
{
	c_string_clear(this->order_name);
	c_string_clear(this->csv_content);
	c_string_clear(this->csv_name);

	this->total_sheet_number = 0;
	this->total_stamps_number = 0;


	this->printed_sheet_number = 0;
	this->wrong_sheet_number = 0;


	this->feeded_sheet_number = 0;
	this->stacked_sheet_number = 0;
	this->rejected_sheet_number = 0;

	this->end_status = true;

	int i, j;
	for(i = 0; i < array_list_size(this->job_list); i++)
	{	
		array_list * job = array_list_get(this->job_list, i);

		if(job != NULL)
		{
			for(j = 0; j < array_list_size(job); j++)
			{
				info_struct * sheet_info = array_list_get(job, j);

				if(sheet_info != NULL)
				{
					if(sheet_info->sheet_order != NULL)
						free(sheet_info->sheet_order);
				
					free(sheet_info);
				}
			}

			array_list_destructor_v2(job);
		}
	}

	array_list_clear(this->job_list);
}	


void job_info_add_job_record(job_info * this)
{
	if(this != NULL)
		array_list_add(this->job_list, array_list_new());
}

void job_info_generate_missing_sheet_records(job_info * this)
{
	job_info_add_job_record(this);
	int k,i,j;

	for(k = 0; k < this->total_sheet_number; k++)
	{
		char sheet_order_str[12];
		sprintf(sheet_order_str, "%d", k+1);		
		array_list * job = NULL;
		bool sheet_printed = false;

		for(i = 0; i < array_list_size(this->job_list)-1; i++)
		{
			job = array_list_get(this->job_list, i);

			if(job != NULL)
			{
				for(j = 0; j < array_list_size(job); j++)
				{
					info_struct * sheet_info = array_list_get(job, j);
					if(sheet_info == NULL)
						printf("sheet info NULL - %d\n", j);

					if(strcmp(sheet_order_str, sheet_info->sheet_order) == 0)
					{
						sheet_printed = true;
						break;		
					}
				}

				if(sheet_printed == true)
					break;				
			}
			else
			{
				break;
			}
		}

		if(sheet_printed == false)
		{
			job_info_add_sheet_record(this, sheet_order_str);
			job_info_set_sheet_record_result(this, "FAIL", k);
		}
	}
}

int8_t job_info_add_sheet_record(job_info * this, char * sheet_order)
{
	if(array_list_size(this->job_list) > 0)
	{
		info_struct * sheet_info = (info_struct*) malloc(sizeof(info_struct));
		sheet_info->sheet_order = (char*) malloc(sizeof(char)*(strlen(sheet_order)+1));
		strcpy(sheet_info->sheet_order, sheet_order);

		sheet_info->result = NULL;

		array_list_add(array_list_get(this->job_list, job_info_get_job_index(this)), sheet_info);

		return 1;
	}
	else
	{
		return -1;
	}
}


void job_info_set_total_sheet_number(job_info * this, int total_sheet_number)
{
	this->total_sheet_number = total_sheet_number;
}

void job_info_set_total_stamp_number(job_info * this, int total_stamp_number)
{
	this->total_stamps_number = total_stamp_number;
}

int job_info_get_total_sheet_number(job_info * this)
{
	return this->total_sheet_number;
}

int job_info_get_total_stamp_number(job_info * this)
{
	return this->total_stamps_number;
}

void job_info_set_sheet_record_result(job_info * this, char * result, int index)
{
	if(index < array_list_size(array_list_get(this->job_list, job_info_get_job_index(this))))
	{
		info_struct * sheet_info = array_list_get(array_list_get(this->job_list, job_info_get_job_index(this)), index);

		if(sheet_info != NULL)
		{
			if(strcmp(result, "PASS") == 0)
				this->printed_sheet_number ++;
			else
				this->wrong_sheet_number ++;

			sheet_info->result = result;
		}
	}
}	


void job_info_set_feeded_sheet_number(job_info * this, uint32_t feeded_sheet_number)
{
	this->feeded_sheet_number = feeded_sheet_number;
}

void job_info_set_stacked_sheet_number(job_info * this, uint32_t stacked_sheet_number)
{
	this->stacked_sheet_number = stacked_sheet_number;
}

void job_info_set_rejected_sheet_number(job_info * this, uint32_t rejected_sheet_number)
{
	this->rejected_sheet_number = rejected_sheet_number;
}


void job_info_set_wrong_sheet_number(job_info * this, uint32_t wrong_sheet_number)
{
	this->wrong_sheet_number = wrong_sheet_number;
}

c_string * job_info_generate_csv(job_info * this, char * time_date_str)
{
	if(array_list_size(this->job_list) > 0)
	{
		c_string_add(this->csv_content, "Job: ");
		c_string_add(this->csv_content, c_string_get_char_array(this->order_name));

		if(this->end_status == true)
			c_string_add(this->csv_content, "\nÚspěšně ukončen\n");
		else
			c_string_add(this->csv_content, "\nPředčasně ukončen\n");

		char str_total_number[10];

		sprintf(str_total_number, "%d\n", this->feeded_sheet_number);		
		c_string_add(this->csv_content, "Celkový počet naložených archů: ");
		c_string_add(this->csv_content, str_total_number);		

		sprintf(str_total_number, "%d\n", this->stacked_sheet_number);
		c_string_add(this->csv_content, "Celkový počet vyložených archů ve vykladači: ");
		c_string_add(this->csv_content, str_total_number);

		sprintf(str_total_number, "%d\n", this->rejected_sheet_number);
		c_string_add(this->csv_content, "Celkový počet špatných archů ve výhybce: ");
		c_string_add(this->csv_content, str_total_number);

		sprintf(str_total_number, "%d\n", this->wrong_sheet_number);
		c_string_add(this->csv_content,  "Celkový počet špatných archů: ");
		c_string_add(this->csv_content, str_total_number);		

		sprintf(str_total_number, "%d\n",  this->total_sheet_number);	
		c_string_add(this->csv_content, "Celkový počet archů: ");
		c_string_add(this->csv_content, str_total_number);

		sprintf(str_total_number,"%d\n", this->total_stamps_number);
		c_string_add(this->csv_content, "Celkový počet kolků: ");
		c_string_add(this->csv_content, str_total_number);

		c_string_add(this->csv_content, "Čas ukončení: ");
		c_string_add(this->csv_content, time_date_str);

		int i,j;
		for(i = 0; i < array_list_size(this->job_list); i++)
		{
			array_list * job = array_list_get(this->job_list, i);
			char order_string[5];
			sprintf(order_string, "%.4d", i+1);
			c_string_add(this->csv_content, "\nPořadí: ");
			c_string_add(this->csv_content, order_string);	

			for(j = 0; j < array_list_size(job); j++)
			{
				info_struct * sheet_info = array_list_get(job, j);
				c_string_add(this->csv_content, "\n");
				c_string_add(this->csv_content, sheet_info->sheet_order);
				c_string_add(this->csv_content, "	");

				if(sheet_info->result != NULL)
					c_string_add(this->csv_content, sheet_info->result);
				else
					c_string_add(this->csv_content, "FAIL");
			}
		}

		return this->csv_content;
	}
	
	return NULL;
}


c_string * job_info_get_csv_name(job_info * this)
{
	return this->csv_name;
}

int8_t job_info_generate_csv_name(job_info * this)
{
	if(this != NULL)
	{
		if(c_string_len(this->order_name) > 0)
		{
			c_string_set_string(this->csv_name, c_string_get_char_array(this->order_name));
			c_string_concat(this->csv_name, ".csv");
	
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}


info_struct * job_info_get_sheet_info(job_info * this, int job_index, int sheet_index)
{
	if(job_index < array_list_size(this->job_list))
	{
		array_list * job = array_list_get(this->job_list, job_index);

		if(job != NULL)
			return array_list_get(job, sheet_index);
		else
			return NULL;
	}
	else
	{
		return NULL;
	}
}

void job_info_finalize(job_info * this)
{
	job_info_clear(this);
	c_string_finalize(&(this->order_name));
	c_string_finalize(&(this->csv_content));
	c_string_finalize(&(this->csv_name));

	if(this->csv_addr != NULL)
		free(this->csv_addr);

	free(this);
}




/****************************************************************** unit tests **************************************************
*********************************************************************************************************************************/

uint8_t job_info_constructor_suite_case()
{
	uint8_t res = 0;

	job_info * this = job_info_new(".");

	res += cu_assert_ptr_not_equal("Memory alocation", this, NULL);
	
	job_info_clear(this);
	res += cu_assert_pass("Try clear empty job info structure, no memory coruption");
	
	c_string *csv_out = job_info_generate_csv(this, "10.01.2019 19:10");
	res += cu_assert_ptr_equal("Try to generate empty sheet list with no jobs and without generated order name and csv file name", csv_out, NULL);

	int8_t csv_generation_res = job_info_generate_csv_name(this);
	res += cu_assert_int_equal("Try generate csv file name without set order name", csv_generation_res, -1);

	job_info_set_order_name(this, "test_file");
	csv_generation_res = job_info_generate_csv_name(this);
	res += cu_assert_int_equal("Try generate csv file name after set order name", csv_generation_res, 1);
	res += cu_assert_string_equal("Check generated csv file name", c_string_get_char_array(this->csv_name),"test_file.csv");

	csv_out = job_info_generate_csv(this,"10.01.2019 19:10");
	res += cu_assert_ptr_equal("Try to generate empty sheet list with no jobs", csv_out, NULL);

	int8_t add_sheet_res = job_info_add_sheet_record(this, "0001");
	res += cu_assert_int_not_equal("Try to add sheet recort to empty job list", add_sheet_res, 1);

	job_info_add_job_record(this);
	add_sheet_res = job_info_add_sheet_record(this, "0001");
	res += cu_assert_int_equal("Try to add sheet recort after adding new job record", add_sheet_res, 1);

	
	job_info_generate_missing_sheet_records(this);
	res += cu_assert_pass("Try to generate missing sheet records without setting number of sheets in job, no memory coruption");

	this->total_sheet_number = 25;
	job_info_generate_missing_sheet_records(this);
	array_list * last_job_record = array_list_get(this->job_list, array_list_size(this->job_list)-1);
	res += cu_assert_pass("Try to generate missing sheet records with setting number of sheets in job, no memory coruption");
	res += cu_assert_int_equal("Check complet sheet records in last job record", array_list_size(last_job_record), this->total_sheet_number);
	
	int missing_sheet_order_str_num = 0;
	for(int i=0; i < array_list_size(array_list_get(this->job_list, array_list_size(this->job_list)-1));i++)
	{
		char temp_str[12];
		sprintf(temp_str,"%d",i+1);
		info_struct * sheet_info = array_list_get(last_job_record,i);
		if((strcmp(temp_str, sheet_info->sheet_order) == 0) && (strcmp(sheet_info->result, "FAIL") == 0))
			missing_sheet_order_str_num++;
	}

	res += cu_assert_int_equal("Check generated missing sheet order strings", array_list_size(last_job_record), missing_sheet_order_str_num);

	csv_out = job_info_generate_csv(this,"10.01.2019 19:10");
	res += cu_assert_ptr_not_equal("Try to generate report csv file with jobs and sheets", csv_out, NULL);

	job_info_clear(this);
	res += cu_assert_int_equal("Try to clear job info structure, job list cleared", array_list_size(this->job_list), 0);
	res += cu_assert_int_equal("Try to clear job info structure, csv content cleared", c_string_len(this->csv_content) , 0);
	res += cu_assert_int_equal("Try to clear job info structure, sheet number variable cleared", this->total_sheet_number, 0);
	res += cu_assert_int_equal("Try to clear job info structure, job list cleared", array_list_size(this->job_list), 0);
	res += cu_assert_int_equal("Try to clear job info structure, job order name cleared", c_string_len(this->order_name), 0);

	job_info_finalize(this);
	res += cu_assert_pass("Memory release, no memory coruption");

	return res;
}

