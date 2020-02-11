/*
 * Error functions
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDARG_H ) || defined( WINAPI )
#include <stdarg.h>
#elif defined( HAVE_VARARGS_H )
#include <varargs.h>
#else
#error Missing headers stdarg.h and varargs.h
#endif

#include "pyfsapfs_error.h"
#include "pyfsapfs_libcerror.h"
#include "pyfsapfs_python.h"

#if defined( HAVE_STDARG_H ) || defined( WINAPI )
#define VARARGS( function, error, error_domain, error_code, type, argument ) \
	function( error, error_domain, error_code, type argument, ... )
#define VASTART( argument_list, type, name ) \
	va_start( argument_list, name )
#define VAEND( argument_list ) \
	va_end( argument_list )

#elif defined( HAVE_VARARGS_H )
#define VARARGS( function, error, error_domain, error_code, type, argument ) \
	function( error, error_domain, error_code, va_alist ) va_dcl
#define VASTART( argument_list, type, name ) \
	{ type name; va_start( argument_list ); name = va_arg( argument_list, type )
#define VAEND( argument_list ) \
	va_end( argument_list ); }

#endif

/* Fetches an error
 */
void VARARGS(
      pyfsapfs_error_fetch,
      libcerror_error_t **error,
      int error_domain,
      int error_code,
      const char *,
      format_string )
{
	va_list argument_list;

	char error_string[ PYFSAPFS_ERROR_STRING_SIZE ];

        PyObject *exception_traceback = NULL;
        PyObject *exception_type      = NULL;
        PyObject *exception_value     = NULL;
        PyObject *string_object       = NULL;
	static char *function         = "pyfsapfs_error_fetch";
	char *exception_string        = NULL;
	size_t error_string_length    = 0;
	int print_count               = 0;

#if PY_MAJOR_VERSION >= 3
	PyObject *utf8_string_object  = NULL;
#endif

	if( format_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing format string.",
		 function );

		return;
	}
	VASTART(
	 argument_list,
	 const char *,
	 format_string );

	print_count = PyOS_vsnprintf(
	               error_string,
	               PYFSAPFS_ERROR_STRING_SIZE,
	               format_string,
	               argument_list );

	VAEND(
	 argument_list );

	if( print_count < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unable to format error string.",
		 function );

		return;
	}
	error_string_length = narrow_string_length(
	                       error_string );

	if( ( error_string_length >= 1 )
	 && ( error_string[ error_string_length - 1 ] == '.' ) )
	{
		error_string[ error_string_length - 1 ] = 0;
	}
	PyErr_Fetch(
	 &exception_type,
	 &exception_value,
	 &exception_traceback );

	string_object = PyObject_Repr(
	                 exception_value );

#if PY_MAJOR_VERSION >= 3
	utf8_string_object = PyUnicode_AsUTF8String(
	                      string_object );

	if( utf8_string_object != NULL )
	{
		exception_string = PyBytes_AsString(
		                    utf8_string_object );
	}
#else
	exception_string = PyString_AsString(
	                    string_object );
#endif
	if( exception_string != NULL )
	{
		libcerror_error_set(
		 error,
		 error_domain,
		 error_code,
		 "%s with error: %s.",
		 error_string,
		 exception_string );
	}
	else
	{
		libcerror_error_set(
		 error,
		 error_domain,
		 error_code,
		 "%s.",
		 error_string );
	}
#if PY_MAJOR_VERSION >= 3
	if( utf8_string_object != NULL )
	{
		Py_DecRef(
		 utf8_string_object );
	}
#endif
	Py_DecRef(
	 string_object );

	return;
}

#undef VARARGS
#undef VASTART
#undef VAEND

#if defined( HAVE_STDARG_H ) || defined( WINAPI )
#define VARARGS( function, exception_object, type, argument ) \
	function( exception_object, type argument, ... )
#define VASTART( argument_list, type, name ) \
	va_start( argument_list, name )
#define VAEND( argument_list ) \
	va_end( argument_list )

#elif defined( HAVE_VARARGS_H )
#define VARARGS( function, exception_object, type, argument ) \
	function( exception_object, va_alist ) va_dcl
#define VASTART( argument_list, type, name ) \
	{ type name; va_start( argument_list ); name = va_arg( argument_list, type )
#define VAEND( argument_list ) \
	va_end( argument_list ); }

#endif

/* Fetches and raises an error
 */
void VARARGS(
      pyfsapfs_error_fetch_and_raise,
      PyObject *exception_object,
      const char *,
      format_string )
{
	va_list argument_list;

	char error_string[ PYFSAPFS_ERROR_STRING_SIZE ];

	PyObject *exception_traceback = NULL;
	PyObject *exception_type      = NULL;
	PyObject *exception_value     = NULL;
	PyObject *string_object       = NULL;
	static char *function         = "pyfsapfs_error_fetch_and_raise";
	char *exception_string        = NULL;
	size_t error_string_length    = 0;
	int print_count               = 0;

#if PY_MAJOR_VERSION >= 3
	PyObject *utf8_string_object  = NULL;
#endif

	if( format_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing format string.",
		 function );

		return;
	}
	VASTART(
	 argument_list,
	 const char *,
	 format_string );

	print_count = PyOS_vsnprintf(
	               error_string,
	               PYFSAPFS_ERROR_STRING_SIZE,
	               format_string,
	               argument_list );

	VAEND(
	 argument_list );

	if( print_count < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unable to format exception string.",
		 function );

		return;
	}
	error_string_length = narrow_string_length(
	                       error_string );

	if( ( error_string_length >= 1 )
	 && ( error_string[ error_string_length - 1 ] == '.' ) )
	{
		error_string[ error_string_length - 1 ] = 0;
	}
	PyErr_Fetch(
	 &exception_type,
	 &exception_value,
	 &exception_traceback );

	string_object = PyObject_Repr(
	                 exception_value );

#if PY_MAJOR_VERSION >= 3
	utf8_string_object = PyUnicode_AsUTF8String(
	                      string_object );

	if( utf8_string_object != NULL )
	{
		exception_string = PyBytes_AsString(
		                    utf8_string_object );
	}
#else
	exception_string = PyString_AsString(
	                    string_object );
#endif
	if( exception_string != NULL )
	{
		PyErr_Format(
		 exception_object,
		 "%s with error: %s.",
		 error_string,
		 exception_string );
	}
	else
	{
		PyErr_Format(
		 exception_object,
		 "%s.",
		 error_string );
	}
	Py_DecRef(
	 string_object );

	return;
}

#undef VARARGS
#undef VASTART
#undef VAEND

#if defined( HAVE_STDARG_H ) || defined( WINAPI )
#define VARARGS( function, error, exception_object, type, argument ) \
	function( error, exception_object, type argument, ... )
#define VASTART( argument_list, type, name ) \
	va_start( argument_list, name )
#define VAEND( argument_list ) \
	va_end( argument_list )

#elif defined( HAVE_VARARGS_H )
#define VARARGS( function, error, exception_object, type, argument ) \
	function( error, exception_object, va_alist ) va_dcl
#define VASTART( argument_list, type, name ) \
	{ type name; va_start( argument_list ); name = va_arg( argument_list, type )
#define VAEND( argument_list ) \
	va_end( argument_list ); }

#endif

/* Raises an error
 */
void VARARGS(
      pyfsapfs_error_raise,
      libcerror_error_t *error,
      PyObject *exception_object,
      const char *,
      format_string )
{
	va_list argument_list;

	char error_string[ PYFSAPFS_ERROR_STRING_SIZE ];
	char exception_string[ PYFSAPFS_ERROR_STRING_SIZE ];

	static char *function     = "pyfsapfs_error_raise";
	size_t error_string_index = 0;
	int print_count           = 0;

	if( format_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing format string.",
		 function );

		return;
	}
	VASTART(
	 argument_list,
	 const char *,
	 format_string );

	print_count = PyOS_vsnprintf(
	               exception_string,
	               PYFSAPFS_ERROR_STRING_SIZE,
	               format_string,
	               argument_list );

	VAEND(
	 argument_list );

	if( print_count < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unable to format exception string.",
		 function );

		return;
	}
	if( error != NULL )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYFSAPFS_ERROR_STRING_SIZE ) != -1 )
		{
			while( error_string_index < PYFSAPFS_ERROR_STRING_SIZE )
			{
				if( error_string[ error_string_index ] == 0 )
				{
					break;
				}
				if( error_string[ error_string_index ] == '\n' )
				{
					error_string[ error_string_index ] = ' ';
				}
				error_string_index++;
			}
			if( error_string_index >= PYFSAPFS_ERROR_STRING_SIZE )
			{
				error_string[ PYFSAPFS_ERROR_STRING_SIZE - 1 ] = 0;
			}
			PyErr_Format(
			 exception_object,
			 "%s %s",
			 exception_string,
			 error_string );

			return;
		}
	}
	PyErr_Format(
	 exception_object,
	 "%s",
	 exception_string );

	return;
}

#undef VARARGS
#undef VASTART
#undef VAEND

