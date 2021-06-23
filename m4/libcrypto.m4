dnl Checks for libcrypto required headers and functions
dnl
dnl Version: 20210623

dnl Function to detect whether openssl/evp.h can be used in combination with zlib.h
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_ZLIB_COMPATIBILE],
  [AC_CACHE_CHECK(
    [if openssl/evp.h can be used in combination with zlib.h],
    [ac_cv_openssl_evp_zlib_compatible],
    [AC_LANG_PUSH(C)
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <zlib.h>
#include <openssl/evp.h>]],
        [[ ]] )],
      [ac_cv_openssl_evp_zlib_compatible=yes],
      [ac_cv_openssl_evp_zlib_compatible=no])
    AC_LANG_POP(C)])
  ])

dnl Function to detect whether EVP_CipherInit_ex can be used with duplicate keys.
AC_DEFUN([AX_LIBCRYPTO_CHECK_XTS_DUPLICATE_KEYS_SUPPORT],
  [AC_CACHE_CHECK(
    [if `EVP_CipherInit_ex' can be used with duplicate keys],
    [ac_cv_openssl_xts_duplicate_keys],
    [AC_LANG_PUSH(C)
    ac_cv_libcrypto_backup_LIBS="$LIBS"
    LIBS="$LIBS $ac_cv_libcrypto_LIBADD"
    AC_RUN_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <openssl/err.h>
#include <openssl/evp.h>]],
        [[unsigned char key[ 16 ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
int result = 0;

#if defined( HAVE_EVP_CIPHER_CTX_INIT )
EVP_CIPHER_CTX ctx;

EVP_CIPHER_CTX_init( &ctx );
#else
EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
#endif

result = EVP_CipherInit_ex(ctx, EVP_aes_128_xts(), NULL, key, key, 0);

#if defined( HAVE_EVP_CIPHER_CTX_INIT )
EVP_CIPHER_CTX_cleanup( &ctx );
#else
EVP_CIPHER_CTX_free( ctx );
#endif

return( result == 1 ); ]] )],
      [ac_cv_openssl_xts_duplicate_keys=yes],
      [ac_cv_openssl_xts_duplicate_keys=no])
    LIBS="$ac_cv_libcrypto_backup_LIBS"
    AC_LANG_POP(C)])
  ])

dnl Function to detect if libcrypto (openssl) EVP functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP],
  [AC_CHECK_HEADERS([openssl/evp.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_evp_h" = xno],
    [ac_cv_libcrypto=no
    ac_cv_libcrypto_evp=no],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_ZLIB_COMPATIBILE

    AS_IF(
      [test "x$ac_cv_openssl_evp_zlib_compatible" = xyes],
      [ac_cv_libcrypto=yes
      ac_cv_libcrypto_evp=yes],
      [ac_cv_libcrypto=no
      ac_cv_libcrypto_evp=no])
    ])
  ])

dnl Function to detect if libcrypto (openssl) EVP MD functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD],
  [ac_cv_libcrypto_evp_md=yes

  AC_CHECK_LIB(
    crypto,
    EVP_MD_CTX_new,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_dummy=no])

  AS_IF(
    [test "x$ac_cv_lib_crypto_EVP_MD_CTX_new" = xyes],
    [dnl Check for the new EVP API functions
    AC_CHECK_LIB(
      crypto,
      EVP_MD_CTX_free,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_md=no])
    ],
    [dnl Check for the old EVP API functions
    AC_CHECK_LIB(
      crypto,
      EVP_MD_CTX_init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_md=no])
    AC_CHECK_LIB(
      crypto,
      EVP_MD_CTX_cleanup,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_md=no])

    AC_CHECK_LIB(
      crypto,
      ERR_remove_thread_state,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_md=no])
    ])

  AC_CHECK_LIB(
    crypto,
    EVP_DigestInit_ex,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_md=no])
  AC_CHECK_LIB(
    crypto,
    EVP_DigestUpdate,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_md=no])
  AC_CHECK_LIB(
    crypto,
    EVP_DigestFinal_ex,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_md=no])

  AS_IF(
    [test "x$ac_cv_enable_openssl_evp_md" = xyes && test "x$ac_cv_libcrypto_evp_md" = xno],
    [AC_MSG_FAILURE(
      [Missing OpenSSL EVP MD support],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_lib_crypto_EVP_MD_CTX_init" = xyes],
    [AC_DEFINE(
      [HAVE_EVP_MD_CTX_INIT],
      [1],
      [Define to 1 if you have the `EVP_MD_CTX_init' function".])
    ])

  AS_IF(
    [test "x$ac_cv_lib_crypto_EVP_MD_CTX_cleanup" = xyes],
    [AC_DEFINE(
      [HAVE_EVP_MD_CTX_CLEANUP],
      [1],
      [Define to 1 if you have the `EVP_MD_CTX_cleanup' function".])
    ])
  ])

dnl Function to detect if openssl EVP MD5 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD5],
  [AC_CHECK_LIB(
    crypto,
    EVP_md5,
    [ac_cv_libcrypto_md5=libcrypto_evp],
    [ac_cv_libcrypto_md5=no])

  AS_IF(
    [test "x$ac_cv_libcrypto_md5" = xlibcrypto_evp],
    [AC_DEFINE(
      [HAVE_EVP_MD5],
      [1],
      [Define to 1 if you have the `EVP_md5' function".])
    ])
  ])

dnl Function to detect if openssl EVP SHA1 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA1],
  [AC_CHECK_LIB(
    crypto,
    EVP_sha1,
    [ac_cv_libcrypto_sha1=libcrypto_evp],
    [ac_cv_libcrypto_sha1=no])

  AS_IF(
    [test "x$ac_cv_libcrypto_sha1" = xlibcrypto_evp],
    [AC_DEFINE(
      [HAVE_EVP_SHA1],
      [1],
      [Define to 1 if you have the `EVP_sha1' function".])
    ])
  ])

dnl Function to detect if openssl EVP SHA224 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA224],
  [AC_CHECK_LIB(
    crypto,
    EVP_sha224,
    [ac_cv_libcrypto_sha224=libcrypto_evp],
    [ac_cv_libcrypto_sha224=no])

  AS_IF(
    [test "x$ac_cv_libcrypto_sha224" = xlibcrypto_evp],
    [AC_DEFINE(
      [HAVE_EVP_SHA224],
      [1],
      [Define to 1 if you have the `EVP_sha224' function".])
    ])
  ])

dnl Function to detect if openssl EVP SHA256 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA256],
  [AC_CHECK_LIB(
    crypto,
    EVP_sha256,
    [ac_cv_libcrypto_sha256=libcrypto_evp],
    [ac_cv_libcrypto_sha256=no])

  AS_IF(
    [test "x$ac_cv_libcrypto_sha256" = xlibcrypto_evp],
    [AC_DEFINE(
      [HAVE_EVP_SHA256],
      [1],
      [Define to 1 if you have the `EVP_sha256' function".])
    ])
  ])

dnl Function to detect if openssl EVP SHA512 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA512],
  [AC_CHECK_LIB(
    crypto,
    EVP_sha512,
    [ac_cv_libcrypto_sha512=libcrypto_evp],
    [ac_cv_libcrypto_sha512=no])

  AS_IF(
    [test "x$ac_cv_libcrypto_sha512" = xlibcrypto_evp],
    [AC_DEFINE(
      [HAVE_EVP_SHA512],
      [1],
      [Define to 1 if you have the `EVP_sha512' function".])
    ])
  ])

dnl Function to detect if openssl MD5 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_MD5],
  [AC_CHECK_HEADERS([openssl/md5.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_md5_h" = xno],
    [ac_cv_libcrypto_md5=no],
    [ac_cv_libcrypto_md5=libcrypto

    AC_CHECK_LIB(
      crypto,
      MD5_Init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_md5=no])
    AC_CHECK_LIB(
      crypto,
      MD5_Update,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_md5=no])
    AC_CHECK_LIB(
      crypto,
      MD5_Final,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_md5=no])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno && test "x$ac_cv_libcrypto_md5" = xlibcrypto],
    [ac_cv_libcrypto=yes])
  ])

dnl Function to detect if openssl SHA1 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_SHA1],
  [AC_CHECK_HEADERS([openssl/sha.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_sha_h" = xno],
    [ac_cv_libcrypto_sha1=no],
    [ac_cv_libcrypto_sha1=libcrypto

    AC_CHECK_LIB(
      crypto,
      SHA1_Init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha1=no])
    AC_CHECK_LIB(
      crypto,
      SHA1_Update,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha1=no])
    AC_CHECK_LIB(
      crypto,
      SHA1_Final,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha1=no])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno && test "x$ac_cv_libcrypto_sha1" = xlibcrypto],
    [ac_cv_libcrypto=yes])
  ])

dnl Function to detect if openssl SHA224 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_SHA224],
  [AC_CHECK_HEADERS([openssl/sha.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_sha_h" = xno],
    [ac_cv_libcrypto_sha224=no],
    [ac_cv_libcrypto_sha224=libcrypto

    AC_CHECK_LIB(
      crypto,
      SHA224_Init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha224=no])
    AC_CHECK_LIB(
      crypto,
      SHA224_Update,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha224=no])
    AC_CHECK_LIB(
      crypto,
      SHA224_Final,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha224=no])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno && test "x$ac_cv_libcrypto_sha224" = xlibcrypto],
    [ac_cv_libcrypto=yes])
  ])

dnl Function to detect if openssl SHA256 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_SHA256],
  [AC_CHECK_HEADERS([openssl/sha.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_sha_h" = xno],
    [ac_cv_libcrypto_sha256=no],
    [ac_cv_libcrypto_sha256=libcrypto

    AC_CHECK_LIB(
      crypto,
      SHA256_Init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha256=no])
    AC_CHECK_LIB(
      crypto,
      SHA256_Update,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha256=no])
    AC_CHECK_LIB(
      crypto,
      SHA256_Final,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha256=no])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno && test "x$ac_cv_libcrypto_sha256" = xlibcrypto],
    [ac_cv_libcrypto=yes])
  ])

dnl Function to detect if openssl SHA512 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_SHA512],
  [AC_CHECK_HEADERS([openssl/sha.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_sha_h" = xno],
    [ac_cv_libcrypto_sha512=no],
    [ac_cv_libcrypto_sha512=libcrypto

    AC_CHECK_LIB(
      crypto,
      SHA512_Init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha512=no])
    AC_CHECK_LIB(
      crypto,
      SHA512_Update,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha512=no])
    AC_CHECK_LIB(
      crypto,
      SHA512_Final,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_sha512=no])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno && test "x$ac_cv_libcrypto_sha512" = xlibcrypto],
    [ac_cv_libcrypto=yes])
  ])

dnl Function to detect if openssl EVP CIPHER functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_EVP_CIPHER],
  [ac_cv_libcrypto_evp_cipher=yes

  dnl Check if all required libcrypto (openssl) EVP CIPHER functions are available
  AC_CHECK_LIB(
    crypto,
    EVP_CIPHER_CTX_new,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_dummy=no])

  AS_IF(
    [test "x$ac_cv_lib_crypto_EVP_CIPHER_CTX_new" = xyes],
    [dnl Check for the new EVP API functions
    AC_CHECK_LIB(
      crypto,
      EVP_CIPHER_CTX_free,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_cipher=no])
    ],
    [dnl Check for the old EVP API functions
    AC_CHECK_LIB(
      crypto,
      EVP_CIPHER_CTX_init,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_cipher=no])
    AC_CHECK_LIB(
      crypto,
      EVP_CIPHER_CTX_cleanup,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_cipher=no])

    AC_CHECK_LIB(
      crypto,
      ERR_remove_thread_state,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_evp_cipher=no])
    ])

  AC_CHECK_LIB(
    crypto,
    EVP_CIPHER_CTX_set_padding,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_cipher=no])

  AC_CHECK_LIB(
    crypto,
    EVP_CipherInit_ex,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_cipher=no])
  AC_CHECK_LIB(
    crypto,
    EVP_CipherUpdate,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_cipher=no])
  AC_CHECK_LIB(
    crypto,
    EVP_CipherFinal_ex,
    [ac_cv_libcrypto_dummy=yes],
    [ac_cv_libcrypto_evp_cipher=no])

  AS_IF(
    [test "x$ac_cv_enable_openssl_evp_cipher" = xyes && test "x$ac_cv_libcrypto_evp_cipher" = xno],
    [AC_MSG_FAILURE(
      [Missing OpenSSL EVP CIPHER support],
      [1])
    ])

  AS_IF(
    [test "x$ac_cv_lib_crypto_EVP_CIPHER_CTX_init" = xyes],
    [AC_DEFINE(
      [HAVE_EVP_CIPHER_CTX_INIT],
      [1],
      [Define to 1 if you have the `EVP_CIPHER_CTX_init' function".])
    ])

  AS_IF(
    [test "x$ac_cv_lib_crypto_EVP_CIPHER_CTX_cleanup" = xyes],
    [AC_DEFINE(
      [HAVE_EVP_CIPHER_CTX_CLEANUP],
      [1],
      [Define to 1 if you have the `EVP_CIPHER_CTX_cleanup' function".])
    ])
  ])

dnl Function to detect if openssl AES-CBC and AES-ECB functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_OPENSSL_AES],
  [AC_CHECK_HEADERS([openssl/aes.h])

  AS_IF(
    [test "x$ac_cv_header_openssl_aes_h" = xno],
    [ac_cv_libcrypto_aes_cbc=no
    ac_cv_libcrypto_aes_ecb=no],
    [ac_cv_libcrypto_aes_cbc=libcrypto
    ac_cv_libcrypto_aes_ecb=libcrypto

    AC_CHECK_LIB(
      crypto,
      AES_set_decrypt_key,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes_cbc=no,
      ac_cv_libcrypto_aes_ecb=no])
    AC_CHECK_LIB(
      crypto,
      AES_set_encrypt_key,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes_cbc=no,
      ac_cv_libcrypto_aes_ecb=no])

    AC_CHECK_LIB(
      crypto,
      AES_cbc_encrypt,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes_cbc=no])

    AC_CHECK_LIB(
      crypto,
      AES_ecb_encrypt,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes_ecb=no])

    AS_IF(
      [test "x$ac_cv_lib_crypto_AES_cbc_encrypt" = xyes],
      [AC_DEFINE(
        [HAVE_AES_CBC_ENCRYPT],
        [1],
        [Define to 1 if you have the `AES_cbc_encrypt' function".])
      ])

    AS_IF(
      [test "x$ac_cv_lib_crypto_AES_ecb_encrypt" = xyes],
      [AC_DEFINE(
        [HAVE_AES_ECB_ENCRYPT],
        [1],
        [Define to 1 if you have the `AES_ecb_encrypt' function".])
      ])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" = xno],
    [AS_IF(
      [test "x$ac_cv_libcrypto_aes_cbc" = xno || test "x$ac_cv_libcrypto_aes_ecb" = xno],
      [ac_cv_libcrypto=yes])
    ])

  dnl ac_cv_libcrypto_aes is deprecated but kept for backwards compatibility
  AS_IF(
    [test "x$ac_cv_libcrypto_aes_cbc" = xno || test "x$ac_cv_libcrypto_aes_ecb" = xno],
    [ac_cv_libcrypto_aes=no],
    [ac_cv_libcrypto_aes=libcrypto])
  ])

dnl Function to detect if libcrypto (openssl) dependencies are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_LIB],
  [AS_IF(
    [test "x$ac_cv_enable_shared_libs" = xno || test "x$ac_cv_with_openssl" = xno],
    [ac_cv_libcrypto=no],
    [dnl Check if the directory provided as parameter exists
    AS_IF(
      [test "x$ac_cv_with_openssl" != x && test "x$ac_cv_with_openssl" != xauto-detect],
      [AS_IF(
        [test -d "$ac_cv_with_openssl"],
        [CFLAGS="$CFLAGS -I${ac_cv_with_openssl}/include"
        LDFLAGS="$LDFLAGS -L${ac_cv_with_openssl}/lib"],
        [AC_MSG_FAILURE(
          [no such directory: $ac_cv_with_openssl],
          [1])
        ])],
      [dnl Check for a pkg-config file
      AS_IF(
        [test "x$cross_compiling" != "xyes" && test "x$PKGCONFIG" != "x"],
        [PKG_CHECK_MODULES(
          [openssl],
          [openssl >= 1.0],
          [ac_cv_libcrypto=yes
          ac_cv_libcrypto_evp=yes],
          [ac_cv_libcrypto=no
          ac_cv_libcrypto_evp=no])
        ])
      AS_IF(
        [test "x$ac_cv_libcrypto" = xyes],
        [ac_cv_libcrypto_CPPFLAGS="$pkg_cv_openssl_CFLAGS"
        ac_cv_libcrypto_LIBADD="$pkg_cv_openssl_LIBS"])
      ])

    AS_IF(
      [test "x$ac_cv_libcrypto" != xyes],
      [dnl Check for headers
      AC_CHECK_HEADERS([openssl/opensslv.h])

      AX_LIBCRYPTO_CHECK_OPENSSL_EVP

      AS_IF(
        [test "x$ac_cv_libcrypto_evp" != xyes && test "$ac_cv_header_openssl_opensslv" = xyes],
        [ac_cv_libcrypto=yes])
      ])

    dnl Setup libcrypto (openssl) parameters
    AS_IF(
      [test "x$ac_cv_libcrypto" = xyes && test "x$ac_cv_libcrypto_evp" = xyes],
      [AC_DEFINE(
        [HAVE_OPENSSL_EVP_H],
        [1],
        [Define to 1 if you have the <openssl/evp.h> header file.])
      AC_SUBST(
        [HAVE_OPENSSL_EVP_H],
        [1]) ],
      [AC_SUBST(
        [HAVE_OPENSSL_EVP_H],
        [0])
      ])

    AS_IF(
      [test "x$ac_cv_libcrypto" != xno],
      [AC_DEFINE(
        [HAVE_LIBCRYPTO],
        [1],
        [Define to 1 if you have the 'crypto' library (-lcrypto).])

      ac_cv_libcrypto_LIBADD="-lcrypto"

      dnl On Cygwin also link zlib since libcrypto relies on it
      AS_CASE(
        [$host],
        [*cygwin*],
        [ac_cv_libcrypto_LIBADD="-lcrypto -lz"])

      dnl Enforce the dynamic loader library to be included if available
      AC_CHECK_LIB(
        dl,
        dlopen,
        [AC_SUBST(
          [LIBDL_LIBADD],
          ["-ldl"])],
        [])
      ])
    ])
  ])

dnl Function to detect if libcrypto MD5 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_MD5],
  [ac_cv_libcrypto_md5=no

  dnl Check for libcrypto (openssl) EVP MD support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_libcrypto_evp_md" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD])

  dnl Check for libcrypto (openssl) EVP MD5 support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp_md" = xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD5])

  dnl Check for libcrypto (openssl) MD5 support
  AS_IF(
    [test "x$ac_cv_libcrypto_md5" = xno],
    [AX_LIBCRYPTO_CHECK_OPENSSL_MD5])
  ])

dnl Function to detect if libcrypto SHA1 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_SHA1],
  [ac_cv_libcrypto_sha1=no

  dnl Check for libcrypto (openssl) EVP MD support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_enable_openssl_evp_md" != xno && test "x$ac_cv_libcrypto_evp_md" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD])

  dnl Check for libcrypto (openssl) EVP SHA1 support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp_md" = xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA1])

  dnl Check for libcrypto (openssl) SHA1 support
  AS_IF(
    [test "x$ac_cv_libcrypto_sha1" = xno],
    [AX_LIBCRYPTO_CHECK_OPENSSL_SHA1])
  ])

dnl Function to detect if libcrypto SHA224 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_SHA224],
  [ac_cv_libcrypto_sha224=no

  dnl Check for libcrypto (openssl) EVP MD support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_enable_openssl_evp_md" != xno && test "x$ac_cv_libcrypto_evp_md" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD])

  dnl Check for libcrypto (openssl) EVP SHA224 support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp_md" = xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA224])

  dnl Check for libcrypto (openssl) SHA224 support
  AS_IF(
    [test "x$ac_cv_libcrypto_sha224" = xno],
    [AX_LIBCRYPTO_CHECK_OPENSSL_SHA224])
  ])

dnl Function to detect if libcrypto SHA256 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_SHA256],
  [ac_cv_libcrypto_sha256=no

  dnl Check for libcrypto (openssl) EVP MD support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_enable_openssl_evp_md" != xno && test "x$ac_cv_libcrypto_evp_md" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD])

  dnl Check for libcrypto (openssl) EVP SHA256 support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp_md" = xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA256])

  dnl Check for libcrypto (openssl) SHA256 support
  AS_IF(
    [test "x$ac_cv_libcrypto_sha256" = xno],
    [AX_LIBCRYPTO_CHECK_OPENSSL_SHA256])
  ])

dnl Function to detect if libcrypto SHA512 functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_SHA512],
  [ac_cv_libcrypto_sha512=no

  dnl Check for libcrypto (openssl) EVP MD support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_enable_openssl_evp_md" != xno && test "x$ac_cv_libcrypto_evp_md" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_MD])

  dnl Check for libcrypto (openssl) EVP SHA512 support
  AS_IF(
    [test "x$ac_cv_libcrypto_evp_md" = xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_SHA512])

  dnl Check for libcrypto (openssl) SHA512 support
  AS_IF(
    [test "x$ac_cv_libcrypto_sha512" = xno],
    [AX_LIBCRYPTO_CHECK_OPENSSL_SHA512])
  ])

dnl Function to detect if libcrypto AES-CBC and AES-ECB functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_AES],
  [dnl Check for libcrypto (openssl) EVP CIPHER AES-CBC and AES-ECB support
  AX_LIBCRYPTO_CHECK_AES_CBC
  AX_LIBCRYPTO_CHECK_AES_ECB

  dnl ac_cv_libcrypto_aes is deprecated but kept for backwards compatibility
  AS_IF(
    [test "x$ac_cv_libcrypto_aes_cbc" = xno || test "x$ac_cv_libcrypto_aes_ecb" = xno],
    [ac_cv_libcrypto_aes=no],
    [ac_cv_libcrypto_aes=libcrypto_evp])

  dnl Check for libcrypto (openssl) AES-CBC and AES-ECB support
  AS_IF(
    [test "x$ac_cv_libcrypto_aes_cbc" = xno || test "x$ac_cv_libcrypto_aes_ecb" = xno],
    [AX_LIBCRYPTO_CHECK_OPENSSL_AES])
  ])

dnl Function to detect if libcrypto AES-CBC functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_AES_CBC],
  [AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_enable_openssl_evp_cipher" != xno && test "x$ac_cv_libcrypto_evp_cipher" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_CIPHER])

  AS_IF(
    [test "x$ac_cv_libcrypto_evp_cipher" != xyes],
    [ac_cv_libcrypto_aes_cbc=no],
    [ac_cv_libcrypto_aes_cbc=libcrypto_evp

    AC_CHECK_LIB(
      crypto,
      EVP_aes_128_cbc,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes_cbc=no])
    AC_CHECK_LIB(
      crypto,
      EVP_aes_192_cbc,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes_cbc=no])
    AC_CHECK_LIB(
      crypto,
      EVP_aes_256_cbc,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes_cbc=no])

    AS_IF(
      [test "x$ac_cv_lib_crypto_EVP_aes_128_cbc" = xyes && test "x$ac_cv_lib_crypto_EVP_aes_192_cbc" = xyes && test "x$ac_cv_lib_crypto_EVP_aes_256_cbc" = xyes],
      [AC_DEFINE(
        [HAVE_EVP_CRYPTO_AES_CBC],
        [1],
        [Define to 1 if you have the `EVP_aes_128_cbc', `EVP_aes_192_cbc' and `EVP_aes_256_cbc' functions".])
      ])
    ])
  ])

dnl Function to detect if libcrypto AES-ECB functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_AES_ECB],
  [AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_enable_openssl_evp_cipher" != xno && test "x$ac_cv_libcrypto_evp_cipher" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_CIPHER])

  AS_IF(
    [test "x$ac_cv_libcrypto_evp_cipher" != xyes],
    [ac_cv_libcrypto_aes_ecb=no],
    [ac_cv_libcrypto_aes_ecb=libcrypto_evp

    AC_CHECK_LIB(
      crypto,
      EVP_aes_128_ecb,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes_ecb=no])
    AC_CHECK_LIB(
      crypto,
      EVP_aes_192_ecb,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes_ecb=no])
    AC_CHECK_LIB(
      crypto,
      EVP_aes_256_ecb,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes_ecb=no])

    AS_IF(
      [test "x$ac_cv_lib_crypto_EVP_aes_128_ecb" = xyes && test "x$ac_cv_lib_crypto_EVP_aes_192_ecb" = xyes && test "x$ac_cv_lib_crypto_EVP_aes_256_ecb" = xyes],
      [AC_DEFINE(
        [HAVE_EVP_CRYPTO_AES_ECB],
        [1],
        [Define to 1 if you have the `EVP_aes_128_ecb', `EVP_aes_192_ecb' and `EVP_aes_256_ecb' functions".])
      ])
    ])
  ])

dnl Function to detect if libcrypto AES-XTS functions are available
AC_DEFUN([AX_LIBCRYPTO_CHECK_AES_XTS],
  [AS_IF(
    [test "x$ac_cv_libcrypto_evp" = xyes && test "x$ac_cv_enable_openssl_evp_cipher" != xno && test "x$ac_cv_libcrypto_evp_cipher" != xyes],
    [AX_LIBCRYPTO_CHECK_OPENSSL_EVP_CIPHER])

  AS_IF(
    [test "x$ac_cv_libcrypto_evp_cipher" != xyes],
    [ac_cv_libcrypto_aes_xts=no],
    [ac_cv_libcrypto_aes_xts=libcrypto_evp

    AC_CHECK_LIB(
      crypto,
      EVP_aes_128_xts,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes_xts=no])
    AC_CHECK_LIB(
      crypto,
      EVP_aes_256_xts,
      [ac_cv_libcrypto_dummy=yes],
      [ac_cv_libcrypto_aes_xts=no])

    dnl OpenSSL 1.1.1c will error with "xts duplicated keys".
    AX_LIBCRYPTO_CHECK_XTS_DUPLICATE_KEYS_SUPPORT
    AS_IF(
      [test "x$ac_cv_openssl_xts_duplicate_keys" = xno],
      [ac_cv_libcrypto_aes_xts=no],
      [AS_IF(
        [test "x$ac_cv_lib_crypto_EVP_aes_128_xts" = xyes && test "x$ac_cv_lib_crypto_EVP_aes_256_xts" = xyes],
        [AC_DEFINE(
          [HAVE_EVP_CRYPTO_AES_XTS],
          [1],
          [Define to 1 if you have the `EVP_aes_128_xts' and `EVP_aes_256_xts' functions".])
        ])
      ])
    ])
  ])

dnl Function to detect how to enable libcrypto (openssl)
AC_DEFUN([AX_LIBCRYPTO_CHECK_ENABLE],
  [AX_COMMON_ARG_WITH(
    [openssl],
    [openssl],
    [search for OpenSSL in includedir and libdir or in the specified DIR, or no if not to use OpenSSL],
    [auto-detect],
    [DIR])

  AX_COMMON_ARG_ENABLE(
    [openssl-evp-cipher],
    [openssl_evp_cipher],
    [enable OpenSSL EVP CIPHER support, or no to disable],
    [auto-detect])

  AX_COMMON_ARG_ENABLE(
    [openssl-evp-md],
    [openssl_evp_md],
    [enable OpenSSL EVP MD support, or no to disable],
    [auto-detect])

  AS_IF(
    [test "x$ac_cv_enable_static_executables" != xyes],
    [dnl Check for a shared library version
    AX_LIBCRYPTO_CHECK_LIB])

  AS_IF(
    [test "x$ac_cv_libcrypto_CPPFLAGS" != "x"],
    [AC_SUBST(
      [LIBCRYPTO_CPPFLAGS],
      [$ac_cv_libcrypto_CPPFLAGS])
    ])
  AS_IF(
    [test "x$ac_cv_libcrypto_LIBADD" != "x"],
    [AC_SUBST(
      [LIBCRYPTO_LIBADD],
      [$ac_cv_libcrypto_LIBADD])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" != xno],
    [AC_SUBST(
      [ax_libcrypto_pc_libs_private],
      [-lcrypto])
    ])

  AS_IF(
    [test "x$ac_cv_libcrypto" != xno],
    [AC_SUBST(
      [ax_libcrypto_spec_requires],
      [openssl])
    AC_SUBST(
      [ax_libcrypto_spec_build_requires],
      [openssl-devel])
    ])
  ])

