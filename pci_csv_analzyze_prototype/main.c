#include <aclib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wchar.h>


c_csv_struct * controler_analyze_csv(c_csv_struct * q_csv, c_csv_struct * c_csv)
{
	if((q_csv != NULL)  && (c_csv != NULL))
	{
		c_csv_struct * resp_csv = c_csv_new(',', '\n');
		
		if(resp_csv != NULL)
		{
			c_csv_set_line_focuse(q_csv, 0);
			c_csv_set_line_focuse(c_csv, 0);
		
			while((c_csv_set_next_line(q_csv) != C_CSV_ERROR))
			{
				c_csv_set_next_line(c_csv);
		
				c_string * q_ceel = NULL;
				c_string * c_ceel = NULL;

				while((q_ceel = c_csv_get_next_ceel_in_line(q_csv)) != NULL && c_csv_get_focused_line_index(q_csv) < 14)
				{
					if((c_ceel = c_csv_get_next_ceel_in_line(c_csv)) != NULL)
					{
						if(c_csv_get_focused_ceel_index_in_line(q_csv) > 1)
						{
							if(strcmp(c_string_get_char_array(q_ceel), c_string_get_char_array(c_ceel)) != 0)
							{
								c_string * sheet_order = c_csv_get_ceel_content(q_csv, c_csv_get_focused_line_index(q_csv), 0);

								printf("%s <> %s\n", c_string_get_char_array(q_ceel), c_string_get_char_array(c_ceel));
								
								c_csv_add_line(resp_csv);
								//char temp[12];
								//sprintf(temp, "%d", c_csv_get_focused_line_index(q_csv));
								//*
								c_csv_add_ceel_to_line_with_init(resp_csv,
                                                                                                 c_csv_get_line_number(resp_csv)-1,
                                                                                                 c_string_get_char_array(c_csv_get_ceel_content_from_focused_line(q_csv, 0)));
								//*/
								//c_csv_add_ceel_to_line_with_init(resp_csv, c_csv_get_line_number(resp_csv)-1, temp);
								break;
							}
						}
					}
					else
					{
						char * str = c_string_get_char_array(q_ceel);
						printf("%s -> NULL\n", str);
						c_csv_add_line(resp_csv);
						//char temp[12];
						//sprintf(temp, "%d", c_csv_get_focused_line_index(q_csv));
						//c_csv_add_ceel_to_line_with_init(resp_csv, c_csv_get_line_number(resp_csv)-1, temp);
						//*
						c_csv_add_ceel_to_line_with_init(resp_csv,
                                                                                 c_csv_get_line_number(resp_csv)-1,
                                                                                 c_string_get_char_array(c_csv_get_ceel_content_from_focused_line(q_csv, 0)));
						//*/
						break;
					}
				}
			}

			return resp_csv;
		}
	}

	return NULL;
}


void util_replace_characters(wchar_t * input, wchar_t * char_to_replace, wchar_t * new_char)
{
         int in_index = 0;
         int out_index = 0;

         wchar_t apos = 0;
         swprintf(&apos, 1, L"%hs", "\"");

         while(input[in_index] != 0)
         {
                 uint8_t replace_index = 0;

                 while(char_to_replace[replace_index] != 0)
                 {
                         if(input[in_index] == char_to_replace[replace_index])
                         {
                                 input[out_index] = new_char[replace_index];
                         }

                         replace_index++;
                 }

                if(input[in_index] != apos)
                {
                        input[out_index] = input[in_index];
                        out_index++;
                }

                in_index++;
        }
}


int main()
{
	c_csv_struct * q_csv = c_csv_parse_csv_from_file("./input/quadient.csv", ',', '\n');
	c_csv_struct * c_csv = c_csv_parse_csv_from_file("./output/pci.csv", ',', '\n');

	c_string * q_ceel =c_csv_get_ceel_content(q_csv, 2,1);
	c_string * c_ceel = c_csv_get_ceel_content(c_csv, 2,1);

	printf("%s - %s\n", c_string_get_char_array(q_ceel), c_string_get_char_array(c_ceel));


	//*
	c_csv_struct * resp_csv = controler_analyze_csv(q_csv, c_csv);


	c_string * res_csv_content = c_csv_get_content(resp_csv);

	printf("\nfeedback csv content:\n");

	if(res_csv_content != NULL)
		printf("%s\n", c_string_get_char_array(res_csv_content));

	c_csv_finalize(resp_csv);


	//*/

	c_csv_finalize(q_csv);
	c_csv_finalize(c_csv);
}
