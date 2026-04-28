// Copyright 2026 Trossen Robotics
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// Purpose:
// This script discovers all arm controllers on a given subnet.
//
// Hardware setup:
// 1. Arms on the 192.168.1.x subnet
//
// The script does the following:
// 1. Scans the given subnet range for arm controllers
// 2. Prints the IP, model, firmware version, and error state of each found arm

#include <iostream>
#include <string>

#include "libtrossen_arm/trossen_arm.hpp"

int main()
{
  const std::string subnet = "192.168.1";
  const uint8_t ip_start = 1;
  const uint8_t ip_end = 254;
  const double timeout = 0.01;

  std::cout << "Scanning " << subnet << "." << static_cast<int>(ip_start)
            << " - " << subnet << "." << static_cast<int>(ip_end)
            << " (timeout=" << timeout << "s)..." << std::endl;

  const auto discover_results =
    trossen_arm::TrossenArmDriver::discover(
      subnet, ip_start, ip_end, timeout
    );

  std::cout << "\nFound " << discover_results.size() << " arm(s):" << std::endl;
  for (const auto & discover_result : discover_results) {
    std::cout << "  ip=" << discover_result.ip
              << "  model="
              << trossen_arm::MODEL_NAME.at(discover_result.model)
              << "  firmware=" << discover_result.firmware_version
              << "  error_state="
              << trossen_arm::ERROR_INFORMATION.at(
                   discover_result.error_state
                 )
              << std::endl;
  }

  return 0;
}
