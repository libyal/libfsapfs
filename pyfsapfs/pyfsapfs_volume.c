/*
 * Python object wrapper of libfsapfs_volume_t
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
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyfsapfs_error.h"
#include "pyfsapfs_file_entry.h"
#include "pyfsapfs_file_object_io_handle.h"
#include "pyfsapfs_guid.h"
#include "pyfsapfs_integer.h"
#include "pyfsapfs_libbfio.h"
#include "pyfsapfs_libcerror.h"
#include "pyfsapfs_libfsapfs.h"
#include "pyfsapfs_python.h"
#include "pyfsapfs_unused.h"
#include "pyfsapfs_volume.h"

#if !defined( LIBFSAPFS_HAVE_BFIO )

LIBFSAPFS_EXTERN \
int libfsapfs_volume_open_file_io_handle(
     libfsapfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsapfs_error_t **error );

#endif /* !defined( LIBFSAPFS_HAVE_BFIO ) */

PyMethodDef pyfsapfs_volume_object_methods[] = {

	{ "unlock",
	  (PyCFunction) pyfsapfs_volume_unlock,
	  METH_NOARGS,
	  "unlock() -> Boolean\n"
	  "\n"
	  "Unlock a volume." },

	{ "get_size",
	  (PyCFunction) pyfsapfs_volume_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer\n"
	  "\n"
	  "Retrieves the size." },

	{ "get_identifier",
	  (PyCFunction) pyfsapfs_volume_get_identifier,
	  METH_NOARGS,
	  "get_identifier() -> Unicode string\n"
	  "\n"
	  "Retrieves the identifier." },

	{ "get_name",
	  (PyCFunction) pyfsapfs_volume_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string\n"
	  "\n"
	  "Retrieves the name." },

	{ "is_locked",
	  (PyCFunction) pyfsapfs_volume_is_locked,
	  METH_NOARGS,
	  "is_locked() -> Boolean or None\n"
	  "\n"
	  "Determines if the volume is locked." },

	{ "set_password",
	  (PyCFunction) pyfsapfs_volume_set_password,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_password(password) -> None\n"
	  "\n"
	  "Sets the password." },

	{ "set_recovery_password",
	  (PyCFunction) pyfsapfs_volume_set_recovery_password,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_recovery_password(recovery_password) -> None\n"
	  "\n"
	  "Sets the recovery password." },

	{ "get_next_file_entry_identifier",
	  (PyCFunction) pyfsapfs_volume_get_next_file_entry_identifier,
	  METH_NOARGS,
	  "get_next_file_entry_identifier() -> Integer\n"
	  "\n"
	  "Retrieves the next file entry identifier." },

	{ "get_file_entry_by_identifier",
	  (PyCFunction) pyfsapfs_volume_get_file_entry_by_identifier,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_file_entry_by_identifier(identifier) -> Object or None\n"
	  "\n"
	  "Retrieves the file entry specified by the identifier." },

	{ "get_root_directory",
	  (PyCFunction) pyfsapfs_volume_get_root_directory,
	  METH_NOARGS,
	  "get_root_directory() -> Object\n"
	  "\n"
	  "Retrieves the root directory file entry." },

	{ "get_file_entry_by_path",
	  (PyCFunction) pyfsapfs_volume_get_file_entry_by_path,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_file_entry_by_path(path) -> Object or None\n"
	  "\n"
	  "Retrieves the file entry for an UTF-8 encoded path specified by the path." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsapfs_volume_object_get_set_definitions[] = {

	{ "size",
	  (getter) pyfsapfs_volume_get_size,
	  (setter) 0,
	  "The size.",
	  NULL },

	{ "identifier",
	  (getter) pyfsapfs_volume_get_identifier,
	  (setter) 0,
	  "The identifier.",
	  NULL },

	{ "name",
	  (getter) pyfsapfs_volume_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "next_file_entry_identifier",
	  (getter) pyfsapfs_volume_get_next_file_entry_identifier,
	  (setter) 0,
	  "The next file entry identifier.",
	  NULL },

	{ "root_directory",
	  (getter) pyfsapfs_volume_get_root_directory,
	  (setter) 0,
	  "The root directory file entry.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsapfs_volume_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsapfs.volume",
	/* tp_basicsize */
	sizeof( pyfsapfs_volume_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsapfs_volume_free,
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
	"pyfsapfs volume object (wraps libfsapfs_volume_t)",
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
	pyfsapfs_volume_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsapfs_volume_object_get_set_definitions,
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
	(initproc) pyfsapfs_volume_init,
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

/* Creates a new volume object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_volume_new(
           libfsapfs_volume_t *volume,
           PyObject *parent_object )
{
	pyfsapfs_volume_t *pyfsapfs_volume = NULL;
	static char *function              = "pyfsapfs_volume_new";

	if( volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pyfsapfs_volume = PyObject_New(
	                   struct pyfsapfs_volume,
	                   &pyfsapfs_volume_type_object );

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize volume.",
		 function );

		goto on_error;
	}
	pyfsapfs_volume->volume        = volume;
	pyfsapfs_volume->parent_object = parent_object;

	if( pyfsapfs_volume->parent_object != NULL )
	{
		Py_IncRef(
		 pyfsapfs_volume->parent_object );
	}
	return( (PyObject *) pyfsapfs_volume );

on_error:
	if( pyfsapfs_volume != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfsapfs_volume );
	}
	return( NULL );
}

/* Intializes a volume object
 * Returns 0 if successful or -1 on error
 */
int pyfsapfs_volume_init(
     pyfsapfs_volume_t *pyfsapfs_volume )
{
	static char *function = "pyfsapfs_volume_init";

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	/* Make sure libfsapfs volume is set to NULL
	 */
	pyfsapfs_volume->volume = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of volume not supported.",
	 function );

	return( -1 );
}

/* Frees a volume object
 */
void pyfsapfs_volume_free(
      pyfsapfs_volume_t *pyfsapfs_volume )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsapfs_volume_free";
	int result                  = 0;

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsapfs_volume );

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
	if( pyfsapfs_volume->volume != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsapfs_volume_free(
		          &( pyfsapfs_volume->volume ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsapfs_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfsapfs volume.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyfsapfs_volume->parent_object != NULL )
	{
		Py_DecRef(
		 pyfsapfs_volume->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfsapfs_volume );
}

/* Unlocks a volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_volume_unlock(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_volume_unlock";
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_volume_unlock(
	          pyfsapfs_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to unlock volume.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Retrieves the size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_volume_get_size(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_volume_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_volume_get_size(
	          pyfsapfs_volume->volume,
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
PyObject *pyfsapfs_volume_get_identifier(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	uint8_t uuid_data[ 16 ];

	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_volume_get_identifier";
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_volume_get_identifier(
	          pyfsapfs_volume->volume,
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

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_volume_get_name(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	const char *errors       = NULL;
	static char *function    = "pyfsapfs_volume_get_name";
	char *utf8_string        = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_volume_get_utf8_name_size(
	          pyfsapfs_volume->volume,
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

	result = libfsapfs_volume_get_utf8_name(
	          pyfsapfs_volume->volume,
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

/* Determines if the volume is locked
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_volume_is_locked(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_volume_is_locked";
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_volume_is_locked(
	          pyfsapfs_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if volume is UTF-16 encoded name.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Sets the password
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_volume_set_password(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsapfs_volume_set_password";
	static char *keyword_list[] = { "password", NULL };
	char *utf8_string           = NULL;
	size_t utf8_string_length   = 0;
	int result                  = 0;

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &utf8_string ) == 0 )
	{
		return( NULL );
	}
	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid .",
		 function );

		return( NULL );
	}
	utf8_string_length = narrow_string_length(
	                      utf8_string );

	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_volume_set_utf8_password(
	          pyfsapfs_volume->volume,
	          (uint8_t *) utf8_string,
	          utf8_string_length,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to set .",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Sets the recovery password
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_volume_set_recovery_password(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsapfs_volume_set_recovery_password";
	static char *keyword_list[] = { "recovery_password", NULL };
	char *utf8_string           = NULL;
	size_t utf8_string_length   = 0;
	int result                  = 0;

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &utf8_string ) == 0 )
	{
		return( NULL );
	}
	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid .",
		 function );

		return( NULL );
	}
	utf8_string_length = narrow_string_length(
	                      utf8_string );

	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_volume_set_utf8_recovery_password(
	          pyfsapfs_volume->volume,
	          (uint8_t *) utf8_string,
	          utf8_string_length,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to set .",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the next file entry identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_volume_get_next_file_entry_identifier(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsapfs_volume_get_next_file_entry_identifier";
	uint64_t value_64bit     = 0;
	int result               = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_volume_get_next_file_entry_identifier(
	          pyfsapfs_volume->volume,
	          &value_64bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve next file entry identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsapfs_integer_unsigned_new_from_64bit(
	                  (uint64_t) value_64bit );

	return( integer_object );
}

/* Retrieves a specific of file entry by identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_volume_get_file_entry_by_identifier(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_entry_object              = NULL;
	libcerror_error_t *error                 = NULL;
	libfsapfs_file_entry_t *file_entry       = NULL;
	static char *function                    = "pyfsapfs_volume_get_file_entry_by_identifier";
	static char *keyword_list[]              = { "file_entry_identifier", NULL };
	unsigned long long file_entry_identifier = 0;
	int result                               = 0;

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "K",
	     keyword_list,
	     &file_entry_identifier ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_volume_get_file_entry_by_identifier(
	          ( (pyfsapfs_volume_t *) pyfsapfs_volume )->volume,
	          (uint64_t) file_entry_identifier,
	          &file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve of file entry: %d.",
		 function,
		 file_entry_identifier );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	file_entry_object = pyfsapfs_file_entry_new(
	                     file_entry,
	                     (PyObject *) pyfsapfs_volume );

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create file entry object.",
		 function );

		goto on_error;
	}
	return( file_entry_object );

on_error:
	if( file_entry != NULL )
	{
		libfsapfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the root directory file entry
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_volume_get_root_directory(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	PyObject *directory_object             = NULL;
	libcerror_error_t *error               = NULL;
	libfsapfs_file_entry_t *root_directory = NULL;
	static char *function                  = "pyfsapfs_volume_get_root_directory";
	int result                             = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_volume_get_root_directory(
	          pyfsapfs_volume->volume,
	          &root_directory,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve root directory file entry.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	directory_object = pyfsapfs_file_entry_new(
	                    root_directory,
	                    (PyObject *) pyfsapfs_volume );

	if( directory_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create root directory file entry object.",
		 function );

		goto on_error;
	}
	return( directory_object );

on_error:
	if( root_directory != NULL )
	{
		libfsapfs_file_entry_free(
		 &root_directory,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the file entry specified by the path
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_volume_get_file_entry_by_path(
           pyfsapfs_volume_t *pyfsapfs_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_entry_object        = NULL;
	libcerror_error_t *error           = NULL;
	libfsapfs_file_entry_t *file_entry = NULL;
	static char *function              = "pyfsapfs_volume_get_file_entry_by_path";
	static char *keyword_list[]        = { "path", NULL };
	char *utf8_path                    = NULL;
	size_t utf8_path_length            = 0;
	int result                         = 0;

	if( pyfsapfs_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &utf8_path ) == 0 )
	{
		goto on_error;
	}
	utf8_path_length = narrow_string_length(
	                    utf8_path );

	Py_BEGIN_ALLOW_THREADS

	result = libfsapfs_volume_get_file_entry_by_utf8_path(
	          pyfsapfs_volume->volume,
	          (uint8_t *) utf8_path,
	          utf8_path_length,
	          &file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve file entry for an UTF-8 encoded path.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	file_entry_object = pyfsapfs_file_entry_new(
	                     file_entry,
	                     (PyObject *) pyfsapfs_volume );

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create file entry object.",
		 function );

		goto on_error;
	}
	return( file_entry_object );

on_error:
	if( file_entry != NULL )
	{
		libfsapfs_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( NULL );
}

