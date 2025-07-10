/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    ESP_ROM_SPIFLASH_RESULT_OK,
    ESP_ROM_SPIFLASH_RESULT_ERR,
    ESP_ROM_SPIFLASH_RESULT_TIMEOUT
} esp_rom_spiflash_result_t;

/**
  * @brief SPI Flash init. CONFIG SPI is unsupported
  *
  * TODO list chips with and w/o auto spi config
  *
  * @param spiconfig: Deprecated compatibility API value, must be 0
  *
  * @param legacy: Deprecated compatibility API value, must be false
  *
  */
extern void esp_rom_spiflash_attach(uint32_t spiconfig, bool legacy);

/**
  * @brief  Read spi flash pads configuration from Efuse
  *
  * @return
  * - 0 for default SPI pins.
  * - 1 for default HSPI pins.
  * - Other values define a custom pin configuration mask. Pins are encoded as per the EFUSE_SPICONFIG_RET_SPICLK,
  *   EFUSE_SPICONFIG_RET_SPIQ, EFUSE_SPICONFIG_RET_SPID, EFUSE_SPICONFIG_RET_SPICS0, EFUSE_SPICONFIG_RET_SPIHD macros.
  *   WP pin (for quad I/O modes) is not saved in efuse and not returned by this function.
  */
uint32_t ets_efuse_get_spiconfig(void);

/**
 * @brief Get SPI Flash GPIO pin configurations from eFuse
 *
 * @return uint32_t
 *          - 0: default SPI pins (ESP_ROM_EFUSE_FLASH_DEFAULT_SPI)
 *          - 1: default HSPI pins (ESP_ROM_EFUSE_FLASH_DEFAULT_HSPI)
 *          - Others: Customized pin configuration mask. Pins are encoded as per the
 *                    EFUSE_SPICONFIG_RET_SPICLK, EFUSE_SPICONFIG_RET_SPIQ, EFUSE_SPICONFIG_RET_SPID,
 *                    EFUSE_SPICONFIG_RET_SPICS0, EFUSE_SPICONFIG_RET_SPIHD macros.
 *
 * @note WP pin (for quad I/O modes) is not saved in eFuse and not returned by this function.
 */
/// TODO
///uint32_t esp_rom_efuse_get_flash_gpio_info(void);

/**
  * @brief Initialize flash_id from SPI_MEM_FLASH_RDID for ROM's flash config data
  *
  */
extern void esp_rom_spi_flash_update_id(void);

extern int esp_rom_spiflash_config_param(uint32_t flash_id, uint32_t chip_size,
                                         uint32_t block_size, uint32_t sector_size,
                                         uint32_t page_size, uint32_t status_mask);

/**
  * @brief Read Data from Flash, you should Erase it yourself if need.
  *        Please do not call this function in SDK.
  *
  * @param  uint32_t src_addr : Address to read, should be 4 bytes aligned.
  *
  * @param  uint32_t *dest : The buf to read the data.
  *
  * @param  uint32_t len : Length to read, should be 4 bytes aligned.
  *
  * @return ESP_ROM_SPIFLASH_RESULT_OK : Read OK.
  *         ESP_ROM_SPIFLASH_RESULT_ERR : Read error.
  *         ESP_ROM_SPIFLASH_RESULT_TIMEOUT : Read timeout.
  */
esp_rom_spiflash_result_t esp_rom_spiflash_read(uint32_t src_addr, uint32_t *dest, int32_t len);

/**
 * @brief Return true if EFuse indicates an opi flash is attached.
 */
bool ets_efuse_flash_octal_mode(void);
