=========
Changelog
=========

Upcoming
========

1.8.2
=====

Trossen Arm Driver
------------------

- Put the default tool frame at the tips of the fingers.
  It was at some point on the contacting surface previously.
- Added pre-run trajectory check.
  Now the driver by default samples 1000 points on the trajectory and checks if they have the corresponding inverse kinematics solutions.
  Check out :func:`trossen_arm::TrossenArmDriver::set_cartesian_positions` for more details.
- Added a demo for joint limits.
  It is available in :ref:`getting_started/demo_scripts:`set_joint_limits`_`.
- Fixed two issues related to mixed interpolation spaces and updated the demo :ref:`getting_started/demo_scripts:`mixed_interpolation_space`_`.

  - Using joint space commands immediately after Cartesian space commands no longer causes unexpected behavior. (`trossen_arm#78`_)
  - Using Cartesian space commands immediately after setting to the corresponding mode no longer causes unexpected behavior.

.. _`trossen_arm#78`: https://github.com/TrossenRobotics/trossen_arm/issues/78

.. warning:: Trossen Arm Driver :ref:`changelog:1.8.1` has been yanked due to potentially unsafe behavior on mode switching.

Trossen Arm Controller Firmware
-------------------------------

- Update default configurations to more reasonable values.

  - Increased :member:`trossen_arm::JointLimit::velocity_max` for the wrist joints from 2 :math:`\pi` rad/s to 3 :math:`\pi` rad/s.
  - Increased :member:`trossen_arm::PIDParameter::imax` in :enumerator:`trossen_arm::Mode::idle` mode for joint 1 from 9 Nm to 27 Nm so the arm can hold itself when extended horizontally.

1.8.1
=====

Trossen Arm Driver
------------------

- Updated mode switching logic to no longer skip joints whose mode did not change.

  This resolves `trossen_arm#71`_, which caused unexpected arm movement during mode transitions.

  The root cause was that the interpolation space resets to joint mode, requiring the interpolator to also reset.
  Without this, Cartesian values could be incorrectly interpreted as joint values.

.. warning:: Trossen Arm Driver :ref:`changelog:1.8.0` has been yanked due to potentially unsafe behavior on mode switching.

.. _`trossen_arm#71`: https://github.com/TrossenRobotics/trossen_arm/issues/71

Trossen Arm Controller Firmware
-------------------------------

- Skip limit checks in idle mode.

1.8.0
=====

Trossen Arm Driver
------------------

- Added a new :enumerator:`trossen_arm::Mode::effort` mode.
  It allows commanding the effort of the joints without any built-in compensation like in the :enumerator:`trossen_arm::Mode::external_effort` mode.
  This mode is useful for applications where full control of the joint efforts is desired.
- Grouped the outputs into a single new class :class:`trossen_arm::RobotOutput` and added the additional fields below:

  - :member:`trossen_arm::RobotOutput::Joint::All::compensation_efforts`
  - :member:`trossen_arm::RobotOutput::Joint::All::rotor_temperatures`
  - :member:`trossen_arm::RobotOutput::Joint::All::driver_temperatures`
  - :member:`trossen_arm::RobotOutput::Cartesian::positions`
  - :member:`trossen_arm::RobotOutput::Cartesian::velocities`
  - :member:`trossen_arm::RobotOutput::Cartesian::external_efforts`

- Added helpful output getters to extract the members of :class:`trossen_arm::RobotOutput`.

  - :func:`trossen_arm::TrossenArmDriver::get_all_positions`
  - :func:`trossen_arm::TrossenArmDriver::get_arm_positions`
  - :func:`trossen_arm::TrossenArmDriver::get_gripper_position`
  - :func:`trossen_arm::TrossenArmDriver::get_joint_position`
  - :func:`trossen_arm::TrossenArmDriver::get_cartesian_positions`
  - :func:`trossen_arm::TrossenArmDriver::get_all_velocities`
  - :func:`trossen_arm::TrossenArmDriver::get_arm_velocities`
  - :func:`trossen_arm::TrossenArmDriver::get_gripper_velocity`
  - :func:`trossen_arm::TrossenArmDriver::get_joint_velocity`
  - :func:`trossen_arm::TrossenArmDriver::get_cartesian_velocities`
  - :func:`trossen_arm::TrossenArmDriver::get_all_efforts`
  - :func:`trossen_arm::TrossenArmDriver::get_arm_efforts`
  - :func:`trossen_arm::TrossenArmDriver::get_gripper_effort`
  - :func:`trossen_arm::TrossenArmDriver::get_joint_effort`
  - :func:`trossen_arm::TrossenArmDriver::get_all_external_efforts`
  - :func:`trossen_arm::TrossenArmDriver::get_arm_external_efforts`
  - :func:`trossen_arm::TrossenArmDriver::get_gripper_external_effort`
  - :func:`trossen_arm::TrossenArmDriver::get_joint_external_effort`
  - :func:`trossen_arm::TrossenArmDriver::get_cartesian_external_efforts`
  - :func:`trossen_arm::TrossenArmDriver::get_all_compensation_efforts`
  - :func:`trossen_arm::TrossenArmDriver::get_arm_compensation_efforts`
  - :func:`trossen_arm::TrossenArmDriver::get_gripper_compensation_effort`
  - :func:`trossen_arm::TrossenArmDriver::get_joint_compensation_effort`
  - :func:`trossen_arm::TrossenArmDriver::get_all_rotor_temperatures`
  - :func:`trossen_arm::TrossenArmDriver::get_arm_rotor_temperatures`
  - :func:`trossen_arm::TrossenArmDriver::get_gripper_rotor_temperature`
  - :func:`trossen_arm::TrossenArmDriver::get_joint_rotor_temperature`
  - :func:`trossen_arm::TrossenArmDriver::get_all_driver_temperatures`
  - :func:`trossen_arm::TrossenArmDriver::get_arm_driver_temperatures`
  - :func:`trossen_arm::TrossenArmDriver::get_gripper_driver_temperature`
  - :func:`trossen_arm::TrossenArmDriver::get_joint_driver_temperature`

  The old getters are deprecated and will be removed in the next major release.

  - :func:`trossen_arm::TrossenArmDriver::get_positions`
  - :func:`trossen_arm::TrossenArmDriver::get_velocities`
  - :func:`trossen_arm::TrossenArmDriver::get_efforts`
  - :func:`trossen_arm::TrossenArmDriver::get_external_efforts`
  - :func:`trossen_arm::TrossenArmDriver::get_compensation_efforts`

- Added methods for commanding Cartesian inputs with respect to a tool frame.
  These features are demonstrated in the scripts listed below:

  - :ref:`getting_started/demo_scripts:`cartesian_position`_`
  - :ref:`getting_started/demo_scripts:`cartesian_velocity`_`
  - :ref:`getting_started/demo_scripts:`cartesian_external_effort`_`

- Added the capability to soft reboot the controller via the driver.
  This feature is demonstrated in :ref:`getting_started/demo_scripts:`set_factory_reset_flag`_`.

- Added more configurations and revised some previous ones.

  - Added

    - :ref:`getting_started/configuration:joint limits`
    - :ref:`getting_started/configuration:motor parameters`
    - :ref:`getting_started/configuration:algorithm parameter`
    - :member:`trossen_arm::EndEffector::pitch_circle_radius`
    - :member:`trossen_arm::EndEffector::t_flange_tool`

  - Removed

    - ``continuity_factor`` in :class:`trossen_arm::JointCharacteristic`
    - ``t_max_factor`` in :class:`trossen_arm::EndEffector`

- Made driver-controller connection more user-friendly with retry, timeout, and allowing interruption.

- For C++ users, ``double`` is now used instead of ``float`` for all the data types.
  This improves compatibility with other modern libraries.

Trossen Arm Controller Firmware
-------------------------------

- Added the associated interface for the new effort mode.
- Added rotor and driver temperatures to the output.
- Added the associated interface for soft reboot.
- Exposed the associated interfaces for the new configurations.
- Added checks on outputs according to the joint limits and on inputs for infinite values.
- Removed deprecated continuity check.
- The connection, disconnection, and data exchange is now handled more robustly.

  - UDP is only used for control and TCP handles the rest of the communication.
  - The controller now return to idle mode if the connection is lost.
  - The controller now only accept one driver at a time.

- Revised default configurations to more reasonable values, more specifically

  - :ref:`getting_started/configuration:joint limits`
  - :ref:`getting_started/configuration:motor parameters`
