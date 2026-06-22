============
Demo Scripts
============

This section describes the demo scripts that come with the Trossen Arm driver.

What You Need
=============

To get started, please make sure you have gone through the :doc:`configuration`.
For the driver's model and how a script is structured, see the :doc:`/programming_guide` (:doc:`/programming_guide/concepts` and :doc:`/programming_guide/writing_a_script`).

Demos
=====

These demos ground the concepts from the :doc:`/programming_guide` in ready-to-run examples.
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

`arm_discovery`_
^^^^^^^^^^^^^^^^

This script discovers all Arm Controllers connected on a given subnet.

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

`gravity_compensation_partial`_
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to compensate for a portion of the gravity.

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

`set_motor_parameters`_
^^^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to set the motor parameters of the arm.

.. _`arm_discovery`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/arm_discovery.py

.. _`cartesian_external_effort`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/cartesian_external_effort.py

.. _`cartesian_position`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/cartesian_position.py

.. _`cartesian_velocity`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/cartesian_velocity.py

.. _`configuration_in_yaml`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/configuration_in_yaml.py

.. _`configure_cleanup`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/configure_cleanup.py

.. _`error_recovery_and_logging`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/error_recovery_and_logging.py

.. _`joint_characteristics_finetune`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/joint_characteristics_finetune.py

.. _`gravity_compensation`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/gravity_compensation.py

.. _`gravity_compensation_partial`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/gravity_compensation_partial.py

.. _`gripper_torque`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/gripper_torque.py

.. _`mixed_interpolation_space`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/mixed_interpolation_space.py

.. _`move_two`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/move_two.py

.. _`move`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/move.py

.. _`set_factory_reset_flag`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/set_factory_reset_flag.py

.. _`set_ip_method`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/set_ip_method.py

.. _`set_joint_limits`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/set_joint_limits.py

.. _`set_manual_ip`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/set_manual_ip.py

.. _`set_mode`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/set_mode.py

.. _`set_joint_characteristics`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/set_joint_characteristics.py

.. _`set_motor_parameters`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/set_motor_parameters.py

.. _`simple_move`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/simple_move.py

.. _`teleoperation`: https://github.com/TrossenRobotics/trossen_arm/blob/main/demos/python/teleoperation.py

What's Next
===========

These demos give you a foundation for developing your own applications.
For more details on the driver API, please refer to the :doc:`/api/library_root`.
