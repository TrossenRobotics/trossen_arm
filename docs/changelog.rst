=========
Changelog
=========

Trossen Arm Driver
==================

1.9.0
-----

- Moved :ref:`getting_started/configuration:position_offset` to EEPROM so it persists across power cycles as other joint characteristics.
- Added :class:`trossen_arm::RobotOutput::Header` that contains :class:`trossen_arm::RobotOutput::Header::id` and :class:`trossen_arm::RobotOutput::Header::timestamp`.

1.8.8
-----

- Warned instead of throwing an exception at calling :func:`trossen_arm::TrossenArmDriver::set_end_effector` when the gripper joint is in position mode.

1.8.7
-----

- Added a temporary implementation for :ref:`getting_started/configuration:position_offset`.
  Different from its fellow joint characteristics, it currently initializes to zeros at driver configuration and resets at cleanup.
  This peculiarity will be addressed in the next minor update.
- Threw exceptions when setting configurations that may cause sudden movements while joints are in position mode.
  The following methods are affected:

  - :func:`trossen_arm::TrossenArmDriver::set_joint_characteristics` and all its helper methods

    - :func:`trossen_arm::TrossenArmDriver::set_effort_corrections`
    - :func:`trossen_arm::TrossenArmDriver::set_friction_constant_terms`
    - :func:`trossen_arm::TrossenArmDriver::set_friction_coulomb_coefs`
    - :func:`trossen_arm::TrossenArmDriver::set_friction_transition_velocities`
    - :func:`trossen_arm::TrossenArmDriver::set_friction_viscous_coefs`
    - :func:`trossen_arm::TrossenArmDriver::set_position_offsets`

  - :func:`trossen_arm::TrossenArmDriver::set_end_effector`

1.8.6
-----

- Fixed a deadlock introduced in 1.8.4 with the new logging system.
  Please update to this version if the driver hangs when throwing an exception in Python.
- Added a new demo :ref:`getting_started/demo_scripts:`gravity_compensation_partial`_` demonstrating how to compensate for a portion of the gravity.
- Relaxed NumPy requirement from ``>= 2.0.0`` to ``>= 1.22.4``.

1.8.5
-----

- Fixed an issue in the C++ driver where some of the configuration getters would fail due to internal logic errors.

1.8.4
-----

- Added the :func:`trossen_arm::TrossenArmDriver::get_is_configured` method that returns whether the driver is configured or not.
- C++ libraries are now built on Ubuntu 20.04 to maintain compatibility with older Linux distributions.
- `spdlog <https://github.com/gabime/spdlog>`_ is now used for logging for the C++ library and its capabilities are exposed to Python for use with `the Python logging module <https://docs.python.org/3/library/logging.html>`_.
  This allows users to configure the driver's logging level, format, and preferred sinks.
  See the :ref:`getting_started/demo_scripts:`error_recovery_and_logging`_` demo for usage.
- Added a demo for finetuning joint characteristics.
  It is available in :ref:`getting_started/demo_scripts:`joint_characteristics_finetune`_`.

1.8.3
-----

- Added new models :enumerator:`trossen_arm::Model::vxai_v0_left` and :enumerator:`trossen_arm::Model::vxai_v0_right`.
  These models are 7-DOF arms to be released in the future.
  All features except for the Cartesian space inputs are supported.
- Adjusted UDP message lost logging behavior to avoid flooding the terminal.
  The driver now only warns when there are more than 1000 messages lost among each 5000 messages.
- Added initial implementation for custom end effectors.
  The original rack-and-pinion gripper can be removed or replaced with a custom end effector.
  For more details, see :ref:`getting_started/configuration:end effector`.

1.8.2
-----

- Moved the default tool frame from a point on the contacting surfaces to the tips of the fingers.
- Added pre-run trajectory check to :func:`trossen_arm::TrossenArmDriver::set_cartesian_positions`.
  Now the driver by default samples 1000 points on the trajectory and checks if they have the corresponding inverse kinematics solutions before moving.
- Added a demo for configuring joint limits.
  It is available in :ref:`getting_started/demo_scripts:`set_joint_limits`_`.
- Fixed two issues related to mixed interpolation spaces and updated the demo :ref:`getting_started/demo_scripts:`mixed_interpolation_space`_`.

  - Using joint space commands immediately after Cartesian space commands no longer causes unexpected behavior. (`trossen_arm#78`_)
  - Using Cartesian space commands immediately after setting to the corresponding mode no longer causes unexpected behavior.

.. _`trossen_arm#78`: https://github.com/TrossenRobotics/trossen_arm/issues/78

.. warning:: Trossen Arm Driver :ref:`changelog:1.8.1` has been yanked due to potentially unsafe behavior on mode switching.

1.8.1
-----

- Updated mode switching logic to no longer skip joints whose mode did not change.

  This resolves `trossen_arm#71`_, which caused unexpected arm movement during mode transitions.

  The root cause was that the interpolation space resets to joint mode, requiring the interpolator to also reset.
  Without this, Cartesian values could be incorrectly interpreted as joint values.

.. warning:: Trossen Arm Driver :ref:`changelog:1.8.0` has been yanked due to potentially unsafe behavior on mode switching.

.. _`trossen_arm#71`: https://github.com/TrossenRobotics/trossen_arm/issues/71

1.8.0
-----

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
===============================

1.9.1
-----

- Fixed an issue introduced in v1.9.0 where the controller would throw an error if the gripper was not detected.
  The controller now behaves as in previous versions if the gripper is not used.

1.9.0
-----

- Moved :ref:`getting_started/configuration:position_offset` to EEPROM so it persists across power cycles as other joint characteristics.
- Added :class:`trossen_arm::RobotOutput::Header` that contains :class:`trossen_arm::RobotOutput::Header::id` and :class:`trossen_arm::RobotOutput::Header::timestamp`.
- In multi-motor joints, following motors copy leading motor's command instead of running independently.
- Bug fixes for multi-motor joint logic.

1.8.4
-----

- Fixed an issue where, when the gripper was not used, NaN external effort output was reported for the joint.
  External effort output is now fixed at 0 for the gripper joint when the gripper is not used.

1.8.3
-----

- Refactored the model definition to reduce repeated logic for different motors.
- Added the new models.
- Allowed removing the original rack-and-pinion end effector or replacing it with a custom one.

1.8.2
-----

- Update default configurations to more reasonable values.

  - Increased :member:`trossen_arm::JointLimit::velocity_max` for the wrist joints from 2 :math:`\pi` rad/s to 3 :math:`\pi` rad/s.
  - Increased :member:`trossen_arm::PIDParameter::imax` in :enumerator:`trossen_arm::Mode::idle` mode for joint 1 from 9 Nm to 27 Nm so the arm can hold itself when extended horizontally.

1.8.1
-----

- Skip limit checks in idle mode.

1.8.0
-----

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
