idf_component_register(SRCS "app_web_server.c"
                       INCLUDE_DIRS "include"
                       REQUIRES cxx newlib freertos esp_hw_support heap log soc hal esp_rom esp_common esp_system xtensa riscv mbedtls esp_http_server esp_wifi nvs_flash)