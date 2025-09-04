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
// This script tests transitions of the interpolation space.
// Reference:
// - https://github.com/TrossenRobotics/trossen_arm/issues/71
// - https://github.com/TrossenRobotics/trossen_arm/issues/78

// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2

// The script does the following:
// 1. Initializes the drivers
// 2. Configures the drivers
// 3. Tests transitions of the interpolation space
// 4. Tests transitions of the modes

#include <iostream>

#include "libtrossen_arm/trossen_arm.hpp"

int main() {
  std::cout << "Initializing the drivers..." << std::endl;
  trossen_arm::TrossenArmDriver driver;

  std::cout << "Configuring the drivers..." << std::endl;
  driver.configure(
    trossen_arm::Model::wxai_v0,
    trossen_arm::StandardEndEffector::wxai_v0_leader,
    "192.168.1.2",
    false
  );

  // Interpolation space transitions
  std::array<double, 6> p = driver.get_cartesian_positions();
  p.at(2) += 0.1;
  driver.set_all_modes(trossen_arm::Mode::position);
  driver.set_cartesian_positions(p, trossen_arm::InterpolationSpace::cartesian);

  driver.set_all_positions(std::vector<double>(driver.get_num_joints(), 0.0));

  // Mode transitions
  p = driver.get_cartesian_positions();
  p.at(0) += 0.1;
  p.at(2) += 0.2;
  driver.set_arm_modes(trossen_arm::Mode::position);
  driver.set_cartesian_positions(p, trossen_arm::InterpolationSpace::cartesian);

  driver.set_gripper_mode(trossen_arm::Mode::external_effort);
  driver.set_gripper_external_effort(20);

  p.at(0)-= 0.1;
  p.at(2)-= 0.2;
  driver.set_cartesian_positions(p, trossen_arm::InterpolationSpace::joint);

  driver.set_gripper_external_effort(-20);

  return 0;
}
