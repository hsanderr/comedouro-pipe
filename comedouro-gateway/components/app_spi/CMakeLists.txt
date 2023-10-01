idf_component_register(SRCS "app_spi.c"
                       INCLUDE_DIRS "include"
                       REQUIRES cxx newlib freertos esp_hw_support heap log soc hal esp_rom esp_common esp_system xtensa riscv mbedtls driver)