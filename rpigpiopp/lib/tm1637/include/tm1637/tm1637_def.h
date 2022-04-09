/*
 * Copyright 2022 saehie.park@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RPIGPIOPP_LIB_TM1637_DEF_H__
#define __RPIGPIOPP_LIB_TM1637_DEF_H__

// TM1637 Comamnd mode
#define TM1637_CMD_DATA 0x40    // 0b0100 ----
#define TM1637_CMD_DISPLAY 0x80 // 0b1000 ----
#define TM1637_CMD_ADDR 0xC0    // 0b1100 ----

// TM1637_CMD_DATA mode
#define TM1637_DATA_WRITE 0x00   // 0b---- --00
#define TM1637_DATA_READ 0x02    // 0b---- --10
#define TM1637_DATA_AUTOINC 0x00 // 0b---- -0--
#define TM1637_DATA_FIXADDR 0x04 // 0b---- -1--
#define TM1637_DATA_NORMAL 0x00  // 0b---- 0---
#define TM1637_DATA_TEST 0x08    // 0b---- 1---

// TM1637_CMD_DISPLAY mode
#define TM1637_DISPLAY_OFF 0x00 // 0b---- 0---
#define TM1637_DISPLAY_ON 0x08  // 0b---- 1---

// TM1637_CMD_ADDR mode
#define TM1637_ADDR_C0H 0x00 // 0b---- 0000
#define TM1637_ADDR_C1H 0x01 // 0b---- 0001
#define TM1637_ADDR_C2H 0x02 // 0b---- 0010
#define TM1637_ADDR_C3H 0x03 // 0b---- 0011
#define TM1637_ADDR_C4H 0x04 // 0b---- 0100
#define TM1637_ADDR_C5H 0x05 // 0b---- 0101

#define TM1637_DBIT_COLON 0x80 // colon led on if given

#endif // __RPIGPIOPP_LIB_TM1637_DEF_H__
