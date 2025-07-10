/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 */

#include <stdbool.h>
#include <stdint.h>

#include <log.h>

#include <target/flash.h>
#include <private/rom_flash.h>

void stub_target_flash_init(void)
{
    STUB_LOG_TRACE();
    // TODO: avoid calling this flash_attach() if the chip has already been initialized
    esp_rom_spiflash_attach(0, false);
}

uint32_t stub_target_flash_get_flash_id(void)
{
    // TODO: remove dev tracing
    STUB_LOG_TRACEF("Uninit ROM's flash_id: 0x%x\n", stub_target_flash_get_config()->flash_id);

    esp_rom_spi_flash_update_id();
    STUB_LOG_TRACEF("Flash ID: 0x%x (from ROM code)\n", stub_target_flash_get_config()->flash_id);
    return stub_target_flash_get_config()->flash_id;
}

stub_lib_err_t stub_target_flash_read_buff(uint32_t addr, void *buffer, uint32_t size)
{
    // TODO: can read unaligned buf and size
    if (addr & 3 || size & 3) {
        STUB_LOGE("Unligned read: 0x%x, %u", addr, size);
        return STUB_LIB_ERR_FLASH_READ_UNALIGNED;
    }

    esp_rom_spiflash_result_t res = esp_rom_spiflash_read(addr, (uint32_t*)buffer, (int32_t)size);
    STUB_LOG_TRACEF("esp_rom_spiflash_read(0x%x, 0x%x, %u) results: %d\n", addr, (uint32_t)buffer, size, res);
    // TODO: a helper function
    switch (res) {
    case ESP_ROM_SPIFLASH_RESULT_OK:
        return STUB_LIB_OK;
    case ESP_ROM_SPIFLASH_RESULT_TIMEOUT:
        return STUB_LIB_ERR_FLASH_ROM_TIMEOUT;
    default:
        return STUB_LIB_ERR_FLASH_ROM_ERR;
    }
}
