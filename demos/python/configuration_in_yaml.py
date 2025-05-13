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
# This script demonstrates how to exchange persistent configurations via a YAML
# file.

# Hardware setup:
# 1. A WXAI V0 arm with leader end effector and ip at 192.168.1.2

# The script does the following:
# 1. Initializes the driver
# 2. Configures the driver with the leader configuration
# 3. Stores the configuration in a YAML file
# 4. Loads the configuration from the YAML file
# 5. Power cycle to apply the new configurations

import trossen_arm


def print_configurations(driver: trossen_arm.TrossenArmDriver):
    print("EEPROM factory reset flag:", driver.get_factory_reset_flag())
    print("EEPROM IP method:", driver.get_ip_method())
    print("EEPROM manual IP:", driver.get_manual_ip())
    print("EEPROM DNS:", driver.get_dns())
    print("EEPROM gateway:", driver.get_gateway())
    print("EEPROM subnet:", driver.get_subnet())
    print("EEPROM effort corrections:", driver.get_effort_corrections())
    print(
        "EEPROM friction transition velocities:",
        driver.get_friction_transition_velocities()
    )
    print(
        "EEPROM friction constant terms:",
        driver.get_friction_constant_terms()
    )
    print("EEPROM friction coulomb coefs:", driver.get_friction_coulomb_coefs())
    print("EEPROM friction viscous coefs:", driver.get_friction_viscous_coefs())
    print("Modes:", [mode.value for mode in driver.get_modes()])

    end_effector = driver.get_end_effector()
    print("End effector:")
    print("  palm:")
    print("    mass:", end_effector.palm.mass)
    print("    inertia:", end_effector.palm.inertia)
    print("    origin xyz:", end_effector.palm.origin_xyz)
    print("    origin rpy:", end_effector.palm.origin_rpy)
    print("  finger left:")
    print("    mass:", end_effector.finger_left.mass)
    print("    inertia:", end_effector.finger_left.inertia)
    print("    origin xyz:", end_effector.finger_left.origin_xyz)
    print("    origin rpy:", end_effector.finger_left.origin_rpy)
    print("  finger right:")
    print("    mass:", end_effector.finger_right.mass)
    print("    inertia:", end_effector.finger_right.inertia)
    print("    origin xyz:", end_effector.finger_right.origin_xyz)
    print("    origin rpy:", end_effector.finger_right.origin_rpy)
    print("  offset finger left:", end_effector.offset_finger_left)
    print("  offset finger right:", end_effector.offset_finger_right)
    print("  pitch circle radius:", end_effector.pitch_circle_radius)
    print("  t flange tool:", end_effector.t_flange_tool)

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

    motor_parameters = driver.get_motor_parameters()
    print("Motor parameters:")
    for i, motor_param in enumerate(motor_parameters):
        print(f"  Joint {i}:")
        for mode, param in motor_param.items():
            print(f"    Mode {mode.value}:")
            print("      Position loop:")
            print(
                f"        kp: {param.position.kp}, ki: {param.position.ki}, "
                f"kd: {param.position.kd}, imax: {param.position.imax}"
            )
            print("      Velocity loop:")
            print(
                f"        kp: {param.velocity.kp}, ki: {param.velocity.ki}, "
                f"kd: {param.velocity.kd}, imax: {param.velocity.imax}"
            )

    algorithm_parameter = driver.get_algorithm_parameter()
    print("Algorithm parameter:")
    print("  singularity threshold:", algorithm_parameter.singularity_threshold)

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

    # Print the configurations
    print("Initial configurations:")
    print_configurations(driver)

    # Store the configurations in a YAML file
    driver.save_configs_to_file("configurations.yaml")

    # Load the configurations from the YAML file
    driver.load_configs_from_file("configurations.yaml")

    # Print the configurations
    print("Configurations after loading from the YAML file:")
    print_configurations(driver)
