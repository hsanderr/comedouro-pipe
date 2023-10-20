idf_component_register(SRCS "app_pn532_hl.c"
                       INCLUDE_DIRS "include" "../../main/include" "../app_pn532/include"
                       REQUIRES cxx newlib freertos esp_hw_support heap log soc hal esp_rom esp_common esp_system xtensa riscv mbedtls app_pn532)