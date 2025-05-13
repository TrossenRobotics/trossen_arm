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
// This script demonstrates how to use velocity control in Cartesian space to
// do admittance control

// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2

// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver for one arm
// 3. Sets the arm joints to position mode
// 4. Moves the end effector to an operation position
// 5. Sets the arm joints to velocity mode
// 6. Sets the properties of the virtual second order system
// 7. Starts the velocity control loop for 20 seconds
// 8. Sets the arm joints to position mode
// 9. Moves the end effector back to the original position
// 10. The driver automatically sets the mode to idle at the destructor

#include <chrono>
#include <iostream>
#include <thread>

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

  // Set the arm joints to position mode
  driver.set_arm_modes(trossen_arm::Mode::position);

  // Get the current cartesian positions
  std::array<double, 6> cartesian_positions = driver.get_cartesian_positions();

  // Move the end effector up by 0.1m
  cartesian_positions[2] += 0.1;
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

  // Declare the running variables
  trossen_arm::RobotOutput robot_output_temp;

  // Set the properties of the virtual second order system
  // Improper gains will make the arm unstable, please modify with caution
  // Low stiffness on y-axis translation
  std::array<double, 6> virtual_mass = { 0.5, 0.5, 0.5, 0.05, 0.05, 0.05 };
  std::array<double, 6> virtual_stiffness = { 1000.0, 200.0, 1000.0, 200.0, 200.0, 200.0 };
  std::array<double, 6> virtual_damping = { 100.0, 20.0, 100.0, 20.0, 20.0, 20.0 };

  // Set the arm joints to velocity mode
  driver.set_arm_modes(trossen_arm::Mode::velocity);

  // Set the start and end time
  auto start_time = std::chrono::steady_clock::now();
  auto end_time = start_time + std::chrono::seconds(20);
  auto loop_start_time = start_time;
  constexpr double dt = 1e-3;
  constexpr auto dt_duration = std::chrono::duration<double>(dt);

  // Start the velocity control loop
  while (true)
  {
    // Get the loop start time
    loop_start_time = std::chrono::steady_clock::now();

    // Check if the end time has been reached
    if (loop_start_time > end_time)
    {
      break;
    }

    // Get the cartesian velocities
    robot_output_temp = driver.get_robot_output();
    for (size_t i = 0; i < 6; ++i)
    {
      robot_output_temp.cartesian.velocities.at(i) += dt * (
        virtual_stiffness.at(i) * (
          cartesian_positions.at(i) - robot_output_temp.cartesian.positions.at(i)
        ) - virtual_damping.at(i) * robot_output_temp.cartesian.velocities.at(i)
        - robot_output_temp.cartesian.external_efforts.at(i)
      ) / virtual_mass.at(i);
    }

    // Set the cartesian velocities
    driver.set_cartesian_velocities(
      robot_output_temp.cartesian.velocities,
      trossen_arm::InterpolationSpace::cartesian,
      0.0,
      false
    );

    // Get the loop end time
    auto elapsed_time = std::chrono::steady_clock::now() - loop_start_time;

    // Check if the loop is taking too long
    if (elapsed_time > dt_duration)
    {
      std::cerr << "Warning: Loop took too long, aborting" << std::endl;
      break;
    }

    // Sleep for the remaining time
    std::this_thread::sleep_for(
      dt_duration - elapsed_time
    );
  }

  // Set the arm joints to position mode
  driver.set_arm_modes(trossen_arm::Mode::position);

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
