// Pico 2 (RP2350) にはRTCが存在しないため、ライブラリを騙すためのダミーヘッダ
#ifndef _MOCK_HARDWARE_RTC_H
#define _MOCK_HARDWARE_RTC_H

#include <stdbool.h>
#include <stdint.h>

// SDKから datetime_t が削除されているため、自作の構造体に置き換える
#define datetime_t mock_datetime_t

typedef struct {
    int16_t year;
    int8_t month;
    int8_t day;
    int8_t dotw;
    int8_t hour;
    int8_t min;
    int8_t sec;
} mock_datetime_t;

// ライブラリから呼ばれるダミー関数群
static inline void rtc_init(void) {}
static inline bool rtc_running(void) { return true; }
static inline bool rtc_set_datetime(mock_datetime_t *t) { return true; }
static inline bool rtc_get_datetime(mock_datetime_t *t) {
    if (t) {
        t->year = 2026;
        t->month = 1;
        t->day = 1;
        t->dotw = 4;
        t->hour = 0;
        t->min = 0;
        t->sec = 0;
    }
    return true;
}

#endif