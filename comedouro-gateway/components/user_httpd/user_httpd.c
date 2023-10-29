/**
 * @file file_name.c
 * @author Henrique Sander Lourenço
 * @brief Functions for setting up and configuring an HTTP daemon on ESP32.
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

#include "main_get_page.h"
#include "main_post_page.h"
#include "user_utils.h"
#include "user_nvs.h"
#include "user_pn532.h"

// >>>>>>>>>>>>>>>>>>>> ESP-IDF includes

#include "esp_http_server.h"

// >>>>>>>>>>>>>>>>>>>> libc includes

// >>>>>>>>>>>>>>>>>>>> Other includes

// >>>>>>>>>>>>>>>>>>>> Defines

// >>>>>>>>>>>>>>>>>>>> Global declarations

static char *TAG = "HttpdModule";
static char home_page_html[] = MAIN_GET_PAGE;			 ///< home page HTML
static char form_submission_res_html[] = MAIN_POST_PAGE; ///< form submission response HTML

// >>>>>>>>>>>>>>>>>>>> User-defined functions.

/**
 * @brief GET / response
 *
 * @param req HTTP request data
 * @return esp_err_t
 */
static esp_err_t httpd__get_main(httpd_req_t *req)
{
	if (httpd_resp_send(req, home_page_html, HTTPD_RESP_USE_STRLEN) != ESP_OK)
		return ESP_FAIL;
	else
		return ESP_OK;
}

/**
 * @brief POST / response
 *
 * @param req HTTP request data
 * @return esp_err_t
 */
static esp_err_t httpd__post_main(httpd_req_t *req)
{
	char content[13] = {0}; // length(uid=xxxxxxxx) + 1 for terminating character
	int ret = httpd_req_recv(req, content, MIN(req->content_len, sizeof(content)));
	if (ret <= 0) // 0 indicates connection closed
	{
		// Check if timeout occurred
		if (ret == HTTPD_SOCK_ERR_TIMEOUT)
		{
			/* In case of timeout one can choose to retry calling
			 * httpd_req_recv(), but to keep it simple, here we
			 * respond with an HTTP 408 (Request Timeout) error */
			ESP_LOGE(TAG, "Timeout receiving POST request");
			httpd_resp_send_408(req);
		}

		ESP_LOGE(TAG, "Error receiving POST request");

		/* In case of error, returning ESP_FAIL will
		 * ensure that the underlying socket is closed */
		return ESP_FAIL;
	}
	httpd_resp_send(req, form_submission_res_html, HTTPD_RESP_USE_STRLEN);
	ESP_LOGI(TAG, "Content: %s", content);

	char uid[9] = {0};
	for (uint8_t i = 0; i < 8; i++)
	{
		uid[i] = content[i + 4];
	}

	ESP_LOGI(TAG, "UID that will be written: %s", uid);
	nvs__write_uid(uid);
	pn532__authorize_uid_str(uid);

	return ESP_OK;
}

/**
 * @brief Initialize HTTP daemon
 *
 * @return esp_err_t
 * @retval ESP_OK if HTTP daemon is correctly initialized
 * @retval ESP_FAIL if HTTP daemon initialization fails
 */
esp_err_t httpd__init(void)
{
	httpd_handle_t httpd_handle = {0};					  // HTTP daemon handler
	httpd_config_t httpd_config = HTTPD_DEFAULT_CONFIG(); // HTTP daemon configuration
	esp_err_t err;
	err = httpd_start(&httpd_handle, &httpd_config);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error %s starting HTTP daemon", esp_err_to_name(err));
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "Success starting HTTP daemon");

	// GET / URI
	httpd_uri_t get_main = {
		.uri = "/",
		.method = HTTP_GET,
		.handler = httpd__get_main,
		.user_ctx = NULL,
	};

	// POST / URI
	httpd_uri_t post_main = {
		.uri = "/",
		.method = HTTP_POST,
		.handler = httpd__post_main,
		.user_ctx = NULL,
	};

	// register URI handlers
	err = httpd_register_uri_handler(httpd_handle, &get_main);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error %s registering URI handler", esp_err_to_name(err));
		return ESP_FAIL;
	}
	err = httpd_register_uri_handler(httpd_handle, &post_main);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error %s registering URI handler", esp_err_to_name(err));
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "Success registering URI handlers");

	return ESP_OK;
}