/**
 * @file app_nvs.h
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
#include "nvs_flash.h"
#include "esp_err.h"

//------------------ 3rd-part Includes End ------------------//

//------------------ App Includes Start ------------------//

#include "app_nvs.h"

//------------------ 3rd-part Includes End ------------------//

//------------------ Defines Start ------------------//

#define STORAGE_NAMESPACE "storage" ///< Namespace for NVS storage

//------------------ Includes End ------------------//

//------------------ Variables declarations Start ------------------//

static const char *TAG = "NvsModule";

//------------------ Variables declarations End ------------------//

//------------------ Functions prototypes Start ------------------//

esp_err_t nvs__init(void);

//------------------ Functions prototypes End ------------------//

//------------------ Functions definitions Start ------------------//

esp_err_t nvs__init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro %d inicializando NVS: %s", err, esp_err_to_name(err));
    }
    else
    {
        ESP_LOGI(TAG, "Partição NVS inicializada com sucesso");
    }
    return err;
}

void nvs__read(void *var, char *key, nvs_var_type_t var_type)
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
    default:
        break;
    }
}

void nvs__write(void *var, char *key, nvs_var_type_t var_type)
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
            err = nvs_set_i8(my_handle, key, *((int8_t *)var));
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
    default:
        break;
    }
}

//------------------ Functions definitions End ------------------//
