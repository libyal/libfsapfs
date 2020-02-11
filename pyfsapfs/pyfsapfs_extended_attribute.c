/*
 * Python object wrapper of libfsapfs_extended_attribute_t
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

#include "pyfsapfs_error.h"
#include "pyfsapfs_extended_attribute.h"
#include "pyfsapfs_integer.h"
#include "pyfsapfs_libcerror.h"
#include "pyfsapfs_libfsapfs.h"
#include "pyfsapfs_python.h"
#include "pyfsapfs_unused.h"

PyMethodDef pyfsapfs_extended_attribute_object_methods[] = {

	{ "get_identifier",
	  (PyCFunction) pyfsapfs_extended_attribute_get_identifier,
	  METH_NOARGS,
	  "get_identifier() -> Integer\n"
	  "\n"
	  "Retrieves the identifier." },

	{ "get_name",
	  (PyCFunction) pyfsapfs_extended_attribute_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string\n"
	  "\n"
	  "Retrieves the name." },

	{ "read_buffer",
	  (PyCFunction) pyfsapfs_extended_attribute_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> Binary string\n"
	  "\n"
	  "Reads a buffer of data." },

	{ "read_buffer_at_offset",
	  (PyCFunction) pyfsapfs_extended_attribute_read_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer_at_offset(size, offset) -> Binary string\n"
	  "\n"
	  "Reads a buffer of data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pyfsapfs_extended_attribute_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the data." },

	{ "get_offset",
	  (PyCFunction) pyfsapfs_extended_attribute_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Retrieves the current offset within the data." },

	{ "read",
	  (PyCFunction) pyfsapfs_extended_attribute_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> Binary string\n"
	  "\n"
	  "Reads a buffer of data." },

	{ "seek",
	  (PyCFunction) pyfsapfs_extended_attribute_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the data." },

	{ "tell",
	  (PyCFunction) pyfsapfs_extended_attribute_get_offset,
	  METH_NOARGS,
	  "tell() -> Integer\n"
	  "\n"
	  "Retrieves the current offset within the data." },

	{ "get_size",
	  (PyCFunction) pyfsapfs_extended_attribute_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer\n"
	  "\n"
	  "Retrieves the size of the data stream object." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsapfs_extended_attribute_object_get_set_definitions[] = {

	{ "identifier",
	  (getter) pyfsapfs_extended_attribute_get_identifier,
	  (setter) 0,
	  "The identifier.",
	  NULL },

	{ "name",
	  (getter) pyfsapfs_extended_attribute_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "size",
	  (getter) pyfsapfs_extended_attribute_get_size,
	  (setter) 0,
	  "The size of the data stream object.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsapfs_extended_attribute_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsapfs.extended_attribute",
	/* tp_basicsize */
	sizeof( pyfsapfs_extended_attribute_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsapfs_extended_attribute_free,
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
	"pyfsapfs extended attribute object (wraps libfsapfs_extended_attribute_t)",
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
	pyfsapfs_extended_attribute_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsapfs_extended_attribute_object_get_set_definitions,
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
	(initproc) pyfsapfs_extended_attribute_init,
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

/* Creates a new extended attribute object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_extended_attribute_new(
           libfsapfs_extended_attribute_t *extended_attribute,
           PyObject *parent_object )
{
	pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute = NULL;
	static char *function                                      = "pyfsapfs_extended_attribute_new";

	if( extended_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid extended attribute.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pyfsapfs_extended_attribute = PyObject_New(
	                               struct pyfsapfs_extended_attribute,
	                               &pyfsapfs_extended_attribute_type_object );

	if( pyfsapfs_extended_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize extended attribute.",
		 function );

		goto on_error;
	}
	pyfsapfs_extended_attribute->extended_attribute = extended_attribute;
	pyfsapfs_extended_attribute->parent_object      = parent_object;

	if( pyfsapfs_extended_attribute->parent_object != NULL )
	{
		Py_IncRef(
		 pyfsapfs_extended_attribute->parent_object );
	}
	return( (PyObject *) pyfsapfs_extended_attribute );

on_error:
	if( pyfsapfs_extended_attribute != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfsapfs_extended_attribute );
	}
	return( NULL );
}

/* Intializes an extended attribute object
 * Returns 0 if successful or -1 on error
 */
int pyfsapfs_extended_attribute_init(
     pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute )
{
	static char *function = "pyfsapfs_extended_attribute_init";

	if( pyfsapfs_extended_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	/* Make sure libfsapfs extended attribute is set to NULL
	 */
	pyfsapfs_extended_attribute->extended_attribute = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of extended attribute not supported.",
	 function );

	return( -1 );
}

/* Frees an extended attribute object
 */
void pyfsapfs_extended_attribute_free(
      pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsapfs_extended_attribute_free";
	int result                  = 0;

	if( pyfsapfs_extended_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid extended attribute.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsapfs_extended_attribute );

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
	if( pyfsapfs_extended_attribute->extended_attribute != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsapfs_extended_attribute_free(
		          &( pyfsapfs_extended_attribute->extended_attribute ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsapfs_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfsapfs extended attribute.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyfsapfs_extended_attribute->parent_object != NULL )
	{
		Py_DecRef(
		 pyfsapfs_extended_attribute->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfsapfs_extended_attribute );
}

/* Retrieves the 
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_extended_attribute_get_identifier(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_extended_attribute_get_identifier";
	uint64_t value_64bit     = 0;
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_extended_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid extended attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_extended_attribute_get_identifier(
	          pyfsapfs_extended_attribute->extended_attribute,
	          &value_64bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve .",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsapfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) value_64bit );

	return( integer_object );
}

/* Retrieves the 
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_extended_attribute_get_name(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	const char *errors       = NULL;
	static char *function    = "pyfsapfs_extended_attribute_get_name";
	char *utf8_string        = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_extended_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid extended attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_extended_attribute_get_utf8_name_size(
	          pyfsapfs_extended_attribute->extended_attribute,
	          &utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine size of  as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( utf8_string_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	utf8_string = (char *) PyMem_Malloc(
	                        sizeof( char ) * utf8_string_size );

	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_extended_attribute_get_utf8_name(
	          pyfsapfs_extended_attribute->extended_attribute,
	          (uint8_t *) utf8_string,
	          utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve  as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8 otherwise it makes
	 * the end of string character is part of the string.
	 */
	string_object = PyUnicode_DecodeUTF8(
	                 utf8_string,
	                 (Py_ssize_t) utf8_string_size - 1,
	                 errors );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UTF-8 string into Unicode object.",
		 function );

		goto on_error;
	}
	PyMem_Free(
	 utf8_string );

	return( string_object );

on_error:
	if( utf8_string != NULL )
	{
		PyMem_Free(
		 utf8_string );
	}
	return( NULL );
}

/* Reads data at the current offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_extended_attribute_read_buffer(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	char *buffer                = NULL;
	static char *function       = "pyfsapfs_extended_attribute_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	ssize_t read_count          = 0;
	int64_t read_size           = 0;
	int result                  = 0;

	if( pyfsapfs_extended_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid extended attribute.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &integer_object ) == 0 )
	{
		return( NULL );
	}
	if( integer_object == NULL )
	{
		result = 0;
	}
	else
	{
		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyLong_Type );

		if( result == -1 )
		{
			pyfsapfs_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type long.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION < 3
		else if( result == 0 )
		{
			PyErr_Clear();

			result = PyObject_IsInstance(
			          integer_object,
			          (PyObject *) &PyInt_Type );

			if( result == -1 )
			{
				pyfsapfs_error_fetch_and_raise(
				 PyExc_RuntimeError,
				 "%s: unable to determine if integer object is of type int.",
				 function );

				return( NULL );
			}
		}
#endif
	}
	if( result != 0 )
	{
		if( pyfsapfs_integer_signed_copy_to_64bit(
		     integer_object,
		     &read_size,
		     &error ) != 1 )
		{
			pyfsapfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else if( ( integer_object == NULL )
	      || ( integer_object == Py_None ) )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsapfs_extended_attribute_get_size(
		          pyfsapfs_extended_attribute->extended_attribute,
		          (size64_t *) &read_size,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsapfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (int64_t) INT_MAX )
	 || ( read_size > (int64_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfsapfs_extended_attribute_read_buffer(
	              pyfsapfs_extended_attribute->extended_attribute,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count == -1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Reads data at a specific offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_extended_attribute_read_buffer_at_offset(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	char *buffer                = NULL;
	static char *function       = "pyfsapfs_extended_attribute_read_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	ssize_t read_count          = 0;
	off64_t read_offset         = 0;
	int64_t read_size           = 0;
	int result                  = 0;

	if( pyfsapfs_extended_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid extended attribute.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "OL",
	     keyword_list,
	     &integer_object,
	     &read_offset ) == 0 )
	{
		return( NULL );
	}
	result = PyObject_IsInstance(
	          integer_object,
	          (PyObject *) &PyLong_Type );

	if( result == -1 )
	{
		pyfsapfs_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if integer object is of type long.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION < 3
	else if( result == 0 )
	{
		PyErr_Clear();

		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyInt_Type );

		if( result == -1 )
		{
			pyfsapfs_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type int.",
			 function );

			return( NULL );
		}
	}
#endif
	if( result != 0 )
	{
		if( pyfsapfs_integer_signed_copy_to_64bit(
		     integer_object,
		     &read_size,
		     &error ) != 1 )
		{
			pyfsapfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (int64_t) INT_MAX )
	 || ( read_size > (int64_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	if( read_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read offset value less than zero.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfsapfs_extended_attribute_read_buffer_at_offset(
	              pyfsapfs_extended_attribute->extended_attribute,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count == -1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Seeks a certain offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_extended_attribute_seek_offset(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsapfs_extended_attribute_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pyfsapfs_extended_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid extended attribute.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "L|i",
	     keyword_list,
	     &offset,
	     &whence ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	offset = libfsapfs_extended_attribute_seek_offset(
	          pyfsapfs_extended_attribute->extended_attribute,
	          offset,
	          whence,
	          &error );

	Py_END_ALLOW_THREADS

 	if( offset == -1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to seek offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the current offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_extended_attribute_get_offset(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_extended_attribute_get_offset";
	off64_t offset           = 0;
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_extended_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid extended attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_extended_attribute_get_offset(
	          pyfsapfs_extended_attribute->extended_attribute,
	          &offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve current offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = pyfsapfs_integer_signed_new_from_64bit(
	                  (int64_t) offset );

	return( integer_object );
}

/* Retrieves the size of the data stream object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_extended_attribute_get_size(
           pyfsapfs_extended_attribute_t *pyfsapfs_extended_attribute,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_extended_attribute_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_extended_attribute == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid extended attribute.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_extended_attribute_get_size(
	          pyfsapfs_extended_attribute->extended_attribute,
	          &size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve size of the data stream object.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsapfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) size );

	return( integer_object );
}

