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
// This script is used to finetune the joint characteristic of one joint.

// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2

// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver
// 3. Sets the joint mode to external effort
// 4. Sets the joint external effort to 0
// 5. Gets the joint characteristics
// 6. Prints the joint characteristics
// 7. Waits for the user to enter an increment
// 8. Increments the joint characteristic
// 9. Sets the joint characteristic
// 10. Repeats steps 5-9 until the user presses Ctrl+C

#include <iostream>
#include <vector>

#include <libtrossen_arm/trossen_arm.hpp>

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

  // Change this to the 0-based joint index you want to finetune
  const int INDEX = 0;

  std::vector<trossen_arm::Mode> modes(driver.get_num_joints(), trossen_arm::Mode::idle);
  modes[INDEX] = trossen_arm::Mode::external_effort;
  driver.set_joint_modes(modes);
  driver.set_joint_external_effort(
    INDEX,
    0.0,
    false
  );

  std::cout << "Recommended increment:" << std::endl;
  std::cout << "1) start with very small value like 1e-3" << std::endl;
  std::cout << "2) multiply the INCREMENT (NOT THE JOINT CHARACTERISTIC) by factor of 2 "
    "until there's noticeable behavioral change when applying the increment" << std::endl;
  std::cout << "3) use this increment to adjust the joint characteristic" << std::endl;

  while (true) {
    try {
      std::vector<trossen_arm::JointCharacteristic> joint_characteristics =
        driver.get_joint_characteristics();

      // std::cout << "Effort correction:" << std::endl;
      // std::cout << joint_characteristics[INDEX].effort_correction << std::endl;

      std::cout << "Friction constant term:" << std::endl;
      std::cout << joint_characteristics[INDEX].friction_constant_term << std::endl;

      // std::cout << "Friction transition velocity:" << std::endl;
      // std::cout << joint_characteristics[INDEX].friction_transition_velocity << std::endl;

      // std::cout << "Friction coulomb coefficient:" << std::endl;
      // std::cout << joint_characteristics[INDEX].friction_coulomb_coef << std::endl;

      // std::cout << "Friction viscous coefficient:" << std::endl;
      // std::cout << joint_characteristics[INDEX].friction_viscous_coef << std::endl;

      // std::cout << "Position offsets:" << std::endl;
      // std::cout << joint_characteristics[INDEX].position_offset << std::endl;

      std::cout << "Enter the increment "
        "(default 0.0, positive to increase, negative to decrease): ";
      std::string input;
      std::getline(std::cin, input);
      float increment = 0.0;
      if (!input.empty()) {
        increment = std::stof(input);
      }

      // joint_characteristics[INDEX].effort_correction += increment;

      joint_characteristics[INDEX].friction_constant_term += increment;

      // joint_characteristics[INDEX].friction_transition_velocity += increment;

      // joint_characteristics[INDEX].friction_coulomb_coef += increment;

      // joint_characteristics[INDEX].friction_viscous_coef += increment;

      // joint_characteristics[INDEX].position_offset += increment;

      driver.set_joint_characteristics(joint_characteristics);
    } catch (const std::exception& e) {
      break;
    }
  }

  return 0;
}
