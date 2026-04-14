# OpenGeoBoard

Arduino Uno/Nano R4 用の AD/DA ボード firmware

**⚠️ このプロジェクトは WIP (作業中) です。**

## 開発の背景

[DigitShowBasic](https://geo.iis.u-tokyo.ac.jp/archives/7629) は、元は国立研究開発法人土木研究所で作成された **DigitShow** をベースに、Windows 2000 時代に東京大学本郷地盤研の本田剛さんが作成した地盤振動解析ソフトウェアです。もともとはコンソールで BASIC を叩くシンプルな構成で、Arduino に毛が生えたようなスペックだったそうです。

東京大学式の制御システムは、CONTEC および Interface 社の AD/DA ボードを用いて多様な試験機をモーター制御するのが基本となっており、電圧でコントロールするだけであれば非常に汎用的な作りになっています。

2025年12月に GitHub で [mkt-kuno/DigitShowBasic](https://github.com/mkt-kuno/DigitShowBasic) としてオープンソース公開しました。しかし、CONTEC/Interface 社のボードに依存しているため、資産活用や新しいハードウェアの選定が困難という課題がありました。

[DigitShowModbusM](https://github.com/mkt-kuno/DigitShowBasicM) は、この CONTEC/Interface 依存を解消する目的で開発されており、ModbusRTU 対応の AD/DA ボードであれば何でも利用可能になります。

本 firmware は、DigitShowModbusM 専用のハードウェア (Arduino Uno/Nano R4) として作成されました。

## 対応ボード

- Arduino Uno/Nano R4

## 使用ライブラリ

| ライブラリ | version | 注釈 |
|-----------|---------|-----------|
| HX711 Arduino Library |0.7.5|bogde/HX711 |
| ADS1115_WE |1.5.7| wollewald/ADS1115_WE |
| DFRobot_GP8403 |1.0.0| DFRobot/DFRobot_GP8403 |
| ModbusRTUSlave |3.1.2| CMB27/ModbusRTUSlave |

## 機能

### AI (Analog Input): 16ch

| デバイス | チャンネル | 用途 |　注釈 |
|---------|-----------|------|------|
| HX711 x8 | AI0～AI7 | ロードセル用 |10SPS,PGA128,最大3.5mV/V程度|
| ADS1115 x2 | AI8～AI15 | ADC | 0～6.114V(int16_max)|

### AO (Analog Output): 8ch

| デバイス | チャンネル | 出力範囲 |
|---------|-----------|----------|
| GP8403 x4 | AO0～AO7 | 0-10V |

## ModbusRTU

### 通信仕様

| 項目 | 値 |
|------|-----|
| ポート | Serial (USB CDC) |
| Baudrate | 38400 |
| Slave ID | 1 |

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

## License

[LICENSE](./LICENSE) を参照してください。

Copyright (c) 2024-2026