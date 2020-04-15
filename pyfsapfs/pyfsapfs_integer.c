/*
 * Integer functions
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
#include "pyfsapfs_libcerror.h"
#include "pyfsapfs_integer.h"
#include "pyfsapfs_python.h"

/* Creates a new signed integer object from a 64-bit value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_integer_signed_new_from_64bit(
           int64_t value_64bit )
{
	PyObject *integer_object = NULL;
	static char *function    = "pyfsapfs_integer_signed_new_from_64bit";

#if defined( HAVE_LONG_LONG )
	if( ( value_64bit < (int64_t) LLONG_MIN )
	 || ( value_64bit > (int64_t) LLONG_MAX ) )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: 64-bit value out of bounds.",
		 function );

		return( NULL );
	}
	integer_object = PyLong_FromLongLong(
	                  (long long) value_64bit );
#else
	if( current_offset > (off64_t) LONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: 64-bit value out of bounds.",
		 function );

		return( NULL );
	}
	integer_object = PyLong_FromLong(
	                  (long) value_64bit );
#endif
	return( integer_object );
}

/* Creates a new unsigned integer object from a 64-bit value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_integer_unsigned_new_from_64bit(
           uint64_t value_64bit )
{
	PyObject *integer_object = NULL;
	static char *function    = "pyfsapfs_integer_unsigned_new_from_64bit";

#if defined( HAVE_LONG_LONG )
	if( value_64bit > (uint64_t) ULLONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: 64-bit value exceeds maximum.",
		 function );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLongLong(
	                  (long long) value_64bit );
#else
	if( value_64bit > (uint64_t) ULONG_MAX )
	{
		PyErr_Format(
		 PyExc_OverflowError,
		 "%s: 64-bit value exceeds maximum.",
		 function );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (long) value_64bit );
#endif
	return( integer_object );
}

/* Copies a Python int or long object to a signed 64-bit value
 * Returns 1 if successful or -1 on error
 */
int pyfsapfs_integer_signed_copy_to_64bit(
     PyObject *integer_object,
     int64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function   = "pyfsapfs_integer_signed_copy_to_64bit";
	int result              = 0;

#if defined( HAVE_LONG_LONG )
	PY_LONG_LONG long_value = 0;
#else
	long long_value         = 0;
#endif

	if( integer_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer object.",
		 function );

		return( -1 );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          integer_object,
	          (PyObject *) &PyLong_Type );

	if( result == -1 )
	{
		pyfsapfs_error_fetch(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if integer object is of type long.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_LONG_LONG )
		long_value = PyLong_AsLongLong(
		              integer_object );
#else
		long_value = PyLong_AsLong(
		              integer_object );
#endif
	}
#if PY_MAJOR_VERSION < 3
	if( result == 0 )
	{
		PyErr_Clear();

		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyInt_Type );

		if( result == -1 )
		{
			pyfsapfs_error_fetch(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if integer object is of type int.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			PyErr_Clear();

			long_value = PyInt_AsLong(
			              integer_object );
		}
	}
#endif /* PY_MAJOR_VERSION < 3 */

	if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unsupported integer object type.",
		 function );

		return( -1 );
	}
	if( PyErr_Occurred() )
	{
		pyfsapfs_error_fetch(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to convert integer object to long.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LONG_LONG ) && ( SIZEOF_LONG_LONG > 8 )
	if( ( long_value < (PY_LONG_LONG) INT64_MIN )
	 || ( long_value > (PY_LONG_LONG) INT64_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid long value out of bounds.",
		 function );

		return( -1 );
	}
#elif ( SIZEOF_LONG > 8 )
	if( ( long_value > (long) INT64_MIN )
	 || ( long_value > (long) INT64_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid long value out of bounds.",
		 function );

		return( -1 );
	}
#endif
	*value_64bit = (int64_t) long_value;

	return( 1 );
}

/* Copies a Python int or long object to an unsigned 64-bit value
 * Returns 1 if successful or -1 on error
 */
int pyfsapfs_integer_unsigned_copy_to_64bit(
     PyObject *integer_object,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function   = "pyfsapfs_integer_unsigned_copy_to_64bit";
	int result              = 0;

#if defined( HAVE_LONG_LONG )
	PY_LONG_LONG long_value = 0;
#else
	long long_value         = 0;
#endif

	if( integer_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid integer object.",
		 function );

		return( -1 );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          integer_object,
	          (PyObject *) &PyLong_Type );

	if( result == -1 )
	{
		pyfsapfs_error_fetch(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if integer object is of type long.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_LONG_LONG )
		long_value = PyLong_AsUnsignedLongLong(
		              integer_object );
#else
		long_value = PyLong_AsUnsignedLong(
		              integer_object );
#endif
	}
#if PY_MAJOR_VERSION < 3
	if( result == 0 )
	{
		PyErr_Clear();

		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyInt_Type );

		if( result == -1 )
		{
			pyfsapfs_error_fetch(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if integer object is of type int.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			PyErr_Clear();

#if defined( HAVE_LONG_LONG )
			long_value = PyInt_AsUnsignedLongLongMask(
				      integer_object );
#else
			long_value = PyInt_AsUnsignedLongMask(
				      integer_object );
#endif
		}
	}
#endif /* PY_MAJOR_VERSION < 3 */

	if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unsupported integer object type.",
		 function );

		return( -1 );
	}
	if( PyErr_Occurred() )
	{
		pyfsapfs_error_fetch(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to convert integer object to long.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LONG_LONG )
#if ( SIZEOF_LONG_LONG > 8 )
	if( ( long_value < (PY_LONG_LONG) 0 )
	 || ( long_value > (PY_LONG_LONG) UINT64_MAX ) )
#else
	if( long_value < (PY_LONG_LONG) 0 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid long value out of bounds.",
		 function );

		return( -1 );
	}
#else
#if ( SIZEOF_LONG > 8 )
	if( ( long_value < (long) 0 )
	 || ( long_value > (long) UINT64_MAX ) )
#else
	if( long_value < (PY_LONG_LONG) 0 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid long value out of bounds.",
		 function );

		return( -1 );
	}
#endif
	*value_64bit = (uint64_t) long_value;

	return( 1 );
}

