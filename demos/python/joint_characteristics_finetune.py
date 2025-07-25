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
# This script is used to finetune the joint characteristic of one joint.

# Hardware setup:
# 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2

# The script does the following:
# 1. Initializes the driver
# 2. Configures the driver
# 3. Sets the joint mode to external effort
# 4. Sets the joint external effort to 0
# 5. Gets the joint characteristics
# 6. Prints the joint characteristics
# 7. Waits for the user to enter an increment
# 8. Increments the joint characteristic
# 9. Sets the joint characteristic
# 10. Repeats steps 5-9 until the user presses Ctrl+C

import trossen_arm

if __name__=='__main__':
    # Initialize the driver
    driver = trossen_arm.TrossenArmDriver()

    # Configure the driver
    driver.configure(
        trossen_arm.Model.wxai_v0,
        trossen_arm.StandardEndEffector.wxai_v0_leader,
        "192.168.1.2",
        False
    )

    # Change this to the 0-based joint index you want to finetune
    INDEX = 0

    modes = [trossen_arm.Mode.idle] * driver.get_num_joints()
    modes[INDEX] = trossen_arm.Mode.external_effort
    driver.set_joint_modes(modes)
    driver.set_joint_external_effort(
        INDEX,
        0.0,
        False
    )

    print("Recommended increment:")
    print("1) start with very small value like 1e-3")
    print(
        "2) multiply the INCREMENT (NOT THE JOINT CHARACTERISTIC) "
        "by factor of 2 until there's noticeable behavioral change "
        "when applying the increment"
    )
    print("3) use this increment to adjust the joint characteristic")

    while True:
        try:
            joint_characteristics = driver.get_joint_characteristics()

            # print("Effort correction:")
            # print(joint_characteristics[INDEX].effort_correction)

            print("Friction constant term:")
            print(joint_characteristics[INDEX].friction_constant_term)

            # print("Friction transition velocity:")
            # print(joint_characteristics[INDEX].friction_transition_velocity)

            # print("Friction coulomb coefficient:")
            # print(joint_characteristics[INDEX].friction_coulomb_coef)

            # print("Friction viscous coefficient:")
            # print(joint_characteristics[INDEX].friction_viscous_coef)

            increment = input(
                "Enter the increment "
                "(default 0.0, positive to increase, negative to decrease): "
            )
            if increment == "":
                increment = 0.0
            else:
                increment = float(increment)

            # joint_characteristics[INDEX].effort_correction += increment

            joint_characteristics[INDEX].friction_constant_term += increment

            # joint_characteristics[INDEX].friction_transition_velocity += increment

            # joint_characteristics[INDEX].friction_coulomb_coef += increment

            # joint_characteristics[INDEX].friction_viscous_coef += increment

            driver.set_joint_characteristics(joint_characteristics)
        except KeyboardInterrupt:
            break
