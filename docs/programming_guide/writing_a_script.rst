================
Writing a Script
================

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
          //
          //     To avoid numerical issues, interpolation type used is determined automatically
          //     based on the time to reach the goal[s]
          //     - if longer than 0.2s, quintic polynomial interpolation is used for
          //       position commands, cubic interpolation is used for velocity commands, and
          //       linear interpolation is used for effort and external effort commands, with
          //       goal derivatives defaulted to zero if not specified
          //     - else if longer than 0.001s, linear interpolation is used
          //     - else, no interpolation is used and the goal values are applied immediately
          driver.set_yyy_zzz[s](...); | driver.set_cartesian_zzzs(...);

          //     Get the robot outputs if needed
          //     The robot output includes
          //     - header
          //       - id
          //       - timestamp
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
            #
            #         To avoid numerical issues, interpolation type used is determined automatically
            #         based on the time to reach the goal[s]
            #         - if longer than 0.2s, quintic polynomial interpolation is used for
            #           position commands, cubic interpolation is used for velocity commands, and
            #           linear interpolation is used for effort and external effort commands, with
            #           goal derivatives defaulted to zero if not specified
            #         - else if longer than 0.001s, linear interpolation is used
            #         - else, no interpolation is used and the goal values are applied immediately
            driver.set_yyy_zzz[s](...) | driver.set_cartesian_zzzs(...)

            #         Get the robot outputs if needed
            #         The robot output includes
            #         - header
            #           - id
            #           - timestamp
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

What's Next
===========

Now that you understand how a script is structured, look up the methods you need in the :doc:`/api/library_root`.
For complete, runnable examples, see the :doc:`/getting_started/demo_scripts`.
