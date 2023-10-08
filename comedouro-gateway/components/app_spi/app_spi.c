/**
 * @file app_spi.c
 * @author Henrique Sander Lourenço
 * @brief
 * @version 0.1
 * @date 2023-10-01
 *
 * @copyright    Copyright 2023 Henrique Sander Lourenço

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 *
 *
 */

//------------------ 3rd party Includes Start ------------------//

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

//------------------ 3rd party Includes End ------------------//

//------------------ App Includes Start ------------------//

//------------------ 3rd party Includes End ------------------//

//------------------ Defines Start ------------------//

#define SPI_MOSI_GPIO 23
#define SPI_MISO_GPIO 19
#define SPI_CLK_GPIO 18
#define SPI_CS_GPIO 5
#define SPI_HOST_ID SPI3_HOST

//------------------ Defines End ------------------//

//------------------ Variables declarations Start ------------------//

static const char *TAG = "SpiModule";

//------------------ Variables declarations End ------------------//

//------------------ Functions prototypes Start ------------------//

//------------------ Functions prototypes End ------------------//

//------------------ Functions definitions Start ------------------//

spi_device_handle_t handler;

esp_err_t spi__test_pn532(void)
{
    // char data[1] = {0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa1, 0xa2, 0xa3};
    char data[] = {0, 0, 0xff, 4, 5, 6, 7, 8, 9, 10, 0xa9, 0xaa, 3, 4, 5, 6, 7, 8, 9, 10};
    char data_res[20] = {0};
    spi_transaction_t trans = {
        // .cmd = 0,
        // .addr = 0xa9,
        .length = 160,
        .rxlength = 160,
        .tx_buffer = data,
        .rx_buffer = data_res,
    };
    esp_err_t err = spi_device_polling_transmit(handler, &trans);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %d transmitting SPI: %s", err, esp_err_to_name(err));
    }
    else
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        ESP_LOGI(TAG, "Success transmitting SPI data, response=%s", data_res);
        for (uint8_t i = 0; i < 20; i++)
        {
            printf("%.2x\n", data_res[i]);
        }
    }
    return err;
}

static esp_err_t spi__add_pn532(void)
{
    spi_host_device_t host_id = SPI_HOST_ID;
    spi_device_interface_config_t dev_config = {
        // .command_bits = 2,
        // .address_bits = 2,
        // .dummy_bits = 0,
        .mode = 0,
        .spics_io_num = -1,
        // .clock_source = SPI_HOST_ID,
        .clock_speed_hz = 4000000,
        .queue_size = 1,
        .spics_io_num = SPI_CS_GPIO,
        // .input_delay_ns = 1000000,
    };
    esp_err_t err = spi_bus_add_device(host_id, &dev_config, &handler);
    return err;
}

esp_err_t spi__init(void)
{
    spi_host_device_t host_id = SPI_HOST_ID;
    spi_bus_config_t bus_config = {
        .mosi_io_num = SPI_MOSI_GPIO,
        .miso_io_num = SPI_MISO_GPIO,
        .sclk_io_num = SPI_CLK_GPIO,
        .data0_io_num = -1,
        .data1_io_num = -1,
        .data2_io_num = -1,
        .data3_io_num = -1,
        .data4_io_num = -1,
        .data5_io_num = -1,
        .data6_io_num = -1,
        .data7_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 256,
        .flags = (SPICOMMON_BUSFLAG_MASTER ||
                  SPICOMMON_BUSFLAG_IOMUX_PINS ||
                  SPICOMMON_BUSFLAG_SCLK ||
                  SPICOMMON_BUSFLAG_MOSI ||
                  SPICOMMON_BUSFLAG_MISO),

    };
    spi_dma_chan_t dma_chan = SPI_DMA_DISABLED;
    esp_err_t err = spi_bus_initialize(host_id, &bus_config, dma_chan);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %d initializing SPI: %s", err, esp_err_to_name(err));
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGI(TAG, "SPI module successfully initialized");
    }
    err = spi__add_pn532();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %d adding PN532: %s", err, esp_err_to_name(err));
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGI(TAG, "PN532 added successfully");
    }
    spi__test_pn532();
    return ESP_OK;
}

//------------------ Functions definitions End ------------------//
