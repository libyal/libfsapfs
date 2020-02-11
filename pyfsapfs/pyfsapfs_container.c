/*
 * Python object wrapper of libfsapfs_container_t
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

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyfsapfs_container.h"
#include "pyfsapfs_error.h"
#include "pyfsapfs_file_object_io_handle.h"
#include "pyfsapfs_guid.h"
#include "pyfsapfs_integer.h"
#include "pyfsapfs_libbfio.h"
#include "pyfsapfs_libcerror.h"
#include "pyfsapfs_libfsapfs.h"
#include "pyfsapfs_python.h"
#include "pyfsapfs_unused.h"
#include "pyfsapfs_volume.h"
#include "pyfsapfs_volumes.h"

#if !defined( LIBFSAPFS_HAVE_BFIO )

LIBFSAPFS_EXTERN \
int libfsapfs_container_open_file_io_handle(
     libfsapfs_container_t *container,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsapfs_error_t **error );

#endif /* !defined( LIBFSAPFS_HAVE_BFIO ) */

PyMethodDef pyfsapfs_container_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pyfsapfs_container_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the container to abort the current activity." },

	{ "open",
	  (PyCFunction) pyfsapfs_container_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> None\n"
	  "\n"
	  "Opens a container." },

	{ "open_file_object",
	  (PyCFunction) pyfsapfs_container_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> None\n"
	  "\n"
	  "Opens a container using a file-like object." },

	{ "close",
	  (PyCFunction) pyfsapfs_container_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a container." },

	{ "get_size",
	  (PyCFunction) pyfsapfs_container_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer\n"
	  "\n"
	  "Retrieves the size." },

	{ "get_identifier",
	  (PyCFunction) pyfsapfs_container_get_identifier,
	  METH_NOARGS,
	  "get_identifier() -> Unicode string\n"
	  "\n"
	  "Retrieves the identifier." },

	{ "get_number_of_volumes",
	  (PyCFunction) pyfsapfs_container_get_number_of_volumes,
	  METH_NOARGS,
	  "get_number_of_volumes() -> Integer\n"
	  "\n"
	  "Retrieves the number of volumes." },

	{ "get_volume",
	  (PyCFunction) pyfsapfs_container_get_volume,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_volume(volume_index) -> Object\n"
	  "\n"
	  "Retrieves the of volume specified by the index." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsapfs_container_object_get_set_definitions[] = {

	{ "size",
	  (getter) pyfsapfs_container_get_size,
	  (setter) 0,
	  "The size.",
	  NULL },

	{ "identifier",
	  (getter) pyfsapfs_container_get_identifier,
	  (setter) 0,
	  "The identifier.",
	  NULL },

	{ "number_of_volumes",
	  (getter) pyfsapfs_container_get_number_of_volumes,
	  (setter) 0,
	  "The number of volumes.",
	  NULL },

	{ "volumes",
	  (getter) pyfsapfs_container_get_volumes,
	  (setter) 0,
	  "The of volumes.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsapfs_container_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsapfs.container",
	/* tp_basicsize */
	sizeof( pyfsapfs_container_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsapfs_container_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pyfsapfs container object (wraps libfsapfs_container_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pyfsapfs_container_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsapfs_container_object_get_set_definitions,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyfsapfs_container_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Intializes a container object
 * Returns 0 if successful or -1 on error
 */
int pyfsapfs_container_init(
     pyfsapfs_container_t *pyfsapfs_container )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_container_init";

	if( pyfsapfs_container == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid container.",
		 function );

		return( -1 );
	}
	/* Make sure libfsapfs container is set to NULL
	 */
	pyfsapfs_container->container      = NULL;
	pyfsapfs_container->file_io_handle = NULL;

	if( libfsapfs_container_initialize(
	     &( pyfsapfs_container->container ),
	     &error ) != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize container.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a container object
 */
void pyfsapfs_container_free(
      pyfsapfs_container_t *pyfsapfs_container )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsapfs_container_free";
	int result                  = 0;

	if( pyfsapfs_container == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid container.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsapfs_container );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyfsapfs_container->container != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsapfs_container_free(
		          &( pyfsapfs_container->container ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsapfs_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfsapfs container.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	ob_type->tp_free(
	 (PyObject*) pyfsapfs_container );
}

/* Signals the container to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_container_signal_abort(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_container_signal_abort";
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_container == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid container.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_container_signal_abort(
	          pyfsapfs_container->container,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to signal abort.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a container
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_container_open(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	const char *filename_narrow  = NULL;
	static char *function        = "pyfsapfs_container_open";
	static char *keyword_list[]  = { "filename", "mode", NULL };
	char *mode                   = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	if( pyfsapfs_container == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid container.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &string_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyfsapfs_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type Unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libfsapfs_container_open_wide(
		          pyfsapfs_container->container,
		          filename_wide,
		          LIBFSAPFS_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyfsapfs_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert Unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
		                   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libfsapfs_container_open(
		          pyfsapfs_container->container,
		          filename_narrow,
		          LIBFSAPFS_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result != 1 )
		{
			pyfsapfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open container.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyfsapfs_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   string_object );
#else
		filename_narrow = PyString_AsString(
		                   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libfsapfs_container_open(
		          pyfsapfs_container->container,
		          filename_narrow,
		          LIBFSAPFS_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsapfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open container.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Opens a container using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_container_open_file_object(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object       = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsapfs_container_open_file_object";
	static char *keyword_list[] = { "file_object", "mode", NULL };
	char *mode                  = NULL;
	int result                  = 0;

	if( pyfsapfs_container == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid container.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &file_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	if( pyfsapfs_container->file_io_handle != NULL )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: invalid container - file IO handle already set.",
		 function );

		goto on_error;
	}
	if( pyfsapfs_file_object_initialize(
	     &( pyfsapfs_container->file_io_handle ),
	     file_object,
	     &error ) != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_container_open_file_io_handle(
	          pyfsapfs_container->container,
	          pyfsapfs_container->file_io_handle,
	          LIBFSAPFS_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open container.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pyfsapfs_container->file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( pyfsapfs_container->file_io_handle ),
		 NULL );
	}
	return( NULL );
}

/* Closes a container
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_container_close(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_container_close";
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_container == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid container.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_container_close(
	          pyfsapfs_container->container,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close container.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( pyfsapfs_container->file_io_handle != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_handle_free(
		          &( pyfsapfs_container->file_io_handle ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsapfs_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libbfio file IO handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_container_get_size(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_container_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_container == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid container.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_container_get_size(
	          pyfsapfs_container->container,
	          &size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsapfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) size );

	return( integer_object );
}

/* Retrieves the identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_container_get_identifier(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	uint8_t uuid_data[ 16 ];

	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_container_get_identifier";
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_container == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid container.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_container_get_identifier(
	          pyfsapfs_container->container,
	          uuid_data,
	          16,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	string_object = pyfsapfs_string_new_from_guid(
	                 uuid_data,
	                 16 );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UUID into Unicode object.",
		 function );

		return( NULL );
	}
	return( string_object );
}

/* Retrieves the number of volumes
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_container_get_number_of_volumes(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_container_get_number_of_volumes";
	int number_of_volumes    = 0;
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_container == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid container.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_container_get_number_of_volumes(
	          pyfsapfs_container->container,
	          &number_of_volumes,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of volumes.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_volumes );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_volumes );
#endif
	return( integer_object );
}

/* Retrieves a specific of volume by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_container_get_volume_by_index(
           PyObject *pyfsapfs_container,
           int volume_index )
{
	PyObject *volume_object    = NULL;
	libcerror_error_t *error   = NULL;
	libfsapfs_volume_t *volume = NULL;
	static char *function      = "pyfsapfs_container_get_volume_by_index";
	int result                 = 0;

	if( pyfsapfs_container == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid container.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_container_get_volume_by_index(
	          ( (pyfsapfs_container_t *) pyfsapfs_container )->container,
	          volume_index,
	          &volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve of volume: %d.",
		 function,
		 volume_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	volume_object = pyfsapfs_volume_new(
	                 volume,
	                 pyfsapfs_container );

	if( volume_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create volume object.",
		 function );

		goto on_error;
	}
	return( volume_object );

on_error:
	if( volume != NULL )
	{
		libfsapfs_volume_free(
		 &volume,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific of volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_container_get_volume(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *volume_object     = NULL;
	static char *keyword_list[] = { "volume_index", NULL };
	int volume_index            = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &volume_index ) == 0 )
	{
		return( NULL );
	}
	volume_object = pyfsapfs_container_get_volume_by_index(
	                 (PyObject *) pyfsapfs_container,
	                 volume_index );

	return( volume_object );
}

/* Retrieves a sequence and iterator object for the volumes
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_container_get_volumes(
           pyfsapfs_container_t *pyfsapfs_container,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfsapfs_container_get_volumes";
	int number_of_volumes     = 0;
	int result                = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_container == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid container.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_container_get_number_of_volumes(
	          pyfsapfs_container->container,
	          &number_of_volumes,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of volumes.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pyfsapfs_volumes_new(
	                   (PyObject *) pyfsapfs_container,
	                   &pyfsapfs_container_get_volume_by_index,
	                   number_of_volumes );

	if( sequence_object == NULL )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

