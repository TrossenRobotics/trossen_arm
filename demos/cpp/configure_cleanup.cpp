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
// This script demonstrates how to configure and cleanup the driver, which is
// useful for switching between different arms without creating a new driver
// object. And this script also demonstrates how to access the driver's states
// and configurations.
//
// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2
// 2. A WXAI V0 arm with follower end effector and ip at 192.168.1.3
//
// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver for one arm
// 3. Prints the current state of the driver
// 4. Cleans up the driver explicitly
// 5. Configures the driver for another arm
// 6. Prints the current state of the driver
// 7. The driver automatically sets the mode to idle at the destructor

#include <iostream>
#include <string>

#include "libtrossen_arm/trossen_arm.hpp"

void print_states(trossen_arm::TrossenArmDriver& driver) {
  std::cout << "Number of joints: " << static_cast<int>(driver.get_num_joints()) << std::endl;
  std::cout << "EEPROM factory reset flag: " << driver.get_factory_reset_flag() << std::endl;
  std::cout << "EEPROM IP method: " << static_cast<int>(driver.get_ip_method()) << std::endl;
  std::cout << "EEPROM manual IP: " << driver.get_manual_ip() << std::endl;
  std::cout << "EEPROM DNS: " << driver.get_dns() << std::endl;
  std::cout << "EEPROM gateway: " << driver.get_gateway() << std::endl;
  std::cout << "EEPROM subnet: " << driver.get_subnet() << std::endl;
  std::cout << "EEPROM effort corrections: ";
  for (float effort_correction : driver.get_effort_correction()) {
    std::cout << effort_correction << " ";
  }
  std::cout << std::endl;
  std::cout << "Error information: " << driver.get_error_information() << std::endl;
  std::cout << "Modes: ";
  for (trossen_arm::Mode mode : driver.get_modes()) {
    std::cout << static_cast<int>(mode) << " ";
  }
  std::cout << std::endl;
  std::cout << "End effector:" << std::endl;
  std::cout << "  palm:" << std::endl;
  std::cout << "    mass: " << driver.get_end_effector().palm.mass << std::endl;
  std::cout << "    inertia: ";
  for (float inertia : driver.get_end_effector().palm.inertia) {
    std::cout << inertia << " ";
  }
  std::cout << std::endl;
  std::cout << "    origin xyz: ";
  for (float origin_xyz : driver.get_end_effector().palm.origin_xyz) {
    std::cout << origin_xyz << " ";
  }
  std::cout << std::endl;
  std::cout << "    origin rpy: ";
  for (float origin_rpy : driver.get_end_effector().palm.origin_rpy) {
    std::cout << origin_rpy << " ";
  }
  std::cout << std::endl;
  std::cout << "  finger left:" << std::endl;
  std::cout << "    mass: " << driver.get_end_effector().finger_left.mass << std::endl;
  std::cout << "    inertia: ";
  for (float inertia : driver.get_end_effector().finger_left.inertia) {
    std::cout << inertia << " ";
  }
  std::cout << std::endl;
  std::cout << "    origin xyz: ";
  for (float origin_xyz : driver.get_end_effector().finger_left.origin_xyz) {
    std::cout << origin_xyz << " ";
  }
  std::cout << std::endl;
  std::cout << "    origin rpy: ";
  for (float origin_rpy : driver.get_end_effector().finger_left.origin_rpy) {
    std::cout << origin_rpy << " ";
  }
  std::cout << std::endl;
  std::cout << "  finger right:" << std::endl;
  std::cout << "    mass: " << driver.get_end_effector().finger_right.mass << std::endl;
  std::cout << "    inertia: ";
  for (float inertia : driver.get_end_effector().finger_right.inertia) {
    std::cout << inertia << " ";
  }
  std::cout << std::endl;
  std::cout << "    origin xyz: ";
  for (float origin_xyz : driver.get_end_effector().finger_right.origin_xyz) {
    std::cout << origin_xyz << " ";
  }
  std::cout << std::endl;
  std::cout << "    origin rpy: ";
  for (float origin_rpy : driver.get_end_effector().finger_right.origin_rpy) {
    std::cout << origin_rpy << " ";
  }
  std::cout << std::endl;
  std::cout << "  offset finger left: " << driver.get_end_effector().offset_finger_left << std::endl;
  std::cout << "  offset finger right: " << driver.get_end_effector().offset_finger_right << std::endl;
  std::cout << "Positions: ";
  for (float position : driver.get_positions()) {
    std::cout << position << " ";
  }
  std::cout << std::endl;
  std::cout << "Velocities: ";
  for (float velocity : driver.get_velocities()) {
    std::cout << velocity << " ";
  }
  std::cout << std::endl;
  std::cout << "Efforts: ";
  for (float effort : driver.get_efforts()) {
    std::cout << effort << " ";
  }
  std::cout << std::endl;
  std::cout << "External efforts: ";
  for (float external_effort : driver.get_external_efforts()) {
    std::cout << external_effort << " ";
  }
  std::cout << std::endl;
}


int main(int argc, char** argv)
{
  // Initialize the driver
  trossen_arm::TrossenArmDriver driver;

  // Configure the driver for one arm
  driver.configure(
    trossen_arm::Model::wxai_v0,
    trossen_arm::StandardEndEffector::wxai_v0_leader,
    "192.168.1.2",
    true
  );

  // Print the current states of the driver
  print_states(driver);

  // Cleanup the driver
  driver.cleanup();

  // Configure the driver for another arm
  driver.configure(
    trossen_arm::Model::wxai_v0,
    trossen_arm::StandardEndEffector::wxai_v0_follower,
    "192.168.1.3",
    false
  );

  // Print the current states of the driver
  print_states(driver);

  return 0;
}
