#include "util.h"

int util_dir_is_empty(char * address)
{
        int res = -1;

	if(address != NULL)
	{
  	      DIR * dir_ref = opendir(address);

  	      if(dir_ref != NULL)
  	      {
  	              struct dirent * dir_cont = NULL;
       	              res = 0;

                	while((dir_cont =readdir(dir_ref)) != NULL)
                	{
                	        if(dir_cont->d_type != DT_DIR)
                       		{
					printf("%s\n", dir_cont->d_name);
                        	        res ++;
                        	}
                	}

                	closedir(dir_ref);
        	}
	}
 	
        return res;
}

uint8_t util_move_file(char * src, char* dest, char* name)
{
	if((src != NULL) && (dest != NULL) && (name != NULL))
	{
		char dest_addr[255];
		char src_addr[255];

		sprintf(dest_addr, "%s/%s", dest, name);
		sprintf(src_addr, "%s/%s", src, name);

		printf("%s -> %s\n", src_addr, dest_addr);
	
		return rename(src_addr, dest_addr);
	}
	else
	{
		return 0;
	}
}

uint8_t util_copy_file(char * src, char * dest, char * name)
{
	if((src != NULL) && (dest != NULL) && (name != NULL))
	{
		FILE  *ptr_src, *ptr_dest;
		int  in;
		char src_addr[255];
		char dest_addr[255];

		sprintf(src_addr, "%s/%s", src, name);
		sprintf(dest_addr, "%s/%s", dest, name);

		printf("%s -> %s\n", src_addr, dest_addr);

		ptr_src = fopen(src_addr, "rb");
		ptr_dest = fopen(dest_addr, "wb");

		if(ptr_src == NULL)
			return  1;

		if(ptr_dest == NULL)
			return  2;

		while((in = fgetc(ptr_src)) != EOF)
		{
			fputc(in, ptr_dest);
		}

		fclose(ptr_src);
		fclose(ptr_dest);

		return  0;
	}
	else
	{
		return 3;
	}
}

void util_delete_file(char* path, char * name)
{
	if(path != NULL && name != NULL)
	{
		char file_addr[255];
		sprintf(file_addr, "%s/%s", path, name);				
		remove(file_addr);
	}
}

/**
** @ingroup Communication
**
*/
int32_t util_save_csv(char* addr, char * name, char* buff)
{
	if((addr != NULL) && (name != NULL) && (buff != NULL))
	{
		char csv_addr[255];
		sprintf(csv_addr, "%s/%s", addr, name);

		FILE * csv_out = fopen(csv_addr, "w");

		if(csv_out != NULL)
		{
			printf("%s\n", csv_addr);
			//printf("csv content - %s\n", buff);
			int32_t size_out = fwrite(buff, sizeof(char), strlen(buff), csv_out);
			fclose(csv_out);

			return size_out;
		}
	}

	return -1;
}

int32_t util_file_size(char * path, char * file_name)
{
	if((path != NULL) && (file_name != NULL))
	{
        	char file_path[255];
        	sprintf(file_path,"%s/%s", path, file_name);

        	struct stat buffer;
        	int status = -1;
        	status = stat(file_path, &buffer);

        	if(status == 0)
        	        return buffer.st_size;
        	else
       		        return 0;
	}
	else
	{
		return 0;
	}
}

/**
** @ingroup Communication
** Function load all content of given csv file and stored it in memory. Memory with csv file content is returned as
** result of the function. Returned memory is nececery released with free function after finish working.
** @param addr address to csv file.
** @param size pointer to size of csv file in bytes as the output parameter
** @return pointer to csv file content or NULL if any problem.
*/
char * util_load_csv(char * addr, char* name, int32_t * size)
{
	if((addr != NULL) && (name != NULL))
	{
		char csv_addr[255];
		sprintf(csv_addr, "%s/%s", addr,name);
    		int file_size = util_file_size(addr, name);

		if(file_size >= 0)
		{
			FILE * csv_in = fopen(csv_addr, "r");

			if (csv_in != NULL)
			{
				char * csv_content = (char *) malloc(sizeof(char)*(file_size+1));

				fread(csv_content, sizeof(char), file_size, csv_in);
				csv_content[file_size] = 0;

				if(size != NULL)
					*size = file_size;
			
				fclose(csv_in);

				return csv_content;
			}
		}	
	}

	return NULL;
}


int32_t util_str_ends_with(char* str, const char * suffix, int offset)
{
	if(!str || !suffix)
		return -1;

	int32_t str_len = strlen(str);
	int32_t suffix_len = strlen(suffix);

	if(str_len-offset >= suffix_len)
		return (int32_t) strncmp(str+str_len-suffix_len-offset, suffix, suffix_len);
	else
		return -1;
}	

char * util_get_time_string()
{
	//time formating
	time_t my_time;
	struct tm* time_info;
	char * time_str = (char*) malloc(sizeof(char)*23);	

	time(&my_time);
	time_info = localtime(&my_time);
	
	strftime(time_str, 22, "%d/%m/%y - %H:%M:%S", time_info);

	return time_str;
}


wchar_t * util_str_to_wstr(char * str)
{
	int len = strlen(str)+1;
	wchar_t * wstr = (wchar_t*) malloc(sizeof(wchar_t)*len);

	swprintf(wstr, len, L"%hs", str);

	return wstr;
}

char * util_wstr_to_str(wchar_t * wstr)
{
	int len = wcslen(wstr);
	char * str = (char*) malloc(sizeof(char)*len);	
	char * str_ref = str;

	for(int index = 0; index < len; index++)
	{
		int i = wctomb(str_ref, wstr[index]);
		str_ref += (sizeof(char)*i);
	}

	*str_ref = 0;

	return str;
}

void util_replace_characters(wchar_t * input, wchar_t * char_to_replace, wchar_t * new_char)
{
	int in_index = 0;
	int out_index = 0;

	wchar_t * apos = util_str_to_wstr("\"");

	while(input[in_index] != 0)
	{
		uint8_t replace_index = 0;

		while(char_to_replace[replace_index] != 0)
		{
			if(input[out_index] == char_to_replace[replace_index])
			{
				input[out_index] = new_char[replace_index];
			}

			replace_index++;
		}

		if(input[in_index] != *apos)
		{
			input[out_index] = input[in_index];
			out_index++;
		}
		
		in_index++;
	}

	input[out_index] = 0;

	if(apos != NULL)
		free(apos);
}










