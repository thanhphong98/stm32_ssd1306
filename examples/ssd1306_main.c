// MIT License

// Copyright (c) 2020 phonght32

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "stm_log.h"
#include "ssd1306.h"


#define TASK_SIZE   512
#define TASK_PRIOR  5

ssd1306_handle_t ssd1306_handle;
const char *TAG = "APP_MAIN";

uint8_t pikachu_map[] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfe, 0x2f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfe, 0x2f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x4f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfe, 0xdf, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfd, 0xbf, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf3, 0x1f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x46, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf7, 0x99, 0x9f, 0x8a, 0x6f, 
  0xff, 0xff, 0xff, 0xfb, 0xbf, 0x57, 0x6e, 0xff, 
  0xff, 0xff, 0xff, 0xf5, 0xff, 0xdf, 0xbf, 0x80, 
  0xff, 0xff, 0xa7, 0xf5, 0xff, 0xef, 0xfe, 0x29, 
  0xff, 0xf8, 0xeb, 0xef, 0xff, 0xfb, 0xe8, 0x1f, 
  0xff, 0xd8, 0xeb, 0xef, 0xff, 0xfa, 0xd4, 0xbf, 
  0xfe, 0x61, 0xff, 0xe5, 0xbf, 0xfd, 0xbf, 0xff, 
  0xfb, 0x4f, 0xe6, 0xcc, 0x7f, 0xff, 0xff, 0xff, 
  0xec, 0x7f, 0xc4, 0x6c, 0xff, 0xfc, 0xff, 0xff, 
  0xe3, 0xff, 0xc8, 0x9d, 0xff, 0xb7, 0xff, 0xff, 
  0xef, 0xff, 0xde, 0x6e, 0x3f, 0xaf, 0xff, 0xff, 
  0xff, 0xff, 0xc7, 0x9b, 0xd9, 0xbb, 0xff, 0xff, 
  0xbf, 0xff, 0x87, 0xd9, 0xf4, 0xb3, 0xff, 0xff, 
  0xbf, 0xff, 0x97, 0x85, 0xfd, 0x19, 0xff, 0xff, 
  0x9f, 0xf5, 0xff, 0xdf, 0xdb, 0xab, 0x7f, 0xff, 
  0xdf, 0xcd, 0x43, 0xc3, 0xdf, 0x74, 0x77, 0xff, 
  0x9f, 0xb5, 0xc7, 0xef, 0xfc, 0xfb, 0xfb, 0xff, 
  0xce, 0x37, 0x4f, 0xe3, 0xff, 0xd5, 0xf7, 0xff, 
  0xce, 0x7f, 0xb1, 0xff, 0xff, 0xfb, 0xcf, 0xff, 
  0xcf, 0xfb, 0xf5, 0xff, 0xe7, 0x9f, 0xef, 0xff, 
  0xf5, 0xff, 0xfb, 0xff, 0xef, 0x1f, 0xcf, 0xff, 
  0xf7, 0xe6, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf4, 0xa3, 0x7f, 0xff, 0xf8, 0xff, 0xff, 
  0xff, 0xe9, 0x49, 0xff, 0xff, 0x81, 0xff, 0xff, 
  0xff, 0xf7, 0x35, 0xff, 0xff, 0xbf, 0xff, 0xff, 
  0xff, 0xef, 0xa9, 0xff, 0xff, 0xcf, 0xff, 0xff, 
  0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0x7f, 0xff, 0xff, 0x5f, 0xff, 0xff, 
  0xff, 0xff, 0xef, 0xff, 0xff, 0xdf, 0xff, 0xff, 
  0xff, 0xff, 0xf7, 0xff, 0xff, 0xef, 0xff, 0xff, 
  0xff, 0xff, 0xcf, 0xff, 0xff, 0xe7, 0xff, 0xff, 
  0xff, 0xff, 0xcf, 0xff, 0xff, 0xf7, 0xff, 0xff, 
  0xff, 0xff, 0x9f, 0xff, 0xff, 0xeb, 0xff, 0xff, 
  0xff, 0xff, 0xbf, 0xff, 0xff, 0xeb, 0xff, 0xff, 
  0xff, 0xff, 0x9f, 0xff, 0xff, 0xf3, 0xff, 0xff, 
  0xff, 0xff, 0xf7, 0xff, 0xff, 0xf7, 0xff, 0xff, 
  0xff, 0xff, 0xa0, 0x1f, 0x9f, 0xf7, 0xff, 0xff, 
  0xff, 0xff, 0x49, 0xf9, 0x89, 0xf7, 0xff, 0xff, 
  0xff, 0xfe, 0x13, 0xdf, 0xc8, 0x39, 0xff, 0xff, 
  0xff, 0xff, 0xa3, 0xff, 0xfe, 0x37, 0xff, 0xff, 
  0xff, 0xff, 0x4f, 0xff, 0xff, 0xf3, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,  
};

static void example_task(void* arg)
{
    ssd1306_hw_info_t ssd1306_hw_info = {
        .i2c_num = I2C_NUM_2,
        .i2c_pins_pack = I2C_PINS_PACK_1,
        .i2c_speed = 400000,
        .is_init = false,
    };

    ssd1306_cfg_t ssd1306_cfg = {
        .hw_info = ssd1306_hw_info,
        .comm_mode = SSD1306_COMM_MODE_I2C,
        .size = SSD1306_SIZE_128_64,
        .inverse = false,
    };

    ssd1306_handle = ssd1306_init(&ssd1306_cfg);
    ssd1306_clear(ssd1306_handle);

    ssd1306_draw_image(ssd1306_handle, 0, 0, 64, 64, pikachu_map);

    ssd1306_set_position(ssd1306_handle, 80, 25);
    ssd1306_write_string(ssd1306_handle, FONT_SIZE_5x8, "STM-IDF");
    ssd1306_set_position(ssd1306_handle, 70, 40);
    ssd1306_write_string(ssd1306_handle, FONT_SIZE_5x8, "Oled 128x64");

    while(1)
    {   

    }
}

int main(void)
{
    /* Set application log output level */
    stm_log_level_set("*", STM_LOG_NONE);
    stm_log_level_set("APP_MAIN", STM_LOG_INFO);
    stm_log_level_set("SSD1306", STM_LOG_DEBUG);

    /* Create task */
    xTaskCreate(example_task, "example_task", TASK_SIZE, NULL, TASK_PRIOR, NULL);

    /* Start RTOS scheduler */
    vTaskStartScheduler();
}