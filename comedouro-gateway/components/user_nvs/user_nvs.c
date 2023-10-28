/**
 * @file user_nvs.c
 * @author Henrique Sander Lourenço
 * @brief Functions to interact with NVS.
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

// >>>>>>>>>>>>>>>>>>>> ESP-IDF includes

#include "nvs_flash.h"

// >>>>>>>>>>>>>>>>>>>> libc includes

// >>>>>>>>>>>>>>>>>>>> Other includes

// >>>>>>>>>>>>>>>>>>>> Defines

#define STORAGE_NAMESPACE "storage" ///< Namespace for NVS storage
#define UID_ENTRY_KEY "uid"

// >>>>>>>>>>>>>>>>>>>> Global declarations

static const char *TAG = "NvsModule";

// >>>>>>>>>>>>>>>>>>>> User-defined functions

/**
 * @brief Initialize NVS module
 *
 * @return esp_err_t
 * @retval ESP_OK if NVS initialization is successful
 * @retval ESP_FAIL if NVS initialization fails
 */
esp_err_t nvs__init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %d initializing NVS: %s", err, esp_err_to_name(err));
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "NVS partition successfully initialized!");
    return ESP_OK;
}

/**
 * @brief Read NVS entry
 *
 * @param var Pointer to variable where the value will be stored
 * @param key Key to be read
 * @param var_type Type of variable
 * @param len Length of variableif var_type is nvs_str
 */
void nvs__read(void *var, char *key, nvs_entry_type_t var_type, size_t len)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    switch (var_type)
    {
    case nvs_int8:
        ESP_LOGI(TAG, "Reading int8 from NVS");
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error %d opening NVS: %s", err, esp_err_to_name(err));
        }
        else
        {
            ESP_LOGI(TAG, "Success opening NVS");
            err = nvs_get_i8(my_handle, key, var);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Error %d reading from NVS: %s", err, esp_err_to_name(err));
            }
            else
            {
                ESP_LOGI(TAG, "Success reading from NVS");
            }
        }
        break;
    case nvs_uint8:
        ESP_LOGI(TAG, "Reading uint8 from NVS");
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error %d opening NVS: %s", err, esp_err_to_name(err));
        }
        else
        {
            ESP_LOGI(TAG, "Success opening NVS");
            err = nvs_get_u8(my_handle, key, var);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Error %d reading from NVS: %s", err, esp_err_to_name(err));
            }
            else
            {
                ESP_LOGI(TAG, "Success reading from NVS");
            }
        }
    case nvs_str:
        ESP_LOGI(TAG, "Reading str from NVS");
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error %d opening NVS: %s", err, esp_err_to_name(err));
        }
        else
        {
            ESP_LOGI(TAG, "Success opening NVS");
            err = nvs_get_str(my_handle, key, var, &len);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Error %d reading from NVS: %s", err, esp_err_to_name(err));
            }
            else
            {
                ESP_LOGI(TAG, "Success reading from NVS, len=%d", (int)len);
            }
        }
    default:
        break;
    }
}

/**
 * @brief Write to NVS entry
 *
 * @param var Variable to be written
 * @param key Key to be written
 * @param var_type Type of variable
 */
void nvs__write(void *var, char *key, nvs_entry_type_t var_type)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    switch (var_type)
    {
    case nvs_int8:
        ESP_LOGI(TAG, "Writing int8 to NVS");
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error %d opening NVS: %s", err, esp_err_to_name(err));
        }
        else
        {
            ESP_LOGI(TAG, "Success opening NVS");
            err = nvs_set_u8(my_handle, key, *((int8_t *)var));
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Error %d writing to NVS: %s", err, esp_err_to_name(err));
            }
            else
            {
                ESP_LOGI(TAG, "Success writing to NVS");
            }
        }
        break;
    case nvs_uint8:
        ESP_LOGI(TAG, "Writing uint8 to NVS");
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error %d opening NVS: %s", err, esp_err_to_name(err));
        }
        else
        {
            ESP_LOGI(TAG, "Success opening NVS");
            err = nvs_set_u8(my_handle, key, *((uint8_t *)var));
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Error %d writing to NVS: %s", err, esp_err_to_name(err));
            }
            else
            {
                ESP_LOGI(TAG, "Success writing to NVS");
            }
        }
    case nvs_str:
        ESP_LOGI(TAG, "Writing str to NVS");
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error %d opening NVS: %s", err, esp_err_to_name(err));
        }
        else
        {
            ESP_LOGI(TAG, "Success opening NVS");
            err = nvs_set_str(my_handle, key, (char *)&var[0]);
            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Error %d writing to NVS: %s", err, esp_err_to_name(err));
            }
            else
            {
                ESP_LOGI(TAG, "Success writing to NVS");
            }
        }
    default:
        break;
    }
}

void nvs__write_uid(char *uid)
{
    nvs__write(uid, UID_ENTRY_KEY, nvs_str);
}

void nvs__read_uid(char *uid)
{
    size_t len = 9;
    nvs__read(uid, UID_ENTRY_KEY, nvs_str, len);
}

//------------------ Functions definitions End ------------------//
