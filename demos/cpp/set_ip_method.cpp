// Copyright 2025 Trossen Robotics
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
// This script demonstrates how to set the IP method to DHCP or MANUAL in the EEPROM.
//
// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2
//
// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver
// 3. Gets and sets the IP method
// 4. Reboots the controller to apply the new IP method
// 5. Acquires the new IP address from the user
// 6. Reconfigures the driver and checks the result

#include <iostream>

#include "libtrossen_arm/trossen_arm.hpp"

int main() {
  // Initialize the driver
  trossen_arm::TrossenArmDriver driver;

  // Configure the driver
  driver.configure(
    trossen_arm::Model::wxai_v0,
    trossen_arm::StandardEndEffector::wxai_v0_leader,
    "192.168.1.2",
    false
  );

  // Print the current IP method
  std::cout << "Current IP method: " << static_cast<int>(driver.get_ip_method()) << std::endl;

  // Set the IP method to DHCP
  driver.set_ip_method(trossen_arm::IPMethod::dhcp);

  // // Set the IP method to MANUAL
  // driver.set_ip_method(trossen_arm::IPMethod::manual);

  // Print the new IP method
  std::cout << "New IP method: " << static_cast<int>(driver.get_ip_method()) << std::endl;

  // Reboot the controller to apply the new IP method
  driver.cleanup(true);  // or driver.reboot_controller();

  // Notify the user about the new IP address assignment
  std::cout << "The new IP address will be assigned by the DHCP server." << std::endl;
  std::cout << "You can find it using the command 'nmap -sn 192.168.1.0/24' in a new terminal.";
  std::cout << std::endl;

  // Acquire the new IP address from the user
  std::string new_ip;
  std::cout << "Enter the new IP address: ";
  std::cin >> new_ip;

  // Reconfigure the driver with the new IP address
  driver.configure(
    trossen_arm::Model::wxai_v0,
    trossen_arm::StandardEndEffector::wxai_v0_leader,
    new_ip,
    false
  );

  // Print the IP method after reboot
  std::cout << "IP method after reboot: " << static_cast<int>(driver.get_ip_method()) << std::endl;

  return 0;
}
