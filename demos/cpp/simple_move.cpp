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
// This script demonstrates how to move a robot to different positions.
//
// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2
//
// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver
// 3. Opens the gripper
// 4. Moves the arm to a position
// 5. Closes the gripper
// 6. Moves the arm to another position
// 7. The driver automatically sets the mode to idle at the destructor

#include <cmath>
#include <iostream>
#include <vector>

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

  std::cout << "Opening the gripper..." << std::endl;
  driver.set_gripper_mode(trossen_arm::Mode::external_effort);
  driver.set_gripper_external_effort(50.0, 5.0, true);

  std::cout << "Moving the arm..." << std::endl;
  driver.set_arm_modes(trossen_arm::Mode::position);
  driver.set_arm_positions(
    {0.0, M_PI_2, M_PI_2, 0.0, 0.0, 0.0},
    2.0f,
    true
  );

  std::cout << "Closing the gripper..." << std::endl;
  driver.set_gripper_mode(trossen_arm::Mode::external_effort);
  driver.set_gripper_external_effort(-50.0, 5.0, true);

  std::cout << "Moving the arm again..." << std::endl;
  driver.set_arm_modes(trossen_arm::Mode::position);
  driver.set_arm_positions(
    std::vector<float>(driver.get_num_joints() - 1, 0.0),
    2.0f,
    true
  );

  return 0;
}
