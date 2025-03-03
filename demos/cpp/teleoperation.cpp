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
// This script demonstrates how to teleoperate the robots with force feedback.
//
// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2
// 2. A WXAI V0 arm with follower end effector and ip at 192.168.1.3
//
// The script does the following:
// 1. Initializes the drivers
// 2. Configures the drivers with the leader and follower configurations
// 3. Records the sleep positions
// 4. Moves the robots to home positions
// 5. For a specified amount of time, feeds the external efforts from the follower robot to the
//    leader robot and feeds the positions from the leader robot to the follower robot
// 6. Moves the robots to home positions
// 7. Moves the robots to sleep positions
// 8. Sets the robots to idle mode
// 9. The driver automatically sets the mode to idle at the destructor
// NOTE: When the time for teleoperation has expired, it will get locked in position and start
// moving to home positions. Please let go of the leader when this happens.

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

#include "libtrossen_arm/trossen_arm.hpp"

int main(int argc, char** argv)
{
  std::cout << "Initializing the drivers..." << std::endl;
  trossen_arm::TrossenArmDriver driver_leader;
  trossen_arm::TrossenArmDriver driver_follower;

  std::cout << "Configuring the drivers..." << std::endl;
  driver_leader.configure(
    trossen_arm::Model::wxai_v0,
    trossen_arm::StandardEndEffector::wxai_v0_leader,
    "192.168.1.2",
    false
  );
  driver_follower.configure(
    trossen_arm::Model::wxai_v0,
    trossen_arm::StandardEndEffector::wxai_v0_follower,
    "192.168.1.3",
    false
  );

  std::cout << "Moving to home positions..." << std::endl;
  driver_leader.set_all_modes(trossen_arm::Mode::position);
  driver_leader.set_all_positions(
    {0.0, M_PI_2, M_PI_2, 0.0, 0.0, 0.0, 0.0},
    2.0f,
    true
  );
  driver_follower.set_all_modes(trossen_arm::Mode::position);
  driver_follower.set_all_positions(
    {0.0, M_PI_2, M_PI_2, 0.0, 0.0, 0.0, 0.0},
    2.0f,
    true
  );

  std::cout << "Starting to teleoperate the robots..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  driver_leader.set_all_modes(trossen_arm::Mode::external_effort);
  driver_follower.set_all_modes(trossen_arm::Mode::position);

  auto start_time = std::chrono::steady_clock::now();
  auto end_time = start_time + std::chrono::seconds(20);
  float force_feedback_gain = 0.1;
  std::vector<float> external_efforts_leader(driver_leader.get_num_joints());
  while (std::chrono::steady_clock::now() < end_time) {
    // Feed the external external efforts from the follower robot to the leader robot
    external_efforts_leader = driver_follower.get_external_efforts();
    for (size_t i = 0; i < driver_leader.get_num_joints(); ++i) {
      external_efforts_leader.at(i) *= -force_feedback_gain;
    }
    driver_leader.set_all_external_efforts(
      external_efforts_leader,
      0.0f,
      false
    );
    // Feed the positions from the leader robot to the follower robot
    driver_follower.set_all_positions(
      driver_leader.get_positions(),
      0.0f,
      false,
      driver_leader.get_velocities()
    );
  }

  std::cout << "Moving to home positions..." << std::endl;
  driver_leader.set_all_modes(trossen_arm::Mode::position);
  driver_leader.set_all_positions(
    {0.0, M_PI_2, M_PI_2, 0.0, 0.0, 0.0, 0.0},
    2.0f,
    true
  );
  driver_follower.set_all_modes(trossen_arm::Mode::position);
  driver_follower.set_all_positions(
    {0.0, M_PI_2, M_PI_2, 0.0, 0.0, 0.0, 0.0},
    2.0f,
    true
  );

  std::cout << "Moving to sleep positions..." << std::endl;
  driver_leader.set_all_modes(trossen_arm::Mode::position);
  driver_leader.set_all_positions(
    std::vector<float>(driver_leader.get_num_joints(), 0.0),
    2.0f,
    true
  );
  driver_follower.set_all_modes(trossen_arm::Mode::position);
  driver_follower.set_all_positions(
    std::vector<float>(driver_leader.get_num_joints(), 0.0),
    2.0f,
    true
  );

  return 0;
}
