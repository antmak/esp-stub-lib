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

typedef struct {
    uint8_t pp_addr_bit_len;
    uint8_t se_addr_bit_len;
    uint8_t be_addr_bit_len;
    uint8_t rd_addr_bit_len;
    uint32_t read_sub_len;
    uint32_t write_sub_len;
    void* unlock;
    void* erase_sector;
    void* erase_block;
    void* read;
    void* write;
    void* encrypt_write;
    void* check_sus;
    void* wren;
    void* wait_idle;
    void* erase_area; // TODO fix C3
} spiflash_legacy_funcs_t;

extern spiflash_legacy_funcs_t *rom_spiflash_legacy_funcs;

static void stub_target_flash_print_funcs(void)
{
    spiflash_legacy_funcs_t *funcs = rom_spiflash_legacy_funcs;
    (void)funcs;
    STUB_LOG_TRACEF("read_sub_len: %d, write_sub_len: %d, pp se be rd: %d %d %d %d\n"
        "unlock 0x%x, erase_sector 0x%x, erase_block 0x%x, erase_area 0x%x\n"
        "read 0x%x, write 0x%x, encrypt_write 0x%x\n"
        "check_sus 0x%x, wren 0x%x, wait_idle %x\n",
        funcs->read_sub_len, funcs->write_sub_len,
        funcs->pp_addr_bit_len, funcs->se_addr_bit_len, funcs->be_addr_bit_len, funcs->rd_addr_bit_len,
        funcs->unlock, funcs->erase_sector, funcs->erase_block, funcs->erase_area,
        funcs->read, funcs->write, funcs->encrypt_write,
        funcs->check_sus, funcs->wren, funcs->wait_idle);
}

static void stub_target_flash_init_funcs(void)
{
    static spiflash_legacy_funcs_t funcs = {
        .se_addr_bit_len = 24,
        .be_addr_bit_len = 24,
        .pp_addr_bit_len = 24,
        .rd_addr_bit_len = 24,
        .read_sub_len = 16,
        .write_sub_len = 32,
    };
    rom_spiflash_legacy_funcs = &funcs;
}

void stub_target_flash_init(void)
{
    STUB_LOG_TRACE();

    stub_target_flash_print_funcs();

    // TODO: avoid calling this flash_attach() if the chip has already been initialized

    // Do not call ets_efuse_get_spiconfig here because
    // it is called inside of esp_rom_spiflash_attach() when spiconfig=0

    uint32_t spiconfig = ets_efuse_get_spiconfig();
    esp_rom_spiflash_attach(spiconfig, false);
    // if _attach, no need "esp_rom_opiflash_pin_config"
    // else call esp_rom_opiflash_pin_config()

    // IDF: flash_init_state()
    // IDF: spi_flash_init_chip_state();

    if (ets_efuse_flash_octal_mode())
    {
        STUB_LOGD("octal mode is on\n");
        stub_target_flash_init_funcs();
    }

    stub_target_flash_print_funcs();
}

uint32_t stub_target_flash_get_flash_id(void)
{
    // TODO: remove dev tracing
    STUB_LOG_TRACEF("Uninit ROM's flash_id: 0x%x\n", stub_target_flash_get_config()->flash_id);

    esp_rom_spi_flash_update_id();
    STUB_LOG_TRACEF("Flash ID: 0x%x (from ROM code)\n", stub_target_flash_get_config()->flash_id);
    return stub_target_flash_get_config()->flash_id;

/* TODO ?
    if (ets_efuse_flash_octal_mode() && (ret & 0xFF) != ESP_FLASH_CHIP_MXIC_OCT) {
        STUB_LOGE("Unsupported octal flash manufacturer\n");
        return 0;
    }
*/
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
