============
Demo Scripts
============

This section describes the demo scripts that come with the Trossen Arm driver.

What You Need
=============

To get started, please make sure you have gone through the :doc:`configuration`.

Scripting Philosophy
====================

A high level overview of scripting with the Trossen Arm driver is given here.
The driver is designed to be flexible and easy to use for a wide range of applications.

.. tabs::

    .. code-tab:: c++

        // Include the header files
        #include "libtrossen_arm/trossen_arm.hpp"

        int main(int argc, char** argv)
        {
          // Create a driver object
          trossen_arm::TrossenArmDriver driver;

          // Configure the driver
          driver.configure(...);

          // Beginning of an action

          //   Get the modes of all joints if needed
          //   Here xxxs are the modes of all the joints where xxx can be
          //   - trossen_arm::Mode::position
          //   - trossen_arm::Mode::velocity
          //   - trossen_arm::Mode::external_effort
          //   - trossen_arm::Mode::effort
          auto xxxs = driver.get_modes();

          //   Set the mode[s] of the joint[s]
          //   Here yyy can be arm, gripper, all, or joint where
          //   - all includes all the joints
          //   - arm includes all joints but the gripper joint
          //   - gripper includes just the gripper joint
          //   - joint includes a specific zero-indexed joint
          driver.set_yyy_mode[s](xxx);

          //   Start moving the joint[s]

          //     Some logic

          //     Command the joint[s]
          //
          //     A joint command includes
          //     - goal[s]
          //     - time to reach the goal[s]
          //     - whether to block until reaching goal[s]
          //     - optionally the goal derivative[s]
          //     where yyy and zzz must be compatible with the mode set above
          //
          //     Alternatively, if the arm joints all have one of the following modes
          //     - trossen_arm::Mode::position
          //       pose of the tool frame measured in the base frame
          //     - trossen_arm::Mode::velocity
          //       linear and angular velocities of the tool frame measured in the base frame
          //     - trossen_arm::Mode::external_effort
          //       linear and angular efforts to be applied at the tool frame
          //       measured in the base frame while compensating for gravity and friction
          //     We can also command the arm joints to move in Cartesian space
          //     The Cartesian command includes an additional argument: interpolation space
          //     - trossen_arm::InterpolationSpace::joint
          //       Interpolate from start to goal state in joint space
          //     - trossen_arm::InterpolationSpace::cartesian
          //       Interpolate from start to goal state in Cartesian space
          driver.set_yyy_zzz[s](...); | driver.set_cartesian_zzzs(...);

          //     Get the robot outputs if needed
          //     The robot output includes
          //     - joint space states
          //       - positions
          //       - velocities
          //       - external_efforts
          //       - efforts
          //       - compensation_efforts
          //       - rotor_temperatures
          //       - driver_temperatures
          //     - Cartesian space states
          //       - positions
          //       - velocities
          //       - external_efforts
          trossen_arm::RobotOutput robot_output = driver.get_robot_output();

          //     Some more logic

          //   Stop moving the joint[s]

          // End of an action

          // More actions if needed
        }

    .. code-tab:: py

        # Import the driver
        import trossen_arm

        if __name__ == "__main__":
            # Create a driver object
            driver = trossen_arm.TrossenArmDriver()

            # Configure the driver
            driver.configure(...)

            # Beginning of an action

            #     Get the modes of all joints if needed
            #     Here xxxs are the modes of all the joints where xxx can be
            #     - trossen_arm.Mode.position
            #     - trossen_arm.Mode.velocity
            #     - trossen_arm.Mode.external_effort
            #     - trossen_arm.Mode.effort
            xxxs = driver.get_modes()

            #     Set the mode[s] of the joint[s]
            #     Here yyy can be arm, gripper, all, or joint where
            #     - all includes all the joints
            #     - arm includes all joints but the gripper joint
            #     - gripper includes just the gripper joint
            #     - joint includes a specific zero-indexed joint
            driver.set_yyy_mode[s](xxx)

            #     Start moving the joint[s]

            #         Some logic

            #         Command the joint[s]
            #
            #         A joint command includes
            #         - goal[s]
            #         - time to reach the goal[s]
            #         - whether to block until reaching goal[s]
            #         - optionally the goal derivative[s]
            #         where yyy and zzz must be compatible with the mode set above
            #
            #         Alternatively, if the arm joints all have one of the following modes
            #         - trossen_arm.Mode.position
            #           pose of the tool frame measured in the base frame
            #         - trossen_arm.Mode.velocity
            #           linear and angular velocities of the tool frame measured in the base frame
            #         - trossen_arm.Mode.external_effort
            #           linear and angular efforts to be applied at the tool frame
            #           measured in the base frame while compensating for gravity and friction
            #         We can also command the arm joints to move in Cartesian space
            #         The Cartesian command includes an additional argument: interpolation space
            #         - trossen_arm.InterpolationSpace.joint
            #           Interpolate from start to goal state in joint space
            #         - trossen_arm.InterpolationSpace.cartesian
            #           Interpolate from start to goal state in Cartesian space
            driver.set_yyy_zzz[s](...) | driver.set_cartesian_zzzs(...)

            #         Get the robot outputs if needed
            #         The robot output includes
            #         - joint space states
            #           - positions
            #           - velocities
            #           - external_efforts
            #           - efforts
            #           - compensation_efforts
            #           - rotor_temperatures
            #           - driver_temperatures
            #         - Cartesian space states
            #           - positions
            #           - velocities
            #           - external_efforts
            robot_output: trossen_arm.RobotOutput = driver.get_robot_output()

            #         Some more logic

            #     Stop moving the joint[s]

            # End of an action

            # More actions if needed

Demos
=====

After understanding the scripting philosophy, specific demos are provided to ground the concepts.
Demos of three levels of complexity are provided with the driver.

.. contents::
    :local:
    :depth: 2

Basics
------

The basic demos show the must-know functionalities to get the arm up and running.

`cartesian_position`_
^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to perform position control in Cartesian space.

`configure_cleanup`_
^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to configure and cleanup the driver.
This is useful for switching between different arms without creating a new driver object.
This script also demonstrates how to access the driver's states and configurations.

`gravity_compensation`_
^^^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to do gravity compensation.
This is useful for manually moving the arm to teach a trajectory or record specific positions.

`gripper_torque`_
^^^^^^^^^^^^^^^^^

This script demonstrates how to open and close the gripper.

`set_mode`_
^^^^^^^^^^^

This script demonstrates how to set the mode of the robot.

`simple_move`_
^^^^^^^^^^^^^^

This script demonstrates how to move a robot to different positions.

`mixed_interpolation_space`_
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This script tests transitions of the interpolation space.

Intermediate
------------

The intermediate demos give examples on commonly-used configurations and application-specific control loops.

`cartesian_external_effort`_
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to use external effort control in Cartesian space to do impedance control.

`cartesian_velocity`_
^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to use velocity control in Cartesian space to do admittance control.

`configuration_in_yaml`_
^^^^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to exchange persistent configurations via a YAML file.

`error_recovery_and_logging`_
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to recover from an error in the driver and how to use and modify the logging capabilities of the driver.

`move`_
^^^^^^^

This script demonstrates how to write a control loop to move the robot to different positions and record the states.

`move_two`_
^^^^^^^^^^^

This script demonstrates how to move two robots to different positions using interpolation.

`set_factory_reset_flag`_
^^^^^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to reset all configuration options to their default values.

`set_ip_method`_
^^^^^^^^^^^^^^^^

This script demonstrates how to set the IP method to DHCP or MANUAL.

`set_joint_limits`_
^^^^^^^^^^^^^^^^^^^

This script demonstrates how to set the joint limits of the arm.

`set_manual_ip`_
^^^^^^^^^^^^^^^^

This script demonstrates how to set the manual IP address.

`teleoperation`_
^^^^^^^^^^^^^^^^

This script demonstrates how to teleoperate the robots with force feedback.

Advanced
--------

The advanced demos show configurations that should be used with full understanding the implications.

`set_joint_characteristics`_
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to set the joint characteristics in the EEPROM, using the effort corrections as an example.

`joint_characteristics_finetune`_
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to finetune the joint characteristic of one joint.

.. _`cartesian_external_effort`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/cartesian_external_effort.py

.. _`cartesian_position`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/cartesian_position.py

.. _`cartesian_velocity`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/cartesian_velocity.py

.. _`configuration_in_yaml`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/configuration_in_yaml.py

.. _`configure_cleanup`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/configure_cleanup.py

.. _`error_recovery_and_logging`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/error_recovery_and_logging.py

.. _`joint_characteristics_finetune`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/joint_characteristics_finetune.py

.. _`gravity_compensation`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/gravity_compensation.py

.. _`gripper_torque`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/gripper_torque.py

.. _`mixed_interpolation_space`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/mixed_interpolation_space.py

.. _`move_two`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/move_two.py

.. _`move`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/move.py

.. _`set_factory_reset_flag`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/set_factory_reset_flag.py

.. _`set_ip_method`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/set_ip_method.py

.. _`set_joint_limits`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/set_joint_limits.py

.. _`set_manual_ip`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/set_manual_ip.py

.. _`set_mode`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/set_mode.py

.. _`set_joint_characteristics`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/set_joint_characteristics.py

.. _`simple_move`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/simple_move.py

.. _`teleoperation`: https://github.com/TrossenRobotics/trossen_arm/tree/main/demos/python/teleoperation.py

What's Next
===========

Hopefully, the provided demos have put you at a good starting point for developing your own applications.
For more details on the driver API, please refer to the :doc:`/api/library_root`.
