idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "./include" "../components/app_nvs/include" "../components/app_pn532_hl/include" "../components/app_web_server/include"
                    REQUIRES app_nvs app_pn532_hl app_web_server)
