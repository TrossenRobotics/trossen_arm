# Copyright 2025 Trossen Robotics
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#
#    * Neither the name of the copyright holder nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# Purpose:
# This script demonstrates how to set the joint limits of the arm.

# Hardware setup:
# 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2

# The script does the following:
# 1. Initializes the driver
# 2. Configures the driver for one arm
# 3. Prints the current joint limits of the robot
# 4. Increases the velocity max of the arm joints
# 5. Disables the velocity limit of the gripper joint
# 6. Sets the new joint limits to the driver
# 7. Prints the new joint limits of the robot

import trossen_arm


def print_joint_limits(driver: trossen_arm.TrossenArmDriver):
    joint_limits = driver.get_joint_limits()
    print("Joint limits:")
    for i, joint_limit in enumerate(joint_limits):
        print(f"  Joint {i}:")
        print("    position min:", joint_limit.position_min)
        print("    position max:", joint_limit.position_max)
        print("    position tolerance:", joint_limit.position_tolerance)
        print("    velocity max:", joint_limit.velocity_max)
        print("    velocity tolerance:", joint_limit.velocity_tolerance)
        print("    effort max:", joint_limit.effort_max)
        print("    effort tolerance:", joint_limit.effort_tolerance)

if __name__=='__main__':
    # Initialize the driver
    driver = trossen_arm.TrossenArmDriver()

    # Configure the driver for one arm
    driver.configure(
        trossen_arm.Model.wxai_v0,
        trossen_arm.StandardEndEffector.wxai_v0_leader,
        "192.168.1.2",
        False
    )

    # Print the current joint limits of the robot
    print_joint_limits(driver)

    # Get the current joint limits of the robot
    joint_limits = driver.get_joint_limits()

    # Increase the velocity max of the arm joints
    for i in range(len(joint_limits) - 1):
        joint_limits[i].velocity_max *= 1.2

    # Disable the velocity limit of the gripper joint
    # Increasing the gripper's velocity max will trigger an error as it's at the motor's max already
    joint_limits[-1].velocity_tolerance = joint_limits[-1].velocity_max * 0.2

    # Set the new joint limits to the driver
    driver.set_joint_limits(joint_limits)

    # Print the new joint limits of the robot
    # These values are kept until the controller is powered off
    print_joint_limits(driver)
