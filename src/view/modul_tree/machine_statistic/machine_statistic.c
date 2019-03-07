#include "machine_statistic.h"


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
};

int8_t machine_statistic_get_day();
bool machine_statistic_is_int_day_interval(int8_t day);
void machine_statistic_count_error_rate(machine_statistic * this, int8_t day);



machine_statistic * machine_statistic_new()
{
	machine_statistic * this = (machine_statistic*) malloc(sizeof(machine_statistic));
	
	if(this != NULL)
	{
		this->day_array = (statistic_day*) malloc(sizeof(statistic_day)*7);

		if(this->day_array != NULL)
		{
			for(int i=0; i<7; i++)
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

void machine_statistic_increment_feeded_sheets(machine_statistic * this)
{
	int8_t day = machine_statistic_get_day();
	this->day_array[day].total_feeded_sheets++;

	machine_statistic_count_error_rate(this, day);
}

void machine_statistic_increment_stacked_sheets(machine_statistic * this)
{
	this->day_array[machine_statistic_get_day()].total_stacked_sheets++;
}

void machine_statistic_increment_rejected_sheets(machine_statistic * this)
{
	int8_t day = machine_statistic_get_day();
	this->day_array[machine_statistic_get_day()].total_rejected_sheets++;
	machine_statistic_count_error_rate(this, day);
}


uint64_t machine_statistic_get_feeded_sheets(machine_statistic * this, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
		return this->day_array[machine_statistic_get_day()].total_feeded_sheets;
	else
		return 0;
}

uint64_t machine_statistic_get_stacked_sheets(machine_statistic * this, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
		return this->day_array[machine_statistic_get_day()].total_stacked_sheets;
	else
		return 0;
}

uint64_t machine_statistic_get_rejected_sheets(machine_statistic * this, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
		return this->day_array[machine_statistic_get_day()].total_rejected_sheets;
	else
		return 0;
}

double machine_statistic_get_error_rate(machine_statistic * this, int8_t day)
{
	if(machine_statistic_is_int_day_interval(day) == true)
		return this->day_array[machine_statistic_get_day()].error_rate;
	else
		return 0;
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

void machine_statistic_count_error_rate(machine_statistic * this, int8_t day)
{
	uint64_t feeded_sheets = this->day_array[day].total_feeded_sheets;
	uint64_t rejected_sheets = this->day_array[day].total_rejected_sheets;

	if((feeded_sheets > 0) && ( rejected_sheets > 0))
		this->day_array[machine_statistic_get_day()].error_rate =  ((double) ((double) feeded_sheets)/((double) (rejected_sheets)) * 100.0);
}

bool machine_statistic_is_int_day_interval(int8_t day)
{
	if((day >= 0) && (day < 7))
		return true;
	else
		return false;
}
