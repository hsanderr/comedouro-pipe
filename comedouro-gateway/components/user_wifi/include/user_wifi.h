/**
 * @file user_wifi.h
 * @author Henrique Sander Lourenço
 * @brief Header file for user_wifi.c file.
 * @version 0.1
 * @date 2023-10-21
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

#ifndef USER_WIFI_H
#define USER_WIFI_H

// >>>>>>>>>>>>>>>>>>>> Common includes

#include "esp_err.h"

// >>>>>>>>>>>>>>>>>>>> App includes

// >>>>>>>>>>>>>>>>>>>> ESP-IDF includes

// >>>>>>>>>>>>>>>>>>>> libc includes

// >>>>>>>>>>>>>>>>>>>> Other includes

// >>>>>>>>>>>>>>>>>>>> Defines

// >>>>>>>>>>>>>>>>>>>> Declarations

esp_err_t wifi__init(void);

#endif