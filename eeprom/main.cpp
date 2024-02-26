#include "eeprom.h"
#include "kaylordut/log/logger.h"
#include "memory"
#include "stdio.h"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "array"

int main() {
  KAYLORDUT_LOG_INFO("This is a eeprom test case");
  auto eeprom = std::make_shared<Eeprom>();

  boost::uuids::random_generator generator;
  boost::uuids::uuid uuid = generator();
  std::array<uint8_t ,256> uuid_array = {0};
  std::copy(uuid.begin(), uuid.end(), uuid_array.begin());
  eeprom->WriteEeprom(uuid_array.data(), 0, 256);


  uint8_t buffer[256] = {0};
  eeprom->ReadEeprom(buffer, 0, 256);
  for (auto data : buffer) {
  }
  for (int i = 0; i < sizeof(buffer); ++i) {
    printf("%02X ", buffer[i]);
    if ((i + 1) % 16 == 0) { std::cout << std::endl; }
  }
  std::cout << std::endl;
  return 0;
}
