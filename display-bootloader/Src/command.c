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

#include "command.h"
#include "flash_if.h"
#include "main.h"
#include "memory_card.h"
#include "third_party.h"

#include "mbedtls/bignum.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/sha256.h"

struct firmware_file_header {
  uint8_t sig_r[32];
  uint8_t sig_s[32];
  uint8_t sha256[32];
} __attribute__((packed));

#define CURVE_COORD_LEN 32
struct fw_upd_pk {
  // just for information: typedef uint32_t mbedtls_mpi_uint
  mbedtls_mpi_uint x[CURVE_COORD_LEN / sizeof(mbedtls_mpi_uint)];
  mbedtls_mpi_uint y[CURVE_COORD_LEN / sizeof(mbedtls_mpi_uint)];
};

#define BYTES_TO_T_UINT_4(a, b, c, d)                                          \
  ((mbedtls_mpi_uint)(a) << 0) | ((mbedtls_mpi_uint)(b) << 8) |                \
      ((mbedtls_mpi_uint)(c) << 16) | ((mbedtls_mpi_uint)(d) << 24)

#define BYTES_TO_T_UINT_2(a, b) BYTES_TO_T_UINT_4(a, b, 0, 0)

#define BYTES_TO_T_UINT_8(a, b, c, d, e, f, g, h)                              \
  BYTES_TO_T_UINT_4(a, b, c, d), BYTES_TO_T_UINT_4(e, f, g, h)

static struct fw_upd_pk fw_upd_pk = {
#include "fw_update_sig_pk.c_inc"
};

#define BUFFERSIZE ((uint16_t)1 * 8192)

uint8_t RAMBuf[BUFFERSIZE] = {0x00};

void COMMAND_JUMP(void) {
  /* Software reset */
  NVIC_SystemReset();
}

/**
 * This commands loads the image from the provided file, validates its signature
 * and calls the callback while reading  the file. The file should contain a
 * header containing the signature, followed by the data to be flashed.
 * @param f pointer to file handle of image file
 * @param data_callback is called while reading the file to further process the
 * data from the image.
 */
uint32_t COMMAND_ProgramFlashMemory(FIL *f,
                                    COMMAND_dataCallback data_callback) {
  __IO uint32_t read_size = 0x00, tmp_read_size = 0x00;
  uint32_t read_flag = TRUE;

  uint32_t offset = 0;

  mbedtls_mpi_uint one[] = {1};
  mbedtls_ecp_point pk_point = {
      .X = {.s = 1,
            .n = CURVE_COORD_LEN / sizeof(mbedtls_mpi_uint),
            .p = fw_upd_pk.x},

      .Y = {.s = 1,
            .n = CURVE_COORD_LEN / sizeof(mbedtls_mpi_uint),
            .p = fw_upd_pk.y},

      .Z = {.s = 1, .n = 1, .p = one}};

  mbedtls_mpi_uint r_uint[CURVE_COORD_LEN / sizeof(mbedtls_mpi_uint)];
  mbedtls_mpi sig_r = {
      .s = 1, .n = CURVE_COORD_LEN / sizeof(mbedtls_mpi_uint), .p = r_uint};

  mbedtls_mpi_uint s_uint[CURVE_COORD_LEN / sizeof(mbedtls_mpi_uint)];
  mbedtls_mpi sig_s = {
      .s = 1, .n = CURVE_COORD_LEN / sizeof(mbedtls_mpi_uint), .p = s_uint};

  struct firmware_file_header header;

  if (f_read(f, &header, sizeof(header), (uint_t *)&read_size) != FR_OK)
    return DOWNLOAD_FILE_FAIL;

  if (read_size != sizeof(header))
    return DOWNLOAD_FILE_FAIL;

  /* fix signature coordinate endianness */
  if (mbedtls_mpi_read_binary(&sig_r, header.sig_r, sizeof(header.sig_r)))
    return DOWNLOAD_FILE_FAIL;

  if (mbedtls_mpi_read_binary(&sig_s, header.sig_s, sizeof(header.sig_s)))
    return DOWNLOAD_FILE_FAIL;

  mbedtls_ecp_group grp;
  mbedtls_ecp_group_init(&grp);

  if (mbedtls_ecp_group_load(&grp, MBEDTLS_ECP_DP_SECP256R1)) {
    mbedtls_ecp_group_free(&grp);
    return DOWNLOAD_FAIL;
  }

  if (!ThirdParty_Allowed() && mbedtls_ecdsa_verify(&grp, header.sha256, sizeof(header.sha256),
                           &pk_point, &sig_r, &sig_s)) {
    mbedtls_ecp_group_free(&grp);
    return DOWNLOAD_SIGNATURE_FAIL;
  }

  mbedtls_ecp_group_free(&grp);

  uint8_t calculated_sha256[32];
  mbedtls_sha256_context sha256_context;

  mbedtls_sha256_init(&sha256_context);
  mbedtls_sha256_starts(&sha256_context, 0); /* SHA-256, not 224 */

  /* While file still contain data */
  while (read_flag == TRUE) {

    if (f_read(f, RAMBuf, BUFFERSIZE, (uint_t *)&read_size) != FR_OK)
      return DOWNLOAD_FILE_FAIL;

    tmp_read_size = read_size;

    if (tmp_read_size < BUFFERSIZE)
      read_flag = FALSE;

    mbedtls_sha256_update(&sha256_context, RAMBuf, read_size);

    if (data_callback) {
      int rc = data_callback(RAMBuf, offset, tmp_read_size);

      if (rc != DOWNLOAD_OK) {
        return rc;
      }
    }

    offset += tmp_read_size;
  }

  mbedtls_sha256_finish(&sha256_context, calculated_sha256);
  mbedtls_sha256_free(&sha256_context);

  if (memcmp(calculated_sha256, header.sha256, sizeof(calculated_sha256))) {
    return DOWNLOAD_SIGNATURE_FAIL;
  }

  return DOWNLOAD_OK;
}

/**
 * @}
 */

/*******************(C)COPYRIGHT STMicroelectronics *****END OF FILE******/
