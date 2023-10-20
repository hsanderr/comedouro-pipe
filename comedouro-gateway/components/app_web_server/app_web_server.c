/**
 * @file app_web_server.c
 * @author Henrique Sander Lourenço
 * @brief
 * @version 0.1
 * @date yyyy-mm-dd
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
 */

//------------------ 3rd party Includes Start ------------------//

#include <stdio.h>
#include <stdlib.h>
#include <string.h> //Requires by memset
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_system.h"
#include "esp_http_server.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_mac.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/netdb.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

//------------------ 3rd party Includes End ------------------//

//------------------ Typedefs Start ------------------//

//------------------ Typedefs End ------------------//

//------------------ App Includes Start ------------------//

//------------------ App Includes End ------------------//

//------------------ Defines Start ------------------//

#define EXAMPLE_ESP_WIFI_SSID "PetDog_Comedouro"
#define EXAMPLE_ESP_WIFI_PASS "senha123"
#define EXAMPLE_ESP_WIFI_CHANNEL 1
#define EXAMPLE_MAX_STA_CONN 1
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

//------------------ Defines End ------------------//

//------------------ Variables declarations Start ------------------//

static char *TAG = "WebServerModule";
static char main_web_page_html[] = "<!DOCTYPE html><html lang=\"pt-BR\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Comedouro Automático PetDog</title><style>body {background-color: goldenrod;color: midnightblue;padding: 10px;font-family: 'Trebuchet MS', monospace;font-size: 1.5rem;text-align: center;}input {margin-top: 10px;margin-bottom: 10px;}input,button {font-size: 1.2rem;padding: 5px;text-align: center;}footer {margin-top: 30px;}.msg-box {display: none;background-color: brown;color: #eee;padding: 20px;margin: 10px 30px;}</style></head><body><h1>Configure seu Comedouro Automático PetDog!</h1><div class=\"msg-box\" id=\"msgBox\"></div><form action=\"/\" method=\"POST\" id=\"uidForm\"><label for=\"uid\">Insira aqui o código de identificação (UID) da coleira do seu pet</label><br><input type=\"text\" name=\"uid\" id=\"uid\" placeholder=\"506c931e\"><br><button type=\"button\" id=\"formBtn\">Enviar</button></form><footer>&copy; 2023 Henrique Sander Lourenço</footer><script>form = document.getElementById('uidForm');uid = document.getElementById('uid');btn = document.getElementById('formBtn');msgBox = document.getElementById('msgBox');function isValidHexString(input) {const hexPattern = /^[0-9a-fA-F:]+$/;return hexPattern.test(input);}btn.addEventListener('click', (evt) => {errMsg = '';if (uid.value.length != 8) {errMsg = 'UID com comprimento errado!';} else if (!isValidHexString(uid.value)) {errMsg = 'UID com caracteres não permitidos!';}if (errMsg.length > 0) {console.log(errMsg);evt.preventDefault();msgBox.textContent = errMsg;msgBox.style.display = 'inline-block';return;}uid.value = uid.value.toLowerCase();form.submit();});</script></body></html>";
static char success_web_page_html[] = "<!DOCTYPE html><html lang=\"pt-BR\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Comedouro Automático PetDog</title><style>body {background-color: goldenrod;color: midnightblue;padding: 10px;font-family: 'Trebuchet MS', monospace;font-size: 1.5rem;text-align: center;}input,button {font-size: 1.2rem;padding: 5px;}footer {margin-top: 30px;}</style></head><body><h1>Sucesso!</h1><a href=\"/\">Voltar</a><footer>&copy; 2023 Henrique Sander Lourenço</footer></body></html>";

//------------------ Variables declarations End ------------------//

//------------------ Functions prototypes Start ------------------//

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data);
esp_err_t get_uid(httpd_req_t *req);
esp_err_t test_handler(httpd_req_t *req);

//------------------ Functions prototypes End ------------------//

//------------------ Functions definitions Start ------------------//

void webserver__init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK},
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    httpd_handle_t httpd_handle = NULL;
    httpd_config_t httpd_config = HTTPD_DEFAULT_CONFIG();
    ESP_ERROR_CHECK(httpd_start(&httpd_handle, &httpd_config));
    httpd_uri_t home_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = test_handler,
        .user_ctx = NULL,
    };
    httpd_uri_t config_uri = {
        .uri = "/",
        .method = HTTP_POST,
        .handler = get_uid,
        .user_ctx = NULL,
    };
    httpd_register_uri_handler(httpd_handle, &home_uri);
    httpd_register_uri_handler(httpd_handle, &config_uri);
    // esp_netif_ip_info_t ipInfo;
    // IP4_ADDR(&ipInfo.ip, 192, 168, 1, 1);
    // IP4_ADDR(&ipInfo.gw, 192, 168, 1, 1);
    // IP4_ADDR(&ipInfo.netmask, 255, 255, 255, 0);
    // esp_netif_dhcps_stop(p_netif);
    // esp_netif_set_ip_info(p_netif, &ipInfo);
    // esp_netif_dhcps_start(p_netif);
}

esp_err_t test_handler(httpd_req_t *req)
{
    httpd_resp_send(req, main_web_page_html, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t get_uid(httpd_req_t *req)
{
    char resposta[] = "<h1>Sucesso</h1>";
    char content[13] = {0};
    int ret = httpd_req_recv(req, content, MIN(req->content_len, sizeof(content)));
    if (ret <= 0)
    { /* 0 return value indicates connection closed */
        /* Check if timeout occurred */
        if (ret == HTTPD_SOCK_ERR_TIMEOUT)
        {
            /* In case of timeout one can choose to retry calling
             * httpd_req_recv(), but to keep it simple, here we
             * respond with an HTTP 408 (Request Timeout) error */
            httpd_resp_send_408(req);
        }
        /* In case of error, returning ESP_FAIL will
         * ensure that the underlying socket is closed */
        return ESP_FAIL;
    }
    httpd_resp_send(req, success_web_page_html, HTTPD_RESP_USE_STRLEN);
    ESP_LOGI(TAG, "content: %s", content);
    return ESP_OK;
}

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

//------------------ Functions definitions End ------------------//