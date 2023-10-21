/**
 * @file user_wifi.c
 * @author Henrique Sander Lourenço
 * @brief Functions related to Wi-Fi.
 * @version 0.1
 * @date 2023-10-21
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

// >>>>>>>>>>>>>>>>>>>> ESP-IDF includes

#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_mac.h"

// >>>>>>>>>>>>>>>>>>>> libc includes

#include "string.h"

// >>>>>>>>>>>>>>>>>>>> Other includes

// >>>>>>>>>>>>>>>>>>>> Defines

#define ESP_WIFI_AP_SSID "PetDog_"			///< AP SSID
#define ESP_WIFI_AP_PWD "senha123"			///< AP password
#define ESP_WIFI_AP_CHANNEL 1				///< AP channel
#define ESP_WIFI_MAX_CONN_TO_AP 1			///< Maximum number of connections to AP
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y)) ///< Calculate minimum

// >>>>>>>>>>>>>>>>>>>> Global declarations

static char *TAG = "WifiModule";
// static char main_web_page_html[] = "<!DOCTYPE html><html lang=\"pt-BR\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Comedouro Automático PetDog</title><style>body {background-color: goldenrod;color: midnightblue;padding: 10px;font-family: 'Trebuchet MS', monospace;font-size: 1.5rem;text-align: center;}input {margin-top: 10px;margin-bottom: 10px;}input,button {font-size: 1.2rem;padding: 5px;text-align: center;}footer {margin-top: 30px;}.msg-box {display: none;background-color: brown;color: #eee;padding: 20px;margin: 10px 30px;}</style></head><body><h1>Configure seu Comedouro Automático PetDog!</h1><div class=\"msg-box\" id=\"msgBox\"></div><form action=\"/\" method=\"POST\" id=\"uidForm\"><label for=\"uid\">Insira aqui o código de identificação (UID) da coleira do seu pet</label><br><input type=\"text\" name=\"uid\" id=\"uid\" placeholder=\"506c931e\"><br><button type=\"button\" id=\"formBtn\">Enviar</button></form><footer>&copy; 2023 Henrique Sander Lourenço</footer><script>form = document.getElementById('uidForm');uid = document.getElementById('uid');btn = document.getElementById('formBtn');msgBox = document.getElementById('msgBox');function isValidHexString(input) {const hexPattern = /^[0-9a-fA-F:]+$/;return hexPattern.test(input);}btn.addEventListener('click', (evt) => {errMsg = '';if (uid.value.length != 8) {errMsg = 'UID com comprimento errado!';} else if (!isValidHexString(uid.value)) {errMsg = 'UID com caracteres não permitidos!';}if (errMsg.length > 0) {console.log(errMsg);evt.preventDefault();msgBox.textContent = errMsg;msgBox.style.display = 'inline-block';return;}uid.value = uid.value.toLowerCase();form.submit();});</script></body></html>";
// static char success_web_page_html[] = "<!DOCTYPE html><html lang=\"pt-BR\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Comedouro Automático PetDog</title><style>body {background-color: goldenrod;color: midnightblue;padding: 10px;font-family: 'Trebuchet MS', monospace;font-size: 1.5rem;text-align: center;}input,button {font-size: 1.2rem;padding: 5px;}footer {margin-top: 30px;}</style></head><body><h1>Sucesso!</h1><a href=\"/\">Voltar</a><footer>&copy; 2023 Henrique Sander Lourenço</footer></body></html>";

// >>>>>>>>>>>>>>>>>>>> User-defined functions

/**
 * @brief Wi-Fi event handler, triggered on connection/disconnection to AP
 *
 * @param arg
 * @param event_base
 * @param event_id
 * @param event_data
 */
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
							   int32_t event_id, void *event_data)
{
	if (event_id == WIFI_EVENT_AP_STACONNECTED)
	{
		wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
		ESP_LOGI(TAG, "station " MACSTR " join, AID=%d",
				 MAC2STR(event->mac), event->aid);
	}
	else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
	{
		wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
		ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d",
				 MAC2STR(event->mac), event->aid);
	}
}

/**
 * @brief Initialize Wi-Fi
 *
 * @return esp_err_t
 * @retval ESP_OK if Wi-Fi initialization is successful
 * @retval ESP_FAIL if Wi-Fi initialization fails
 */
esp_err_t wifi__init(void)
{
	esp_err_t err;
	esp_netif_t *netif_instance = NULL; ///< Pointer to netif instance

	///< Wi-Fi configuration
	wifi_config_t wifi_config = {
		.ap = {
			.ssid = ESP_WIFI_AP_SSID,
			.ssid_len = strlen(ESP_WIFI_AP_PWD),
			.channel = ESP_WIFI_AP_CHANNEL,
			.password = ESP_WIFI_AP_PWD,
			.max_connection = ESP_WIFI_MAX_CONN_TO_AP,
			.authmode = WIFI_AUTH_WPA_WPA2_PSK},
	};
	if (strlen(ESP_WIFI_AP_PWD) == 0)
	{
		wifi_config.ap.authmode = WIFI_AUTH_OPEN;
	}

	///< Wi-Fi initialization configurations
	wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();

	err = esp_netif_init();
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error initializing TCP/IP stack");
		return ESP_FAIL;
	}

	err = esp_event_loop_create_default();
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error creating default event loop");
		return ESP_FAIL;
	}

	netif_instance = esp_netif_create_default_wifi_ap();
	if (netif_instance == NULL)
	{
		ESP_LOGE(TAG, "Error creating default Wi-Fi AP");
		return ESP_FAIL;
	}

	err = esp_event_handler_instance_register(WIFI_EVENT,
											  ESP_EVENT_ANY_ID,
											  &wifi_event_handler,
											  NULL, NULL);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error registering event handler");
		return ESP_FAIL;
	}

	err = esp_wifi_init(&wifi_init_config);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error registering initializing Wi-Fi");
		return ESP_FAIL;
	}

	err = esp_wifi_set_mode(WIFI_MODE_AP);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error setting Wi-Fi mode to AP");
		return ESP_FAIL;
	}

	err = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error setting Wi-Fi configuration");
		return ESP_FAIL;
	}

	err = esp_wifi_start();
	if (err != ESP_OK)
	{
		ESP_LOGE(TAG, "Error starting Wi-Fi");
		return ESP_FAIL;
	}

	uint8_t mac[6] = {0};
	esp_base_mac_addr_get(mac);
	// for ()

	ESP_LOGI(TAG, "Wi-Fi inicializado com sucesso");
	return ESP_OK;
}