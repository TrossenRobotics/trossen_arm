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
#include <map>
#include <string>
#include <vector>

#include "libtrossen_arm/trossen_arm.hpp"

void print_states(trossen_arm::TrossenArmDriver& driver) {
  std::cout << "Number of joints: " << static_cast<int>(driver.get_num_joints()) << std::endl;
  std::cout << "EEPROM factory reset flag: " << driver.get_factory_reset_flag() << std::endl;
  std::cout << "EEPROM IP method: " << static_cast<int>(driver.get_ip_method()) << std::endl;
  std::cout << "EEPROM manual IP: " << driver.get_manual_ip() << std::endl;
  std::cout << "EEPROM DNS: " << driver.get_dns() << std::endl;
  std::cout << "EEPROM gateway: " << driver.get_gateway() << std::endl;
  std::cout << "EEPROM subnet: " << driver.get_subnet() << std::endl;
  std::vector<trossen_arm::JointCharacteristic> joint_characteristics =
    driver.get_joint_characteristics();
  std::cout << "EEPROM Joint characteristics:" << std::endl;
  for (size_t i = 0; i < joint_characteristics.size(); ++i) {
    const trossen_arm::JointCharacteristic& joint_characteristic = joint_characteristics.at(i);
    std::cout << "  Joint " << i << ":" << std::endl;
    std::cout <<
      "    Effort correction: " <<
      joint_characteristic.effort_correction <<
      std::endl;
    std::cout <<
      "    Friction constant term: " <<
      joint_characteristic.friction_constant_term <<
      std::endl;
    std::cout <<
      "    Friction transition velocity: " <<
      joint_characteristic.friction_transition_velocity <<
      std::endl;
    std::cout <<
      "    Friction coulomb coefficient: " <<
      joint_characteristic.friction_coulomb_coef <<
      std::endl;
    std::cout <<
      "    Friction viscous coefficient: " <<
      joint_characteristic.friction_viscous_coef <<
      std::endl;
    std::cout <<
      "    Position offset: " <<
      joint_characteristic.position_offset <<
      std::endl;
  }
  std::cout << "Error information: " << driver.get_error_information() << std::endl;
  std::cout << "Modes: ";
  for (trossen_arm::Mode mode : driver.get_modes()) {
    std::cout << static_cast<int>(mode) << " ";
  }
  std::cout << std::endl;
  trossen_arm::EndEffector end_effector = driver.get_end_effector();
  std::cout << "End effector:" << std::endl;
  std::cout << "  palm:" << std::endl;
  std::cout << "    mass: " << end_effector.palm.mass << std::endl;
  std::cout << "    inertia: ";
  for (double inertia : end_effector.palm.inertia) {
    std::cout << inertia << " ";
  }
  std::cout << std::endl;
  std::cout << "    origin xyz: ";
  for (double origin_xyz : end_effector.palm.origin_xyz) {
    std::cout << origin_xyz << " ";
  }
  std::cout << std::endl;
  std::cout << "    origin rpy: ";
  for (double origin_rpy : end_effector.palm.origin_rpy) {
    std::cout << origin_rpy << " ";
  }
  std::cout << std::endl;
  std::cout << "  finger left:" << std::endl;
  std::cout << "    mass: " << end_effector.finger_left.mass << std::endl;
  std::cout << "    inertia: ";
  for (double inertia : end_effector.finger_left.inertia) {
    std::cout << inertia << " ";
  }
  std::cout << std::endl;
  std::cout << "    origin xyz: ";
  for (double origin_xyz : end_effector.finger_left.origin_xyz) {
    std::cout << origin_xyz << " ";
  }
  std::cout << std::endl;
  std::cout << "    origin rpy: ";
  for (double origin_rpy : end_effector.finger_left.origin_rpy) {
    std::cout << origin_rpy << " ";
  }
  std::cout << std::endl;
  std::cout << "  finger right:" << std::endl;
  std::cout << "    mass: " << end_effector.finger_right.mass << std::endl;
  std::cout << "    inertia: ";
  for (double inertia : end_effector.finger_right.inertia) {
    std::cout << inertia << " ";
  }
  std::cout << std::endl;
  std::cout << "    origin xyz: ";
  for (double origin_xyz : end_effector.finger_right.origin_xyz) {
    std::cout << origin_xyz << " ";
  }
  std::cout << std::endl;
  std::cout << "    origin rpy: ";
  for (double origin_rpy : end_effector.finger_right.origin_rpy) {
    std::cout << origin_rpy << " ";
  }
  std::cout << std::endl;
  std::cout << "  offset finger left: " << end_effector.offset_finger_left << std::endl;
  std::cout << "  offset finger right: " << end_effector.offset_finger_right << std::endl;
  std::cout << "  pitch circle radius: " << end_effector.pitch_circle_radius << std::endl;
  std::cout << "  t flange tool: ";
  for (double t_flange_tool : end_effector.t_flange_tool) {
    std::cout << t_flange_tool << " ";
  }
  std::cout << std::endl;
  std::vector<trossen_arm::JointLimit> joint_limits = driver.get_joint_limits();
  std::cout << "Joint limits:" << std::endl;
  for (size_t i = 0; i < joint_limits.size(); ++i) {
    const trossen_arm::JointLimit& joint_limit = joint_limits.at(i);
    std::cout << "  Joint " << i << ":" << std::endl;
    std::cout << "    position min: " << joint_limit.position_min << std::endl;
    std::cout << "    position max: " << joint_limit.position_max << std::endl;
    std::cout << "    position tolerance: " << joint_limit.position_tolerance << std::endl;
    std::cout << "    velocity max: " << joint_limit.velocity_max << std::endl;
    std::cout << "    velocity tolerance: " << joint_limit.velocity_tolerance << std::endl;
    std::cout << "    effort max: " << joint_limit.effort_max << std::endl;
    std::cout << "    effort tolerance: " << joint_limit.effort_tolerance << std::endl;
  }
  std::vector<std::map<trossen_arm::Mode, trossen_arm::MotorParameter>> motor_parameters =
    driver.get_motor_parameters();
  std::cout << "Motor parameters:" << std::endl;
  for (size_t i = 0; i < motor_parameters.size(); ++i) {
    const std::map<trossen_arm::Mode, trossen_arm::MotorParameter>& motor_parameter =
      motor_parameters.at(i);
    std::cout << "  Joint " << i << ":" << std::endl;
    for (const auto& [mode, parameter] : motor_parameter) {
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
  trossen_arm::RobotOutput robot_output = driver.get_robot_output();
  std::cout << "Robot output:" << std::endl;
  std::cout << "  Header:" << std::endl;
  std::cout << "    id: " << robot_output.header.id << std::endl;
  std::cout << "    timestamp: " << robot_output.header.timestamp << std::endl;
  std::cout << "  Joint outputs:" << std::endl;
  std::cout << "    positions: ";
  for (double position : robot_output.joint.all.positions) {
    std::cout << position << " ";
  }
  std::cout << std::endl;
  std::cout << "    velocities: ";
  for (double velocity : robot_output.joint.all.velocities) {
    std::cout << velocity << " ";
  }
  std::cout << std::endl;
  std::cout << "    efforts: ";
  for (double effort : robot_output.joint.all.efforts) {
    std::cout << effort << " ";
  }
  std::cout << std::endl;
  std::cout << "    external efforts: ";
  for (double external_effort : robot_output.joint.all.external_efforts) {
    std::cout << external_effort << " ";
  }
  std::cout << std::endl;
  std::cout << "    compensation efforts: ";
  for (double compensation_effort : robot_output.joint.all.compensation_efforts) {
    std::cout << compensation_effort << " ";
  }
  std::cout << std::endl;
  std::cout << "    driver temperatures: ";
  for (double driver_temperature : robot_output.joint.all.driver_temperatures) {
    std::cout << driver_temperature << " ";
  }
  std::cout << std::endl;
  std::cout << "    rotor temperatures: ";
  for (double rotor_temperature : robot_output.joint.all.rotor_temperatures) {
    std::cout << rotor_temperature << " ";
  }
  std::cout << std::endl;
  std::cout << "  Cartesian outputs:" << std::endl;
  std::cout << "    positions: ";
  for (double position : robot_output.cartesian.positions) {
    std::cout << position << " ";
  }
  std::cout << std::endl;
  std::cout << "    velocities: ";
  for (double velocity : robot_output.cartesian.velocities) {
    std::cout << velocity << " ";
  }
  std::cout << std::endl;
  std::cout << "    accelerations: ";
  for (double acceleration : robot_output.cartesian.accelerations) {
    std::cout << acceleration << " ";
  }
  std::cout << std::endl;
  std::cout << "    external efforts: ";
  for (double external_effort : robot_output.cartesian.external_efforts) {
    std::cout << external_effort << " ";
  }
  std::cout << std::endl;
  trossen_arm::AlgorithmParameter algorithm_parameter = driver.get_algorithm_parameter();
  std::cout << "Algorithm parameter:" << std::endl;
  std::cout << "  singularity threshold: ";
  std::cout << algorithm_parameter.singularity_threshold << std::endl;
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
    false
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
