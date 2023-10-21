idf_component_register(SRCS "user_nvs.c"
                       INCLUDE_DIRS "include"
                       REQUIRES cxx newlib freertos esp_hw_support heap log soc hal esp_rom esp_common esp_system xtensa riscv mbedtls nvs_flash)