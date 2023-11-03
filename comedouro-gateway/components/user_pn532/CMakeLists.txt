idf_component_register(SRCS "user_pn532.c"
                       INCLUDE_DIRS "include" "../pn532" "../user_nvs/include"
                       REQUIRES pn532 user_nvs)