idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "./include" "../components/app_nvs/include" "../components/app_spi/include" "../components/app_pn532/include"
                    REQUIRES app_nvs)
