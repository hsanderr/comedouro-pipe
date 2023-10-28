/**
 * @file task_pn532.c
 * @author Henrique Sander Lourenço
 * @brief PN532 task
 * @version 0.1
 * @date 2023-10-28
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

#include "user_pn532.h"

// >>>>>>>>>>>>>>>>>>>> ESP-IDF includes

// >>>>>>>>>>>>>>>>>>>> libc includes

// >>>>>>>>>>>>>>>>>>>> Other includes

// >>>>>>>>>>>>>>>>>>>> Defines

#define TASK_DELAY_MS 1000 ///< Task delay in miliseconds

// >>>>>>>>>>>>>>>>>>>> Global declarations

static const char *TAG = "pn532TaskModule";

// >>>>>>>>>>>>>>>>>>>> User-defined functions

/**
 * @brief Task to keep trying to read tags
 *
 * @param params
 */
void task__pn532(void *params)
{
	ESP_LOGI(TAG, "PN532 task begin");
	for (;;)
	{
		uint8_t uid[4] = {0};
		int8_t uid_len = 0;
		uid_len = pn532__read_uid(uid);
		if (uid_len == 4)
		{
			if (pn532__is_uid_auth(uid))
			{
				ESP_LOGI(TAG, "Opening feeder...");
			}
		}
		vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_MS));
	}
	vTaskDelete(NULL);
}