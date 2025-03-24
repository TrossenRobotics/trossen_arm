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
// This script demonstrates how to set the joint characteristics in the EEPROM, using the effort
// corrections as an example.
//
// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2
//
// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver with the leader configuration
// 3. Sets the effort corrections via joint characteristics
// 4. Sets the effort corrections via dedicated helper function
// 5. The driver cleans up automatically at the destructor

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

  // Print the current effort corrections via joint characteristics
  std::cout << "Current effort corrections: ";
  for (const auto& joint_characteristic : driver.get_joint_characteristics()) {
    std::cout << joint_characteristic.effort_correction << " ";
  }
  std::cout << std::endl;

  // Set the effort corrections via joint characteristics
  std::vector<trossen_arm::JointCharacteristic> joint_characteristics = driver.get_joint_characteristics();
  joint_characteristics[0].effort_correction = 1.1;
  joint_characteristics[1].effort_correction = 1.1;
  joint_characteristics[2].effort_correction = 1.1;
  joint_characteristics[3].effort_correction = 1.25;
  joint_characteristics[4].effort_correction = 1.15;
  joint_characteristics[5].effort_correction = 1.15;
  joint_characteristics[6].effort_correction = 1.15;
  driver.set_joint_characteristics(joint_characteristics);

  // Print the new effort corrections via joint characteristics
  std::cout << "New effort corrections: ";
  for (const auto& joint_characteristic : driver.get_joint_characteristics()) {
    std::cout << joint_characteristic.effort_correction << " ";
  }
  std::cout << std::endl;

  // Print the current effort corrections via dedicated helper function
  std::cout << "Current effort corrections: ";
  for (const auto& correction : driver.get_effort_corrections()) {
    std::cout << correction << " ";
  }
  std::cout << std::endl;

  // Set the effort corrections via dedicated helper function
  std::vector<float> effort_corrections = {1.1, 1.1, 1.1, 1.25, 1.15, 1.15, 1.15};
  driver.set_effort_corrections(effort_corrections);

  // Print the new effort corrections via dedicated helper function
  std::cout << "New effort corrections: ";
  for (const auto& correction : driver.get_effort_corrections()) {
    std::cout << correction << " ";
  }
  std::cout << std::endl;

  return 0;
}
