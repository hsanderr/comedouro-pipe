/**
 * @file app_pn532_hl.c
 * @author Henrique Sander Lourenço
 * @brief
 * @version 0.1
 * @date 2023-10-15
 *
 * @copyright    Copyright 2023 Henrique Sander Lourenço

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

//------------------ 3rd party Includes Start ------------------//

#include "sdkconfig.h"
#define LOG_LOCAL_LEVEL ESP_LOG_INFO // set log level
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//------------------ 3rd party Includes End ------------------//

//------------------ Typedefs Start ------------------//

//------------------ Typedefs End ------------------//

//------------------ App Includes Start ------------------//

#include "global_prm.h"
#include "app_pn532.h"

//------------------ App Includes End ------------------//

//------------------ Defines Start ------------------//

//------------------ Defines End ------------------//

//------------------ Variables declarations Start ------------------//

static char *TAG = "Pn532HlModule";
pn532_t nfc = {0};

//------------------ Variables declarations End ------------------//

//------------------ Functions prototypes Start ------------------//

//------------------ Functions prototypes End ------------------//

//------------------ Functions declarations Start ------------------//

esp_err_t pn532hl__init(void)
{
  esp_err_t err;
  uint8_t ic_ver, fw_ver, fw_rev;
  pn532__spi_init(&nfc, PN532_CLK, PN532_MISO, PN532_MOSI, PN532_CS);
  pn532__wakeup(&nfc);
  err = pn532__get_fw_ver(&nfc, &ic_ver, &fw_ver, &fw_rev);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Didn't find PN53x board");
    return ESP_FAIL;
  }
  err = pn532__sam_config(&nfc);
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to configure SAM");
  }
  ESP_LOGI(TAG, "Found chip PN5%2.2x", ic_ver);
  ESP_LOGI(TAG, "Firmware version: %d,  firmware revision: %d", (int)fw_ver, (int)fw_rev);
  return ESP_OK;
}

int8_t pn532hl__read_uid(uint8_t uid[])
{
  ESP_LOGD(TAG, "Checking for ISO14443-3A card or tag...");
  uint8_t success;
  uint8_t len; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and len will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = pn532_readPassiveTargetID(&nfc, PN532_MIFARE_ISO14443A, uid, &len, 1000);

  if (success)
  {
    // Display some basic information about the card
    ESP_LOGD(TAG, "Found an ISO14443A card or tag");
    ESP_LOGD(TAG, "UID Length: %d bytes", len);
    // ESP_LOGD(TAG, "UID Value:");
    // esp_log_buffer_hexdump_internal(TAG, uid, len, ESP_LOG_INFO);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    return len;
  }
  else
  {
    // PN532 probably timed out waiting for a card
    ESP_LOGD(TAG, "Timed out waiting for a card");
    return ESP_FAIL;
  }
}

//------------------ Functions declarations End ------------------//