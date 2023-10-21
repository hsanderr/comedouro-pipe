/**
 * @file user_nvs.h
 * @author Henrique Sander Lourenço
 * @brief Header file for user_nvs.c file.
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright 2023 Henrique Sander Lourenço

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

#ifndef USER_NVS_H
#define USER_NVS_H

// >>>>>>>>>>>>>>>>>>>> Common includes

#include "esp_err.h"

// >>>>>>>>>>>>>>>>>>>> App includes

// >>>>>>>>>>>>>>>>>>>> ESP-IDF includes

// >>>>>>>>>>>>>>>>>>>> libc includes

// >>>>>>>>>>>>>>>>>>>> Other includes

// >>>>>>>>>>>>>>>>>>>> Defines

typedef enum
{
  nvs_int8,
  nvs_uint8
} nvs_entry_type_t;

// >>>>>>>>>>>>>>>>>>>> Declarations

esp_err_t nvs__init(void);
void nvs__read(void *var, char *key, nvs_entry_type_t var_type);
void nvs__write(void *var, char *key, nvs_entry_type_t var_type);

#endif