idf_component_register(SRCS "user_httpd.c"
                       INCLUDE_DIRS "include" "../user_utils/include" "../user_nvs/include"
                       REQUIRES esp_http_server)