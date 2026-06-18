========
Concepts
========

The Trossen Arm driver is the library your application uses to control the arm.
It communicates with the Arm Controller over the network: TCP for commands and configuration, UDP for the continuous stream of state feedback.
Your application creates a driver object, configures it, then issues commands and reads state in a loop.

This page explains the model behind the API.
For the method-by-method reference, see the :doc:`/api/library_root`; for a fully annotated example, see :doc:`writing_a_script`.

.. note::

    The C++ and Python APIs are equivalent.
    The Python ``trossen_arm`` package is a direct binding of the C++ ``libtrossen_arm`` library, with the same classes, methods, enums, and behavior.
    Examples below use C++ names; the Python names are identical.

.. contents::
    :local:
    :depth: 2

The Driver Lifecycle
====================

Every application follows the same arc:

#.  **Construct** a :class:`trossen_arm::TrossenArmDriver`.
#.  **Configure** it with ``configure(model, end_effector, serv_ip, clear_error, timeout)``, passing the robot :enum:`trossen_arm::Model`, the :class:`trossen_arm::EndEffector` properties, and the Arm Controller's IP address.
    This opens the connection and, if ``clear_error`` is true, clears any existing error state.
#.  **Use** the arm: set modes, send commands, read state.
#.  **Clean up** with ``cleanup()`` (or let the destructor do it).
    ``cleanup(true)`` or ``reboot_controller()`` also reboots the controller.

Joints and Indexing
===================

An arm has several revolute joints plus a gripper.
Joints are **0-indexed**, and the gripper is always the **last** joint (for example ``Joint 6`` on the 6-DOF WidowX AI).
Most operations come in four flavors so you can act on whatever scope you need:

-   ``set_all_*`` / ``get_all_*``: every joint, including the gripper.
-   ``set_arm_*`` / ``get_arm_*``: the arm joints only.
-   ``set_gripper_*`` / ``get_gripper_*``: the gripper only.
-   ``set_joint_*`` / ``get_joint_*``: a single joint by index.

Modes
=====

Each joint is always in one of five modes, set independently:

.. list-table::
    :header-rows: 1
    :widths: 25 75

    *   -   :enum:`trossen_arm::Mode`
        -   Behavior
    *   -   ``idle``
        -   The joint is braked and holds position.
    *   -   ``position``
        -   The joint tracks a commanded position.
    *   -   ``velocity``
        -   The joint tracks a commanded velocity.
    *   -   ``external_effort``
        -   The joint applies the commanded effort on top of gravity- and friction-compensated efforts.
    *   -   ``effort``
        -   The joint applies the commanded effort directly, without compensation.

In any mode other than ``idle`` the Arm Controller's LED is solid blue (see :ref:`troubleshooting:LED Status`).
Set modes with ``set_all_modes``, ``set_arm_modes``, ``set_gripper_mode``, or ``set_joint_modes``.
The commands you send must match the mode each joint is configured in, otherwise the Arm Controller rejects the input (see :doc:`/troubleshooting/errors`).

Commanding Motion
=================

Once joints are in a motion mode, command them in **joint space** or **Cartesian space**.

Joint-space commands take a goal, a ``goal_time`` to reach it, a ``blocking`` flag, and optional feed-forward derivatives:

-   Positions: ``set_all_positions``, ``set_arm_positions``, ``set_gripper_position``, ``set_joint_position``
-   Velocities: ``set_all_velocities`` and the ``arm`` / ``gripper`` / ``joint`` variants
-   Efforts: ``set_all_efforts`` and ``set_all_external_efforts`` (and their variants)

Cartesian commands move the end effector frame relative to the base frame, and take an :enum:`trossen_arm::InterpolationSpace` (``joint`` or ``cartesian``) that selects whether the path is interpolated in joint space or Cartesian space:

-   ``set_cartesian_positions``, ``set_cartesian_velocities``, ``set_cartesian_external_efforts``

When ``blocking`` is true, the call returns once the goal is reached; when false, it returns immediately and the motion continues in the background.
The interpolation *type* is chosen automatically from ``goal_time``:

-   longer than ``0.2 s``: smooth polynomial interpolation (quintic for positions, cubic for velocities)
-   between ``0.001 s`` and ``0.2 s``: linear interpolation
-   ``0.001 s`` or shorter: the goal is applied immediately, with no interpolation

Frames and Units
================

Arm joints use radians; the gripper uses meters; efforts are in Nm for the arm and N for the gripper.

.. list-table::
    :header-rows: 1
    :widths: 34 33 33

    *   -   Quantity
        -   Arm joints
        -   Gripper
    *   -   Position
        -   rad
        -   m
    *   -   Velocity
        -   rad/s
        -   m/s
    *   -   Effort
        -   Nm
        -   N

A Cartesian pose is a 6-element vector: the first 3 elements are the translation ``[x, y, z]`` in meters, and the last 3 are the rotation in **angle-axis** form.
Cartesian velocities pair linear and angular parts the same way, and Cartesian efforts pair force and torque.
All Cartesian quantities are measured in the base frame, and external efforts are applied at the end effector frame.

Reading State
=============

``get_robot_output()`` returns a :class:`trossen_arm::RobotOutput` snapshot containing:

-   ``header``: a message ``id`` and a ``timestamp`` in microseconds since configuration.
-   ``joint``: per-joint state, available as ``all``, ``arm``, and ``gripper`` views, each with positions, velocities, accelerations, efforts, external efforts, compensation efforts, and rotor and driver temperatures.
-   ``cartesian``: the end effector positions, velocities, and external efforts in the base frame.

For convenience, the same values are reachable through getters like ``get_arm_positions()``, ``get_cartesian_positions()``, or ``get_joint_effort(index)`` without handling the full struct.

Gravity and Friction Compensation
=================================

In ``external_effort`` mode, gravity and joint friction are compensated automatically, so a commanded external effort of zero leaves the arm weightless and back-drivable.
The compensating efforts are reported in the ``compensation_efforts`` field of the robot output.
This is the basis for gravity-compensation and teleoperation applications, demonstrated in the :doc:`/getting_started/demo_scripts`.

Error Handling
==============

When the Arm Controller detects a fault it stops, sets all joints to idle, and records an error state.
Query it with ``get_error_information()``, and read the log for the specific joint and value.
The full list of error codes and how to clear them is in :doc:`/troubleshooting/errors`.

Logging
=======

The driver logs to ``stderr`` in C++ and to Python's ``logging`` module by default.
Control the verbosity with ``set_log_level(...)``, choosing a :enum:`trossen_arm::LogLevel` from ``trace`` (most verbose) to ``critical`` (least verbose).
To capture or redirect the messages yourself, for example into your application's logger, install a handler with ``set_logger_backend(...)``.
See the :ref:`getting_started/demo_scripts:`error_recovery_and_logging`_` demo for logging and error recovery in practice.

Configuration
=============

Beyond motion, the driver exposes the arm's configuration: joint characteristics, end effector properties, joint limits, motor parameters, IP settings, and more.
Some changes apply immediately, others at the next boot, and some persist across power cycles while others reset.
The complete reference, including which is which, is in :doc:`/getting_started/configuration`.
Configurations can be saved to and loaded from YAML with ``save_configs_to_file`` and ``load_configs_from_file``.

What's Next
===========

-   Walk through a complete script in :doc:`writing_a_script`.
-   Browse runnable examples in the :doc:`/getting_started/demo_scripts` catalog.
-   Look up specific methods in the :doc:`/api/library_root`.
