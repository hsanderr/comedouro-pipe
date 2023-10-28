/**
 * @file user_pn532.c
 * @author Henrique Sander Lourenço
 * @brief Functions to interact with PN532 module.
 * @version 0.1
 * @date 2023-10-15
 *
 * @copyright Copyright 2023 Henrique Sander Lourenço

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *
 */

// >>>>>>>>>>>>>>>>>>>> Common includes

#include "sdkconfig.h"
#define LOG_LOCAL_LEVEL ESP_LOG_INFO // set log level (see esp_log_level_t)
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// >>>>>>>>>>>>>>>>>>>> App includes

#include "pn532.h"

// >>>>>>>>>>>>>>>>>>>> ESP-IDF includes

// >>>>>>>>>>>>>>>>>>>> libc includes

// >>>>>>>>>>>>>>>>>>>> Other includes

// >>>>>>>>>>>>>>>>>>>> Defines

#define PN532_CLK 18  ///< PN532 clock GPIO number
#define PN532_MOSI 23 ///< PN532 MOSI GPIO number
#define PN532_CS 5	  ///< PN532 chip select GPIO number
#define PN532_MISO 19 ///< PN532 MISO GPIO number

// >>>>>>>>>>>>>>>>>>>> Global declarations

static char *TAG = "UserPn532Module";
pn532_t nfc;

// >>>>>>>>>>>>>>>>>>>> User-defined functions

/**
 * @brief Initialize PN532 communication: configure GPIOS,
 * send command to wake-up, get firmware version and configure SAM
 *
 * @return esp_err_t
 * @retval ESP_OK if PN532 communication is initialized successfully
 * @retval ESP_FAIL if PN532 communication initialization fails
 */
esp_err_t pn532__init(void)
{
	uint32_t fw_ver_ret; ///< store pn532_getFirmwareVersion() response
	pn532_spi_init(&nfc, PN532_CLK, PN532_MISO, PN532_MOSI, PN532_CS);
	pn532_begin(&nfc);
	fw_ver_ret = pn532_getFirmwareVersion(&nfc);
	if (!fw_ver_ret)
	{
		ESP_LOGE(TAG, "Failed to get PN532 firmware version");
		return ESP_FAIL;
	}
	if (!pn532_SAMConfig(&nfc))
	{
		ESP_LOGE(TAG, "Failed to configure SAM");
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "Found chip PN5%2.2x", (unsigned int)((fw_ver_ret >> 24) & 0xff));
	ESP_LOGI(TAG, "Firmware version: %d,  firmware revision: %d",
			 (int)((fw_ver_ret >> 16) & 0xff), (int)((fw_ver_ret >> 8) & 0xff));
	return ESP_OK;
}

/**
 * @brief Read ISO14443-3A card/tag UID
 *
 * @param uid Pointer to uint8 array with minimum length of 7 bytes
 * where UID will be stored
 * @return int8_t
 * @retval ESP_FAIL (-1) if no card/tag is detected
 * @retval UID length if a card/tag is detected
 */
int8_t pn532__read_uid(uint8_t uid[])
{
	ESP_LOGD(TAG, "Checking for ISO14443-3A card or tag...");
	uint8_t len; // store length of the UID read (4 or 7 bytes depending on ISO14443A card type)

	if (pn532_readPassiveTargetID(&nfc, PN532_MIFARE_ISO14443A, uid, &len, 1000))
	{
		// Display card/tag information
		ESP_LOGI(TAG, "Found an ISO14443A card or tag");
		ESP_LOGI(TAG, "UID Length: %d bytes", len);
		if (len == 4)
		{
			ESP_LOGI(TAG, "UID Value: %2.2x:%2.2x:%2.2x:%2.2x",
					 uid[0], uid[1], uid[2], uid[3]);
		}
		else if (len == 7)
		{
			ESP_LOGI(TAG, "UID Value: %2.2x:%2.2x:%2.2x:%2.2x:"
						  "%2.2x:%2.2x:%2.2x",
					 uid[0], uid[1], uid[2], uid[3], uid[4], uid[5], uid[6]);
		}
		else
		{
			ESP_LOGW(TAG, "Unexpected UID length");
		}
		return len;
	}
	else
	{
		// Timed out waiting for a card/tag
		ESP_LOGD(TAG, "Timed out waiting for a card");
		return ESP_FAIL;
	}
}