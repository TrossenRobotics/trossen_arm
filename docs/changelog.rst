=========
Changelog
=========

Upcoming
========

1.7 --> 1.8
===========

Trossen Arm Driver
------------------

- Added a new :enumerator:`trossen_arm::Mode::effort` mode.
  It allows commanding the effort of the joints without any built-in compensation like in the :enumerator:`trossen_arm::Mode::external_effort` mode.
  This mode is useful for applications where full control of the joint efforts is desired.
- Grouped the outputs in :class:`trossen_arm::RobotOutput` and added more fields including

  - :member:`trossen_arm::RobotOutput::Joint::All::compensation_efforts`
  - :member:`trossen_arm::RobotOutput::Joint::All::rotor_temperatures`
  - :member:`trossen_arm::RobotOutput::Joint::All::driver_temperatures`
  - :member:`trossen_arm::RobotOutput::Cartesian::positions`
  - :member:`trossen_arm::RobotOutput::Cartesian::velocities`
  - :member:`trossen_arm::RobotOutput::Cartesian::external_efforts`

- Added methods for Cartesian inputs.

  - :ref:`getting_started/demo_scripts:`cartesian_position`_`
  - :ref:`getting_started/demo_scripts:`cartesian_velocity`_`
  - :ref:`getting_started/demo_scripts:`cartesian_external_effort`_`

- Added the capability to soft reboot the controller via the driver.
  This feature can be used in :ref:`getting_started/demo_scripts:`set_factory_reset_flag`_` for example.

- Added more configurations and revised some previous ones.

  - Added

    - :ref:`getting_started/configuration:joint limits`
    - :ref:`getting_started/configuration:motor parameters`
    - :ref:`getting_started/configuration:algorithm parameter`

  - revised

    - :ref:`getting_started/configuration:joint characteristics`
    - :ref:`getting_started/configuration:end effector`

- The connection, disconnection, and data exchange is now handled more robustly.

  - UDP is only used for control and TCP handles the rest of the communication.
  - Made connection more user-friendly with retry, timeout, and allowing interruption.

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

  - The controller now return to idle mode if the connection is lost.
  - The controller now only accept one driver at a time.

- Revised default configurations to more reasonable values, more specifically

  - :ref:`getting_started/configuration:joint limits`
  - :ref:`getting_started/configuration:motor parameters`
