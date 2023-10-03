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

//------------------ 3rd-part Includes Start ------------------//

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//------------------ 3rd-part Includes End ------------------//

//------------------ App Includes Start ------------------//

#include "app_nvs.h"
#include "app_spi.h"
#include "pn532.h"

#define PN532_SCK 18
#define PN532_MOSI 23
#define PN532_SS 5
#define PN532_MISO 19
// #define PN532_SCK 32
// #define PN532_MOSI 26
// #define PN532_SS 25
// #define PN532_MISO 33

//------------------ 3rd-part Includes End ------------------//

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
    // err = spi__init();
    // if (err != ESP_OK)
    // {
    //     ESP_LOGE(TAG, "Error %d initializing SPI", err);
    // }
    pn532_spi_init(&nfc, PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
    pn532_begin(&nfc);
    uint32_t versiondata = pn532_getFirmwareVersion(&nfc);
    if (!versiondata)
    {
        ESP_LOGI(TAG, "Didn't find PN53x board");
        while (1)
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
    // Got ok data, print it out!
    ESP_LOGI(TAG, "Found chip PN5 %x", (unsigned int)((versiondata >> 24) & 0xFF));
    ESP_LOGI(TAG, "Firmware ver. %d.%d", (int)((versiondata >> 16) & 0xFF), (int)((versiondata >> 8) & 0xFF));
}

//------------------ Functions definitions End ------------------//