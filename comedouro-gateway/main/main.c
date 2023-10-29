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

#include "user_nvs.h"
#include "user_pn532.h"
#include "user_wifi.h"
#include "user_httpd.h"
#include "task_pn532.h"

// >>>>>>>>>>>>>>>>>>>> ESP-IDF includes

#include "freertos/FreeRTOSConfig.h"

// >>>>>>>>>>>>>>>>>>>> libc includes

// >>>>>>>>>>>>>>>>>>>> Other includes

// >>>>>>>>>>>>>>>>>>>> Defines

#define MAIN_LOOP_DELAY_MS 1000 ///< Main task delay in miliseconds

// >>>>>>>>>>>>>>>>>>>> Global declarations

static const char *TAG = "MainModule";
static void main__init(void);
TaskHandle_t pn532_task_handle; ///< PN532 task handle

// >>>>>>>>>>>>>>>>>>>> Main

void app_main(void)
{
    ESP_LOGI(TAG, "Hello world! Initializing main modules...");
    main__init();
    ESP_LOGI(TAG, "Creating PN532 task");
    xTaskCreate(task__pn532, "task_pn532", 2500, NULL, 0, &pn532_task_handle);
}

// >>>>>>>>>>>>>>>>>>>> User-defined functions

/**
 * @brief Initialize necessary modules
 *
 */
static void main__init(void)
{
    esp_err_t err;
    err = nvs__init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing NVS");
    }
    err = pn532__init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing PN532 communication");
    }
    err = wifi__init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing Wi-Fi");
    }
    err = httpd__init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error initializing HTTP daemon");
    }
}