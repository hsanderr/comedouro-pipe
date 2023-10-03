idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "../components/app_nvs/include" "../components/app_spi/include" "../components/pn532"
                    REQUIRES app_nvs)
