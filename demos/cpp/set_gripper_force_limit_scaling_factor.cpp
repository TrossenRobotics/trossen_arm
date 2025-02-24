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
// This script demonstrates how to set the gripper force limit scaling factor.

// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2

// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver with the follower configuration
// 3. Sets the gripper force limit scaling factor via end effector properties
// 4. Sets the gripper force limit scaling factor via dedicated helper function

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

  // Print the current scaling factor via end effector properties
  // The leader end effector has a default scaling factor of 0.5
  std::cout << "Current gripper force limit scaling factor: "
            << driver.get_end_effector().t_max_factor
            << std::endl;

  // Set the scaling factor via end effector properties
  trossen_arm::EndEffectorProperties end_effector_properties = driver.get_end_effector();
  end_effector_properties.t_max_factor = 0.0;
  driver.set_end_effector(end_effector_properties);

  // Print the new scaling factor via end effector properties
  std::cout << "New gripper force limit scaling factor: "
            << driver.get_end_effector().t_max_factor
            << std::endl;

  // Print the current scaling factor via dedicated helper function
  std::cout << "Current gripper force limit scaling factor: "
            << driver.get_gripper_force_limit_scaling_factor()
            << std::endl;

  // Set the scaling factor via dedicated helper function
  driver.set_gripper_force_limit_scaling_factor(0.5);

  // Print the new scaling factor via dedicated helper function
  std::cout << "New gripper force limit scaling factor: "
            << driver.get_gripper_force_limit_scaling_factor()
            << std::endl;

  // The change takes effect immediately and reset after power cycle
  std::cout << "The change takes effect immediately and resets after power cycle"
            << std::endl;

  return 0;
}
