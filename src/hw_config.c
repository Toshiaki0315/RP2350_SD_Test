/* src/hw_config.c */
#include "ff.h"       // 追加: FatFs本体のヘッダ
#include "diskio.h"   // 追加: STA_NOINIT が定義されているヘッダ
#include "hw_config.h"

// SPIポートの設定
spi_t spi_descs[] = {
    {
        .hw_inst = spi0,           // 使用するSPIハードウェア (spi0)
        .miso_gpio = 21,           // MISO (SD_D0)
        .mosi_gpio = 20,           // MOSI (SD_CMD)
        .sck_gpio = 19,            // SCK  (SD_SCK)
        .baud_rate = 12500 * 1000  // SPIクロック周波数 (12.5 MHz)
    }
};

// SDカードのSPIインターフェース設定
sd_card_t sd_cards[] = {
    {
        .pcName = "0:",            // FatFsでのドライブ名
        .spi = &spi_descs[0],      // 上記で定義したSPI設定を使用
        .ss_gpio = 24,             // CS (チップセレクト / SD_D3)
        .use_card_detect = false,  // カード挿入検知ピン(CD)は使用しない
        .m_Status = STA_NOINIT     // 初期ステータス
    }
};

// ライブラリから要求される配列サイズの取得関数
size_t spi_get_num() { return count_of(spi_descs); }
size_t sd_get_num() { return count_of(sd_cards); }

// 番号からSDカードの構造体ポインタを返す関数
sd_card_t *sd_get_by_num(size_t num) {
    if (num < sd_get_num()) {
        return &sd_cards[num];
    } else {
        return NULL;
    }
}

// 番号からSPIの構造体ポインタを返す関数
spi_t *spi_get_by_num(size_t num) {
    if (num < spi_get_num()) {
        return &spi_descs[num];
    } else {
        return NULL;
    }
}