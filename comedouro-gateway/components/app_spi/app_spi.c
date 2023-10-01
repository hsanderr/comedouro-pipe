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

//------------------ 3rd-part Includes Start ------------------//

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/spi_master.h"

//------------------ 3rd-part Includes End ------------------//

//------------------ App Includes Start ------------------//

//------------------ 3rd-part Includes End ------------------//

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

esp_err_t spi__add_pn532(void)
{
    spi_host_device_t host_id = SPI_HOST_ID;
    spi_device_interface_config_t dev_config = {
        .command_bits = 2,
        .address_bits = 2,
        .dummy_bits = 0,
        .mode = 0,
        .spics_io_num = -1,
        .clock_speed_hz = 80000000,
        .queue_size = 8,
    };
    spi_device_handle_t handler;
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
        .max_transfer_sz = SPI_MAX_DMA_LEN,
        .flags = (SPICOMMON_BUSFLAG_MASTER ||
                  SPICOMMON_BUSFLAG_IOMUX_PINS ||
                  SPICOMMON_BUSFLAG_SCLK ||
                  SPICOMMON_BUSFLAG_MOSI ||
                  SPICOMMON_BUSFLAG_MISO),

    };
    spi_dma_chan_t dma_chan = SPI_DMA_CH1;
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
    return ESP_OK;
}

//------------------ Functions definitions End ------------------//
