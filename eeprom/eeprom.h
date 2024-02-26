//
// Created by kaylor on 2/26/24.
//

#pragma once
#include "string"
#include "fstream"

class Eeprom {
 public:
  Eeprom(std::string path = "/sys/bus/i2c/devices/5-0050/eeprom", uint32_t size = 256);
  ~Eeprom();
  void ReadEeprom(uint8_t *data, const uint32_t offset, const uint32_t size);
  void WriteEeprom(const uint8_t *data, const uint32_t offset, const uint32_t size);
 private:
  std::string object_path_;
  uint32_t size_;
  std::fstream file_;
};
