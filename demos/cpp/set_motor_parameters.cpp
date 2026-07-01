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
// This script demonstrates how to set the motor parameters of the arm.

// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2

// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver
// 3. Gets and sets the motor parameters

#include <iostream>
#include <map>
#include <vector>

#include "libtrossen_arm/trossen_arm.hpp"

void print_motor_parameters(
  const std::vector<std::map<trossen_arm::Mode, trossen_arm::MotorParameter>> & motor_parameters
)
{
  std::cout << "Motor parameters:" << std::endl;
  for (size_t i = 0; i < motor_parameters.size(); ++i) {
    const std::map<trossen_arm::Mode, trossen_arm::MotorParameter> & motor_parameter =
      motor_parameters.at(i);
    std::cout << "  Joint " << i << ":" << std::endl;
    for (const auto & [mode, parameter] : motor_parameter) {
      std::cout << "    Mode " << static_cast<int>(mode) << ":" << std::endl;
      std::cout << "      Position loop:";
      std::cout << " kp: " << parameter.position.kp;
      std::cout << ", ki: " << parameter.position.ki;
      std::cout << ", kd: " << parameter.position.kd;
      std::cout << ", imax: " << parameter.position.imax << std::endl;
      std::cout << "      Velocity loop:";
      std::cout << " kp: " << parameter.velocity.kp;
      std::cout << ", ki: " << parameter.velocity.ki;
      std::cout << ", kd: " << parameter.velocity.kd;
      std::cout << ", imax: " << parameter.velocity.imax << std::endl;
    }
  }
}

int main()
{
  // Initialize the driver
  trossen_arm::TrossenArmDriver driver;

  // Configure the driver
  driver.configure(
    trossen_arm::Model::wxai_v0,
    trossen_arm::StandardEndEffector::wxai_v0_leader,
    "192.168.1.2",
    false
  );

  // Get the current motor parameters
  std::vector<std::map<trossen_arm::Mode, trossen_arm::MotorParameter>> motor_parameters =
    driver.get_motor_parameters();
  print_motor_parameters(motor_parameters);

  // Set new motor parameters

  // Note: changing motor parameters changes the arm's behavior
  // For example
  // - Applications overfitting to specific motor parameters may not generalize well
  // - Poorly chosen motor parameters may cause oscillation or instability

  // Option 1 (Recommended): use the latest standard motor parameters
  driver.set_motor_parameters(trossen_arm::StandardMotorParameters::wxai_v0_latest);

  // // Option 2: use the older standard motor parameters
  // driver.set_motor_parameters(trossen_arm::StandardMotorParameters::wxai_v0_default);
  // driver.set_motor_parameters(trossen_arm::StandardMotorParameters::wxai_v0_20250509);

  // // Option 3: use custom motor parameters
  // motor_parameters.at(0).at(trossen_arm::Mode::position).position.kp = 12.0;
  // motor_parameters.at(1).at(trossen_arm::Mode::position).position.kp = 12.0;
  // motor_parameters.at(2).at(trossen_arm::Mode::position).position.kp = 12.0;
  // driver.set_motor_parameters(motor_parameters);

  // Get the new motor parameters
  motor_parameters = driver.get_motor_parameters();
  print_motor_parameters(motor_parameters);

  return 0;
}
