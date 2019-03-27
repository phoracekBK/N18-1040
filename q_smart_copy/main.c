#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <dirent.h>


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


int main(int argv, char ** argc)
{
	HWND var = GetConsoleWindow();

	if(argv >= 4)
	{
		ShowWindow(var, SW_HIDE);

		while(1)
		{
			WIN32_FIND_DATA fdFile; 
			HANDLE hFind = NULL; 


			DIR * dir_ref = opendir(argc[0]);

			if(dir_ref != NULL)
			{
				struct dirent * dir_cont;

				while((dir_cont = readdir(dir_ref)) != NULL)
				{
					if(dir_cont->d_type != DT_DIR)
					{
						util_copy_file(argc[0], argc[1], dir_cont->d_name);
						util_move_file(argc[0], argc[2], dir_cont->d_name);
					}
				}
	
				closedir(dir_ref);
			}

			Sleep(1);
		}
	}
	else
	{
		ShowWindow(var, SW_SHOW);
		printf("No input paths!\n");
	}

	return 0;
}

