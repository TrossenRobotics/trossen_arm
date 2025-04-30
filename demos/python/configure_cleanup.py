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

'''
Purpose:
This script demonstrates how to configure and cleanup the driver, which is useful for
switching between different arms without creating a new driver object.

Also, this script demonstrates how to access the driver's states and configurations.

Hardware setup:
1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2
2. A WXAI V0 arm with follower end effector and ip at 192.168.1.3

The script does the following:
1. Initializes the driver
2. Configures the driver for one arm
3. Prints the current state of the driver
4. Cleans up the driver explicitly
5. Configures the driver for another arm
6. Prints the current state of the driver
7. The driver automatically sets the mode to idle at the destructor
'''

import trossen_arm


def print_states(driver: trossen_arm.TrossenArmDriver):
    print("Number of joints: ", driver.get_num_joints())
    print("EEPROM factory reset flag: ", driver.get_factory_reset_flag())
    print("EEPROM IP method: ", driver.get_ip_method())
    print("EEPROM manual IP: ", driver.get_manual_ip())
    print("EEPROM DNS: ", driver.get_dns())
    print("EEPROM gateway: ", driver.get_gateway())
    print("EEPROM subnet: ", driver.get_subnet())
    print("EEPROM effort corrections: ", driver.get_effort_corrections())
    print("EEPROM friction transition velocities: ", driver.get_friction_transition_velocities())
    print("EEPROM friction constant terms: ", driver.get_friction_constant_terms())
    print("EEPROM friction coulomb coefs: ", driver.get_friction_coulomb_coefs())
    print("EEPROM friction viscous coefs: ", driver.get_friction_viscous_coefs())
    print("EEPROM continuity factors: ", driver.get_continuity_factors())
    print("Error information: ", driver.get_error_information())
    print("Modes: ", driver.get_modes())
    print("End effector:")
    print("  palm:")
    print("    mass: ", driver.get_end_effector().palm.mass)
    print("    inertia: ", driver.get_end_effector().palm.inertia)
    print("    origin xyz: ", driver.get_end_effector().palm.origin_xyz)
    print("    origin rpy: ", driver.get_end_effector().palm.origin_rpy)
    print("  finger left:")
    print("    mass: ", driver.get_end_effector().finger_left.mass)
    print("    inertia: ", driver.get_end_effector().finger_left.inertia)
    print("    origin xyz: ", driver.get_end_effector().finger_left.origin_xyz)
    print("    origin rpy: ", driver.get_end_effector().finger_left.origin_rpy)
    print("  finger right:")
    print("    mass: ", driver.get_end_effector().finger_right.mass)
    print("    inertia: ", driver.get_end_effector().finger_right.inertia)
    print("    origin xyz: ", driver.get_end_effector().finger_right.origin_xyz)
    print("    origin rpy: ", driver.get_end_effector().finger_right.origin_rpy)
    print("  offset finger left: ", driver.get_end_effector().offset_finger_left)
    print("  offset finger right: ", driver.get_end_effector().offset_finger_right)
    print("Positions: ", driver.get_positions())
    print("Velocities: ", driver.get_velocities())
    print("Efforts: ", driver.get_efforts())
    print("External efforts: ", driver.get_external_efforts())

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

    # Print the current state of the driver
    print_states(driver)

    # Clean up the driver
    driver.cleanup()

    # Configure the driver again
    driver.configure(
        trossen_arm.Model.wxai_v0,
        trossen_arm.StandardEndEffector.wxai_v0_follower,
        "192.168.1.3",
        False
    )

    # Print the current state of the driver
    print_states(driver)
