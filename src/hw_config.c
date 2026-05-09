/* src/hw_config.c */
#include "ff.h"       
#include "diskio.h"   
#include "hw_config.h"

// ▼ SDIOインターフェースの設定 (回路図のピン配置と完全一致させます)
sd_sdio_if_t sdio_if = {
    .CMD_gpio = 20,
    .D0_gpio  = 21,
    .D1_gpio  = 22,   // 回路図の SD_D1
    .D2_gpio  = 23,   // 回路図の SD_D2
    .D3_gpio  = 24,   // 回路図の SD_D3 (CS兼用)
    .CLK_gpio = 19,
    .baud_rate = 12500 * 1000 // 12.5 MHz
};

// ▼ SDカード設定
sd_card_t sd_cards[] = {
    {
        .type = SD_IF_SDIO,
        .sdio_if_p = &sdio_if,
        .use_card_detect = false,
    }
};

size_t sd_get_num() { return count_of(sd_cards); }

sd_card_t *sd_get_by_num(size_t num) {
    if (num < sd_get_num()) {
        return &sd_cards[num];
    }
    return NULL;
}

// SPIは使用しないため空の関数にしておきます
size_t spi_get_num() { return 0; }
spi_t *spi_get_by_num(size_t num) { return NULL; }