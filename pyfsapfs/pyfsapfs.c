/*
 * Python bindings module for libfsapfs (pyfsapfs)
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

#include "pyfsapfs.h"
#include "pyfsapfs_container.h"
#include "pyfsapfs_error.h"
#include "pyfsapfs_extended_attribute.h"
#include "pyfsapfs_extended_attributes.h"
#include "pyfsapfs_file_entries.h"
#include "pyfsapfs_file_entry.h"
#include "pyfsapfs_file_object_io_handle.h"
#include "pyfsapfs_libcerror.h"
#include "pyfsapfs_libfsapfs.h"
#include "pyfsapfs_python.h"
#include "pyfsapfs_unused.h"
#include "pyfsapfs_volume.h"
#include "pyfsapfs_volumes.h"

#if !defined( LIBFSAPFS_HAVE_BFIO )

LIBFSAPFS_EXTERN \
int libfsapfs_check_container_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libfsapfs_error_t **error );

#endif /* !defined( LIBFSAPFS_HAVE_BFIO ) */

/* The pyfsapfs module methods
 */
PyMethodDef pyfsapfs_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pyfsapfs_get_version,
	  METH_NOARGS,
	  "get_version() -> String\n"
	  "\n"
	  "Retrieves the version." },

	{ "check_container_signature",
	  (PyCFunction) pyfsapfs_check_container_signature,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_container_signature(filename) -> Boolean\n"
	  "\n"
	  "Checks if a container has an APFS container signature." },

	{ "check_container_signature_file_object",
	  (PyCFunction) pyfsapfs_check_container_signature_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_container_signature_file_object(file_object) -> Boolean\n"
	  "\n"
	  "Checks if a container has an APFS container signature using a file-like object." },

	{ "open",
	  (PyCFunction) pyfsapfs_open_new_container,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> Object\n"
	  "\n"
	  "Opens a container." },

	{ "open_file_object",
	  (PyCFunction) pyfsapfs_open_new_container_with_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> Object\n"
	  "\n"
	  "Opens a container using a file-like object." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

/* Retrieves the pyfsapfs/libfsapfs version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_get_version(
           PyObject *self PYFSAPFS_ATTRIBUTE_UNUSED,
           PyObject *arguments PYFSAPFS_ATTRIBUTE_UNUSED )
{
	const char *errors           = NULL;
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( self )
	PYFSAPFS_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libfsapfs_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         errors ) );
}

/* Checks if a container has an APFS container signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_check_container_signature(
           PyObject *self PYFSAPFS_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pyfsapfs_check_container_signature";
	static char *keyword_list[]  = { "filename", NULL };
	const char *filename_narrow  = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	PYFSAPFS_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &string_object ) == 0 )
	{
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

		result = libfsapfs_check_container_signature_wide(
		          filename_wide,
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

		result = libfsapfs_check_container_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

		if( result == -1 )
		{
			pyfsapfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check container signature.",
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

		result = libfsapfs_check_container_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pyfsapfs_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check container signature.",
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
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Checks if a container has an APFS container signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_check_container_signature_file_object(
           PyObject *self PYFSAPFS_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error         = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	PyObject *file_object            = NULL;
	static char *function            = "pyfsapfs_check_container_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	PYFSAPFS_UNREFERENCED_PARAMETER( self )

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &file_object ) == 0 )
	{
		return( NULL );
	}
	if( pyfsapfs_file_object_initialize(
	     &file_io_handle,
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

	result = libfsapfs_check_container_signature_file_io_handle(
	          file_io_handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to check container signature.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     &error ) != 1 )
	{
		pyfsapfs_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
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

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

/* Creates a new container object and opens it
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_open_new_container(
           PyObject *self PYFSAPFS_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *pyfsapfs_container = NULL;

	PYFSAPFS_UNREFERENCED_PARAMETER( self )

	pyfsapfs_container_init(
	 (pyfsapfs_container_t *) pyfsapfs_container );

	pyfsapfs_container_open(
	 (pyfsapfs_container_t *) pyfsapfs_container,
	 arguments,
	 keywords );

	return( pyfsapfs_container );
}

/* Creates a new container object and opens it using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsapfs_open_new_container_with_file_object(
           PyObject *self PYFSAPFS_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *pyfsapfs_container = NULL;

	PYFSAPFS_UNREFERENCED_PARAMETER( self )

	pyfsapfs_container_init(
	 (pyfsapfs_container_t *) pyfsapfs_container );

	pyfsapfs_container_open_file_object(
	 (pyfsapfs_container_t *) pyfsapfs_container,
	 arguments,
	 keywords );

	return( pyfsapfs_container );
}

#if PY_MAJOR_VERSION >= 3

/* The pyfsapfs module definition
 */
PyModuleDef pyfsapfs_module_definition = {
	PyModuleDef_HEAD_INIT,

	/* m_name */
	"pyfsapfs",
	/* m_doc */
	"Python libfsapfs module (pyfsapfs).",
	/* m_size */
	-1,
	/* m_methods */
	pyfsapfs_module_methods,
	/* m_reload */
	NULL,
	/* m_traverse */
	NULL,
	/* m_clear */
	NULL,
	/* m_free */
	NULL,
};

#endif /* PY_MAJOR_VERSION >= 3 */

/* Initializes the pyfsapfs module
 */
#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_pyfsapfs(
                void )
#else
PyMODINIT_FUNC initpyfsapfs(
                void )
#endif
{
	PyObject *module           = NULL;
	PyGILState_STATE gil_state = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libfsapfs_notify_set_stream(
	 stderr,
	 NULL );
	libfsapfs_notify_set_verbose(
	 1 );
#endif

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
#if PY_MAJOR_VERSION >= 3
	module = PyModule_Create(
	          &pyfsapfs_module_definition );
#else
	module = Py_InitModule3(
	          "pyfsapfs",
	          pyfsapfs_module_methods,
	          "Python libfsapfs module (pyfsapfs)." );
#endif
	if( module == NULL )
	{
#if PY_MAJOR_VERSION >= 3
		return( NULL );
#else
		return;
#endif
	}
	PyEval_InitThreads();

	gil_state = PyGILState_Ensure();

	/* Setup the container type object
	 */
	pyfsapfs_container_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsapfs_container_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsapfs_container_type_object );

	PyModule_AddObject(
	 module,
	 "container",
	 (PyObject *) &pyfsapfs_container_type_object );

	/* Setup the extended_attribute type object
	 */
	pyfsapfs_extended_attribute_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsapfs_extended_attribute_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsapfs_extended_attribute_type_object );

	PyModule_AddObject(
	 module,
	 "extended_attribute",
	 (PyObject *) &pyfsapfs_extended_attribute_type_object );

	/* Setup the extended_attributes type object
	 */
	pyfsapfs_extended_attributes_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsapfs_extended_attributes_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsapfs_extended_attributes_type_object );

	PyModule_AddObject(
	 module,
	 "extended_attributes",
	 (PyObject *) &pyfsapfs_extended_attributes_type_object );


	/* Setup the file_entries type object
	 */
	pyfsapfs_file_entries_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsapfs_file_entries_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsapfs_file_entries_type_object );

	PyModule_AddObject(
	 module,
	 "file_entries",
	 (PyObject *) &pyfsapfs_file_entries_type_object );

	/* Setup the file_entry type object
	 */
	pyfsapfs_file_entry_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsapfs_file_entry_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsapfs_file_entry_type_object );

	PyModule_AddObject(
	 module,
	 "file_entry",
	 (PyObject *) &pyfsapfs_file_entry_type_object );

	/* Setup the volume type object
	 */
	pyfsapfs_volume_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsapfs_volume_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsapfs_volume_type_object );

	PyModule_AddObject(
	 module,
	 "volume",
	 (PyObject *) &pyfsapfs_volume_type_object );

	/* Setup the volumes type object
	 */
	pyfsapfs_volumes_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyfsapfs_volumes_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyfsapfs_volumes_type_object );

	PyModule_AddObject(
	 module,
	 "volumes",
	 (PyObject *) &pyfsapfs_volumes_type_object );

	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( module );
#else
	return;
#endif

on_error:
	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( NULL );
#else
	return;
#endif
}

