idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "../components/user_nvs/include" "../components/user_pn532/include" "../components/app_web_server/include"
                    REQUIRES user_nvs user_pn532 app_web_server)
