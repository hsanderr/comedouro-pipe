idf_component_register(SRCS "user_pn532.c"
                       INCLUDE_DIRS "include" "../app_pn532/include"
                       REQUIRES cxx newlib freertos esp_hw_support heap log soc hal esp_rom esp_common esp_system xtensa riscv mbedtls app_pn532)