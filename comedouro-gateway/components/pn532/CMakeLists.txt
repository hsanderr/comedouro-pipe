# set(COMPONENT_ADD_INCLUDEDIRS .)

# set(COMPONENT_SRCS "pn532.c")

# register_component()
idf_component_register(SRCS "pn532.c"
                       INCLUDE_DIRS "."
                       REQUIRES cxx newlib freertos esp_hw_support heap log soc hal esp_rom esp_common esp_system xtensa riscv mbedtls driver)