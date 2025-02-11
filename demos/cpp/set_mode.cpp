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
// This script demonstrates how to set the mode of the robot.
//
// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2
//
// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver with the leader configuration
// 3. Sets the gripper mode to "position"
// 4. Sets the arm mode to "position"
// 5. Sets the all mode to "idle"
// 6. The driver cleans up automatically at the destructor

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

  // Set the modes of the gripper joints to "position"
  driver.set_gripper_mode(trossen_arm::Mode::position);

  // Print the modes of the gripper joints
  std::cout << "Modes: ";
  for (const auto& mode : driver.get_modes()) {
    std::cout << static_cast<int>(mode) << " ";
  }
  std::cout << std::endl;

  // Press Enter to continue
  std::cout << "Press Enter to continue...";
  std::cin.ignore();

  // Set the modes of the arm joints to "position"
  driver.set_arm_modes(trossen_arm::Mode::position);

  // Print the modes of the arm joints
  std::cout << "Modes: ";
  for (const auto& mode : driver.get_modes()) {
    std::cout << static_cast<int>(mode) << " ";
  }
  std::cout << std::endl;

  // Press Enter to continue
  std::cout << "Press Enter to continue...";
  std::cin.ignore();

  // Set the modes of all joints to "idle"
  driver.set_all_modes(trossen_arm::Mode::idle);

  // Print the modes of all joints
  std::cout << "Modes: ";
  for (const auto& mode : driver.get_modes()) {
    std::cout << static_cast<int>(mode) << " ";
  }
  std::cout << std::endl;

  // Press Enter to continue
  std::cout << "Press Enter to continue...";
  std::cin.ignore();

  return 0;
}
