#!/bin/sh
# Script that synchronizes the local library dependencies
#
# Version: 20180728

EXIT_SUCCESS=0;
EXIT_FAILURE=1;

GIT_URL_PREFIX="https://github.com/libyal";
LOCAL_LIBS="libbfio libcdata libcerror libcfile libclocale libcnotify libcpath libcsplit libcthreads libfdatetime libuna";

OLDIFS=$IFS;
IFS=" ";

for LOCAL_LIB in ${LOCAL_LIBS};
do
	GIT_URL="${GIT_URL_PREFIX}/${LOCAL_LIB}.git";

	git clone --quiet ${GIT_URL} ${LOCAL_LIB}-$$;

	if ! test -d ${LOCAL_LIB}-$$;
	then
		echo "Unable to git clone: ${GIT_URL}";

		IFS=$OLDIFS;

		exit ${EXIT_FAILURE};
	fi
	(cd ${LOCAL_LIB}-$$ && git fetch --quiet --all --tags --prune)

	LATEST_TAG=`cd ${LOCAL_LIB}-$$ && git describe --tags --abbrev=0`;

	if test -n ${LATEST_TAG} && test "$1" != "--use-head";
	then
		echo "Synchronizing: ${LOCAL_LIB} from ${GIT_URL} tag ${LATEST_TAG}";

		(cd ${LOCAL_LIB}-$$ && git checkout --quiet tags/${LATEST_TAG});
	else
		echo "Synchronizing: ${LOCAL_LIB} from ${GIT_URL} HEAD";
	fi

	rm -rf ${LOCAL_LIB};
	mkdir ${LOCAL_LIB};

	if ! test -d ${LOCAL_LIB};
	then
		echo "Missing directory: ${LOCAL_LIB}";

		IFS=$OLDIFS;

		exit ${EXIT_FAILURE};
	fi

	LOCAL_LIB_UPPER=`echo "${LOCAL_LIB}" | tr "[a-z]" "[A-Z]"`;
	LOCAL_LIB_VERSION=`grep -A 2 AC_INIT ${LOCAL_LIB}-$$/configure.ac | tail -n 1 | sed 's/^\s*\[\([0-9]*\)\],\s*$/\1/'`;
	LOCAL_LIB_MAKEFILE_AM="${LOCAL_LIB}/Makefile.am";

	cp ${LOCAL_LIB}-$$/${LOCAL_LIB}/*.[chly] ${LOCAL_LIB};
	cp ${LOCAL_LIB}-$$/${LOCAL_LIB_MAKEFILE_AM} ${LOCAL_LIB_MAKEFILE_AM};

	# Make the necessary changes to libyal/Makefile.am

SED_SCRIPT="/AM_CPPFLAGS = / {
	i\\
if HAVE_LOCAL_${LOCAL_LIB_UPPER}
}

/lib_LTLIBRARIES = / {
	s/lib_LTLIBRARIES/noinst_LTLIBRARIES/
}

/${LOCAL_LIB}\.c/ {
	d
}

/${LOCAL_LIB}_la_LIBADD/ {
:loop1
	/${LOCAL_LIB}_la_LDFLAGS/ {
		N
		i\\
endif
		d
	}
	/${LOCAL_LIB}_la_LDFLAGS/ !{
		N
		b loop1
	}
}

/${LOCAL_LIB}_la_LDFLAGS/ {
	N
	i\\
endif
	d
}

/distclean: clean/ {
	n
	N
	d
}";
	echo "${SED_SCRIPT}" >> ${LOCAL_LIB}-$$.sed;
	sed -i'~' -f ${LOCAL_LIB}-$$.sed ${LOCAL_LIB_MAKEFILE_AM};
	rm -f ${LOCAL_LIB}-$$.sed;

	sed -i'~' "/AM_CPPFLAGS = /,/noinst_LTLIBRARIES = / { N; s/\\\\\\n.@${LOCAL_LIB_UPPER}_DLL_EXPORT@//; P; D; }" ${LOCAL_LIB_MAKEFILE_AM};
	sed -i'~' "/${LOCAL_LIB}_definitions.h.in/d" ${LOCAL_LIB_MAKEFILE_AM};
	sed -i'~' "/${LOCAL_LIB}.rc/d" ${LOCAL_LIB_MAKEFILE_AM};

	if test ${LOCAL_LIB} = "libfplist";
	then
		# TODO: make this more generic to strip the last \\
		sed -i'~' 's/libfplist_xml_scanner.c \\/libfplist_xml_scanner.c/' ${LOCAL_LIB_MAKEFILE_AM};

	elif test ${LOCAL_LIB} = "libodraw";
	then
		# TODO: make this more generic to strip the last \\
		sed -i'~' 's/libodraw_cue_scanner.c \\/libodraw_cue_scanner.c/' ${LOCAL_LIB_MAKEFILE_AM};

	else
		sed -i'~' '/EXTRA_DIST = /,/^$/d' ${LOCAL_LIB_MAKEFILE_AM};
	fi

SED_SCRIPT="/^$/ {
	x
	N
	/endif$/ {
		a\\

		D
	}
}";
	echo "${SED_SCRIPT}" >> ${LOCAL_LIB}-$$.sed;
	sed -i'~' -f ${LOCAL_LIB}-$$.sed ${LOCAL_LIB_MAKEFILE_AM};
	rm -f ${LOCAL_LIB}-$$.sed;

	# Make the necessary changes to libcfile/Makefile.am
	if test ${LOCAL_LIB} = "libcfile";
	then
		if ! test -f "m4/libuna.m4";
		then
			sed -i'~' 's?@LIBUNA_CPPFLAGS@?-I$(top_srcdir)/libuna?' ${LOCAL_LIB_MAKEFILE_AM};
		fi
	fi

	# Make the necessary changes to libfvalue/Makefile.am
	if test ${LOCAL_LIB} = "libfvalue";
	then
		if ! test -f "m4/libfdatetime.m4";
		then
			sed -i'~' '/@LIBFDATETIME_CPPFLAGS@/d' ${LOCAL_LIB_MAKEFILE_AM};
		fi
		if ! test -f "m4/libfguid.m4";
		then
			sed -i'~' '/@LIBFGUID_CPPFLAGS@/d' ${LOCAL_LIB_MAKEFILE_AM};
		fi
		if ! test -f "m4/libfwnt.m4";
		then
			sed -i'~' '/@LIBFWNT_CPPFLAGS@/d' ${LOCAL_LIB_MAKEFILE_AM};
		fi
		if ! test -f "m4/libuna.m4";
		then
			sed -i'~' '/@LIBUNA_CPPFLAGS@/d' ${LOCAL_LIB_MAKEFILE_AM};
		fi
	fi

	# Remove libyal/libyal.c
	rm -f ${LOCAL_LIB}/${LOCAL_LIB}.c;

	# Make the necessary changes to libyal/libyal_defitions.h
	cp ${LOCAL_LIB}-$$/${LOCAL_LIB}/${LOCAL_LIB}_definitions.h.in ${LOCAL_LIB}/${LOCAL_LIB}_definitions.h;
	sed -i'~' "s/@VERSION@/${LOCAL_LIB_VERSION}/" ${LOCAL_LIB}/${LOCAL_LIB}_definitions.h;

	rm -rf ${LOCAL_LIB}-$$;
done

IFS=$OLDIFS;

exit ${EXIT_SUCCESS};

