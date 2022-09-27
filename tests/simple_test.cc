#include <interconnect_interface.hpp>
#include <iostream>
int main() {
  // remember to delete after use
  auto intersim = InterconnectInterface::New("examples/icnt_config");
  intersim->CreateInterconnect(8, 8);
  intersim->Init();
  std::cout << intersim->HasBuffer(0, 1) << std::endl;
  std::cout << intersim->HasBuffer(1, 1) << std::endl;
  std::cout << intersim->HasBuffer(2, 1) << std::endl;
  std::cout << intersim->HasBuffer(3, 1) << std::endl;
  std::cout << intersim->HasBuffer(15, 1) << std::endl;
  delete intersim;
}