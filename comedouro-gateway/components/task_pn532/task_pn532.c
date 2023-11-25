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
#include "user_pwm.h"

// >>>>>>>>>>>>>>>>>>>> ESP-IDF includes

// >>>>>>>>>>>>>>>>>>>> libc includes

// >>>>>>>>>>>>>>>>>>>> Other includes

// >>>>>>>>>>>>>>>>>>>> Defines

#define TASK_DELAY_MS 10		///< Task delay in miliseconds
#define MAX_READ_TAG_RETRIES 15 ///< Maximum number of retries to read tag after lid is open

// >>>>>>>>>>>>>>>>>>>> Global declarations

static const char *TAG = "pn532TaskModule";
uint8_t lid_open = 0;
/* count number of times that no tag or no authorized tag
is found only when open is lid, indicated by lid_open flag */
uint8_t no_tag_found_sequence = 0;

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
		ESP_LOGI(
			TAG,
			"Inside PN532 task, lid_open=%d, no_tag_found_sequence=%d, "
			"unused words=%d",
			(int)lid_open,
			(int)no_tag_found_sequence,
			(int)uxTaskGetStackHighWaterMark(NULL));
		uint8_t uid[4] = {0};
		int8_t uid_len = 0;
		uid_len = pn532__read_uid(uid);
		if (uid_len == -1 && lid_open)
		{ /** this means that no tag was found */
			no_tag_found_sequence++;
			if (no_tag_found_sequence > MAX_READ_TAG_RETRIES)
			{
				ESP_LOGI(TAG, "Max number of read tag retries exceeded, closing lid...");
				if (pwm__set_duty_min() != ESP_OK)
				{
					ESP_LOGE(TAG, "Error closing lid");
				}
				lid_open = 0;
			}
		}
		else if (uid_len == 4)
		{
			if (pn532__is_uid_auth(uid))
			{
				no_tag_found_sequence = 0;
				if (!lid_open)
				{
					ESP_LOGI(TAG, "Opening lid...");
					if (pwm__set_duty_max() != ESP_OK)
					{
						ESP_LOGE(TAG, "Error opening lid");
					}
					lid_open = 1;
				}
				else
				{
					ESP_LOGI(TAG, "Lid is already open");
				}
			}
			else if (lid_open)
			{
				ESP_LOGI(TAG, "Unauthorized tag found and lid is open, closing lid...");
				ESP_LOGI(TAG, "Opening lid...");
				if (pwm__set_duty_min() != ESP_OK)
				{
					ESP_LOGE(TAG, "Error closing lid");
				}
				lid_open = 0;
			}
		}
		vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_MS));
	}
	vTaskDelete(NULL);
}