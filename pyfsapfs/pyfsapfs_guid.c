/*
 * GUID functions
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
#include <types.h>

#include "pyfsapfs_error.h"
#include "pyfsapfs_guid.h"
#include "pyfsapfs_libfguid.h"
#include "pyfsapfs_python.h"

/* Creates a new string object from a GUID
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_string_new_from_guid(
           const uint8_t *guid_buffer,
           size_t guid_buffer_size )
{
	char guid_string[ 48 ];

	libcerror_error_t *error    = NULL;
	libfguid_identifier_t *guid = NULL;
	PyObject *string_object     = NULL;
	const char *errors          = NULL;
	static char *function       = "pyfsapfs_string_new_from_guid";

	if( libfguid_identifier_initialize(
	     &guid,
	     &error ) != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to create GUID.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libfguid_identifier_copy_from_byte_stream(
	     guid,
	     guid_buffer,
	     guid_buffer_size,
	     LIBFGUID_ENDIAN_BIG,
	     &error ) != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to copy byte stream to GUID.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libfguid_identifier_copy_to_utf8_string(
	     guid,
	     (uint8_t *) guid_string,
	     48,
	     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
	     &error ) != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to copy GUID to string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libfguid_identifier_free(
	     &guid,
	     &error ) != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to free GUID.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 guid_string,
			 (Py_ssize_t) 36,
			 errors );

	return( string_object );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( NULL );
}

