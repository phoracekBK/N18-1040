#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

bool controler_date_lower_limit(int day_from, int month_from, int year_from, int day, int month, int year)
{
        bool A = (year_from < year);
        bool B = (year_from == year);
        bool C = (month_from < month);
        bool D = (month_from == month);
        bool E = (day_from < day);
        bool F = (day_from == day);

	return ((A || B) && (A || C || D) && (A || C || E || F) && ((!E) || (!F)) && ((!C) || (!D)) && ((!A) || (!B)));
}

bool controler_date_upper_limit(int day_to, int month_to, int year_to, int day, int month, int year)
{
         bool A = (year_to > year);
         bool B = (year_to == year);
         bool C = (month_to > month);
         bool D = (month_to == month);
         bool E = (day_to > day);
         bool F = (day_to == day);
	
	 return ((A || B) && (A || C || D) && (A || C || E || F) && ((!E) || (!F)) && ((!C) || (!D)) && ((!A) || (!B)));
}


int main()
{
	int year = 2018;
	int month = 10;
	int day = 26;
	
	int year_from = 2018;
	int month_from = 10;
	int day_from = 20;

	int year_to = 2018;
	int month_to = 10;
	int day_to = 27;

	bool date_from = controler_date_lower_limit(day_from, month_from, year_from, day, month, year);
	bool date_to = controler_date_upper_limit(day_to,  month_to, year_to, day, month, year);

	printf("date from = %s, date to %s\n", ((date_from == true) ? "TRUE" : "FALSE") , ((date_to == true) ? "TRUE" : "FALSE"));

	if(date_from && date_to)
        {
		printf("passd\n");
	}
	else
	{
		printf("faild\n");
	}


	return 0;
}
