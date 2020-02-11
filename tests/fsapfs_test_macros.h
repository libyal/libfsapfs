/*
 * Macros for testing
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

#if !defined( _FSAPFS_TEST_MACROS_H )
#define _FSAPFS_TEST_MACROS_H

#include <common.h>
#include <file_stream.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#define FSAPFS_TEST_ASSERT_EQUAL_INT( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%d) != %d\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_NOT_EQUAL_INT( name, value, expected_value ) \
	if( value == expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%d) == %d\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_GREATER_THAN_INT( name, value, expected_value ) \
	if( value <= expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%d) <= %d\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_LESS_THAN_INT( name, value, expected_value ) \
	if( value >= expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%d) >= %d\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_EQUAL_INTPTR( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (0x%08x" PRIjx ") != 0x%08x" PRIjx "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_NOT_EQUAL_INTPTR( name, value, expected_value ) \
	if( value == expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (0x%08x" PRIjx ") == 0x%08x" PRIjx "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_EQUAL_FLOAT( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%f) != %f\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_NOT_EQUAL_FLOAT( name, value, expected_value ) \
	if( value == expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%f) == %f\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_EQUAL_SIZE( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIzd ") != %" PRIzd "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_EQUAL_SSIZE( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIzd ") != %" PRIzd "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_NOT_EQUAL_SSIZE( name, value, expected_value ) \
	if( value == expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIzd ") == %" PRIzd "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_EQUAL_INT8( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIi8 ") != %" PRIi8 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_NOT_EQUAL_INT8( name, value, expected_value ) \
	if( value == expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIi8 ") == %" PRIi8 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_EQUAL_UINT8( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIi8 ") != %" PRIu8 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_LESS_THAN_UINT8( name, value, expected_value ) \
	if( value >= expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIi8 ") >= %" PRIu8 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_EQUAL_INT16( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIi16 ") != %" PRIi16 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_NOT_EQUAL_INT16( name, value, expected_value ) \
	if( value == expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIi16 ") == %" PRIi16 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_EQUAL_UINT16( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIi16 ") != %" PRIu16 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_LESS_THAN_UINT16( name, value, expected_value ) \
	if( value >= expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIi16 ") >= %" PRIu16 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_EQUAL_INT32( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIi32 ") != %" PRIi32 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_NOT_EQUAL_INT32( name, value, expected_value ) \
	if( value == expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIi32 ") == %" PRIi32 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_EQUAL_UINT32( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIu32 ") != %" PRIu32 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_LESS_THAN_UINT32( name, value, expected_value ) \
	if( value >= expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIu32 ") >= %" PRIu32 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_EQUAL_INT64( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIi64 ") != %" PRIi64 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_NOT_EQUAL_INT64( name, value, expected_value ) \
	if( value == expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIi64 ") == %" PRIi64 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_EQUAL_UINT64( name, value, expected_value ) \
	if( value != expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIu64 ") != %" PRIu64 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_LESS_THAN_UINT64( name, value, expected_value ) \
	if( value >= expected_value ) \
	{ \
		fprintf( stdout, "%s:%d %s (%" PRIu64 ") >= %" PRIu64 "\n", __FILE__, __LINE__, name, value, expected_value ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_IS_NOT_NULL( name, value ) \
	if( value == NULL ) \
	{ \
		fprintf( stdout, "%s:%d %s == NULL\n", __FILE__, __LINE__, name ); \
		goto on_error; \
	}

#define FSAPFS_TEST_ASSERT_IS_NULL( name, value ) \
	if( value != NULL ) \
	{ \
		fprintf( stdout, "%s:%d %s != NULL\n", __FILE__, __LINE__, name ); \
		goto on_error; \
	}

#define FSAPFS_TEST_RUN( name, function ) \
	if( function() != 1 ) \
	{ \
		fprintf( stdout, "Unable to run test: %s\n", name ); \
		goto on_error; \
	}

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

#define FSAPFS_TEST_RUN_WITH_ARGS( name, function, ... ) \
	if( function( __VA_ARGS__ ) != 1 ) \
	{ \
		fprintf( stdout, "Unable to run test: %s\n", name ); \
		goto on_error; \
	}

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#define FSAPFS_TEST_FPRINT_ERROR( error ) \
	libcerror_error_backtrace_fprint( error, stdout );

#endif /* !defined( _FSAPFS_TEST_MACROS_H ) */

