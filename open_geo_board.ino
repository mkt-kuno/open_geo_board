/*
 * open_geo_board.ino
 * Copyright (C) 2026 Makoto KUNO
 * License: GPLv3
 */

#include <Arduino.h>
#include <Wire.h>
#include <HX711.h>
#include <ADS1115_WE.h>
#include <DFRobot_GP8403.h>
#include <ModbusRTUSlave.h>

//====================================================================
// ハードウェア設定
//====================================================================
#define HX711_NUM   8
#define ADS1115_NUM 2
#define GP8403_NUM  4

HX711 hx711[HX711_NUM];

const uint8_t hx711_dout[HX711_NUM] = {2, 4, 6, 8, 10, 12, 14, 16};
const uint8_t hx711_sck[HX711_NUM]  = {3, 5, 7, 9, 11, 13, 15, 17};

ADS1115_WE ads1115[ADS1115_NUM] = {
  ADS1115_WE(&Wire, 0x48),
  ADS1115_WE(&Wire, 0x49)
};

DFRobot_GP8403 gp8403[GP8403_NUM] = {
  DFRobot_GP8403(&Wire, 0x58),
  DFRobot_GP8403(&Wire, 0x59),
  DFRobot_GP8403(&Wire, 0x5A),
  DFRobot_GP8403(&Wire, 0x5B)
};

ModbusRTUSlave modbus(Serial);

static int16_t  inputReg[HX711_NUM + ADS1115_NUM * 4];
static uint16_t holdReg[GP8403_NUM * 2];

static uint8_t ads_current_channel = 0;
static uint8_t gp_current_ch = 0;

//====================================================================
// setup()
//====================================================================
void setup() {
  Wire.begin();
  Wire.setClock(400000);

  for (uint8_t i = 0; i < HX711_NUM; i++) {
    hx711[i].begin(hx711_dout[i], hx711_sck[i], 128);
  }

  for (uint8_t i = 0; i < ADS1115_NUM; i++) {
    ads1115[i].init();
    ads1115[i].setVoltageRange_mV(ADS1115_RANGE_6144);
    ads1115[i].setConvRate(ADS1115_128_SPS);
    ads1115[i].setCompareChannels(ADS1115_COMP_0_GND);
    ads1115[i].startSingleMeasurement();
  }

  for (uint8_t i = 0; i < GP8403_NUM; i++) {
    gp8403[i].begin();
    gp8403[i].setDACOutRange(gp8403[i].eOutputRange10V);
    gp8403[i].setDACOutVoltage(0, 0);
    gp8403[i].setDACOutVoltage(0, 1);
  }

  const unsigned long modbus_baudrate = 38400;
  modbus.configureInputRegisters((uint16_t*)inputReg, HX711_NUM + ADS1115_NUM * 4);
  modbus.configureHoldingRegisters(holdReg, GP8403_NUM * 2);
  modbus.begin(0x01, modbus_baudrate);
  Serial.begin(modbus_baudrate);
}

//====================================================================
// loop()
//====================================================================
void loop() {
  // 1. HX711 Task: 全ICのready確認、readyならread
  for (uint8_t i = 0; i < HX711_NUM; i++) {
    if (hx711[i].is_ready()) {
      long raw = hx711[i].read();
      inputReg[i] = (int16_t)(raw >> 8);
    }
  }

  // 2. ADS1115 Task: IC0のreadyを待って両ICを読み込み、次のchへ変換開始
  if (!ads1115[0].isBusy()) {
    inputReg[HX711_NUM + ads_current_channel] = ads1115[0].getRawResult();
    inputReg[HX711_NUM + 4 + ads_current_channel] = ads1115[1].getRawResult();

    if (++ads_current_channel >= 4) {
      ads_current_channel = 0;
    }

    if (ads_current_channel == 0) {
      ads1115[0].setCompareChannels(ADS1115_COMP_0_GND);
      ads1115[1].setCompareChannels(ADS1115_COMP_0_GND);
    } else if (ads_current_channel == 1) {
      ads1115[0].setCompareChannels(ADS1115_COMP_1_GND);
      ads1115[1].setCompareChannels(ADS1115_COMP_1_GND);
    } else if (ads_current_channel == 2) {
      ads1115[0].setCompareChannels(ADS1115_COMP_2_GND);
      ads1115[1].setCompareChannels(ADS1115_COMP_2_GND);
    } else {
      ads1115[0].setCompareChannels(ADS1115_COMP_3_GND);
      ads1115[1].setCompareChannels(ADS1115_COMP_3_GND);
    }

    ads1115[0].startSingleMeasurement();
    ads1115[1].startSingleMeasurement();
  }

  // 3. ModbusRTU Task
  modbus.poll();

  // 4. GP8403 Task: 1 loopで1chだけ更新
  {
    uint8_t ic = gp_current_ch / 2;
    uint8_t ch = gp_current_ch % 2;
    gp8403[ic].setDACOutVoltage(holdReg[gp_current_ch], ch);

    if (++gp_current_ch >= GP8403_NUM * 2) {
      gp_current_ch = 0;
    }
  }
}
