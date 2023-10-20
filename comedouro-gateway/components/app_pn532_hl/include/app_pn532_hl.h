/**
 * @file app_pn532_hl.h
 * @author Henrique Sander Lourenço
 * @brief
 * @version 0.1
 * @date 2023-10-15
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
 */

#ifndef APP_PN532_HL_H
#define APP_PN532_HL_H

//------------------ 3rd party Includes Start ------------------//

#include "esp_err.h"
#include "app_pn532.h"

//------------------ 3rd party Includes End ------------------//

//------------------ Typedefs Start ------------------//

//------------------ Typedefs End ------------------//

//------------------ App Includes Start ------------------//

//------------------ App Includes End ------------------//

//------------------ Defines Start ------------------//

//------------------ Defines End ------------------//

//------------------ Variables declarations Start ------------------//

extern pn532_t nfc;

//------------------ Variables declarations End ------------------//

//------------------ Functions prototypes Start ------------------//

//------------------ Functions prototypes End ------------------//

//------------------ Functions declarations Start ------------------//

esp_err_t pn532hl__init(void);
int8_t pn532hl__read_uid(uint8_t uid[]);

//------------------ Functions declarations End ------------------//

#endif