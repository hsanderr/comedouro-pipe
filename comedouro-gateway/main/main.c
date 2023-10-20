/**
 * @file main.c
 * @author Henrique Sander Lourenço
 * @brief Main file for the Proximity-Triggered Pet Feeder (PTPF) ESP32 firmware.
 * @version 0.1
 * @date 2023-09-23
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

#include "global_prm.h"
#include "app_nvs.h"
#include "app_pn532.h"
#include "app_pn532_hl.h"
#include "app_web_server.h"

// >>>>>>>>>>>>>>>>>>>> Definitions

#define MAIN_LOOP_DELAY_MS 10 ///< Main task delay in miliseconds

// >>>>>>>>>>>>>>>>>>>> Global declarations

static const char *TAG = "MainModule";
static void main__init(void);

// >>>>>>>>>>>>>>>>>>>> Main

void app_main(void)
{
    ESP_LOGI(TAG, "Hello world! Initializing modules...");
    main__init();
    while (1)
    {
        uint8_t uid[10] = {0};
        int8_t len = pn532hl__read_uid(uid);
        if (len == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Could not read UID");
        }
        if (uid[0] == 0x50 && uid[1] == 0x6c && uid[2] == 0x93 && uid[3] == 0x1e)
        {
            ESP_LOGI(TAG, "Authorized tag found! Opening lid...");
        }
        else
        {
            ESP_LOGW(TAG, "Unauthorized tag found!");
        }
        vTaskDelay(pdMS_TO_TICKS(MAIN_LOOP_DELAY_MS));
    }
}

// >>>>>>>>>>>>>>>>>>>> User-defined functions

/**
 * @brief Initialize necessary modules.
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
    err = pn532hl__init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %d initializing PN532", err);
    }
    webserver__init();
}