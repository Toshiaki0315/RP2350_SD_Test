/* src/main.c */
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "ff.h" // FatFsヘッダ

int main() {
    // 標準入出力(USBシリアル)の初期化
    stdio_init_all();
    
    // シリアルコンソール接続のための待機時間
    // ターミナルを立ち上げる時間を確保するため、少し長めに5秒（5000ms）待機します
    sleep_ms(5000); 
    printf("\n\n=== Raspberry Pi Pico 2 SD Card R/W Test ===\n");
    printf("システム起動。テストを開始します...\n\n");

    FATFS fs;
    FIL fil;
    FRESULT fr;
    UINT bw, br;
    char buffer[128];
    const char* filename = "0:test_rw.txt"; // "0:" は hw_config.c で定義したドライブ名

    // 5回繰り返すループ
    for (int i = 1; i <= 5; i++) {
        printf("=== テスト実行 %d 回目 ===\n", i);

        // 1. SDカードのマウント
        printf("[%d] Mounting SD card...\n", i);
        fr = f_mount(&fs, "0:", 1);
        if (fr != FR_OK) {
            printf("[%d] Error: Mount failed! (Code: %d)\n", i, fr);
            printf("※SDカードがFAT32でフォーマットされているか確認してください。\n");
            
            // マウント失敗時は次のテストまで5秒待ってやり直す
            if (i < 5) {
                printf("\n5秒後に再試行します...\n\n");
                sleep_ms(5000);
            }
            continue; 
        }
        printf("[%d] Mount successful!\n", i);

        // 2. ファイルへの書き込みテスト
        printf("[%d] Writing to file: %s\n", i, filename);
        fr = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
        if (fr == FR_OK) {
            // 書き込むテキストに回数を含めて、本当に毎回上書きされているか確認しやすくします
            char text[64];
            snprintf(text, sizeof(text), "Hello from Pico 2! Test Run: %d\n", i);
            f_write(&fil, text, strlen(text), &bw);
            f_close(&fil);
            printf("[%d] Write successful! (%d bytes written)\n", i, bw);
        } else {
            printf("[%d] Error: File open for write failed! (Code: %d)\n", i, fr);
        }

        // 3. ファイルからの読み出しテスト
        printf("[%d] Reading from file: %s\n", i, filename);
        fr = f_open(&fil, filename, FA_READ);
        if (fr == FR_OK) {
            fr = f_read(&fil, buffer, sizeof(buffer) - 1, &br);
            if (fr == FR_OK) {
                buffer[br] = '\0'; // 文字列の終端処理
                printf("--- Read Data ---\n");
                printf("%s", buffer);
                printf("-----------------\n");
            } else {
                printf("[%d] Error: File read failed! (Code: %d)\n", i, fr);
            }
            f_close(&fil);
        } else {
            printf("[%d] Error: File open for read failed! (Code: %d)\n", i, fr);
        }

        // 4. マウント解除
        f_unmount("0:");
        printf("[%d] Unmount successful.\n", i);

        // 次のループまで5秒待機（最後の1回は待たない）
        if (i < 5) {
            printf("\n次のテストまで5秒待機します...\n\n");
            sleep_ms(5000);
        }
    }

    printf("\n=== All Tests Completed! ===\n");

    // 終了後の無限ループ
    while (true) {
        sleep_ms(1000);
    }
    
    return 0;
}