
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <dirent.h>
#include <features.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <array_list.h>

/* global variables */
array_list * front_name;
array_list * front_size;


/* functions declarations */
int32_t save_csv(char* addr, char * name, char* buff);
char * load_csv(char * addr, char* name, int32_t * size);
int32_t str_ends_with(char* str, const char * suffix, int offset);
double modify_csv(char * src, char * dest, char * name);
void delete_file(char* path, char * name);
void read_hotfolder(char * src, char *dest);
int32_t file_size(char * path, char *file_name);

/* functions definitions */

int32_t file_size(char * path, char * file_name)
{
	char file_path[255];
	sprintf(file_path,"%s%s", path, file_name);

	struct stat buffer;
	int status;

	status = stat(file_path, &buffer);
	if(status == 0)
	{
		return buffer.st_size;
	}
	else
	{
		return 0;
	}
}

int32_t save_csv(char* addr, char * name, char* buff)
{
        char csv_addr[255];
        sprintf(csv_addr, "%s%s", addr, name);

        FILE * csv_out = fopen(csv_addr, "w");

        if(csv_out != NULL)
        {
                int32_t size_out = fwrite(buff, sizeof(char), strlen(buff), csv_out);
                fclose(csv_out);
                return size_out;
        }

        return -1;
}

char * load_csv(char * addr, char* name, int32_t * size)
{
        char csv_addr[255];
        sprintf(csv_addr, "%s%s", addr,name);

	int32_t csv_size = file_size(addr, name);

        FILE * csv_in = fopen(csv_addr, "r");

        if (csv_in != NULL)
         {
                 char * csv_content = (char *) malloc(sizeof(char)*(csv_size+1));

                 fread(csv_content, sizeof(char), csv_size, csv_in);
                 csv_content[csv_size] = 0;

                 if(size != NULL)
                        *size = csv_size;

                 fclose(csv_in);
                 return csv_content;
        }

        return NULL;
}

int32_t str_ends_with(char* str, const char * suffix, int offset)
{
        if(!str || !suffix)
                return -1;

        int32_t str_len = strlen(str);
        int32_t suffix_len = strlen(suffix);

        if(str_len-offset >= suffix_len)
        {
                return (int32_t) strncmp(str+str_len-suffix_len-offset, suffix, suffix_len);
        }
        else
        {
                return -1;
        }
}


double modify_csv(char * src, char * dest, char * name)
{
	int32_t out_size = 0;
	char * input_csv = load_csv(src, name, &out_size);
	int32_t percent = 0;
	int line = 0;
	
	if(input_csv != NULL)
	{
		srand(time(NULL));

		int out_csv_pos = 0;
		int in_csv_pos = 0;
		char * output_csv = (char*) malloc(sizeof(char) * (out_size+1));
		uint8_t write = rand() % 2;		
		int pos_to_write = rand()%200;
		int cur_pos_to_write = 0;

		while(input_csv[in_csv_pos] != 0)
		{
			if(write > 0 )
			{
				output_csv[out_csv_pos] = 'X';
				write = 0;
			}
			else
			{
				output_csv[out_csv_pos] = input_csv[in_csv_pos];
			}

			out_csv_pos++;

			if(input_csv[in_csv_pos] == '\n')
			{
				line ++;
				write = rand() % 2;
				if(write > 0)
					percent++;
			}

			in_csv_pos ++;
		}

		save_csv(dest, name, output_csv);
		free(input_csv);
		free(output_csv);
	}
	else
	{
		printf("Can't open csv file\n");
	}

	if(percent == 0)
		return 0.0;
	else
		return ((double)((double) percent)/((double) line))*100.0;
}


void delete_file(char* path, char * name)
{
        char file_addr[255];
        strcpy(file_addr, path);
        strcat(file_addr, name);
        remove(file_addr);
}

int find_file(char * name)
{
	int i = 0;
	while(i < array_list_size(front_name))
	{
		if(strcmp(array_list_get(front_name,i),name) == 0)
		{
			return i;
		}
		i++;
	}

	return -1;
}


void read_hotfolder(char * src, char *dest)
{
 	DIR * dir_ref = opendir(src);

	int i = 0;
	if(dir_ref != NULL)
	{
		struct dirent * dir_cont;
		while((dir_cont = readdir(dir_ref)) != NULL)
                {
                        if(dir_cont->d_type != DT_DIR)
                        {
				int index = find_file(dir_cont->d_name);
				int32_t file_size_current = file_size(src, dir_cont->d_name);

				if(index >= 0)
				{
					if((file_size_current > 0) && (*((int32_t*)array_list_get(front_size, index)) == file_size_current))
					{
						if(str_ends_with(dir_cont->d_name, ".csv", 0) == 0)
						{
							usleep(500000);
							double success = modify_csv(src, dest, dir_cont->d_name);
							printf("file %s processed with %lf%% success.\n", dir_cont->d_name, success);
						}
						else
						{		
							printf("Unrecognized file \'%s\', deleting!\n", dir_cont->d_name);
						}
	
						delete_file(src, dir_cont->d_name);
						array_list_remove_with_release(front_name, index, free);
						array_list_remove_with_release(front_size, index, free);
					}
					else
					{
						int32_t * new_size = array_list_get(front_size, index);
						*new_size = file_size_current;
						printf("update the size of file %s = %d\n", dir_cont->d_name, file_size_current);
					}
				}
				else
				{
					printf("add new file to front: %s\n", dir_cont->d_name);
					char * name =(char*) malloc(sizeof(char)*(strlen(dir_cont->d_name)+1));
					strcpy(name, dir_cont->d_name);
					array_list_add(front_name, name);
					int32_t * size = (int32_t *) malloc(sizeof(int32_t));
					*size = file_size_current;
					array_list_add(front_size, size);
				}
				i++;
			}
			else
			{
				if(strcmp(dir_cont->d_name, ".") && strcmp(dir_cont->d_name, ".."))
					printf("not e regular gile %s\n", dir_cont->d_name);
			}
		}

		if(i > 0)
			printf("readed %d items\n", i);

		closedir(dir_ref);
	}
	else
	{
		printf("cant read hotfolder\n");
	}
}

int main(int argv, char ** argc)
{
	if(argv >= 3)
	{
		front_name = array_list_new();
		front_size = array_list_new();

		printf("PCI simulation run.\n");

		while(1)
		{
			read_hotfolder(argc[1], argc[2]);
			sleep(1);
		}
	}
	else
	{
		printf("No hotfolder address specified!\n");
	}
	
	return 0;
}

