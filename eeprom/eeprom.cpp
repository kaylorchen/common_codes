//
// Created by kaylor on 2/26/24.
//

#include "eeprom.h"
#include "fstream"
#include "kaylordut/log/logger.h"

Eeprom::Eeprom(std::string path, uint32_t size) {
  object_path_ = path;
  size_ = size;
  file_.open(object_path_, std::ios::binary | std::ios::ate | std::ios::in | std::ios::out);
  if (!file_){
    KAYLORDUT_LOG_ERROR("open eeprom failed!");
    exit(EXIT_FAILURE);
  }
}

Eeprom::~Eeprom() {
  file_.close();
  KAYLORDUT_LOG_INFO("close eeprom")
}

void Eeprom::ReadEeprom(uint8_t *data, const uint32_t offset, const uint32_t size) {
  if (offset + size > size_){
    KAYLORDUT_LOG_ERROR("reading range out of bounds");
    return;
  }
  file_.seekg(offset);
  file_.read((char *)data, size);
}

void Eeprom::WriteEeprom(const uint8_t *data, const uint32_t offset, const uint32_t size) {
  if (offset + size > size_){
    KAYLORDUT_LOG_ERROR("writing range out of bounds");
    return;
  }
  file_.seekp(offset);
  file_.write((char *)data, size);
}