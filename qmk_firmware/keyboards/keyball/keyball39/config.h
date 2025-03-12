/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// Key matrix parameters
#define MATRIX_ROWS         (4 * 2)  // split keyboard
#define MATRIX_COLS         6
#define MATRIX_ROW_PINS     { GP29, GP28, GP27, GP26 }
#define MATRIX_COL_PINS     { GP4, GP5, GP6, GP7, GP8, GP9}
#define MATRIX_MASKED
#define DEBOUNCE            5
#define DIODE_DIRECTION     COL2ROW

// Split parameters
#define SOFT_SERIAL_PIN         GP1
#define SPLIT_HAND_MATRIX_GRID  GP27, GP9
#define SPLIT_HAND_MATRIX_GRID_LOW_IS_LEFT

// USB Configuration for new async endpoints
#define USB_ENDPOINTS_MAX 8
#define USB_POLLING_INTERVAL_MS 1
//#define SPLIT_USB_DETECT
//#define SPLIT_USB_TIMEOUT       500

// If your PC does not recognize Keyball, try setting this macro. This macro
// increases the firmware size by 200 bytes, so it is disabled by default, but
// it has been reported to work well in such cases.
//#define SPLIT_WATCHDOG_ENABLE

#define SPLIT_TRANSACTION_IDS_KB KEYBALL_GET_INFO, KEYBALL_GET_MOTION, KEYBALL_SET_CPI

// RGB LED settings
#define WS2812_DI_PIN       GP0
#ifdef RGBLIGHT_ENABLE
#    define RGBLED_NUM      48
#    define RGBLED_SPLIT    { 24, 24 }  // (24 + 22)
#    ifndef RGBLIGHT_LIMIT_VAL
#        define RGBLIGHT_LIMIT_VAL  125 // limitated for power consumption
#    endif
#    ifndef RGBLIGHT_VAL_STEP
#        define RGBLIGHT_VAL_STEP   15
#    endif
#    ifndef RGBLIGHT_HUE_STEP
#        define RGBLIGHT_HUE_STEP   17
#    endif
#    ifndef RGBLIGHT_SAT_STEP
#        define RGBLIGHT_SAT_STEP   17
#    endif
//#    define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_RAINBOW_SWIRL
#endif
#ifdef RGB_MATRIX_ENABLE
#    define RGB_MATRIX_SPLIT    { 24, 24 }
#endif

#ifndef OLED_FONT_H
#    define OLED_FONT_H "keyboards/keyball/lib/logofont/logofont.c"
#    define OLED_FONT_START 32
#    define OLED_FONT_END 195
#endif

#if !defined(LAYER_STATE_8BIT) && !defined(LAYER_STATE_16BIT) && !defined(LAYER_STATE_32BIT)
#    define LAYER_STATE_8BIT
#endif

// SPI Configuration for PMW3360
#define SPI_DRIVER SPID0
#define SPI_SCK_PIN GP22
#define SPI_MISO_PIN GP20
#define SPI_MOSI_PIN GP23

// Pointing Device Configuration
#define POINTING_DEVICE_ROTATION_90
#define POINTING_DEVICE_TASK_THROTTLE_MS 1  // Improved responsiveness for QMK 0.27.13

#ifndef POINTING_DEVICE_DEBUG
#    define POINTING_DEVICE_DEBUG FALSE
#endif

// PMW3360 Settings for QMK 0.27.13
#define PMW3360_CS_PIN GP21
#define PMW3360_CLOCK_SPEED 2000000
#define PMW3360_SPI_DIVISOR 2
#define PMW3360_SPI_LSBFIRST false

// Pointing Device Auto Mouse Settings
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
#    define AUTO_MOUSE_DEFAULT_LAYER 2
#endif

/* define RP2040 boot用 */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET // Activates the double-tap behavior
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 200U // Timeout window in ms in which the double tap can occur.
