#include <globals.hpp>
#include <interconnect_interface.hpp>
#include <iostream>
struct Message {
  Message(unsigned from, unsigned to) : from(from), to(to) {}

  unsigned from;
  unsigned to;
};
int main() {
  // remember to delete after use
  // init the instance
  // remember to setup the global g_icnt_interface pointer!
  g_icnt_interface = InterconnectInterface::New("examples/2060.config");

  // create the nodes and routers
  g_icnt_interface->CreateInterconnect(8, 8);

  // clear the stats
  g_icnt_interface->Init();

  std::cout << g_icnt_interface->HasBuffer(0, 1) << std::endl;
  std::cout << g_icnt_interface->HasBuffer(1, 1) << std::endl;
  std::cout << g_icnt_interface->HasBuffer(2, 1) << std::endl;
  std::cout << g_icnt_interface->HasBuffer(3, 1) << std::endl;
  std::cout << g_icnt_interface->HasBuffer(15, 1) << std::endl;
  g_icnt_interface->Advance();

  // a breaf test
  // test from cpu to mem

  // the number of packages need to be sent
  unsigned int remainings[8] = {0};
  // received packages
  unsigned int received[8] = {0};
  for (int i = 0; i < 8; ++i) {
    remainings[i] = 1000;
  }
  Message *dram_buffer[8] = {0};

  unsigned cycle = 0;
  while (true) {
    cycle++;
    // advance the simulation
    g_icnt_interface->Advance();
    // send the packages
    for (int i = 0; i < 8; ++i) {
      if (remainings[i] > 0 && g_icnt_interface->HasBuffer(i, 64)) {
        auto msg = new Message(i, i + 8);

        g_icnt_interface->Push(i, i + 8, msg, 64, Flit::FlitType::READ_REQUEST);
        --remainings[i];
      }
    }

    // receive the packages sent to mem
    for (int i = 8; i < 16; i++) {
      if (dram_buffer[i - 8] == nullptr) {
        auto msg = (Message *)g_icnt_interface->Pop(i);
        if (msg) {
          dram_buffer[i - 8] = msg;
        }
      }
    }

    // send the packages back
    for (int i = 8; i < 16; ++i) {
      if (dram_buffer[i - 8] != nullptr && g_icnt_interface->HasBuffer(i, 64)) {
        g_icnt_interface->Push(i, dram_buffer[i - 8]->from, dram_buffer[i - 8],
                               64, Flit::FlitType::READ_REPLY);
        dram_buffer[i - 8] = nullptr;
      }
    }

    // cpu receive the packages
    for (int i = 0; i < 8; ++i) {
      auto msg = (Message *)g_icnt_interface->Pop(i);
      if (msg) {
        ++received[i];
        // remmeber to delete the message!!!
        delete msg;
      }
    }
    // check if all packages are received
    bool all_received = true;
    for (int i = 0; i < 8; ++i) {
      if (received[i] != 1000) {
        all_received = false;
        break;
      }
    }
    if (all_received) {
      break;
    }
  }

  std::cout << "cycle: " << cycle << std::endl;
  std::cout << "speed: " << (1000.0 * 8 * 2) / (double)cycle << std::endl;
  delete g_icnt_interface;
  g_icnt_interface = 0;
  std::cout << "finished" << std::endl;

  return 0;
}