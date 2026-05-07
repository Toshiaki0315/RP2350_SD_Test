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

project(RP2350_SD_Test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# SDKの初期化
pico_sdk_init()

# ====== 魔法の解決策: ダミーの hardware_rtc ライブラリを作成 ======
# 外部ライブラリが要求するハードコード依存を吸収する、実体を持たない空のターゲットを作ります
add_library(hardware_rtc INTERFACE)
target_include_directories(hardware_rtc INTERFACE src/mock)

# ====== 外部ライブラリ (FatFs) の追加 ======
add_subdirectory(lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI)

# 実行ファイルの追加
add_executable(RP2350_SD_Test
    src/main.c
    src/hw_config.c
)

# ====== リンクするライブラリ ======
target_link_libraries(RP2350_SD_Test 
    pico_stdlib 
    hardware_spi 
    FatFs_SPI
)

# 標準入出力設定 (USBシリアルを有効化)
pico_enable_stdio_usb(RP2350_SD_Test 1)
pico_enable_stdio_uart(RP2350_SD_Test 0)

# UF2ファイルの生成
pico_add_extra_outputs(RP2350_SD_Test)