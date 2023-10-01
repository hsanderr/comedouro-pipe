/**
 * @file app_spi.h
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

#ifndef APP_SPI_H
#define APP_SPI_H

//------------------ 3rd-part Includes Start ------------------//

#include "esp_err.h"

//------------------ 34d-part Includes End ------------------//

//------------------ Typedefs Start ------------------//

//------------------ Typedefs End ------------------//

//------------------ Functions declarations Start ------------------//

esp_err_t spi__init(void);

//------------------ Functions declarations End ------------------//

#endif