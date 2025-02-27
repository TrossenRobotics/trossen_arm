============
Demo Scripts
============

This section describes the demo scripts that come with the Trossen Arm driver.

What You Need
=============

To get started, please make sure you have gone through the :doc:`hardware_setup` and :doc:`software_setup`.

Demos
=====

Demos of three levels of complexity are provided with the driver.

.. contents::
    :local:
    :depth: 2

Basics
------

The basic demos show the must-know functionalities to get the arm up and running.

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

Intermediate
------------

The intermediate demos give examples on commonly-used configurations and application-specific control loops.

`set_factory_reset_flag`_
^^^^^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to reset all configuration options to their default values.

`set_ip_method`_
^^^^^^^^^^^^^^^^

This script demonstrates how to set the IP method to DHCP or MANUAL.

`set_manual_ip`_
^^^^^^^^^^^^^^^^

This script demonstrates how to set the manual IP address.

`move_two`_
^^^^^^^^^^^

This script demonstrates how to move two robots to different positions using interpolation.

`move`_
^^^^^^^

This script demonstrates how to write a control loop to move the robot to different positions and record the states.

`teleoperation`_
^^^^^^^^^^^^^^^^

This script demonstrates how to teleoperate the robots with force feedback.

Advanced
--------

The advanced demos show configurations that should be used with full understanding the implications.

`set_torque_correction`_
^^^^^^^^^^^^^^^^^^^^^^^^

This script demonstrates how to set the effort corrections values.


.. _`configure_cleanup`: https://github.com/TrossenRobotics/libtrossen_arm/tree/main/demos/python/configure_cleanup.py

.. _`gravity_compensation`: https://github.com/TrossenRobotics/libtrossen_arm/tree/main/demos/python/gravity_compensation.py

.. _`gripper_torque`: https://github.com/TrossenRobotics/libtrossen_arm/tree/main/demos/python/gripper_torque.py

.. _`move_two`: https://github.com/TrossenRobotics/libtrossen_arm/tree/main/demos/python/move_two.py

.. _`move`: https://github.com/TrossenRobotics/libtrossen_arm/tree/main/demos/python/move.py

.. _`set_factory_reset_flag`: https://github.com/TrossenRobotics/libtrossen_arm/tree/main/demos/python/set_factory_reset_flag.py

.. _`set_ip_method`: https://github.com/TrossenRobotics/libtrossen_arm/tree/main/demos/python/set_ip_method.py

.. _`set_manual_ip`: https://github.com/TrossenRobotics/libtrossen_arm/tree/main/demos/python/set_manual_ip.py

.. _`set_mode`: https://github.com/TrossenRobotics/libtrossen_arm/tree/main/demos/python/set_mode.py

.. _`set_torque_correction`: https://github.com/TrossenRobotics/libtrossen_arm/tree/main/demos/python/set_torque_correction.py

.. _`simple_move`: https://github.com/TrossenRobotics/libtrossen_arm/tree/main/demos/python/simple_move.py

.. _`teleoperation`: https://github.com/TrossenRobotics/libtrossen_arm/tree/main/demos/python/teleoperation.py

What's Next
===========

Hopefully, the provided demos have put you at a good starting point for developing your own applications.
For more details on the driver API, please refer to the :doc:`/api/library_root`.
