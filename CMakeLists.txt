cmake_minimum_required(VERSION 3.13)

# 1. 拡張機能の設定が読み込まれる前にボードを固定する
set(PICO_BOARD pico2 CACHE STRING "Board type" FORCE)
set(PICO_PLATFORM rp2350 CACHE STRING "Platform type" FORCE)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# Raspberry Pi Pico SDK のインポート
include(pico_sdk_import.cmake)

# プロジェクト名をSDテスト用に変更
project(RP2350_SD_Test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# SDKの初期化
pico_sdk_init()

# ====== 外部ライブラリ (FatFs) の追加 ======
# libディレクトリに配置するFatFsライブラリをビルド対象に追加
add_subdirectory(lib/no-OS-FatFS-SD-SPI-RPi-Pico)

# 実行ファイルの追加 (今回は main.c とハードウェア設定用の hw_config.c)
add_executable(RP2350_SD_Test
    src/main.c
    src/hw_config.c
)

# リンクするライブラリ
target_link_libraries(RP2350_SD_Test 
    pico_stdlib 
    hardware_spi 
    no-OS-FatFS-SD-SPI-RPi-Pico # FatFsラッパーライブラリをリンク
)

# 標準入出力設定 (USBシリアルを有効化)
pico_enable_stdio_usb(RP2350_SD_Test 1)
pico_enable_stdio_uart(RP2350_SD_Test 0)

# UF2ファイルの生成
pico_add_extra_outputs(RP2350_SD_Test)
