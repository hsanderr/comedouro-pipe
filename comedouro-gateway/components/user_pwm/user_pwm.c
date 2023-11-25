/**
 * @file user_pwm.c
 * @author Henrique Sander Lourenço
 * @brief PWM functions.
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

#include "user_pwm.h"

// >>>>>>>>>>>>>>>>>>>> ESP-IDF includes

// #include "driver/gpio.h"
#include "driver/ledc.h"

// >>>>>>>>>>>>>>>>>>>> libc includes

// >>>>>>>>>>>>>>>>>>>> Other includes

// >>>>>>>>>>>>>>>>>>>> Defines

// >>>>>>>>>>>>>>>>>>>> Global declarations

static const char *TAG = "pwmModule";

// >>>>>>>>>>>>>>>>>>>> User-defined functions

esp_err_t pwm__init(void)
{
	esp_err_t err;
	ledc_timer_config_t pwm_timer_config = {
		.speed_mode = LEDC_LOW_SPEED_MODE,
		.duty_resolution = LEDC_TIMER_20_BIT,
		.timer_num = LEDC_TIMER_0,
		.freq_hz = 50,
		.clk_cfg = LEDC_AUTO_CLK,
	};
	ledc_channel_config_t pwm_channel_config = {
		.gpio_num = 2,
		.speed_mode = LEDC_LOW_SPEED_MODE,
		.channel = LEDC_CHANNEL_0,
		.intr_type = LEDC_INTR_DISABLE,
		.timer_sel = LEDC_TIMER_0,
		.duty = 0,
		.hpoint = 0};
	err = ledc_timer_config(&pwm_timer_config);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error configuring PWM timer");
		return ESP_FAIL;
	}
	err = ledc_channel_config(&pwm_channel_config);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error configuring PWM channel");
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "Setting duty cycle to 0.5 ms...");
	return pwm__set_duty_min();
}

esp_err_t pwm__set_duty_min(void)
{
	esp_err_t err = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 26214); // 0.5 ms (min)
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error setting PWM duty cycle");
		return ESP_FAIL;
	}
	err = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error updating PWM duty cycle");
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "Duty cycle set to minimum");
	return ESP_OK;
}

esp_err_t pwm__set_duty_max(void)
{
	esp_err_t err = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 131072); // 0.5 ms (min)
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error setting PWM duty cycle");
		return ESP_FAIL;
	}
	err = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error updating PWM duty cycle");
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "Duty cycle set to maximum");
	return ESP_OK;
}