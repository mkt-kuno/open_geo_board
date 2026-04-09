# OpenGeoBoard

Arduino Uno/Nano R4 用の AD/DA ボード firmware  
Copyright (C) 2026 Makoto KUNO  
License: [GPLv3](./LICENSE)  

## 対応ボード

- Arduino Uno/Nano R4

## 使用ライブラリ

| ライブラリ | version | 注釈 |
|-----------|-----------|-----------|
| HX711 Arduino Library |0.7.5|bogde/HX711 |
| ADS1115_WE |1.5.7| wollewald/ADS1115_WE |
| DFRobot_GP8403 |1.0.0| DFRobot/DFRobot_GP8403 |
| ModbusRTUSlave |3.1.2| CMB27/ModbusRTUSlave |

## 機能

- **AI (Analog Input)**: 16ch
  - HX711 x8 (AI0～7): ロードセル入力, PGA128,最大3.5mV/V程度
  - ADS1115 x2 (AI8～15): 電圧入力,0～6.114V(int16_max)
- **AO (Analog Output)**: 8ch
  - GP8403 x4 (AO0～7): 電圧出力,0～10V

## ModbusRTU

- ポート: Serial (USB CDC)
- Baudrate: 38400
- Slave ID: 1

### レジスタ

| Address | Type | Name | Description |
|---------|------|------|-------------|
| 0-15 | Input Register | AI0-AI15 | アナログ入力値 (int16_t) |
| 0-7 | Holding Register | AO0-AO7 | アナログ出力値 (mV, uint16_t) |


### Arduino IDE

1. ライブラリをインストール
   - HX711 Arduino Library (bogde/HX711)
   - ADS1115_WE
   - DFRobot_GP8403
   - ModbusRTUSlave
2. ボード: Arduino Uno/Nano R4
3. ビルド・アップロード
