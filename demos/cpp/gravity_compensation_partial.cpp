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
// This script demonstrates how to compensate for a portion of the gravity.

// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2

// The script does the following:
// 1. Configures what portion of gravity to compensate
// 2. Initializes the driver
// 3. Configures the driver
// 4. Sets the arm joints to effort mode
// 5. Starts the gravity compensation loop, applying the compensation efforts
// 6. The user can stop the loop with Ctrl+C

#include <iostream>
#include <vector>
#include <csignal>
#include <atomic>

#include "libtrossen_arm/trossen_arm.hpp"

std::atomic<bool> keep_running(true);

void signal_handler(int) {
  keep_running = false;
}

int main() {
  // Configure what portion of gravity to compensate
  // 0.0 means no compensation, 1.0 means full compensation
  const double GRAVITY_COMPENSATION_FACTOR = 0.5;

  // Initialize the driver
  trossen_arm::TrossenArmDriver driver;

  // Configure the driver
  driver.configure(
    trossen_arm::Model::wxai_v0,
    trossen_arm::StandardEndEffector::wxai_v0_leader,
    "192.168.1.2",
    false
  );

  // Set up signal handler for Ctrl+C
  std::signal(SIGINT, signal_handler);

  // Start gravity compensation
  trossen_arm::RobotOutput robot_output;
  driver.set_all_modes(trossen_arm::Mode::effort);
  std::cout << "Gravity compensation started. Press Ctrl+C to stop." << std::endl;
  while (keep_running) {
    robot_output = driver.get_robot_output();
    for (size_t i = 0; i < robot_output.joint.all.compensation_efforts.size(); ++i) {
      // Apply the compensation efforts scaled by the factor
      robot_output.joint.all.compensation_efforts[i] *= GRAVITY_COMPENSATION_FACTOR;
    }
    driver.set_all_efforts(
      robot_output.joint.all.compensation_efforts,
      0.0,
      false
    );
  }
  std::cout << "Gravity compensation stopped." << std::endl;
}
