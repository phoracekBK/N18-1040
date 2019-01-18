#include "hotfolder.h"

/* global constants and variables */
const char* available_formats[2] = {".pdf", ".csv"};
const char* available_types_pdf[1] = {"_pdf"};
const char* available_types_csv[2] = {"_camera", "_bkcore"};
const char* available_flags[3] = {"_p", "_k", "_e"};



/* static functions declarations */
void hot_bkcore_csv_info(q_job * job, char * address);
int8_t hot_check_filename_param(char* filename, const char** params, int param_number, int offset);
int8_t hot_parse_file_name(array_list * job_list, char * file_name, c_string * q_hotfolder_main_path);
int32_t hot_get_order(char* str, int offset, int len);
void hot_bkcore_csv_info(q_job * job, char * address);






array_list * hot_read_dir_content(c_string * q_hotfolder_main_path)
{
	DIR * dir_ref = opendir(c_string_get_char_array(q_hotfolder_main_path));
	
	array_list * job_list = array_list_new();

	if(dir_ref != NULL)
	{
		struct dirent * dir_cont;
		while((dir_cont = readdir(dir_ref)) != NULL)
		{
			if(dir_cont->d_type != DT_DIR)
			{
				if (hot_parse_file_name(job_list, dir_cont->d_name, q_hotfolder_main_path) == 0)
				{
					util_delete_file(c_string_get_char_array(q_hotfolder_main_path), dir_cont->d_name);					
				}
			}
		}

		closedir(dir_ref);
	}

	return job_list;
}


array_list * hot_copy_job_list(array_list * job_list)
{
	array_list * job_list_pre = array_list_new();
	
	for(int i = 0;i < array_list_size(job_list); i++)
	{
		array_list_add(job_list_pre, q_job_copy(array_list_get(job_list, i)));
	}

	return job_list_pre;
}


uint8_t hot_compare_job_lists(array_list * job_list, array_list * job_list_pre)
{
	if((job_list_pre == NULL) || (array_list_size(job_list) != array_list_size(job_list_pre)))
	{
		return 1;
	}
	else
	{
		for(int i = 0; i < array_list_size(job_list); i++)
		{
			if(q_job_compare(array_list_get(job_list,i), array_list_get(job_list_pre, i)) == 0)
				return 1;
		}
	}

	return 0;
}


int8_t hot_check_filename_param(char* filename, const char** params, int param_number, int offset)
{
	int i = 0;

	while(i < param_number)
	{
		if(util_str_ends_with(filename, params[i], offset) == 0)
			return i;

		i++;
	}

	return -1;
}


q_job * hot_find_job(array_list * job_list, char * name, int32_t *index)
{
	for(int i = 0; i < array_list_size(job_list); i++)
	{
		q_job * job = array_list_get(job_list, i);
		if(strcmp(q_job_get_job_name(job), name) == 0)
		{	
			if(index != NULL)
				*index = i;

			return job;
		}
	}

	return NULL;
}


int32_t hot_get_order(char* str, int offset, int len)
{
	int str_len = strlen(str);

	if(str_len-offset >= len)
	{
		char str_num[32];
		strncpy(str_num, str+str_len-offset-len, len);
		str_num[len] = 0;
		int32_t res = 0;
		str = (str+str_len-offset-len);
	
		int i;
		for(i = 0; i < len; i++)
		{
			if(isdigit(*(str+i)) == 0)
			{	
				return -1;
			}

			res = res * 10 + (str_num[i] - '0');
		}	
				
		return res;
	}
	else
	{
		return -1;
	}
}


int8_t hot_parse_file_name(array_list * job_list, char * file_name, c_string * q_hotfolder_main_path)
{
	int8_t offset = 0;
	int8_t file_format = hot_check_filename_param(file_name, available_formats, 2, offset);

	/* pdf or csv */			
	if(file_format == 0 || file_format == 1)
	{
		int8_t file_type = -1; 
 		offset += strlen(available_formats[file_format]); //length of substring .pdf or .csv

		if(file_format == 0)
		{
			file_type = hot_check_filename_param(file_name, available_types_pdf, 1, offset);
			if(file_type == 0)
				offset += strlen(available_types_pdf[file_type]);
		}
		else
		{
			file_type = hot_check_filename_param(file_name, available_types_csv, 2, offset);
			if((file_type == 0) || (file_type == 1))
				offset += strlen(available_types_csv[file_type]);
		}

		if(((file_format == 0) && (file_type == 0)) || ((file_format == 1) && ((file_type == 0) || (file_type == 1))))
		{
			int8_t flag_param = hot_check_filename_param(file_name, available_flags, 3, offset);
				
			if(flag_param >= 0 && flag_param < 3)
			{
				offset += strlen(available_flags[flag_param]);
				int16_t job_order = hot_get_order(file_name, offset, 4);
						
				if(job_order > 0)
				{
					offset += 5;
					int8_t job_name_len = strlen(file_name)-offset;
								
					if(job_name_len > 0)
					{
						char * job_name = (char*) malloc(sizeof(char)*(job_name_len+1));
						memset(job_name, 0, job_name_len+1);
						strncpy(job_name, file_name, job_name_len);
									
						q_job * job =  hot_find_job(job_list, job_name, NULL);

						if(job == NULL)
						{
							char flag = available_flags[flag_param][1];	
							job = q_job_new(job_name, job_order, flag, util_get_time_string());
							array_list_add(job_list, job);
						}
						else
						{
							free(job_name);
						}

						if(file_format == 0)
						{
							q_job_set_pdf_name(job, file_name);
						}
						else
						{
							if(file_type == 0)
							{
								q_job_set_camera_csv_name(job, file_name);
							}
							else
							{
								q_job_set_bkcore_csv_name(job, file_name);
							
								if(q_job_get_flag(job) == 'k')
									hot_bkcore_csv_info(job, c_string_get_char_array(q_hotfolder_main_path));
							}
						}

						return 1;
					}
				}
			}
		}
	}

	return 0;	
}



void hot_bkcore_csv_info(q_job * job, char * address)
{
	/* open and read csv */
	char * bkcore_csv_content = util_load_csv(address, q_job_get_bkcore_csv_name(job), NULL);
	
	if(bkcore_csv_content != NULL)
	{
		/* read sheet and stamp number */
		int sheet_number = 0;
		int stamp_number = 0;
		int row = 0;
		int pos = 0;

		while((row < 2) && (bkcore_csv_content[pos] != 0))
		{
			if(bkcore_csv_content[pos] == '\n')
			{
				row++;
			}
	
			if(isdigit(bkcore_csv_content[pos]))
			{
				if(row == 0)
				{
					stamp_number = (stamp_number*10) + (bkcore_csv_content[pos]-48);
				}
				else if(row == 1)
				{
					sheet_number = (sheet_number * 10) + (bkcore_csv_content[pos]-48);
				}
			}

			pos++;
		}

		q_job_set_sheet_number(job, sheet_number);
		q_job_set_stamp_number(job, stamp_number);

		free(bkcore_csv_content);
	}
}



