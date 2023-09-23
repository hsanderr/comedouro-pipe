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

//------------------ Includes Start //------------------

#include "sdkconfig.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_err.h"

//------------------ Includes End ------------------//

//------------------ Defines Start ------------------//

#define STORAGE_NAMESPACE "storage" ///< Namespace for NVS storage

//------------------ Includes End ------------------//

//------------------ Variables declarations Start ------------------//

static const char *TAG = "MainModule";

//------------------ Variables declarations End ------------------//

//------------------ Functions prototypes Start ------------------//

static void main__init(void);

//------------------ Functions prototypes End ------------------//

//------------------ Main Start ------------------//

void app_main(void)
{
  ESP_LOGI(TAG, "Olá mundo! Inicializando módulos principais...");
  main__init();
}

//------------------ Main End ------------------//

//------------------ Functions definitions Start ------------------//

/**
 * @brief Initialize basic modules.
 *
 */
void main__init(void)
{
  esp_err_t err;
  err = nvs_flash_init();
  if (err != ESP_OK)
  {
    ESP_LOGE(TAG, "Erro %d inicializando NVS: %s", err, esp_err_to_name(err));
  }
  else
  {
    ESP_LOGI(TAG, "Partição NVS inicializada com sucesso");
  }
}

//------------------ Functions definitions End ------------------//
