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
//    * Neither the name of the the copyright holder nor the names of its
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
// This script demonstrates how to perform position control in Cartesian space.

// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2

// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver for one arm
// 3. Sets the arm joints to position mode
// 4. Moves the end effector around in Cartesian space
// 5. The driver automatically sets the mode to idle at the destructor

#include "libtrossen_arm/trossen_arm.hpp"

int main(int argc, char** argv)
{
  // Initialize the driver
  trossen_arm::TrossenArmDriver driver;

  // Configure the driver for one arm
  driver.configure(
    trossen_arm::Model::wxai_v0,
    trossen_arm::StandardEndEffector::wxai_v0_leader,
    "192.168.1.2",
    false
  );

  // Set a custom pose of the tool frame measured in the flange frame
  trossen_arm::EndEffector end_effector = driver.get_end_effector();
  end_effector.t_flange_tool.at(0) = 0.0;
  driver.set_end_effector(end_effector);

  // Set the arm joints to position mode
  driver.set_arm_modes(trossen_arm::Mode::position);

  // Get the current Cartesian positions
  std::array<double, 6> cartesian_positions = driver.get_cartesian_positions();

  // Move the end effector up by 0.1m
  cartesian_positions[2] += 0.1;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Move the end effector up by 0.1m
  cartesian_positions[2] += 0.1;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Move the end effector left by 0.1m
  cartesian_positions[1] += 0.1;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Move the end effector forward by 0.1m
  cartesian_positions[0] += 0.1;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Rotate the end effector about the z-axis by 0.5 rad
  cartesian_positions[5] += 0.5;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Rotate the end effector about the y-axis by 0.5 rad
  cartesian_positions[4] += 0.5;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Rotate the end effector about the x-axis by 0.5 rad
  cartesian_positions[3] += 0.5;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Rotate the end effector about the z-axis by -0.5 rad
  cartesian_positions[5] -= 0.5;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Rotate the end effector about the y-axis by -0.5 rad
  cartesian_positions[4] -= 0.5;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Rotate the end effector about the x-axis by -0.5 rad
  cartesian_positions[3] -= 0.5;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Move the end effector down by 0.1m
  cartesian_positions[2] -= 0.1;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Move the end effector right by 0.1m
  cartesian_positions[1] -= 0.1;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Move the end effector back by 0.1m
  cartesian_positions[0] -= 0.1;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  // Move the end effector down by 0.1m
  cartesian_positions[2] -= 0.1;
  driver.set_cartesian_positions(
    cartesian_positions,
    trossen_arm::InterpolationSpace::cartesian
  );

  return 0;
}
