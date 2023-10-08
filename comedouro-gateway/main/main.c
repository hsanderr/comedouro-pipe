/**
 * @file main.c
 * @author Henrique Sander Lourenço
 * @brief
 * @version 0.1
 * @date 2023-09-23
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
 *
 */

//------------------ 3rd party Includes Start ------------------//

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//------------------ 3rd party Includes End ------------------//

//------------------ App Includes Start ------------------//

#include "global_prm.h"
#include "app_nvs.h"
#include "app_spi.h"
#include "app_pn532.h"

//------------------ 3rd party Includes End ------------------//

//------------------ Defines Start ------------------//

#define STORAGE_NAMESPACE "storage" ///< Namespace for NVS storage

//------------------ Defines End ------------------//

//------------------ Variables declarations Start ------------------//

static const char *TAG = "MainModule";
static pn532_t nfc;

//------------------ Variables declarations End ------------------//

//------------------ Functions prototypes Start ------------------//

static void main__init(void);

//------------------ Functions prototypes End ------------------//

//------------------ Main Start ------------------//

void app_main(void)
{
    ESP_LOGI(TAG, "Hello world! Initializing modules...");
    main__init();
}

//------------------ Main End ------------------//

//------------------ Functions definitions Start ------------------//

/**
 * @brief Initialize basic modules.
 *
 */
static void main__init(void)
{
    esp_err_t err;
    err = nvs__init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %d initializing NVS", err);
    }
    pn532__spi_init(&nfc, PN532_CLK, PN532_MISO, PN532_MOSI, PN532_CS);
    pn532__wakeup(&nfc);
    uint8_t ic_ver, fw_ver, fw_rev;
    err = pn532__get_fw_ver(&nfc, &ic_ver, &fw_ver, &fw_rev);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Didn't find PN53x board");
    }
    ESP_LOGI(TAG, "Found chip PN5%2.2x", ic_ver);
    ESP_LOGI(TAG, "Firmware version: %d,  firmware revision: %d", (int)fw_ver, (int)fw_rev);
}

//------------------ Functions definitions End ------------------//