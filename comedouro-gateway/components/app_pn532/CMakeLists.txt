idf_component_register(SRCS "app_pn532.c"
                       INCLUDE_DIRS "include" "../../main/include"
                       REQUIRES cxx newlib freertos esp_hw_support heap log soc hal esp_rom esp_common esp_system xtensa riscv mbedtls driver)