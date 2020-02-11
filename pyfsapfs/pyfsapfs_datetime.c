/*
 * Date and time functions
 *
 * Copyright (C) 2018-2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <types.h>

#include "pyfsapfs_datetime.h"
#include "pyfsapfs_python.h"

#include <datetime.h>

/* Creates a new datetime object from a FAT date time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_datetime_new_from_fat_date_time(
           uint32_t fat_date_time )
{
	PyObject *datetime_object = NULL;
	static char *function     = "pyfsapfs_datetime_new_from_fat_date_time";
	uint16_t year             = 0;
	uint8_t day_of_month      = 0;
	uint8_t days_in_month     = 0;
	uint8_t hours             = 0;
	uint8_t minutes           = 0;
	uint8_t month             = 0;
	uint8_t seconds           = 0;

	/* The day of month is stored in the next 5 bits
	 */
	day_of_month    = fat_date_time & 0x1f;
	fat_date_time >>= 5;

	/* The month is stored in the next 4 bits
	 */
	month           = fat_date_time & 0x0f;
	fat_date_time >>= 4;

	/* The year is stored in the next 7 bits starting at 1980
	 */
	year            = 1980 + ( fat_date_time & 0x7f );
	fat_date_time >>= 7;

	/* The number of seconds are stored in the lower 5 bits
	 * in intervals of 2 seconds
	 */
	seconds         = ( fat_date_time & 0x1f ) * 2;
	fat_date_time >>= 5;

	/* The number of minutes are stored in the next 6 bits
	 */
	minutes         = fat_date_time & 0x3f;
	fat_date_time >>= 6;

	/* The number of hours are stored in the next 5 bits
	 */
	hours = fat_date_time & 0x1f;

	/* February (2)
	 */
	if( month == 2 )
	{
		if( ( ( ( year % 4 ) == 0 )
		  &&  ( ( year % 100 ) != 0 ) )
		 || ( ( year % 400 ) == 0 ) )
		{
			days_in_month = 29;
		}
		else
		{
			days_in_month = 28;
		}
	}
	/* April (4), June (6), September (9), November (11)
	 */
	else if( ( month == 4 )
	      || ( month == 6 )
	      || ( month == 9 )
	      || ( month == 11 ) )
	{
		days_in_month = 30;
	}
	/* Januari (1), March (3), May (5), July (7), August (8), October (10), December (12)
	 */
	else if( ( month == 1 )
	      || ( month == 3 )
	      || ( month == 5 )
	      || ( month == 7 )
	      || ( month == 8 )
	      || ( month == 10 )
	      || ( month == 12 ) )
	{
		days_in_month = 31;
	}
	else
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unsupported month: %" PRIu8 ".",
		 function,
		 month );

		return( NULL );
	}
	if( ( day_of_month == 0 )
	 || ( day_of_month > days_in_month ) )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unsupported day of month: %" PRIu8 ".",
		 function,
		 day_of_month );

		return( NULL );
	}
	PyDateTime_IMPORT;

	datetime_object = (PyObject *) PyDateTime_FromDateAndTime(
	                                (int) year,
	                                (int) month,
	                                (int) day_of_month,
	                                (int) hours,
	                                (int) minutes,
	                                (int) seconds,
	                                0 );

	return( datetime_object );
}

/* Creates a new datetime object from a FILETIME
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_datetime_new_from_filetime(
           uint64_t filetime )
{
	PyObject *datetime_object = NULL;
	static char *function     = "pyfsapfs_datetime_new_from_filetime";
	uint32_t days_in_century  = 0;
	uint32_t micro_seconds    = 0;
	uint16_t days_in_year     = 0;
	uint16_t year             = 0;
	uint8_t day_of_month      = 0;
	uint8_t days_in_month     = 0;
	uint8_t hours             = 0;
	uint8_t minutes           = 0;
	uint8_t month             = 0;
	uint8_t seconds           = 0;

	/* The timestamp is in units of 100 nano seconds correct the value to seconds
	 */
	micro_seconds = (uint32_t) ( filetime % 10000000 ) / 10;
	filetime      /= 10000000;

	/* There are 60 seconds in a minute correct the value to minutes
	 */
	seconds   = (uint8_t) ( filetime % 60 );
	filetime /= 60;

	/* There are 60 minutes in an hour correct the value to hours
	 */
	minutes   = (uint8_t) ( filetime % 60 );
	filetime /= 60;

	/* There are 24 hours in a day correct the value to days
	 */
	hours     = (uint8_t) ( filetime % 24 );
	filetime /= 24;

	/* Add 1 day to compensate that Jan 1 1601 is represented as 0
	 */
	filetime += 1;

	/* Determine the number of years starting at '1 Jan 1601 00:00:00'
	 * correct the value to days within the year
	 */
	year = 1601;

	if( filetime >= 36159 )
	{
		year = 1700;

		filetime -= 36159;
	}
	while( filetime > 0 )
	{
		if( ( year % 400 ) == 0 )
		{
			days_in_century = 36525;
		}
		else
		{
			days_in_century = 36524;
		}
		if( filetime <= days_in_century )
		{
			break;
		}
		filetime -= days_in_century;

		year += 100;
	}
	while( filetime > 0 )
	{
		/* Check for a leap year
		 * The year is ( ( dividable by 4 ) and ( not dividable by 100 ) ) or ( dividable by 400 )
		 */
		if( ( ( ( year % 4 ) == 0 )
		  &&  ( ( year % 100 ) != 0 ) )
		 || ( ( year % 400 ) == 0 ) )
		{
			days_in_year = 366;
		}
		else
		{
			days_in_year = 365;
		}
		if( filetime <= days_in_year )
		{
			break;
		}
		filetime -= days_in_year;

		year += 1;
	}
	/* Determine the month correct the value to days within the month
	 */
	month = 1;

	while( filetime > 0 )
	{
		/* February (2)
		 */
		if( month == 2 )
		{
			if( ( ( ( year % 4 ) == 0 )
			  &&  ( ( year % 100 ) != 0 ) )
			 || ( ( year % 400 ) == 0 ) )
			{
				days_in_month = 29;
			}
			else
			{
				days_in_month = 28;
			}
		}
		/* April (4), June (6), September (9), November (11)
		 */
		else if( ( month == 4 )
		      || ( month == 6 )
		      || ( month == 9 )
		      || ( month == 11 ) )
		{
			days_in_month = 30;
		}
		/* Januari (1), March (3), May (5), July (7), August (8), October (10), December (12)
		 */
		else if( ( month == 1 )
		      || ( month == 3 )
		      || ( month == 5 )
		      || ( month == 7 )
		      || ( month == 8 )
		      || ( month == 10 )
		      || ( month == 12 ) )
		{
			days_in_month = 31;
		}
		/* This should never happen, but just in case
		 */
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unsupported month: %" PRIu8 ".",
			 function,
			 month );

			return( NULL );
		}
		if( filetime <= days_in_month )
		{
			break;
		}
		filetime -= days_in_month;

		month += 1;
	}
	/* Determine the day
	 */
	day_of_month = (uint8_t) filetime;

	PyDateTime_IMPORT;

	datetime_object = (PyObject *) PyDateTime_FromDateAndTime(
	                                (int) year,
	                                (int) month,
	                                (int) day_of_month,
	                                (int) hours,
	                                (int) minutes,
	                                (int) seconds,
	                                (int) micro_seconds );

	return( datetime_object );
}

/* Creates a new datetime object from a floatingtime
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_datetime_new_from_floatingtime(
           uint64_t floatingtime )
{
	byte_stream_float64_t timestamp;

	PyObject *datetime_object = NULL;
	static char *function     = "pyfsapfs_datetime_new_from_floatingtime";
	uint32_t days_in_century  = 0;
	uint32_t micro_seconds    = 0;
	uint16_t days_in_year     = 0;
	uint16_t year             = 0;
	uint8_t day_of_month      = 0;
	uint8_t days_in_month     = 0;
	uint8_t hours             = 0;
	uint8_t minutes           = 0;
	uint8_t month             = 0;
	uint8_t seconds           = 0;

	timestamp.integer = floatingtime;

	/* Determine the number of years starting at '30 Dec 1899 00:00:00'
	 * correct the value to days within the year
	 */
	year = 1899;

	if( timestamp.floating_point >= 2 )
	{
		year = 1900;

		timestamp.floating_point -= 2;
	}
	while( timestamp.floating_point > 0 )
	{
		if( ( year % 400 ) == 0 )
		{
			days_in_century = 36525;
		}
		else
		{
			days_in_century = 36524;
		}
		if( timestamp.floating_point <= days_in_century )
		{
			break;
		}
		timestamp.floating_point -= days_in_century;

		year += 100;
	}
	while( timestamp.floating_point > 0 )
	{
		/* Check for a leap year
		 * The year is ( ( dividable by 4 ) and ( not dividable by 100 ) ) or ( dividable by 400 )
		 */
		if( ( ( ( year % 4 ) == 0 )
		  &&  ( ( year % 100 ) != 0 ) )
		 || ( ( year % 400 ) == 0 ) )
		{
			days_in_year = 366;
		}
		else
		{
			days_in_year = 365;
		}
		if( timestamp.floating_point <= days_in_year )
		{
			break;
		}
		timestamp.floating_point -= days_in_year;

		year += 1;
	}
	/* Determine the month correct the value to days within the month
	 */
	month = 1;

	while( timestamp.floating_point > 0 )
	{
		/* February (2)
		 */
		if( month == 2 )
		{
			if( ( ( ( year % 4 ) == 0 )
			  &&  ( ( year % 100 ) != 0 ) )
			 || ( ( year % 400 ) == 0 ) )
			{
				days_in_month = 29;
			}
			else
			{
				days_in_month = 28;
			}
		}
		/* April (4), June (6), September (9), November (11)
		 */
		else if( ( month == 4 )
		      || ( month == 6 )
		      || ( month == 9 )
		      || ( month == 11 ) )
		{
			days_in_month = 30;
		}
		/* Januari (1), March (3), May (5), July (7), August (8), October (10), December (12)
		 */
		else if( ( month == 1 )
		      || ( month == 3 )
		      || ( month == 5 )
		      || ( month == 7 )
		      || ( month == 8 )
		      || ( month == 10 )
		      || ( month == 12 ) )
		{
			days_in_month = 31;
		}
		/* This should never happen, but just in case
		 */
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unsupported month: %" PRIu8 ".",
			 function,
			 month );

			return( NULL );
		}
		if( timestamp.floating_point <= days_in_month )
		{
			break;
		}
		timestamp.floating_point -= days_in_month;

		month += 1;
	}
	/* Determine the day
	 */
	day_of_month              = (uint8_t) timestamp.floating_point;
	timestamp.floating_point -= day_of_month;

	/* There are 24 hours in a day correct the value to hours
	 */
	timestamp.floating_point *= 24;
	hours                     = (uint8_t) timestamp.floating_point;
	timestamp.floating_point -= hours;

	/* There are 60 minutes in an hour correct the value to minutes
	 */
	timestamp.floating_point *= 60;
	minutes                   = (uint8_t) timestamp.floating_point;
	timestamp.floating_point -= minutes;

	/* There are 60 seconds in a minute correct the value to seconds
	 */
	timestamp.floating_point *= 60;
	seconds                   = (uint8_t) timestamp.floating_point;
	timestamp.floating_point -= seconds;

	/* There are 1000 micro seconds in a seconds correct the value to micro seconds
	 */
	timestamp.floating_point *= 1000000;
	micro_seconds             = (uint8_t) timestamp.floating_point;
	timestamp.floating_point -= micro_seconds;

	PyDateTime_IMPORT;

	datetime_object = (PyObject *) PyDateTime_FromDateAndTime(
	                                (int) year,
	                                (int) month,
	                                (int) day_of_month,
	                                (int) hours,
	                                (int) minutes,
	                                (int) seconds,
	                                (int) micro_seconds );

	return( datetime_object );
}

/* Creates a new datetime object from a POSIX time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_datetime_new_from_posix_time(
           int32_t posix_time )
{
	PyObject *datetime_object = NULL;
	static char *function     = "pyfsapfs_datetime_new_from_posix_time";
	uint16_t days_in_year     = 0;
	uint16_t year             = 0;
	uint8_t day_of_month      = 0;
	uint8_t days_in_month     = 0;
	uint8_t hours             = 0;
	uint8_t minutes           = 0;
	uint8_t month             = 0;
	uint8_t seconds           = 0;

	/* There are 60 seconds in a minute correct the value to minutes
	 */
	seconds     = posix_time % 60;
	posix_time /= 60;

	/* There are 60 minutes in an hour correct the value to hours
	 */
	minutes     = posix_time % 60;
	posix_time /= 60;

	/* There are 24 hours in a day correct the value to days
	 */
	hours       = posix_time % 24;
	posix_time /= 24;

	/* Add 1 day to compensate that Jan 1 1970 is represented as 0
	 */
	posix_time += 1;

	/* Determine the number of years starting at '1 Jan 1970 00:00:00'
	 * correct the value to days within the year
	 */
	year = 1970;

	if( posix_time >= 10957 )
	{
		year = 2000;

		posix_time -= 10957;
	}
	while( posix_time > 0 )
	{
		/* Check for a leap year
		 * The year is ( ( dividable by 4 ) and ( not dividable by 100 ) ) or ( dividable by 400 )
		 */
		if( ( ( ( year % 4 ) == 0 )
		  &&  ( ( year % 100 ) != 0 ) )
		 || ( ( year % 400 ) == 0 ) )
		{
			days_in_year = 366;
		}
		else
		{
			days_in_year = 365;
		}
		if( posix_time <= days_in_year )
		{
			break;
		}
		posix_time -= days_in_year;

		year += 1;
	}
	/* Determine the month correct the value to days within the month
	 */
	month = 1;

	while( posix_time > 0 )
	{
		/* February (2)
		 */
		if( month == 2 )
		{
			if( ( ( ( year % 4 ) == 0 )
			  &&  ( ( year % 100 ) != 0 ) )
			 || ( ( year % 400 ) == 0 ) )
			{
				days_in_month = 29;
			}
			else
			{
				days_in_month = 28;
			}
		}
		/* April (4), June (6), September (9), November (11)
		 */
		else if( ( month == 4 )
		      || ( month == 6 )
		      || ( month == 9 )
		      || ( month == 11 ) )
		{
			days_in_month = 30;
		}
		/* Januari (1), March (3), May (5), July (7), August (8), October (10), December (12)
		 */
		else if( ( month == 1 )
		      || ( month == 3 )
		      || ( month == 5 )
		      || ( month == 7 )
		      || ( month == 8 )
		      || ( month == 10 )
		      || ( month == 12 ) )
		{
			days_in_month = 31;
		}
		/* This should never happen, but just in case
		 */
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unsupported month: %" PRIu8 ".",
			 function,
			 month );

			return( NULL );
		}
		if( posix_time <= days_in_month )
		{
			break;
		}
		posix_time -= days_in_month;

		month += 1;
	}
	/* Determine the day
	 */
	day_of_month = (uint8_t) posix_time;

	PyDateTime_IMPORT;

	datetime_object = (PyObject *) PyDateTime_FromDateAndTime(
	                                (int) year,
	                                (int) month,
	                                (int) day_of_month,
	                                (int) hours,
	                                (int) minutes,
	                                (int) seconds,
	                                0 );

	return( datetime_object );
}

/* Creates a new datetime object from a POSIX time in micro seconds
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_datetime_new_from_posix_time_in_micro_seconds(
           int64_t posix_time )
{
	PyObject *datetime_object = NULL;
	static char *function     = "pyfsapfs_datetime_new_from_posix_time_in_micro_seconds";
	uint32_t micro_seconds    = 0;
	uint16_t days_in_year     = 0;
	uint16_t year             = 0;
	uint8_t day_of_month      = 0;
	uint8_t days_in_month     = 0;
	uint8_t hours             = 0;
	uint8_t minutes           = 0;
	uint8_t month             = 0;
	uint8_t seconds           = 0;

	/* There are 1000000 micro seconds in a second correct the value to seconds
	 */
	micro_seconds = (uint32_t) ( posix_time % 1000000 );
	posix_time   /= 1000000;

	/* There are 60 seconds in a minute correct the value to minutes
	 */
	seconds     = posix_time % 60;
	posix_time /= 60;

	/* There are 60 minutes in an hour correct the value to hours
	 */
	minutes     = posix_time % 60;
	posix_time /= 60;

	/* There are 24 hours in a day correct the value to days
	 */
	hours       = posix_time % 24;
	posix_time /= 24;

	/* Add 1 day to compensate that Jan 1 1970 is represented as 0
	 */
	posix_time += 1;

	/* Determine the number of years starting at '1 Jan 1970 00:00:00'
	 * correct the value to days within the year
	 */
	year = 1970;

	if( posix_time >= 10957 )
	{
		year = 2000;

		posix_time -= 10957;
	}
	while( posix_time > 0 )
	{
		/* Check for a leap year
		 * The year is ( ( dividable by 4 ) and ( not dividable by 100 ) ) or ( dividable by 400 )
		 */
		if( ( ( ( year % 4 ) == 0 )
		  &&  ( ( year % 100 ) != 0 ) )
		 || ( ( year % 400 ) == 0 ) )
		{
			days_in_year = 366;
		}
		else
		{
			days_in_year = 365;
		}
		if( posix_time <= days_in_year )
		{
			break;
		}
		posix_time -= days_in_year;

		year += 1;
	}
	/* Determine the month correct the value to days within the month
	 */
	month = 1;

	while( posix_time > 0 )
	{
		/* February (2)
		 */
		if( month == 2 )
		{
			if( ( ( ( year % 4 ) == 0 )
			  &&  ( ( year % 100 ) != 0 ) )
			 || ( ( year % 400 ) == 0 ) )
			{
				days_in_month = 29;
			}
			else
			{
				days_in_month = 28;
			}
		}
		/* April (4), June (6), September (9), November (11)
		 */
		else if( ( month == 4 )
		      || ( month == 6 )
		      || ( month == 9 )
		      || ( month == 11 ) )
		{
			days_in_month = 30;
		}
		/* Januari (1), March (3), May (5), July (7), August (8), October (10), December (12)
		 */
		else if( ( month == 1 )
		      || ( month == 3 )
		      || ( month == 5 )
		      || ( month == 7 )
		      || ( month == 8 )
		      || ( month == 10 )
		      || ( month == 12 ) )
		{
			days_in_month = 31;
		}
		/* This should never happen, but just in case
		 */
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unsupported month: %" PRIu8 ".",
			 function,
			 month );

			return( NULL );
		}
		if( posix_time <= days_in_month )
		{
			break;
		}
		posix_time -= days_in_month;

		month += 1;
	}
	/* Determine the day
	 */
	day_of_month = (uint8_t) posix_time;

	PyDateTime_IMPORT;

	datetime_object = (PyObject *) PyDateTime_FromDateAndTime(
	                                (int) year,
	                                (int) month,
	                                (int) day_of_month,
	                                (int) hours,
	                                (int) minutes,
	                                (int) seconds,
	                                (int) micro_seconds );

	return( datetime_object );
}

