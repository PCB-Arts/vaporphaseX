/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Copyright (c) 2021 - PCB Arts GmbH
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

/**
 * \name SECTION: System support
 *
 * This section sets system specific settings.
 * \{
 */

/**
 * \def MBEDTLS_HAVE_ASM
 *
 * The compiler has support for asm().
 *
 * Requires support for asm() in compiler.
 *
 * Used in:
 *      library/aria.c
 *      library/timing.c
 *      include/mbedtls/bn_mul.h
 *
 * Required by:
 *      MBEDTLS_AESNI_C
 *      MBEDTLS_PADLOCK_C
 *
 * Comment to disable the use of assembly code.
 */
#define MBEDTLS_HAVE_ASM

/**
 * \def MBEDTLS_NO_UDBL_DIVISION
 *
 * The platform lacks support for double-width integer division (64-bit
 * division on a 32-bit platform, 128-bit division on a 64-bit platform).
 *
 * Used in:
 *      include/mbedtls/bignum.h
 *      library/bignum.c
 *
 * The bignum code uses double-width division to speed up some operations.
 * Double-width division is often implemented in software that needs to
 * be linked with the program. The presence of a double-width integer
 * type is usually detected automatically through preprocessor macros,
 * but the automatic detection cannot know whether the code needs to
 * and can be linked with an implementation of division for that type.
 * By default division is assumed to be usable if the type is present.
 * Uncomment this option to prevent the use of double-width division.
 *
 * Note that division for the native integer type is always required.
 * Furthermore, a 64-bit type is always required even on a 32-bit
 * platform, but it need not support multiplication or division. In some
 * cases it is also desirable to disable some double-width operations. For
 * example, if double-width division is implemented in software, disabling
 * it can reduce code size in some embedded targets.
 */
//#define MBEDTLS_NO_UDBL_DIVISION

/**
 * \def MBEDTLS_NO_64BIT_MULTIPLICATION
 *
 * The platform lacks support for 32x32 -> 64-bit multiplication.
 *
 * Used in:
 *      library/poly1305.c
 *
 * Some parts of the library may use multiplication of two unsigned 32-bit
 * operands with a 64-bit result in order to speed up computations. On some
 * platforms, this is not available in hardware and has to be implemented in
 * software, usually in a library provided by the toolchain.
 *
 * Sometimes it is not desirable to have to link to that library. This option
 * removes the dependency of that library on platforms that lack a hardware
 * 64-bit multiplier by embedding a software implementation in Mbed TLS.
 *
 * Note that depending on the compiler, this may decrease performance compared
 * to using the library function provided by the toolchain.
 */
//#define MBEDTLS_NO_64BIT_MULTIPLICATION

/**
 * \def MBEDTLS_HAVE_SSE2
 *
 * CPU supports SSE2 instruction set.
 *
 * Uncomment if the CPU supports SSE2 (IA-32 specific).
 */
//#define MBEDTLS_HAVE_SSE2

/**
 * \def MBEDTLS_HAVE_TIME
 *
 * System has time.h and time().
 * The time does not need to be correct, only time differences are used,
 * by contrast with MBEDTLS_HAVE_TIME_DATE
 *
 * Defining MBEDTLS_HAVE_TIME allows you to specify MBEDTLS_PLATFORM_TIME_ALT,
 * MBEDTLS_PLATFORM_TIME_MACRO, MBEDTLS_PLATFORM_TIME_TYPE_MACRO and
 * MBEDTLS_PLATFORM_STD_TIME.
 *
 * Comment if your system does not support time functions
 */
//#define MBEDTLS_HAVE_TIME

/**
 * \def MBEDTLS_HAVE_TIME_DATE
 *
 * System has time.h, time(), and an implementation for
 * mbedtls_platform_gmtime_r() (see below).
 * The time needs to be correct (not necessarily very accurate, but at least
 * the date should be correct). This is used to verify the validity period of
 * X.509 certificates.
 *
 * Comment if your system does not have a correct clock.
 *
 * \note mbedtls_platform_gmtime_r() is an abstraction in platform_util.h that
 * behaves similarly to the gmtime_r() function from the C standard. Refer to
 * the documentation for mbedtls_platform_gmtime_r() for more information.
 *
 * \note It is possible to configure an implementation for
 * mbedtls_platform_gmtime_r() at compile-time by using the macro
 * MBEDTLS_PLATFORM_GMTIME_R_ALT.
 */
//#define MBEDTLS_HAVE_TIME_DATE

/**
 * \def MBEDTLS_PLATFORM_MEMORY
 *
 * Enable the memory allocation layer.
 *
 * By default mbed TLS uses the system-provided calloc() and free().
 * This allows different allocators (self-implemented or provided) to be
 * provided to the platform abstraction layer.
 *
 * Enabling MBEDTLS_PLATFORM_MEMORY without the
 * MBEDTLS_PLATFORM_{FREE,CALLOC}_MACROs will provide
 * "mbedtls_platform_set_calloc_free()" allowing you to set an alternative calloc() and
 * free() function pointer at runtime.
 *
 * Enabling MBEDTLS_PLATFORM_MEMORY and specifying
 * MBEDTLS_PLATFORM_{CALLOC,FREE}_MACROs will allow you to specify the
 * alternate function at compile time.
 *
 * Requires: MBEDTLS_PLATFORM_C
 *
 * Enable this layer to allow use of alternative memory allocators.
 */
//#define MBEDTLS_PLATFORM_MEMORY

/**
 * \def MBEDTLS_PLATFORM_NO_STD_FUNCTIONS
 *
 * Do not assign standard functions in the platform layer (e.g. calloc() to
 * MBEDTLS_PLATFORM_STD_CALLOC and printf() to MBEDTLS_PLATFORM_STD_PRINTF)
 *
 * This makes sure there are no linking errors on platforms that do not support
 * these functions. You will HAVE to provide alternatives, either at runtime
 * via the platform_set_xxx() functions or at compile time by setting
 * the MBEDTLS_PLATFORM_STD_XXX defines, or enabling a
 * MBEDTLS_PLATFORM_XXX_MACRO.
 *
 * Requires: MBEDTLS_PLATFORM_C
 *
 * Uncomment to prevent default assignment of standard functions in the
 * platform layer.
 */
//#define MBEDTLS_PLATFORM_NO_STD_FUNCTIONS

/**
 * \def MBEDTLS_PLATFORM_EXIT_ALT
 *
 * MBEDTLS_PLATFORM_XXX_ALT: Uncomment a macro to let mbed TLS support the
 * function in the platform abstraction layer.
 *
 * Example: In case you uncomment MBEDTLS_PLATFORM_PRINTF_ALT, mbed TLS will
 * provide a function "mbedtls_platform_set_printf()" that allows you to set an
 * alternative printf function pointer.
 *
 * All these define require MBEDTLS_PLATFORM_C to be defined!
 *
 * \note MBEDTLS_PLATFORM_SNPRINTF_ALT is required on Windows;
 * it will be enabled automatically by check_config.h
 *
 * \warning MBEDTLS_PLATFORM_XXX_ALT cannot be defined at the same time as
 * MBEDTLS_PLATFORM_XXX_MACRO!
 *
 * Requires: MBEDTLS_PLATFORM_TIME_ALT requires MBEDTLS_HAVE_TIME
 *
 * Uncomment a macro to enable alternate implementation of specific base
 * platform function
 */
//#define MBEDTLS_PLATFORM_EXIT_ALT
//#define MBEDTLS_PLATFORM_TIME_ALT
//#define MBEDTLS_PLATFORM_FPRINTF_ALT
//#define MBEDTLS_PLATFORM_PRINTF_ALT
//#define MBEDTLS_PLATFORM_SNPRINTF_ALT
//#define MBEDTLS_PLATFORM_VSNPRINTF_ALT
//#define MBEDTLS_PLATFORM_NV_SEED_ALT
//#define MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT

/**
 * \def MBEDTLS_DEPRECATED_WARNING
 *
 * Mark deprecated functions and features so that they generate a warning if
 * used. Functionality deprecated in one version will usually be removed in the
 * next version. You can enable this to help you prepare the transition to a
 * new major version by making sure your code is not using this functionality.
 *
 * This only works with GCC and Clang. With other compilers, you may want to
 * use MBEDTLS_DEPRECATED_REMOVED
 *
 * Uncomment to get warnings on using deprecated functions and features.
 */
//#define MBEDTLS_DEPRECATED_WARNING

/**
 * \def MBEDTLS_DEPRECATED_REMOVED
 *
 * Remove deprecated functions and features so that they generate an error if
 * used. Functionality deprecated in one version will usually be removed in the
 * next version. You can enable this to help you prepare the transition to a
 * new major version by making sure your code is not using this functionality.
 *
 * Uncomment to get errors on using deprecated functions and features.
 */
//#define MBEDTLS_DEPRECATED_REMOVED

/**
 * \def MBEDTLS_CHECK_PARAMS
 *
 * This configuration option controls whether the library validates more of
 * the parameters passed to it.
 *
 * When this flag is not defined, the library only attempts to validate an
 * input parameter if: (1) they may come from the outside world (such as the
 * network, the filesystem, etc.) or (2) not validating them could result in
 * internal memory errors such as overflowing a buffer controlled by the
 * library. On the other hand, it doesn't attempt to validate parameters whose
 * values are fully controlled by the application (such as pointers).
 *
 * When this flag is defined, the library additionally attempts to validate
 * parameters that are fully controlled by the application, and should always
 * be valid if the application code is fully correct and trusted.
 *
 * For example, when a function accepts as input a pointer to a buffer that may
 * contain untrusted data, and its documentation mentions that this pointer
 * must not be NULL:
 * - The pointer is checked to be non-NULL only if this option is enabled.
 * - The content of the buffer is always validated.
 *
 * When this flag is defined, if a library function receives a parameter that
 * is invalid:
 * 1. The function will invoke the macro MBEDTLS_PARAM_FAILED().
 * 2. If MBEDTLS_PARAM_FAILED() did not terminate the program, the function
 *   will immediately return. If the function returns an Mbed TLS error code,
 *   the error code in this case is MBEDTLS_ERR_xxx_BAD_INPUT_DATA.
 *
 * When defining this flag, you also need to arrange a definition for
 * MBEDTLS_PARAM_FAILED(). You can do this by any of the following methods:
 * - By default, the library defines MBEDTLS_PARAM_FAILED() to call a
 *   function mbedtls_param_failed(), but the library does not define this
 *   function. If you do not make any other arrangements, you must provide
 *   the function mbedtls_param_failed() in your application.
 *   See `platform_util.h` for its prototype.
 * - If you enable the macro #MBEDTLS_CHECK_PARAMS_ASSERT, then the
 *   library defines MBEDTLS_PARAM_FAILED(\c cond) to be `assert(cond)`.
 *   You can still supply an alternative definition of
 *   MBEDTLS_PARAM_FAILED(), which may call `assert`.
 * - If you define a macro MBEDTLS_PARAM_FAILED() before including `config.h`
 *   or you uncomment the definition of MBEDTLS_PARAM_FAILED() in `config.h`,
 *   the library will call the macro that you defined and will not supply
 *   its own version. Note that if MBEDTLS_PARAM_FAILED() calls `assert`,
 *   you need to enable #MBEDTLS_CHECK_PARAMS_ASSERT so that library source
 *   files include `<assert.h>`.
 *
 * Uncomment to enable validation of application-controlled parameters.
 */
//#define MBEDTLS_CHECK_PARAMS

/**
 * \def MBEDTLS_CHECK_PARAMS_ASSERT
 *
 * Allow MBEDTLS_PARAM_FAILED() to call `assert`, and make it default to
 * `assert`. This macro is only used if #MBEDTLS_CHECK_PARAMS is defined.
 *
 * If this macro is not defined, then MBEDTLS_PARAM_FAILED() defaults to
 * calling a function mbedtls_param_failed(). See the documentation of
 * #MBEDTLS_CHECK_PARAMS for details.
 *
 * Uncomment to allow MBEDTLS_PARAM_FAILED() to call `assert`.
 */
//#define MBEDTLS_CHECK_PARAMS_ASSERT

/* \} name SECTION: System support */

/**
 * \name SECTION: mbed TLS feature support
 *
 * This section sets support for features that are or are not needed
 * within the modules that are enabled.
 * \{
 */

/**
 * \def MBEDTLS_TIMING_ALT
 *
 * Uncomment to provide your own alternate implementation for mbedtls_timing_hardclock(),
 * mbedtls_timing_get_timer(), mbedtls_set_alarm(), mbedtls_set/get_delay()
 *
 * Only works if you have MBEDTLS_TIMING_C enabled.
 *
 * You will need to provide a header "timing_alt.h" and an implementation at
 * compile time.
 */
//#define MBEDTLS_TIMING_ALT

/**
 * \def MBEDTLS_AES_ALT
 *
 * MBEDTLS__MODULE_NAME__ALT: Uncomment a macro to let mbed TLS use your
 * alternate core implementation of a symmetric crypto, an arithmetic or hash
 * module (e.g. platform specific assembly optimized implementations). Keep
 * in mind that the function prototypes should remain the same.
 *
 * This replaces the whole module. If you only want to replace one of the
 * functions, use one of the MBEDTLS__FUNCTION_NAME__ALT flags.
 *
 * Example: In case you uncomment MBEDTLS_AES_ALT, mbed TLS will no longer
 * provide the "struct mbedtls_aes_context" definition and omit the base
 * function declarations and implementations. "aes_alt.h" will be included from
 * "aes.h" to include the new function definitions.
 *
 * Uncomment a macro to enable alternate implementation of the corresponding
 * module.
 *
 * \warning   MD2, MD4, MD5, ARC4, DES and SHA-1 are considered weak and their
 *            use constitutes a security risk. If possible, we recommend
 *            avoiding dependencies on them, and considering stronger message
 *            digests and ciphers instead.
 *
 */
//#define MBEDTLS_AES_ALT
//#define MBEDTLS_ARC4_ALT
//#define MBEDTLS_ARIA_ALT
//#define MBEDTLS_BLOWFISH_ALT
//#define MBEDTLS_CAMELLIA_ALT
//#define MBEDTLS_CCM_ALT
//#define MBEDTLS_CHACHA20_ALT
//#define MBEDTLS_CHACHAPOLY_ALT
//#define MBEDTLS_CMAC_ALT
//#define MBEDTLS_DES_ALT
//#define MBEDTLS_DHM_ALT
//#define MBEDTLS_ECJPAKE_ALT
//#define MBEDTLS_GCM_ALT
//#define MBEDTLS_NIST_KW_ALT
//#define MBEDTLS_MD2_ALT
//#define MBEDTLS_MD4_ALT
//#define MBEDTLS_MD5_ALT
//#define MBEDTLS_POLY1305_ALT
//#define MBEDTLS_RIPEMD160_ALT
//#define MBEDTLS_RSA_ALT
//#define MBEDTLS_SHA1_ALT
//#define MBEDTLS_SHA256_ALT
//#define MBEDTLS_SHA512_ALT
//#define MBEDTLS_XTEA_ALT

/*
 * When replacing the elliptic curve module, pleace consider, that it is
 * implemented with two .c files:
 *      - ecp.c
 *      - ecp_curves.c
 * You can replace them very much like all the other MBEDTLS__MODULE_NAME__ALT
 * macros as described above. The only difference is that you have to make sure
 * that you provide functionality for both .c files.
 */
//#define MBEDTLS_ECP_ALT

/**
 * \def MBEDTLS_MD2_PROCESS_ALT
 *
 * MBEDTLS__FUNCTION_NAME__ALT: Uncomment a macro to let mbed TLS use you
 * alternate core implementation of symmetric crypto or hash function. Keep in
 * mind that function prototypes should remain the same.
 *
 * This replaces only one function. The header file from mbed TLS is still
 * used, in contrast to the MBEDTLS__MODULE_NAME__ALT flags.
 *
 * Example: In case you uncomment MBEDTLS_SHA256_PROCESS_ALT, mbed TLS will
 * no longer provide the mbedtls_sha1_process() function, but it will still provide
 * the other function (using your mbedtls_sha1_process() function) and the definition
 * of mbedtls_sha1_context, so your implementation of mbedtls_sha1_process must be compatible
 * with this definition.
 *
 * \note Because of a signature change, the core AES encryption and decryption routines are
 *       currently named mbedtls_aes_internal_encrypt and mbedtls_aes_internal_decrypt,
 *       respectively. When setting up alternative implementations, these functions should
 *       be overridden, but the wrapper functions mbedtls_aes_decrypt and mbedtls_aes_encrypt
 *       must stay untouched.
 *
 * \note If you use the AES_xxx_ALT macros, then is is recommended to also set
 *       MBEDTLS_AES_ROM_TABLES in order to help the linker garbage-collect the AES
 *       tables.
 *
 * Uncomment a macro to enable alternate implementation of the corresponding
 * function.
 *
 * \warning   MD2, MD4, MD5, DES and SHA-1 are considered weak and their use
 *            constitutes a security risk. If possible, we recommend avoiding
 *            dependencies on them, and considering stronger message digests
 *            and ciphers instead.
 *
 * \warning   If both MBEDTLS_ECDSA_SIGN_ALT and MBEDTLS_ECDSA_DETERMINISTIC are
 *            enabled, then the deterministic ECDH signature functions pass the
 *            the static HMAC-DRBG as RNG to mbedtls_ecdsa_sign(). Therefore
 *            alternative implementations should use the RNG only for generating
 *            the ephemeral key and nothing else. If this is not possible, then
 *            MBEDTLS_ECDSA_DETERMINISTIC should be disabled and an alternative
 *            implementation should be provided for mbedtls_ecdsa_sign_det_ext()
 *            (and for mbedtls_ecdsa_sign_det() too if backward compatibility is
 *            desirable).
 *
 */
//#define MBEDTLS_MD2_PROCESS_ALT
//#define MBEDTLS_MD4_PROCESS_ALT
//#define MBEDTLS_MD5_PROCESS_ALT
//#define MBEDTLS_RIPEMD160_PROCESS_ALT
//#define MBEDTLS_SHA1_PROCESS_ALT
//#define MBEDTLS_SHA256_PROCESS_ALT
//#define MBEDTLS_SHA512_PROCESS_ALT
//#define MBEDTLS_DES_SETKEY_ALT
//#define MBEDTLS_DES_CRYPT_ECB_ALT
//#define MBEDTLS_DES3_CRYPT_ECB_ALT
//#define MBEDTLS_AES_SETKEY_ENC_ALT
//#define MBEDTLS_AES_SETKEY_DEC_ALT
//#define MBEDTLS_AES_ENCRYPT_ALT
//#define MBEDTLS_AES_DECRYPT_ALT
//#define MBEDTLS_ECDH_GEN_PUBLIC_ALT
//#define MBEDTLS_ECDH_COMPUTE_SHARED_ALT
//#define MBEDTLS_ECDSA_VERIFY_ALT
//#define MBEDTLS_ECDSA_SIGN_ALT
//#define MBEDTLS_ECDSA_GENKEY_ALT

/**
 * \def MBEDTLS_ECP_INTERNAL_ALT
 *
 * Expose a part of the internal interface of the Elliptic Curve Point module.
 *
 * MBEDTLS_ECP__FUNCTION_NAME__ALT: Uncomment a macro to let mbed TLS use your
 * alternative core implementation of elliptic curve arithmetic. Keep in mind
 * that function prototypes should remain the same.
 *
 * This partially replaces one function. The header file from mbed TLS is still
 * used, in contrast to the MBEDTLS_ECP_ALT flag. The original implementation
 * is still present and it is used for group structures not supported by the
 * alternative.
 *
 * Any of these options become available by defining MBEDTLS_ECP_INTERNAL_ALT
 * and implementing the following functions:
 *      unsigned char mbedtls_internal_ecp_grp_capable(
 *          const mbedtls_ecp_group *grp )
 *      int  mbedtls_internal_ecp_init( const mbedtls_ecp_group *grp )
 *      void mbedtls_internal_ecp_free( const mbedtls_ecp_group *grp )
 * The mbedtls_internal_ecp_grp_capable function should return 1 if the
 * replacement functions implement arithmetic for the given group and 0
 * otherwise.
 * The functions mbedtls_internal_ecp_init and mbedtls_internal_ecp_free are
 * called before and after each point operation and provide an opportunity to
 * implement optimized set up and tear down instructions.
 *
 * Example: In case you uncomment MBEDTLS_ECP_INTERNAL_ALT and
 * MBEDTLS_ECP_DOUBLE_JAC_ALT, mbed TLS will still provide the ecp_double_jac
 * function, but will use your mbedtls_internal_ecp_double_jac if the group is
 * supported (your mbedtls_internal_ecp_grp_capable function returns 1 when
 * receives it as an argument). If the group is not supported then the original
 * implementation is used. The other functions and the definition of
 * mbedtls_ecp_group and mbedtls_ecp_point will not change, so your
 * implementation of mbedtls_internal_ecp_double_jac and
 * mbedtls_internal_ecp_grp_capable must be compatible with this definition.
 *
 * Uncomment a macro to enable alternate implementation of the corresponding
 * function.
 */
/* Required for all the functions in this section */
//#define MBEDTLS_ECP_INTERNAL_ALT
/* Support for Weierstrass curves with Jacobi representation */
//#define MBEDTLS_ECP_RANDOMIZE_JAC_ALT
//#define MBEDTLS_ECP_ADD_MIXED_ALT
//#define MBEDTLS_ECP_DOUBLE_JAC_ALT
//#define MBEDTLS_ECP_NORMALIZE_JAC_MANY_ALT
//#define MBEDTLS_ECP_NORMALIZE_JAC_ALT
/* Support for curves with Montgomery arithmetic */
//#define MBEDTLS_ECP_DOUBLE_ADD_MXZ_ALT
//#define MBEDTLS_ECP_RANDOMIZE_MXZ_ALT
//#define MBEDTLS_ECP_NORMALIZE_MXZ_ALT

/**
 * \def MBEDTLS_TEST_NULL_ENTROPY
 *
 * Enables testing and use of mbed TLS without any configured entropy sources.
 * This permits use of the library on platforms before an entropy source has
 * been integrated (see for example the MBEDTLS_ENTROPY_HARDWARE_ALT or the
 * MBEDTLS_ENTROPY_NV_SEED switches).
 *
 * WARNING! This switch MUST be disabled in production builds, and is suitable
 * only for development.
 * Enabling the switch negates any security provided by the library.
 *
 * Requires MBEDTLS_ENTROPY_C, MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
 *
 */
//#define MBEDTLS_TEST_NULL_ENTROPY

/**
 * \def MBEDTLS_ENTROPY_HARDWARE_ALT
 *
 * Uncomment this macro to let mbed TLS use your own implementation of a
 * hardware entropy collector.
 *
 * Your function must be called \c mbedtls_hardware_poll(), have the same
 * prototype as declared in entropy_poll.h, and accept NULL as first argument.
 *
 * Uncomment to use your own hardware entropy collector.
 */
//#define MBEDTLS_ENTROPY_HARDWARE_ALT

/**
 * \def MBEDTLS_AES_ROM_TABLES
 *
 * Use precomputed AES tables stored in ROM.
 *
 * Uncomment this macro to use precomputed AES tables stored in ROM.
 * Comment this macro to generate AES tables in RAM at runtime.
 *
 * Tradeoff: Using precomputed ROM tables reduces RAM usage by ~8kb
 * (or ~2kb if \c MBEDTLS_AES_FEWER_TABLES is used) and reduces the
 * initialization time before the first AES operation can be performed.
 * It comes at the cost of additional ~8kb ROM use (resp. ~2kb if \c
 * MBEDTLS_AES_FEWER_TABLES below is used), and potentially degraded
 * performance if ROM access is slower than RAM access.
 *
 * This option is independent of \c MBEDTLS_AES_FEWER_TABLES.
 *
 */
//#define MBEDTLS_AES_ROM_TABLES

/**
 * \def MBEDTLS_AES_FEWER_TABLES
 *
 * Use less ROM/RAM for AES tables.
 *
 * Uncommenting this macro omits 75% of the AES tables from
 * ROM / RAM (depending on the value of \c MBEDTLS_AES_ROM_TABLES)
 * by computing their values on the fly during operations
 * (the tables are entry-wise rotations of one another).
 *
 * Tradeoff: Uncommenting this reduces the RAM / ROM footprint
 * by ~6kb but at the cost of more arithmetic operations during
 * runtime. Specifically, one has to compare 4 accesses within
 * different tables to 4 accesses with additional arithmetic
 * operations within the same table. The performance gain/loss
 * depends on the system and memory details.
 *
 * This option is independent of \c MBEDTLS_AES_ROM_TABLES.
 *
 */
//#define MBEDTLS_AES_FEWER_TABLES

/**
 * \def MBEDTLS_CAMELLIA_SMALL_MEMORY
 *
 * Use less ROM for the Camellia implementation (saves about 768 bytes).
 *
 * Uncomment this macro to use less memory for Camellia.
 */
//#define MBEDTLS_CAMELLIA_SMALL_MEMORY

/**
 * \def MBEDTLS_CIPHER_MODE_CBC
 *
 * Enable Cipher Block Chaining mode (CBC) for symmetric ciphers.
 */
//#define MBEDTLS_CIPHER_MODE_CBC

/**
 * \def MBEDTLS_CIPHER_MODE_CFB
 *
 * Enable Cipher Feedback mode (CFB) for symmetric ciphers.
 */
//#define MBEDTLS_CIPHER_MODE_CFB

/**
 * \def MBEDTLS_CIPHER_MODE_CTR
 *
 * Enable Counter Block Cipher mode (CTR) for symmetric ciphers.
 */
//#define MBEDTLS_CIPHER_MODE_CTR

/**
 * \def MBEDTLS_CIPHER_MODE_OFB
 *
 * Enable Output Feedback mode (OFB) for symmetric ciphers.
 */
//#define MBEDTLS_CIPHER_MODE_OFB

/**
 * \def MBEDTLS_CIPHER_MODE_XTS
 *
 * Enable Xor-encrypt-xor with ciphertext stealing mode (XTS) for AES.
 */
#define MBEDTLS_CIPHER_MODE_XTS

/**
 * \def MBEDTLS_CIPHER_NULL_CIPHER
 *
 * Enable NULL cipher.
 * Warning: Only do so when you know what you are doing. This allows for
 * encryption or channels without any security!
 *
 * Requires MBEDTLS_ENABLE_WEAK_CIPHERSUITES as well to enable
 * the following ciphersuites:
 *      MBEDTLS_TLS_ECDH_ECDSA_WITH_NULL_SHA
 *      MBEDTLS_TLS_ECDH_RSA_WITH_NULL_SHA
 *      MBEDTLS_TLS_ECDHE_ECDSA_WITH_NULL_SHA
 *      MBEDTLS_TLS_ECDHE_RSA_WITH_NULL_SHA
 *      MBEDTLS_TLS_ECDHE_PSK_WITH_NULL_SHA384
 *      MBEDTLS_TLS_ECDHE_PSK_WITH_NULL_SHA256
 *      MBEDTLS_TLS_ECDHE_PSK_WITH_NULL_SHA
 *      MBEDTLS_TLS_DHE_PSK_WITH_NULL_SHA384
 *      MBEDTLS_TLS_DHE_PSK_WITH_NULL_SHA256
 *      MBEDTLS_TLS_DHE_PSK_WITH_NULL_SHA
 *      MBEDTLS_TLS_RSA_WITH_NULL_SHA256
 *      MBEDTLS_TLS_RSA_WITH_NULL_SHA
 *      MBEDTLS_TLS_RSA_WITH_NULL_MD5
 *      MBEDTLS_TLS_RSA_PSK_WITH_NULL_SHA384
 *      MBEDTLS_TLS_RSA_PSK_WITH_NULL_SHA256
 *      MBEDTLS_TLS_RSA_PSK_WITH_NULL_SHA
 *      MBEDTLS_TLS_PSK_WITH_NULL_SHA384
 *      MBEDTLS_TLS_PSK_WITH_NULL_SHA256
 *      MBEDTLS_TLS_PSK_WITH_NULL_SHA
 *
 * Uncomment this macro to enable the NULL cipher and ciphersuites
 */
//#define MBEDTLS_CIPHER_NULL_CIPHER

/**
 * \def MBEDTLS_CIPHER_PADDING_PKCS7
 *
 * MBEDTLS_CIPHER_PADDING_XXX: Uncomment or comment macros to add support for
 * specific padding modes in the cipher layer with cipher modes that support
 * padding (e.g. CBC)
 *
 * If you disable all padding modes, only full blocks can be used with CBC.
 *
 * Enable padding modes in the cipher layer.
 */
#define MBEDTLS_CIPHER_PADDING_PKCS7
#define MBEDTLS_CIPHER_PADDING_ONE_AND_ZEROS
#define MBEDTLS_CIPHER_PADDING_ZEROS_AND_LEN
#define MBEDTLS_CIPHER_PADDING_ZEROS

/** \def MBEDTLS_CTR_DRBG_USE_128_BIT_KEY
 *
 * Uncomment this macro to use a 128-bit key in the CTR_DRBG module.
 * By default, CTR_DRBG uses a 256-bit key.
 */
//#define MBEDTLS_CTR_DRBG_USE_128_BIT_KEY

/**
 * \def MBEDTLS_ENABLE_WEAK_CIPHERSUITES
 *
 * Enable weak ciphersuites in SSL / TLS.
 * Warning: Only do so when you know what you are doing. This allows for
 * channels with virtually no security at all!
 *
 * This enables the following ciphersuites:
 *      MBEDTLS_TLS_RSA_WITH_DES_CBC_SHA
 *      MBEDTLS_TLS_DHE_RSA_WITH_DES_CBC_SHA
 *
 * Uncomment this macro to enable weak ciphersuites
 *
 * \warning   DES is considered a weak cipher and its use constitutes a
 *            security risk. We recommend considering stronger ciphers instead.
 */
//#define MBEDTLS_ENABLE_WEAK_CIPHERSUITES

/**
 * \def MBEDTLS_REMOVE_ARC4_CIPHERSUITES
 *
 * Remove RC4 ciphersuites by default in SSL / TLS.
 * This flag removes the ciphersuites based on RC4 from the default list as
 * returned by mbedtls_ssl_list_ciphersuites(). However, it is still possible to
 * enable (some of) them with mbedtls_ssl_conf_ciphersuites() by including them
 * explicitly.
 *
 * Uncomment this macro to remove RC4 ciphersuites by default.
 */
#define MBEDTLS_REMOVE_ARC4_CIPHERSUITES

/**
 * \def MBEDTLS_REMOVE_3DES_CIPHERSUITES
 *
 * Remove 3DES ciphersuites by default in SSL / TLS.
 * This flag removes the ciphersuites based on 3DES from the default list as
 * returned by mbedtls_ssl_list_ciphersuites(). However, it is still possible
 * to enable (some of) them with mbedtls_ssl_conf_ciphersuites() by including
 * them explicitly.
 *
 * A man-in-the-browser attacker can recover authentication tokens sent through
 * a TLS connection using a 3DES based cipher suite (see "On the Practical
 * (In-)Security of 64-bit Block Ciphers" by Karthikeyan Bhargavan and Gaëtan
 * Leurent, see https://sweet32.info/SWEET32_CCS16.pdf). If this attack falls
 * in your threat model or you are unsure, then you should keep this option
 * enabled to remove 3DES based cipher suites.
 *
 * Comment this macro to keep 3DES in the default ciphersuite list.
 */
#define MBEDTLS_REMOVE_3DES_CIPHERSUITES

/**
 * \def MBEDTLS_ECP_DP_SECP192R1_ENABLED
 *
 * MBEDTLS_ECP_XXXX_ENABLED: Enables specific curves within the Elliptic Curve
 * module.  By default all supported curves are enabled.
 *
 * Comment macros to disable the curve and functions for it
 */
/* Short Weierstrass curves (supporting ECP, ECDH, ECDSA) */
//#define MBEDTLS_ECP_DP_SECP192R1_ENABLED
//#define MBEDTLS_ECP_DP_SECP224R1_ENABLED
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
//#define MBEDTLS_ECP_DP_SECP384R1_ENABLED
//#define MBEDTLS_ECP_DP_SECP521R1_ENABLED
//#define MBEDTLS_ECP_DP_SECP192K1_ENABLED
//#define MBEDTLS_ECP_DP_SECP224K1_ENABLED
//#define MBEDTLS_ECP_DP_SECP256K1_ENABLED
//#define MBEDTLS_ECP_DP_BP256R1_ENABLED
//#define MBEDTLS_ECP_DP_BP384R1_ENABLED
//#define MBEDTLS_ECP_DP_BP512R1_ENABLED
/* Montgomery curves (supporting ECP) */
//#define MBEDTLS_ECP_DP_CURVE25519_ENABLED
//#define MBEDTLS_ECP_DP_CURVE448_ENABLED

/**
 * \def MBEDTLS_ECP_NIST_OPTIM
 *
 * Enable specific 'modulo p' routines for each NIST prime.
 * Depending on the prime and architecture, makes operations 4 to 8 times
 * faster on the corresponding curve.
 *
 * Comment this macro to disable NIST curves optimisation.
 */
#define MBEDTLS_ECP_NIST_OPTIM

/**
 * \def MBEDTLS_ECP_NO_INTERNAL_RNG
 *
 * When this option is disabled, mbedtls_ecp_mul() will make use of an
 * internal RNG when called with a NULL \c f_rng argument, in order to protect
 * against some side-channel attacks.
 *
 * This protection introduces a dependency of the ECP module on one of the
 * DRBG modules. For very constrained implementations that don't require this
 * protection (for example, because you're only doing signature verification,
 * so not manipulating any secret, or because local/physical side-channel
 * attacks are outside your threat model), it might be desirable to get rid of
 * that dependency.
 *
 * \warning Enabling this option makes some uses of ECP vulnerable to some
 * side-channel attacks. Only enable it if you know that's not a problem for
 * your use case.
 *
 * Uncomment this macro to disable some counter-measures in ECP.
 */
#define MBEDTLS_ECP_NO_INTERNAL_RNG

/**
 * \def MBEDTLS_NO_PLATFORM_ENTROPY
 *
 * Do not use built-in platform entropy functions.
 * This is useful if your platform does not support
 * standards like the /dev/urandom or Windows CryptoAPI.
 *
 * Uncomment this macro to disable the built-in platform entropy functions.
 */
#define MBEDTLS_NO_PLATFORM_ENTROPY

/**
 * \def MBEDTLS_ENTROPY_FORCE_SHA256
 *
 * Force the entropy accumulator to use a SHA-256 accumulator instead of the
 * default SHA-512 based one (if both are available).
 *
 * Requires: MBEDTLS_SHA256_C
 *
 * On 32-bit systems SHA-256 can be much faster than SHA-512. Use this option
 * if you have performance concerns.
 *
 * This option is only useful if both MBEDTLS_SHA256_C and
 * MBEDTLS_SHA512_C are defined. Otherwise the available hash module is used.
 */
#define MBEDTLS_ENTROPY_FORCE_SHA256

/**
 * \def MBEDTLS_SHA256_SMALLER
 *
 * Enable an implementation of SHA-256 that has lower ROM footprint but also
 * lower performance.
 *
 * The default implementation is meant to be a reasonnable compromise between
 * performance and size. This version optimizes more aggressively for size at
 * the expense of performance. Eg on Cortex-M4 it reduces the size of
 * mbedtls_sha256_process() from ~2KB to ~0.5KB for a performance hit of about
 * 30%.
 *
 * Uncomment to enable the smaller implementation of SHA256.
 */
#define MBEDTLS_SHA256_SMALLER


/**
 * \def MBEDTLS_BIGNUM_C
 *
 * Enable the multi-precision integer library.
 *
 * Module:  library/bignum.c
 * Caller:  library/dhm.c
 *          library/ecp.c
 *          library/ecdsa.c
 *          library/rsa.c
 *          library/rsa_internal.c
 *          library/ssl_tls.c
 *
 * This module is required for RSA, DHM and ECC (ECDH, ECDSA) support.
 */
#define MBEDTLS_BIGNUM_C

/**
 * \def MBEDTLS_ECDSA_C
 *
 * Enable the elliptic curve DSA library.
 *
 * Module:  library/ecdsa.c
 * Caller:
 *
 * This module is used by the following key exchanges:
 *      ECDHE-ECDSA
 *
 * Requires: MBEDTLS_ECP_C, MBEDTLS_ASN1_WRITE_C, MBEDTLS_ASN1_PARSE_C,
 *           and at least one MBEDTLS_ECP_DP_XXX_ENABLED for a
 *           short Weierstrass curve.
 */
#define MBEDTLS_ECDSA_C
#define MBEDTLS_ASN1_WRITE_C
#define MBEDTLS_ASN1_PARSE_C

/**
 * \def MBEDTLS_ECP_C
 *
 * Enable the elliptic curve over GF(p) library.
 *
 * Module:  library/ecp.c
 * Caller:  library/ecdh.c
 *          library/ecdsa.c
 *          library/ecjpake.c
 *
 * Requires: MBEDTLS_BIGNUM_C and at least one MBEDTLS_ECP_DP_XXX_ENABLED
 */
#define MBEDTLS_ECP_C

/**
 * \def MBEDTLS_ERROR_C
 *
 * Enable error code to error string conversion.
 *
 * Module:  library/error.c
 * Caller:
 *
 * This module enables mbedtls_strerror().
 */
#define MBEDTLS_ERROR_C

/**
 * \def MBEDTLS_MD_C
 *
 * Enable the generic message digest layer.
 *
 * Module:  library/md.c
 * Caller:
 *
 * Uncomment to enable generic message digest wrappers.
 */
#define MBEDTLS_MD_C

/**
 * \def MBEDTLS_PK_C
 *
 * Enable the generic public (asymetric) key layer.
 *
 * Module:  library/pk.c
 * Caller:  library/ssl_tls.c
 *          library/ssl_cli.c
 *          library/ssl_srv.c
 *
 * Requires: MBEDTLS_RSA_C or MBEDTLS_ECP_C
 *
 * Uncomment to enable generic public key wrappers.
 */
#define MBEDTLS_PK_C

/**
 * \def MBEDTLS_PK_PARSE_C
 *
 * Enable the generic public (asymetric) key parser.
 *
 * Module:  library/pkparse.c
 * Caller:  library/x509_crt.c
 *          library/x509_csr.c
 *
 * Requires: MBEDTLS_PK_C
 *
 * Uncomment to enable generic public key parse functions.
 */
#define MBEDTLS_PK_PARSE_C

/**
 * \def MBEDTLS_PLATFORM_C
 *
 * Enable the platform abstraction layer that allows you to re-assign
 * functions like calloc(), free(), snprintf(), printf(), fprintf(), exit().
 *
 * Enabling MBEDTLS_PLATFORM_C enables to use of MBEDTLS_PLATFORM_XXX_ALT
 * or MBEDTLS_PLATFORM_XXX_MACRO directives, allowing the functions mentioned
 * above to be specified at runtime or compile time respectively.
 *
 * \note This abstraction layer must be enabled on Windows (including MSYS2)
 * as other module rely on it for a fixed snprintf implementation.
 *
 * Module:  library/platform.c
 * Caller:  Most other .c files
 *
 * This module enables abstraction of common (libc) functions.
 */
#define MBEDTLS_PLATFORM_C

/**
 * \def MBEDTLS_SHA256_C
 *
 * Enable the SHA-224 and SHA-256 cryptographic hash algorithms.
 *
 * Module:  library/sha256.c
 * Caller:  library/entropy.c
 *          library/md.c
 *          library/ssl_cli.c
 *          library/ssl_srv.c
 *          library/ssl_tls.c
 *
 * This module adds support for SHA-224 and SHA-256.
 * This module is required for the SSL/TLS 1.2 PRF function.
 */
#define MBEDTLS_SHA256_C

/**
 * \def MBEDTLS_VERSION_C
 *
 * Enable run-time version information.
 *
 * Module:  library/version.c
 *
 * This module provides run-time version information.
 */
#define MBEDTLS_VERSION_C

/* \} name SECTION: mbed TLS modules */

/**
 * \name SECTION: Module configuration options
 *
 * This section allows for the setting of module specific sizes and
 * configuration options. The default values are already present in the
 * relevant header files and should suffice for the regular use cases.
 *
 * Our advice is to enable options and change their values here
 * only if you have a good reason and know the consequences.
 *
 * Please check the respective header file for documentation on these
 * parameters (to prevent duplicate documentation).
 * \{
 */

/* MPI / BIGNUM options */
//#define MBEDTLS_MPI_WINDOW_SIZE            6 /**< Maximum window size used. */
//#define MBEDTLS_MPI_MAX_SIZE            1024 /**< Maximum number of bytes for usable MPIs. */

/* CTR_DRBG options */
//#define MBEDTLS_CTR_DRBG_ENTROPY_LEN               48 /**< Amount of entropy used per seed by default (48 with SHA-512, 32 with SHA-256) */
//#define MBEDTLS_CTR_DRBG_RESEED_INTERVAL        10000 /**< Interval before reseed is performed by default */
//#define MBEDTLS_CTR_DRBG_MAX_INPUT                256 /**< Maximum number of additional input bytes */
//#define MBEDTLS_CTR_DRBG_MAX_REQUEST             1024 /**< Maximum number of requested bytes per call */
//#define MBEDTLS_CTR_DRBG_MAX_SEED_INPUT           384 /**< Maximum size of (re)seed buffer */

/* HMAC_DRBG options */
//#define MBEDTLS_HMAC_DRBG_RESEED_INTERVAL   10000 /**< Interval before reseed is performed by default */
//#define MBEDTLS_HMAC_DRBG_MAX_INPUT           256 /**< Maximum number of additional input bytes */
//#define MBEDTLS_HMAC_DRBG_MAX_REQUEST        1024 /**< Maximum number of requested bytes per call */
//#define MBEDTLS_HMAC_DRBG_MAX_SEED_INPUT      384 /**< Maximum size of (re)seed buffer */

/* ECP options */
//#define MBEDTLS_ECP_MAX_BITS             521 /**< Maximum bit size of groups */
//#define MBEDTLS_ECP_WINDOW_SIZE            6 /**< Maximum window size used */
//#define MBEDTLS_ECP_FIXED_POINT_OPTIM      1 /**< Enable fixed-point speed-up */

/* Entropy options */
//#define MBEDTLS_ENTROPY_MAX_SOURCES                20 /**< Maximum number of sources supported */
//#define MBEDTLS_ENTROPY_MAX_GATHER                128 /**< Maximum amount requested from entropy sources */
//#define MBEDTLS_ENTROPY_MIN_HARDWARE               32 /**< Default minimum number of bytes required for the hardware entropy source mbedtls_hardware_poll() before entropy is released */

/* Memory buffer allocator options */
//#define MBEDTLS_MEMORY_ALIGN_MULTIPLE      4 /**< Align on multiples of this value */

/* Platform options */
//#define MBEDTLS_PLATFORM_STD_MEM_HDR   <stdlib.h> /**< Header to include if MBEDTLS_PLATFORM_NO_STD_FUNCTIONS is defined. Don't define if no header is needed. */
//#define MBEDTLS_PLATFORM_STD_CALLOC        calloc /**< Default allocator to use, can be undefined */
//#define MBEDTLS_PLATFORM_STD_FREE            free /**< Default free to use, can be undefined */
//#define MBEDTLS_PLATFORM_STD_EXIT            exit /**< Default exit to use, can be undefined */
//#define MBEDTLS_PLATFORM_STD_TIME            time /**< Default time to use, can be undefined. MBEDTLS_HAVE_TIME must be enabled */
//#define MBEDTLS_PLATFORM_STD_FPRINTF      fprintf /**< Default fprintf to use, can be undefined */
//#define MBEDTLS_PLATFORM_STD_PRINTF        printf /**< Default printf to use, can be undefined */
/* Note: your snprintf must correctly zero-terminate the buffer! */
//#define MBEDTLS_PLATFORM_STD_SNPRINTF    snprintf /**< Default snprintf to use, can be undefined */
//#define MBEDTLS_PLATFORM_STD_EXIT_SUCCESS       0 /**< Default exit value to use, can be undefined */
//#define MBEDTLS_PLATFORM_STD_EXIT_FAILURE       1 /**< Default exit value to use, can be undefined */
//#define MBEDTLS_PLATFORM_STD_NV_SEED_READ   mbedtls_platform_std_nv_seed_read /**< Default nv_seed_read function to use, can be undefined */
//#define MBEDTLS_PLATFORM_STD_NV_SEED_WRITE  mbedtls_platform_std_nv_seed_write /**< Default nv_seed_write function to use, can be undefined */
//#define MBEDTLS_PLATFORM_STD_NV_SEED_FILE  "seedfile" /**< Seed file to read/write with default implementation */

/* To Use Function Macros MBEDTLS_PLATFORM_C must be enabled */
/* MBEDTLS_PLATFORM_XXX_MACRO and MBEDTLS_PLATFORM_XXX_ALT cannot both be defined */
//#define MBEDTLS_PLATFORM_CALLOC_MACRO        calloc /**< Default allocator macro to use, can be undefined */
//#define MBEDTLS_PLATFORM_FREE_MACRO            free /**< Default free macro to use, can be undefined */
//#define MBEDTLS_PLATFORM_EXIT_MACRO            exit /**< Default exit macro to use, can be undefined */
//#define MBEDTLS_PLATFORM_TIME_MACRO            time /**< Default time macro to use, can be undefined. MBEDTLS_HAVE_TIME must be enabled */
//#define MBEDTLS_PLATFORM_TIME_TYPE_MACRO       time_t /**< Default time macro to use, can be undefined. MBEDTLS_HAVE_TIME must be enabled */
//#define MBEDTLS_PLATFORM_FPRINTF_MACRO      fprintf /**< Default fprintf macro to use, can be undefined */
//#define MBEDTLS_PLATFORM_PRINTF_MACRO        printf /**< Default printf macro to use, can be undefined */
/* Note: your snprintf must correctly zero-terminate the buffer! */
//#define MBEDTLS_PLATFORM_SNPRINTF_MACRO    snprintf /**< Default snprintf macro to use, can be undefined */
//#define MBEDTLS_PLATFORM_VSNPRINTF_MACRO    vsnprintf /**< Default vsnprintf macro to use, can be undefined */
//#define MBEDTLS_PLATFORM_NV_SEED_READ_MACRO   mbedtls_platform_std_nv_seed_read /**< Default nv_seed_read function to use, can be undefined */
//#define MBEDTLS_PLATFORM_NV_SEED_WRITE_MACRO  mbedtls_platform_std_nv_seed_write /**< Default nv_seed_write function to use, can be undefined */

/**
 * \brief       This macro is invoked by the library when an invalid parameter
 *              is detected that is only checked with #MBEDTLS_CHECK_PARAMS
 *              (see the documentation of that option for context).
 *
 *              When you leave this undefined here, the library provides
 *              a default definition. If the macro #MBEDTLS_CHECK_PARAMS_ASSERT
 *              is defined, the default definition is `assert(cond)`,
 *              otherwise the default definition calls a function
 *              mbedtls_param_failed(). This function is declared in
 *              `platform_util.h` for the benefit of the library, but
 *              you need to define in your application.
 *
 *              When you define this here, this replaces the default
 *              definition in platform_util.h (which no longer declares the
 *              function mbedtls_param_failed()) and it is your responsibility
 *              to make sure this macro expands to something suitable (in
 *              particular, that all the necessary declarations are visible
 *              from within the library - you can ensure that by providing
 *              them in this file next to the macro definition).
 *              If you define this macro to call `assert`, also define
 *              #MBEDTLS_CHECK_PARAMS_ASSERT so that library source files
 *              include `<assert.h>`.
 *
 *              Note that you may define this macro to expand to nothing, in
 *              which case you don't have to worry about declarations or
 *              definitions. However, you will then be notified about invalid
 *              parameters only in non-void functions, and void function will
 *              just silently return early on invalid parameters, which
 *              partially negates the benefits of enabling
 *              #MBEDTLS_CHECK_PARAMS in the first place, so is discouraged.
 *
 * \param cond  The expression that should evaluate to true, but doesn't.
 */
//#define MBEDTLS_PARAM_FAILED( cond )               assert( cond )

/* SSL Cache options */
//#define MBEDTLS_SSL_CACHE_DEFAULT_TIMEOUT       86400 /**< 1 day  */
//#define MBEDTLS_SSL_CACHE_DEFAULT_MAX_ENTRIES      50 /**< Maximum entries in cache */

/* SSL options */

/** \def MBEDTLS_SSL_MAX_CONTENT_LEN
 *
 * Maximum length (in bytes) of incoming and outgoing plaintext fragments.
 *
 * This determines the size of both the incoming and outgoing TLS I/O buffers
 * in such a way that both are capable of holding the specified amount of
 * plaintext data, regardless of the protection mechanism used.
 *
 * To configure incoming and outgoing I/O buffers separately, use
 * #MBEDTLS_SSL_IN_CONTENT_LEN and #MBEDTLS_SSL_OUT_CONTENT_LEN,
 * which overwrite the value set by this option.
 *
 * \note When using a value less than the default of 16KB on the client, it is
 *       recommended to use the Maximum Fragment Length (MFL) extension to
 *       inform the server about this limitation. On the server, there
 *       is no supported, standardized way of informing the client about
 *       restriction on the maximum size of incoming messages, and unless
 *       the limitation has been communicated by other means, it is recommended
 *       to only change the outgoing buffer size #MBEDTLS_SSL_OUT_CONTENT_LEN
 *       while keeping the default value of 16KB for the incoming buffer.
 *
 * Uncomment to set the maximum plaintext size of both
 * incoming and outgoing I/O buffers.
 */
//#define MBEDTLS_SSL_MAX_CONTENT_LEN             16384

/** \def MBEDTLS_SSL_IN_CONTENT_LEN
 *
 * Maximum length (in bytes) of incoming plaintext fragments.
 *
 * This determines the size of the incoming TLS I/O buffer in such a way
 * that it is capable of holding the specified amount of plaintext data,
 * regardless of the protection mechanism used.
 *
 * If this option is undefined, it inherits its value from
 * #MBEDTLS_SSL_MAX_CONTENT_LEN.
 *
 * \note When using a value less than the default of 16KB on the client, it is
 *       recommended to use the Maximum Fragment Length (MFL) extension to
 *       inform the server about this limitation. On the server, there
 *       is no supported, standardized way of informing the client about
 *       restriction on the maximum size of incoming messages, and unless
 *       the limitation has been communicated by other means, it is recommended
 *       to only change the outgoing buffer size #MBEDTLS_SSL_OUT_CONTENT_LEN
 *       while keeping the default value of 16KB for the incoming buffer.
 *
 * Uncomment to set the maximum plaintext size of the incoming I/O buffer
 * independently of the outgoing I/O buffer.
 */
//#define MBEDTLS_SSL_IN_CONTENT_LEN              16384

/** \def MBEDTLS_SSL_CID_IN_LEN_MAX
 *
 * The maximum length of CIDs used for incoming DTLS messages.
 *
 */
//#define MBEDTLS_SSL_CID_IN_LEN_MAX 32

/** \def MBEDTLS_SSL_CID_OUT_LEN_MAX
 *
 * The maximum length of CIDs used for outgoing DTLS messages.
 *
 */
//#define MBEDTLS_SSL_CID_OUT_LEN_MAX 32

/** \def MBEDTLS_SSL_CID_PADDING_GRANULARITY
 *
 * This option controls the use of record plaintext padding
 * when using the Connection ID extension in DTLS 1.2.
 *
 * The padding will always be chosen so that the length of the
 * padded plaintext is a multiple of the value of this option.
 *
 * Note: A value of \c 1 means that no padding will be used
 *       for outgoing records.
 *
 * Note: On systems lacking division instructions,
 *       a power of two should be preferred.
 *
 */
//#define MBEDTLS_SSL_CID_PADDING_GRANULARITY 16

/** \def MBEDTLS_SSL_TLS1_3_PADDING_GRANULARITY
 *
 * This option controls the use of record plaintext padding
 * in TLS 1.3.
 *
 * The padding will always be chosen so that the length of the
 * padded plaintext is a multiple of the value of this option.
 *
 * Note: A value of \c 1 means that no padding will be used
 *       for outgoing records.
 *
 * Note: On systems lacking division instructions,
 *       a power of two should be preferred.
 */
//#define MBEDTLS_SSL_TLS1_3_PADDING_GRANULARITY 1

/** \def MBEDTLS_SSL_OUT_CONTENT_LEN
 *
 * Maximum length (in bytes) of outgoing plaintext fragments.
 *
 * This determines the size of the outgoing TLS I/O buffer in such a way
 * that it is capable of holding the specified amount of plaintext data,
 * regardless of the protection mechanism used.
 *
 * If this option undefined, it inherits its value from
 * #MBEDTLS_SSL_MAX_CONTENT_LEN.
 *
 * It is possible to save RAM by setting a smaller outward buffer, while keeping
 * the default inward 16384 byte buffer to conform to the TLS specification.
 *
 * The minimum required outward buffer size is determined by the handshake
 * protocol's usage. Handshaking will fail if the outward buffer is too small.
 * The specific size requirement depends on the configured ciphers and any
 * certificate data which is sent during the handshake.
 *
 * Uncomment to set the maximum plaintext size of the outgoing I/O buffer
 * independently of the incoming I/O buffer.
 */
//#define MBEDTLS_SSL_OUT_CONTENT_LEN             16384

/** \def MBEDTLS_SSL_DTLS_MAX_BUFFERING
 *
 * Maximum number of heap-allocated bytes for the purpose of
 * DTLS handshake message reassembly and future message buffering.
 *
 * This should be at least 9/8 * MBEDTLSSL_IN_CONTENT_LEN
 * to account for a reassembled handshake message of maximum size,
 * together with its reassembly bitmap.
 *
 * A value of 2 * MBEDTLS_SSL_IN_CONTENT_LEN (32768 by default)
 * should be sufficient for all practical situations as it allows
 * to reassembly a large handshake message (such as a certificate)
 * while buffering multiple smaller handshake messages.
 *
 */
//#define MBEDTLS_SSL_DTLS_MAX_BUFFERING             32768

//#define MBEDTLS_SSL_DEFAULT_TICKET_LIFETIME     86400 /**< Lifetime of session tickets (if enabled) */
//#define MBEDTLS_PSK_MAX_LEN               32 /**< Max size of TLS pre-shared keys, in bytes (default 256 bits) */
//#define MBEDTLS_SSL_COOKIE_TIMEOUT        60 /**< Default expiration delay of DTLS cookies, in seconds if HAVE_TIME, or in number of cookies issued */

/**
 * Complete list of ciphersuites to use, in order of preference.
 *
 * \warning No dependency checking is done on that field! This option can only
 * be used to restrict the set of available ciphersuites. It is your
 * responsibility to make sure the needed modules are active.
 *
 * Use this to save a few hundred bytes of ROM (default ordering of all
 * available ciphersuites) and a few to a few hundred bytes of RAM.
 *
 * The value below is only an example, not the default.
 */
//#define MBEDTLS_SSL_CIPHERSUITES MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,MBEDTLS_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256

/* X509 options */
//#define MBEDTLS_X509_MAX_INTERMEDIATE_CA   8   /**< Maximum number of intermediate CAs in a verification chain. */
//#define MBEDTLS_X509_MAX_FILE_PATH_LEN     512 /**< Maximum length of a path/filename string in bytes including the null terminator character ('\0'). */

/**
 * Allow SHA-1 in the default TLS configuration for certificate signing.
 * Without this build-time option, SHA-1 support must be activated explicitly
 * through mbedtls_ssl_conf_cert_profile. Turning on this option is not
 * recommended because of it is possible to generate SHA-1 collisions, however
 * this may be safe for legacy infrastructure where additional controls apply.
 *
 * \warning   SHA-1 is considered a weak message digest and its use constitutes
 *            a security risk. If possible, we recommend avoiding dependencies
 *            on it, and considering stronger message digests instead.
 *
 */
//#define MBEDTLS_TLS_DEFAULT_ALLOW_SHA1_IN_CERTIFICATES

/**
 * Allow SHA-1 in the default TLS configuration for TLS 1.2 handshake
 * signature and ciphersuite selection. Without this build-time option, SHA-1
 * support must be activated explicitly through mbedtls_ssl_conf_sig_hashes.
 * The use of SHA-1 in TLS <= 1.1 and in HMAC-SHA-1 is always allowed by
 * default. At the time of writing, there is no practical attack on the use
 * of SHA-1 in handshake signatures, hence this option is turned on by default
 * to preserve compatibility with existing peers, but the general
 * warning applies nonetheless:
 *
 * \warning   SHA-1 is considered a weak message digest and its use constitutes
 *            a security risk. If possible, we recommend avoiding dependencies
 *            on it, and considering stronger message digests instead.
 *
 */
#define MBEDTLS_TLS_DEFAULT_ALLOW_SHA1_IN_KEY_EXCHANGE

/**
 * Uncomment the macro to let mbed TLS use your alternate implementation of
 * mbedtls_platform_zeroize(). This replaces the default implementation in
 * platform_util.c.
 *
 * mbedtls_platform_zeroize() is a widely used function across the library to
 * zero a block of memory. The implementation is expected to be secure in the
 * sense that it has been written to prevent the compiler from removing calls
 * to mbedtls_platform_zeroize() as part of redundant code elimination
 * optimizations. However, it is difficult to guarantee that calls to
 * mbedtls_platform_zeroize() will not be optimized by the compiler as older
 * versions of the C language standards do not provide a secure implementation
 * of memset(). Therefore, MBEDTLS_PLATFORM_ZEROIZE_ALT enables users to
 * configure their own implementation of mbedtls_platform_zeroize(), for
 * example by using directives specific to their compiler, features from newer
 * C standards (e.g using memset_s() in C11) or calling a secure memset() from
 * their system (e.g explicit_bzero() in BSD).
 */
//#define MBEDTLS_PLATFORM_ZEROIZE_ALT

/**
 * Uncomment the macro to let Mbed TLS use your alternate implementation of
 * mbedtls_platform_gmtime_r(). This replaces the default implementation in
 * platform_util.c.
 *
 * gmtime() is not a thread-safe function as defined in the C standard. The
 * library will try to use safer implementations of this function, such as
 * gmtime_r() when available. However, if Mbed TLS cannot identify the target
 * system, the implementation of mbedtls_platform_gmtime_r() will default to
 * using the standard gmtime(). In this case, calls from the library to
 * gmtime() will be guarded by the global mutex mbedtls_threading_gmtime_mutex
 * if MBEDTLS_THREADING_C is enabled. We recommend that calls from outside the
 * library are also guarded with this mutex to avoid race conditions. However,
 * if the macro MBEDTLS_PLATFORM_GMTIME_R_ALT is defined, Mbed TLS will
 * unconditionally use the implementation for mbedtls_platform_gmtime_r()
 * supplied at compile time.
 */
//#define MBEDTLS_PLATFORM_GMTIME_R_ALT

/**
 * Enable the verified implementations of ECDH primitives from Project Everest
 * (currently only Curve25519). This feature changes the layout of ECDH
 * contexts and therefore is a compatibility break for applications that access
 * fields of a mbedtls_ecdh_context structure directly. See also
 * MBEDTLS_ECDH_LEGACY_CONTEXT in include/mbedtls/ecdh.h.
 */
//#define MBEDTLS_ECDH_VARIANT_EVEREST_ENABLED

#include "mbedtls/check_config.h"

#endif /* MBEDTLS_CONFIG_H */
