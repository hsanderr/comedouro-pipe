/**
 * @file user_tasks.c
 * @author Henrique Sander Lourenço
 * @brief Functions to control and interact with all the tasks.
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

#include "task_pn532.h"

// >>>>>>>>>>>>>>>>>>>> ESP-IDF includes

// >>>>>>>>>>>>>>>>>>>> libc includes

// >>>>>>>>>>>>>>>>>>>> Other includes

// >>>>>>>>>>>>>>>>>>>> Defines

// >>>>>>>>>>>>>>>>>>>> Global declarations

static char *TAG = "UserTasksModule";
TaskHandle_t *pn532_task_handle = NULL; ///< PN532 task handle

// >>>>>>>>>>>>>>>>>>>> User-defined functions

esp_err_t tasks__create_all(void)
{
	BaseType_t ret;
	ret = xTaskCreate(task__pn532, "task_pn532", 2500, NULL, 0, pn532_task_handle);
	if (ret != pdPASS)
	{
		ESP_LOGE(TAG, "Error creating PN532 task");
		return ESP_FAIL;
	}
	return ESP_OK;
}