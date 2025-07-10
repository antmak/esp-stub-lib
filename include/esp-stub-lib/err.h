/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 */

#pragma once

typedef int stub_lib_err_t;

#define STUB_LIB_OK         0       // stub_lib_err_t value indicating success (no error)
#define STUB_LIB_FAIL       -1      // Generic stub_lib_err_t code indicating failure

#define STUB_LIB_ERR_FLASH_BASE             (0x100)
#define STUB_LIB_ERR_FLASH_ROM_ERR          (STUB_LIB_ERR_FLASH_BASE + 0x01)
#define STUB_LIB_ERR_FLASH_ROM_TIMEOUT      (STUB_LIB_ERR_FLASH_BASE + 0x02)
#define STUB_LIB_ERR_FLASH_READ_UNALIGNED   (STUB_LIB_ERR_FLASH_BASE + 0x03)

// Provided a wide range for STUB_LIB_ERR_FLASH_* codes, would start the next BASE from 0x300
