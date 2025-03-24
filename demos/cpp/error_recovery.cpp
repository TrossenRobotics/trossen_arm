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
// This script demonstrates how to recover from an error in the driver.
//
// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2
//
// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver
// 3. Sets the arm to position mode
// 4. Moves the arm to the home position
// 5. Triggers an error by setting a joint position to an invalid value
// 6. Recovers from the error
// 7. Moves the arm to the sleep position

#include <cmath>
#include <iostream>
#include <vector>
#include "libtrossen_arm/trossen_arm.hpp"

int main() {
  std::cout << "Initializing the driver..." << std::endl;
  trossen_arm::TrossenArmDriver driver;

  std::cout << "Configuring the driver..." << std::endl;
  driver.configure(
    trossen_arm::Model::wxai_v0,
    trossen_arm::StandardEndEffector::wxai_v0_leader,
    "192.168.1.2",
    false
  );

  driver.set_all_modes(trossen_arm::Mode::position);

  std::vector<float> sleep_positions = driver.get_positions();
  std::vector<float> home_positions(driver.get_num_joints(), 0.0f);
  home_positions[1] = M_PI_2;
  home_positions[2] = M_PI_2;

  try {
    std::cout << "Moving the arm to the home position..." << std::endl;
    driver.set_all_positions(home_positions);

    std::cout << "Triggering a discontinuity error..." << std::endl;
    // Command a huge step change that the arm cannot physically follow
    // which triggers an error for safety reasons
    home_positions[5] += M_PI;
    driver.set_all_positions(home_positions, 0.0f);

    std::cout << "Moving the arm to the sleep position..." << std::endl;
    driver.set_all_positions(sleep_positions);
  } catch (const std::exception &e) {
    std::cout << "An error occurred: " << e.what() << std::endl;
    std::cout << "Recovering from the error..." << std::endl;
    driver.cleanup();
    driver.configure(
      trossen_arm::Model::wxai_v0,
      trossen_arm::StandardEndEffector::wxai_v0_leader,
      "192.168.1.2",
      true
    );
    driver.set_all_modes(trossen_arm::Mode::position);
    driver.set_all_positions(sleep_positions);
  }

  return 0;
}
