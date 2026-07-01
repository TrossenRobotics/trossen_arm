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
// This script demonstrates how to get and set the joint kinematic properties of the arm.
// The joints vector contains every arm joint and both finger joints
// (size = number of joints + 1).

// Hardware setup:
// 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2

// The script does the following:
// 1. Initializes the driver
// 2. Configures the driver
// 3. Gets the current joints and prints them
// 4. Slightly perturbs the first joint's origin_xyz and sets the new joints
// 5. Prints the updated joints

#include <iostream>
#include <vector>

#include "libtrossen_arm/trossen_arm.hpp"

void print_joints(const std::vector<trossen_arm::Joint> & joints)
{
  std::cout << "Joint properties (" << joints.size() << " joints):" << std::endl;
  for (size_t i = 0; i < joints.size(); ++i) {
    const trossen_arm::Joint & joint = joints.at(i);
    std::cout << "  Joint " << i << ":" << std::endl;
    std::cout << "    axis:";
    for (double v : joint.axis) {
      std::cout << " " << v;
    }
    std::cout << std::endl;
    std::cout << "    origin_xyz:";
    for (double v : joint.origin_xyz) {
      std::cout << " " << v;
    }
    std::cout << std::endl;
    std::cout << "    origin_rpy:";
    for (double v : joint.origin_rpy) {
      std::cout << " " << v;
    }
    std::cout << std::endl;
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

  // Get and print the current joints
  std::vector<trossen_arm::Joint> joints = driver.get_joints();
  print_joints(joints);

  // Set new joints

  // Note: changing joint kinematic properties affects gravity compensation and
  // dynamics. Inaccurate values may degrade tracking performance.
  // The standard properties below are nominal CAD values for the WXAI V0 and only
  // approximate any given physical arm. Calibrate per-arm for best performance.

  // Option 1: use the dated standard joints
  driver.set_joints(trossen_arm::StandardJoints::wxai_v0_20260626);

  // // Option 2: use custom joints
  // joints.front().origin_xyz[2] += 0.001;
  // driver.set_joints(joints);

  // Get and print the new joints
  print_joints(driver.get_joints());

  return 0;
}
