#include "machine_statistic.h"
#include <stdio.h>

struct _statistic_day_
{
	uint64_t total_feeded_sheets;
	uint64_t total_stacked_sheets;
	uint64_t total_rejected_sheets;
	double error_rate;
};

struct _machine_statistic_
{
	statistic_day * day_array;
	
	uint64_t total_feeded_sheets;
	uint64_t total_stacked_sheets;
	uint64_t total_rejected_sheets;
	double error_rate;
	int day_pre;
};

bool machine_statistic_is_int_day_interval(int8_t day);
void machine_statistic_count_error_rate(machine_statistic * this, int8_t day);
uint32_t machine_statistics_get_days();


machine_statistic * machine_statistic_new()
{
	machine_statistic * this = (machine_statistic*) malloc(sizeof(machine_statistic));
	
	this->total_feeded_sheets = 0;
	this->total_stacked_sheets = 0;
	this->total_rejected_sheets = 0;
	this->error_rate = 0.0;
	this->day_pre = 0;


	if(this != NULL)
	{
		this->day_array = (statistic_day*) malloc(sizeof(statistic_day)*7);

		printf("today - %d\n", machine_statistic_get_day());

		this->day_pre = 0;

		if(this->day_array != NULL)
		{
			for(int i=0; i < 7; i++)
			{

				this->day_array[i].total_feeded_sheets = 0;
				this->day_array[i].total_stacked_sheets = 0;
				this->day_array[i].total_rejected_sheets = 0;
				this->day_array[i].error_rate = 0;
			}
		}
		else
		{
			free(this);
			return NULL;
		}
	}

	return this;
}


void machine_statistic_set_total_error_rate(machine_statistic * this, double error_rate)
{
	this->error_rate = error_rate;
}

void machine_statistic_set_total_feeded_sheets(machine_statistic * this, uint64_t feeded_sheets)
{
	this->total_feeded_sheets = feeded_sheets;
}

void machine_statistic_set_total_stacked_sheets(machine_statistic * this, uint64_t stacked_sheets)
{
	this->total_stacked_sheets = stacked_sheets;
}

void machine_statistic_set_total_rejected_sheets(machine_statistic * this, uint64_t rejected_sheets)
{
	this->total_rejected_sheets = rejected_sheets;
}

void machine_statistic_set_day_pre(machine_statistic * this, int day_pre)
{
	this->day_pre = day_pre;
}

int machine_statistic_get_day_pre(machine_statistic * this)
{
	return this->day_pre;
}

void machine_statistic_check_day(machine_statistic * this)
{
	if(this->day_pre != machine_statistics_get_days())
	{
		for(int i=0;(i < (machine_statistics_get_days()-this->day_pre) && (i < 7)) ; i++)
		{
			uint8_t day = machine_statistic_get_day();
			if(i > day)
				day = i - day;
			else
				day = day - i;			


			machine_statistic_clear_day(this, day);
		}
	}

	this->day_pre = machine_statistics_get_days();
}

void machine_statistic_increment_feeded_sheets(machine_statistic * this)
{
	int8_t day = machine_statistic_get_day();
	this->day_array[day].total_feeded_sheets++;
	this->total_feeded_sheets++;

	machine_statistic_count_error_rate(this, day);
}

void machine_statistic_increment_stacked_sheets(machine_statistic * this)
{
	this->day_array[machine_statistic_get_day()].total_stacked_sheets++;
	this->total_stacked_sheets++;
}

void machine_statistic_increment_rejected_sheets(machine_statistic * this)
{
	int8_t day = machine_statistic_get_day();
	this->day_array[machine_statistic_get_day()].total_rejected_sheets++;
	this->total_rejected_sheets++;
	machine_statistic_count_error_rate(this, day);
}


uint64_t machine_statistic_get_feeded_sheets(machine_statistic * this, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
		return this->day_array[day].total_feeded_sheets;
	else
		return 0;
}

uint64_t machine_statistic_get_stacked_sheets(machine_statistic * this, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
		return this->day_array[day].total_stacked_sheets;
	else
		return 0;
}

uint64_t machine_statistic_get_rejected_sheets(machine_statistic * this, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
		return this->day_array[day].total_rejected_sheets;
	else
		return 0;
}

double machine_statistic_get_error_rate(machine_statistic * this, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
		return this->day_array[day].error_rate;
	else
		return 0;
}


uint64_t machine_statistic_get_total_feeded_sheets(machine_statistic * this)
{
	return this->total_feeded_sheets;
}

uint64_t machine_statistic_get_total_stacked_sheets(machine_statistic * this)
{
	return this->total_stacked_sheets;
}

uint64_t machine_statistic_get_total_rejected_sheets(machine_statistic * this)
{
	return this->total_rejected_sheets;
}

double machine_statistic_get_total_error_rate(machine_statistic * this)
{
	if((this->total_feeded_sheets > 0) && (this->total_rejected_sheets > 0))
		return this->error_rate;
	else
		return 0.0;
}

void machine_statistic_restore_feeded_sheets(machine_statistic * this, uint64_t feeded_sheets, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
		this->day_array[day].total_feeded_sheets = feeded_sheets;
}

void machine_statistic_restore_stacked_sheets(machine_statistic * this, uint64_t stacked_sheets, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
		this->day_array[day].total_stacked_sheets = stacked_sheets;
}

void machine_statistic_restore_rejected_sheets(machine_statistic * this, uint64_t rejected_sheets, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
		this->day_array[day].total_rejected_sheets = rejected_sheets;
}

void machine_statistic_restore_error_rate(machine_statistic * this, double error_rate, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
		this->day_array[day].error_rate = error_rate;
}


void machine_statistic_clear_day(machine_statistic * this, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
	{
		this->day_array[day].total_feeded_sheets = 0;
		this->day_array[day].total_stacked_sheets = 0;
		this->day_array[day].total_rejected_sheets = 0;
		this->day_array[day].error_rate = 0;
	}
}

void machine_statistic_finalize(machine_statistic ** this)
{
	if(*this != NULL)
	{
		free((*this)->day_array);
		free(*this);
		this = NULL;
	}
}

/************************************** static functions ********************************************/

int8_t machine_statistic_get_day()
{
	time_t t = time(NULL);
	struct tm *now = localtime(&t);
	return now->tm_wday;
}


uint32_t machine_statistics_get_days()
{
	time_t T= time(NULL);
	struct tm tm = *localtime(&T);
	return tm.tm_year*365+((uint32_t)(((double)(tm.tm_mon+1))*30.4166))+tm.tm_mday;
}

void machine_statistic_count_error_rate(machine_statistic * this, int8_t day)
{
	uint64_t feeded_sheets = this->day_array[day].total_feeded_sheets;
	uint64_t rejected_sheets = this->day_array[day].total_rejected_sheets;

	if((feeded_sheets > 0) && ( rejected_sheets > 0))
		this->day_array[machine_statistic_get_day()].error_rate =  ((double) ((double) rejected_sheets)/((double) (feeded_sheets)) * 100.0);


	if((this->total_feeded_sheets > 0) && ( this->total_rejected_sheets > 0))
		this->error_rate =  ((double) ((double) this->total_rejected_sheets)/((double) (this->total_feeded_sheets)) * 100.0);
}

bool machine_statistic_is_int_day_interval(int8_t day)
{
	if((day >= 0) && (day < 7))
		return true;
	else
		return false;
}
