#ifndef _UTIL_H_
#define _UTIL_H_

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
#include <wchar.h>


int util_dir_is_empty(char * address);
int32_t util_file_size(char * path, char * file_name);
int32_t util_save_csv(char* addr, char* name, char* buff);
char * util_load_csv(char * addr, char* name, int32_t* size);
uint8_t util_move_file(char * src, char* dest, char* name);
uint8_t util_copy_file(char * src, char *dest, char *name);
void util_delete_file(char *addr, char* name);
int32_t util_str_ends_with(char* str, const char * suffix, int offset);
char * util_get_time_string();
void util_replace_characters(wchar_t * input, wchar_t * char_to_replace, wchar_t * new_char);
char * util_wstr_to_str(wchar_t * wstr);
wchar_t * util_str_to_wstr(char * str);

#endif
