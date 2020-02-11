/*
 * Python file object IO handle functions
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
#include <types.h>

#include "pyfsapfs_error.h"
#include "pyfsapfs_file_object_io_handle.h"
#include "pyfsapfs_integer.h"
#include "pyfsapfs_libbfio.h"
#include "pyfsapfs_libcerror.h"
#include "pyfsapfs_python.h"

/* Creates a file object IO handle
 * Make sure the value file_object_io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int pyfsapfs_file_object_io_handle_initialize(
     pyfsapfs_file_object_io_handle_t **file_object_io_handle,
     PyObject *file_object,
     libcerror_error_t **error )
{
	static char *function = "pyfsapfs_file_object_io_handle_initialize";

	if( file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object IO handle.",
		 function );

		return( -1 );
	}
	if( *file_object_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file object IO handle value already set.",
		 function );

		return( -1 );
	}
	if( file_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object.",
		 function );

		return( -1 );
	}
	*file_object_io_handle = (pyfsapfs_file_object_io_handle_t *) PyMem_Malloc(
	                                                               sizeof( pyfsapfs_file_object_io_handle_t ) );

	if( *file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file object IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_object_io_handle,
	     0,
	     sizeof( pyfsapfs_file_object_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file object IO handle.",
		 function );

		goto on_error;
	}
	( *file_object_io_handle )->file_object = file_object;

	Py_IncRef(
	 ( *file_object_io_handle )->file_object );

	return( 1 );

on_error:
	if( *file_object_io_handle != NULL )
	{
		PyMem_Free(
		 *file_object_io_handle );

		*file_object_io_handle = NULL;
	}
	return( -1 );
}

/* Initializes the file object IO handle
 * Returns 1 if successful or -1 on error
 */
int pyfsapfs_file_object_initialize(
     libbfio_handle_t **handle,
     PyObject *file_object,
     libcerror_error_t **error )
{
	pyfsapfs_file_object_io_handle_t *file_object_io_handle = NULL;
	static char *function                                   = "pyfsapfs_file_object_initialize";

	if( handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid handle.",
		 function );

		return( -1 );
	}
	if( *handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid handle value already set.",
		 function );

		return( -1 );
	}
	if( pyfsapfs_file_object_io_handle_initialize(
	     &file_object_io_handle,
	     file_object,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file object IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_initialize(
	     handle,
	     (intptr_t *) file_object_io_handle,
	     (int (*)(intptr_t **, libcerror_error_t **)) pyfsapfs_file_object_io_handle_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) pyfsapfs_file_object_io_handle_clone,
	     (int (*)(intptr_t *, int, libcerror_error_t **)) pyfsapfs_file_object_io_handle_open,
	     (int (*)(intptr_t *, libcerror_error_t **)) pyfsapfs_file_object_io_handle_close,
	     (ssize_t (*)(intptr_t *, uint8_t *, size_t, libcerror_error_t **)) pyfsapfs_file_object_io_handle_read,
	     (ssize_t (*)(intptr_t *, const uint8_t *, size_t, libcerror_error_t **)) pyfsapfs_file_object_io_handle_write,
	     (off64_t (*)(intptr_t *, off64_t, int, libcerror_error_t **)) pyfsapfs_file_object_io_handle_seek_offset,
	     (int (*)(intptr_t *, libcerror_error_t **)) pyfsapfs_file_object_io_handle_exists,
	     (int (*)(intptr_t *, libcerror_error_t **)) pyfsapfs_file_object_io_handle_is_open,
	     (int (*)(intptr_t *, size64_t *, libcerror_error_t **)) pyfsapfs_file_object_io_handle_get_size,
	     LIBBFIO_FLAG_IO_HANDLE_MANAGED | LIBBFIO_FLAG_IO_HANDLE_CLONE_BY_FUNCTION,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_object_io_handle != NULL )
	{
		pyfsapfs_file_object_io_handle_free(
		 &file_object_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Frees a file object IO handle
 * Returns 1 if succesful or -1 on error
 */
int pyfsapfs_file_object_io_handle_free(
     pyfsapfs_file_object_io_handle_t **file_object_io_handle,
     libcerror_error_t **error )
{
	static char *function      = "pyfsapfs_file_object_io_handle_free";
	PyGILState_STATE gil_state = 0;

	if( file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object IO handle.",
		 function );

		return( -1 );
	}
	if( *file_object_io_handle != NULL )
	{
		gil_state = PyGILState_Ensure();

		Py_DecRef(
		 ( *file_object_io_handle )->file_object );

		PyGILState_Release(
		 gil_state );

		PyMem_Free(
		 *file_object_io_handle );

		*file_object_io_handle = NULL;
	}
	return( 1 );
}

/* Clones (duplicates) the file object IO handle and its attributes
 * Returns 1 if succesful or -1 on error
 */
int pyfsapfs_file_object_io_handle_clone(
     pyfsapfs_file_object_io_handle_t **destination_file_object_io_handle,
     pyfsapfs_file_object_io_handle_t *source_file_object_io_handle,
     libcerror_error_t **error )
{
	static char *function = "pyfsapfs_file_object_io_handle_clone";

	if( destination_file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination file object IO handle.",
		 function );

		return( -1 );
	}
	if( *destination_file_object_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination file object IO handle already set.",
		 function );

		return( -1 );
	}
	if( source_file_object_io_handle == NULL )
	{
		*destination_file_object_io_handle = NULL;

		return( 1 );
	}
	if( pyfsapfs_file_object_io_handle_initialize(
	     destination_file_object_io_handle,
	     source_file_object_io_handle->file_object,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file object IO handle.",
		 function );

		return( -1 );
	}
	if( *destination_file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination file object IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the file object IO handle
 * Returns 1 if successful or -1 on error
 */
int pyfsapfs_file_object_io_handle_open(
     pyfsapfs_file_object_io_handle_t *file_object_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	static char *function = "pyfsapfs_file_object_io_handle_open";

	if( file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object IO handle.",
		 function );

		return( -1 );
	}
	if( file_object_io_handle->file_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file object IO handle - missing file object.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBBFIO_ACCESS_FLAG_READ ) != 0 )
	 && ( ( access_flags & LIBBFIO_ACCESS_FLAG_WRITE ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBBFIO_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	/* No need to do anything here, because the file object is already open
	 */
	file_object_io_handle->access_flags = access_flags;

	return( 1 );
}

/* Closes the file object IO handle
 * Returns 0 if successful or -1 on error
 */
int pyfsapfs_file_object_io_handle_close(
     pyfsapfs_file_object_io_handle_t *file_object_io_handle,
     libcerror_error_t **error )
{
	static char *function = "pyfsapfs_file_object_io_handle_close";

	if( file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object IO handle.",
		 function );

		return( -1 );
	}
	if( file_object_io_handle->file_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file object IO handle - missing file object.",
		 function );

		return( -1 );
	}
	/* Do not close the file object, have Python deal with it
	 */
	file_object_io_handle->access_flags = 0;

	return( 0 );
}

/* Reads a buffer from the file object
 * Make sure to hold the GIL state before calling this function
 * Returns the number of bytes read if successful, or -1 on error
 */
ssize_t pyfsapfs_file_object_read_buffer(
         PyObject *file_object,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	PyObject *argument_size    = NULL;
	PyObject *method_name      = NULL;
	PyObject *method_result    = NULL;
	static char *function      = "pyfsapfs_file_object_read_buffer";
	char *safe_buffer          = NULL;
	Py_ssize_t safe_read_count = 0;
	ssize_t read_count         = 0;
	int result                 = 0;

	if( file_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( size > 0 )
	{
#if PY_MAJOR_VERSION >= 3
		method_name = PyUnicode_FromString(
			       "read" );
#else
		method_name = PyString_FromString(
			       "read" );
#endif
		argument_size = PyLong_FromSize_t(
				 size );

		PyErr_Clear();

		method_result = PyObject_CallMethodObjArgs(
				 file_object,
				 method_name,
				 argument_size,
				 NULL );

		if( PyErr_Occurred() )
		{
			pyfsapfs_error_fetch(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from file object.",
			 function );

			goto on_error;
		}
		if( method_result == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing method result.",
			 function );

			goto on_error;
		}
#if PY_MAJOR_VERSION >= 3
		result = PyObject_IsInstance(
		          method_result,
		          (PyObject *) &PyBytes_Type );
#else
		result = PyObject_IsInstance(
		          method_result,
		          (PyObject *) &PyString_Type );
#endif
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if method result is a binary string object.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid method result value is not a binary string object.",
			 function );

			goto on_error;
		}
#if PY_MAJOR_VERSION >= 3
		result = PyBytes_AsStringAndSize(
			  method_result,
			  &safe_buffer,
			  &safe_read_count );
#else
		result = PyString_AsStringAndSize(
			  method_result,
			  &safe_buffer,
			  &safe_read_count );
#endif
		if( result == -1 )
		{
			pyfsapfs_error_fetch(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from file object.",
			 function );

			goto on_error;
		}
		if( safe_read_count > (Py_ssize_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid read count value exceeds maximum.",
			 function );

			goto on_error;
		}
		read_count = (ssize_t) safe_read_count;

		if( memory_copy(
		     buffer,
		     safe_buffer,
		     read_count ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to data to buffer.",
			 function );

			goto on_error;
		}
		Py_DecRef(
		 method_result );

		Py_DecRef(
		 argument_size );

		Py_DecRef(
		 method_name );
	}
	return( read_count );

on_error:
	if( method_result != NULL )
	{
		Py_DecRef(
		 method_result );
	}
	if( argument_size != NULL )
	{
		Py_DecRef(
		 argument_size );
	}
	if( method_name != NULL )
	{
		Py_DecRef(
		 method_name );
	}
	return( -1 );
}

/* Reads a buffer from the file object IO handle
 * Returns the number of bytes read if successful, or -1 on error
 */
ssize_t pyfsapfs_file_object_io_handle_read(
         pyfsapfs_file_object_io_handle_t *file_object_io_handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	static char *function      = "pyfsapfs_file_object_io_handle_read";
	PyGILState_STATE gil_state = 0;
	ssize_t read_count         = 0;

	if( file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object IO handle.",
		 function );

		return( -1 );
	}
	gil_state = PyGILState_Ensure();

	read_count = pyfsapfs_file_object_read_buffer(
	              file_object_io_handle->file_object,
	              buffer,
	              size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file object.",
		 function );

		goto on_error;
	}
	PyGILState_Release(
	 gil_state );

	return( read_count );

on_error:
	PyGILState_Release(
	 gil_state );

	return( -1 );
}

/* Writes a buffer to the file object
 * Make sure to hold the GIL state before calling this function
 * Returns the number of bytes written if successful, or -1 on error
 */
ssize_t pyfsapfs_file_object_write_buffer(
         PyObject *file_object,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	PyObject *argument_string = NULL;
	PyObject *method_name     = NULL;
	PyObject *method_result   = NULL;
	static char *function     = "pyfsapfs_file_object_write_buffer";

	if( file_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
#if SIZEOF_SIZE_T > SIZEOF_INT
	if( size > (size_t) INT_MAX )
#else
	if( size > (size_t) SSIZE_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( size > 0 )
	{
#if PY_MAJOR_VERSION >= 3
		method_name = PyUnicode_FromString(
			       "write" );
#else
		method_name = PyString_FromString(
			       "write" );
#endif
#if PY_MAJOR_VERSION >= 3
		argument_string = PyBytes_FromStringAndSize(
		                   (char *) buffer,
		                   size );
#else
		argument_string = PyString_FromStringAndSize(
		                   (char *) buffer,
		                   size );
#endif
		PyErr_Clear();

		method_result = PyObject_CallMethodObjArgs(
				 file_object,
				 method_name,
				 argument_string,
				 NULL );

		if( PyErr_Occurred() )
		{
			pyfsapfs_error_fetch(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_WRITE_FAILED,
			 "%s: unable to write to file object.",
			 function );

			goto on_error;
		}
		if( method_result == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing method result.",
			 function );

			goto on_error;
		}
		Py_DecRef(
		 method_result );

		Py_DecRef(
		 argument_string );

		Py_DecRef(
		 method_name );
	}
	return( (ssize_t) size );

on_error:
	if( method_result != NULL )
	{
		Py_DecRef(
		 method_result );
	}
	if( argument_string != NULL )
	{
		Py_DecRef(
		 argument_string );
	}
	if( method_name != NULL )
	{
		Py_DecRef(
		 method_name );
	}
	return( -1 );
}

/* Writes a buffer to the file object IO handle
 * Returns the number of bytes written if successful, or -1 on error
 */
ssize_t pyfsapfs_file_object_io_handle_write(
         pyfsapfs_file_object_io_handle_t *file_object_io_handle,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error )
{
	static char *function      = "pyfsapfs_file_object_io_handle_write";
	PyGILState_STATE gil_state = 0;
	ssize_t write_count        = 0;

	if( file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object IO handle.",
		 function );

		return( -1 );
	}
	gil_state = PyGILState_Ensure();

	write_count = pyfsapfs_file_object_write_buffer(
	               file_object_io_handle->file_object,
	               buffer,
	               size,
	               error );

	if( write_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write from file object.",
		 function );

		goto on_error;
	}
	PyGILState_Release(
	 gil_state );

	return( write_count );

on_error:
	PyGILState_Release(
	 gil_state );

	return( -1 );
}

/* Seeks a certain offset within the file object
 * Make sure to hold the GIL state before calling this function
 * Returns 1 if successful or -1 on error
 */
int pyfsapfs_file_object_seek_offset(
     PyObject *file_object,
     off64_t offset,
     int whence,
     libcerror_error_t **error )
{
	PyObject *argument_offset = NULL;
	PyObject *argument_whence = NULL;
	PyObject *method_name     = NULL;
	PyObject *method_result   = NULL;
	static char *function     = "pyfsapfs_file_object_seek_offset";

	if( file_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LONG_LONG )
	if( offset > (off64_t) INT64_MAX )
#else
	if( offset > (off64_t) LONG_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid offset value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
#if PY_MAJOR_VERSION >= 3
	method_name = PyUnicode_FromString(
	               "seek" );
#else
	method_name = PyString_FromString(
	               "seek" );
#endif
#if defined( HAVE_LONG_LONG )
	argument_offset = PyLong_FromLongLong(
	                   (PY_LONG_LONG) offset );
#else
	argument_offset = PyLong_FromLongLong(
	                   (long) offset );
#endif
#if PY_MAJOR_VERSION >= 3
	argument_whence = PyLong_FromLong(
	                   (long) whence );
#else
	argument_whence = PyInt_FromLong(
	                   (long) whence );
#endif
	PyErr_Clear();

	method_result = PyObject_CallMethodObjArgs(
	                 file_object,
	                 method_name,
	                 argument_offset,
	                 argument_whence,
	                 NULL );

	if( PyErr_Occurred() )
	{
		pyfsapfs_error_fetch(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek in file object.",
		 function );

		goto on_error;
	}
	if( method_result == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing method result.",
		 function );

		goto on_error;
	}
	Py_DecRef(
	 method_result );

	Py_DecRef(
	 argument_whence );

	Py_DecRef(
	 argument_offset );

	Py_DecRef(
	 method_name );

	return( 1 );

on_error:
	if( method_result != NULL )
	{
		Py_DecRef(
		 method_result );
	}
	if( argument_whence != NULL )
	{
		Py_DecRef(
		 argument_whence );
	}
	if( argument_offset != NULL )
	{
		Py_DecRef(
		 argument_offset );
	}
	if( method_name != NULL )
	{
		Py_DecRef(
		 method_name );
	}
	return( -1 );
}

/* Retrieves the current offset within the file object
 * Make sure to hold the GIL state before calling this function
 * Returns 1 if successful or -1 on error
 */
int pyfsapfs_file_object_get_offset(
     PyObject *file_object,
     off64_t *offset,
     libcerror_error_t **error )
{
	PyObject *method_name   = NULL;
	PyObject *method_result = NULL;
	static char *function   = "pyfsapfs_file_object_get_offset";
	int result              = 0;

	if( file_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
#if PY_MAJOR_VERSION >= 3
	method_name = PyUnicode_FromString(
	               "get_offset" );
#else
	method_name = PyString_FromString(
	               "get_offset" );
#endif
	PyErr_Clear();

	/* Determine if the file object has the get_offset method
	 */
	result = PyObject_HasAttr(
	          file_object,
	          method_name );

	if( result == 0 )
	{
		Py_DecRef(
		 method_name );

		/* Fall back to the tell method
		 */
#if PY_MAJOR_VERSION >= 3
		method_name = PyUnicode_FromString(
		               "tell" );
#else
		method_name = PyString_FromString(
		               "tell" );
#endif
	}
	PyErr_Clear();

	method_result = PyObject_CallMethodObjArgs(
	                 file_object,
	                 method_name,
	                 NULL );

	if( PyErr_Occurred() )
	{
		pyfsapfs_error_fetch(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve current offset in file object.",
		 function );

		goto on_error;
	}
	if( method_result == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing method result.",
		 function );

		goto on_error;
	}
	if( pyfsapfs_integer_signed_copy_to_64bit(
	     method_result,
	     offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to convert method result into current offset of file object.",
		 function );

		goto on_error;
	}
	Py_DecRef(
	 method_result );

	Py_DecRef(
	 method_name );

	return( 1 );

on_error:
	if( method_result != NULL )
	{
		Py_DecRef(
		 method_result );
	}
	if( method_name != NULL )
	{
		Py_DecRef(
		 method_name );
	}
	return( -1 );
}

/* Seeks a certain offset within the file object IO handle
 * Returns the offset if the seek is successful or -1 on error
 */
off64_t pyfsapfs_file_object_io_handle_seek_offset(
         pyfsapfs_file_object_io_handle_t *file_object_io_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function      = "pyfsapfs_file_object_io_handle_seek_offset";
	PyGILState_STATE gil_state = 0;

	if( file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object IO handle.",
		 function );

		return( -1 );
	}
	gil_state = PyGILState_Ensure();

	if( pyfsapfs_file_object_seek_offset(
	     file_object_io_handle->file_object,
	     offset,
	     whence,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek in file object.",
		 function );

		goto on_error;
	}
	if( pyfsapfs_file_object_get_offset(
	     file_object_io_handle->file_object,
	     &offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to retrieve current offset in file object.",
		 function );

		goto on_error;
	}
	PyGILState_Release(
	 gil_state );

	return( offset );

on_error:
	PyGILState_Release(
	 gil_state );

	return( -1 );
}

/* Function to determine if a file exists
 * Returns 1 if file exists, 0 if not or -1 on error
 */
int pyfsapfs_file_object_io_handle_exists(
     pyfsapfs_file_object_io_handle_t *file_object_io_handle,
     libcerror_error_t **error )
{
	static char *function = "pyfsapfs_file_object_io_handle_exists";

	if( file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object IO handle.",
		 function );

		return( -1 );
	}
	if( file_object_io_handle->file_object == NULL )
	{
		return( 0 );
	}
	return( 1 );
}

/* Check if the file is open
 * Returns 1 if open, 0 if not or -1 on error
 */
int pyfsapfs_file_object_io_handle_is_open(
     pyfsapfs_file_object_io_handle_t *file_object_io_handle,
     libcerror_error_t **error )
{
	static char *function = "pyfsapfs_file_object_io_handle_is_open";

	if( file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object IO handle.",
		 function );

		return( -1 );
	}
	if( file_object_io_handle->file_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file object IO handle - missing file object.",
		 function );

		return( -1 );
	}
	/* As far as BFIO is concerned the file object is always open
	 */
	return( 1 );
}

/* Retrieves the size of the file object
 * Make sure to hold the GIL state before calling this function
 * Returns 1 if successful or -1 on error
 */
int pyfsapfs_file_object_get_size(
     PyObject *file_object,
     size64_t *size,
     libcerror_error_t **error )
{
	PyObject *method_name   = NULL;
	PyObject *method_result = NULL;
	static char *function   = "pyfsapfs_file_object_get_size";

	if( file_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
#if PY_MAJOR_VERSION >= 3
	method_name = PyUnicode_FromString(
	               "get_size" );
#else
	method_name = PyString_FromString(
	               "get_size" );
#endif
	PyErr_Clear();

	method_result = PyObject_CallMethodObjArgs(
	                 file_object,
	                 method_name,
	                 NULL );

	if( PyErr_Occurred() )
	{
		pyfsapfs_error_fetch(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of file object.",
		 function );

		goto on_error;
	}
	if( method_result == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing method result.",
		 function );

		goto on_error;
	}
	if( pyfsapfs_integer_unsigned_copy_to_64bit(
	     method_result,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to convert method result into size of file object.",
		 function );

		goto on_error;
	}
	Py_DecRef(
	 method_result );

	Py_DecRef(
	 method_name );

	return( 1 );

on_error:
	if( method_result != NULL )
	{
		Py_DecRef(
		 method_result );
	}
	if( method_name != NULL )
	{
		Py_DecRef(
		 method_name );
	}
	return( -1 );
}

/* Retrieves the file size
 * Returns 1 if successful or -1 on error
 */
int pyfsapfs_file_object_io_handle_get_size(
     pyfsapfs_file_object_io_handle_t *file_object_io_handle,
     size64_t *size,
     libcerror_error_t **error )
{
	PyObject *method_name      = NULL;
	static char *function      = "pyfsapfs_file_object_io_handle_get_size";
	PyGILState_STATE gil_state = 0;
	off64_t current_offset     = 0;
	int result                 = 0;

	if( file_object_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file object IO handle.",
		 function );

		return( -1 );
	}
	if( file_object_io_handle->file_object == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file object IO handle - missing file object.",
		 function );

		return( -1 );
	}
	gil_state = PyGILState_Ensure();

#if PY_MAJOR_VERSION >= 3
	method_name = PyUnicode_FromString(
	               "get_size" );
#else
	method_name = PyString_FromString(
	               "get_size" );
#endif
	PyErr_Clear();

	/* Determine if the file object has the get_size method
	 */
	result = PyObject_HasAttr(
	          file_object_io_handle->file_object,
	          method_name );

	if( result != 0 )
	{
		if( pyfsapfs_file_object_get_size(
		     file_object_io_handle->file_object,
		     size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of file object.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( pyfsapfs_file_object_get_offset(
		     file_object_io_handle->file_object,
		     &current_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve current offset in file object.",
			 function );

			goto on_error;
		}
		if( pyfsapfs_file_object_seek_offset(
		     file_object_io_handle->file_object,
		     0,
		     SEEK_END,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek end of file object.",
			 function );

			goto on_error;
		}
		if( pyfsapfs_file_object_get_offset(
		     file_object_io_handle->file_object,
		     (off64_t *) size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve end offset in file object.",
			 function );

			pyfsapfs_file_object_seek_offset(
			 file_object_io_handle->file_object,
			 current_offset,
			 SEEK_SET,
			 NULL );

			goto on_error;
		}
		if( pyfsapfs_file_object_seek_offset(
		     file_object_io_handle->file_object,
		     current_offset,
		     SEEK_SET,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek current offset in file object.",
			 function );

			goto on_error;
		}
	}
	Py_DecRef(
	 method_name );

	PyGILState_Release(
	 gil_state );

	return( 1 );

on_error:
	if( method_name != NULL )
	{
		Py_DecRef(
		 method_name );
	}
	PyGILState_Release(
	 gil_state );

	return( 1 );
}

