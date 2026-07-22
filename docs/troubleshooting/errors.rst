===========
Error Codes
===========

When the arm detects an error, it stops and reports an error code.
For how to read the arm's state from its LEDs, see :ref:`troubleshooting:LED Status`.

Reading an Error
================

There are two levels of detail available when an error occurs:

-   **The error code** (the summary state, e.g. *Joint Limit Exceeded*), shown in the log and listed in the *Reported text* column of the `Error Code Reference`_ below.
-   **The detailed log line.** For runtime errors the Arm Controller also logs a message that names the *specific joint* and the *offending value* (for example the allowed range and what the motor actually reported).
    Read it to pinpoint which joint errored and by how much, e.g.

    .. code-block:: text

        [Motor Interface] Joint 3 position limit exceeded: expected in range [-3.054, 3.054], motor reported 3.210. Setting to idle.

When a runtime error occurs, the Arm Controller sets all joints to idle as a safety measure.

.. note::

    **Joint numbering.** Joints are 0-indexed and the gripper is the last joint. See :ref:`programming_guide/concepts:joints and indexing` for details.

.. tip::

    **Seeing the logs.** By default the library logs to ``stderr`` in C++ and to Python's ``logging`` module in Python.
    To see more detail, raise the verbosity with ``set_log_level(...)``.
    To capture or redirect the messages yourself (for example into your application's logger), install a handler with ``set_logger_backend(...)``.

Clearing an Error
=================

After applying the relevant solution from the `Error Code Reference`_ below, do one of the following to clear the error:

-   Restart the Arm Controller (recommended).
-   Clear the error at the driver's ``configure(...)`` call or with ``clear_error()``.

If the problem persists, please submit a support ticket at `Trossen Robotics Support <https://www.trossenrobotics.com/support>`_.

Error Code Reference
====================

The codes are grouped into families that share a root cause and fix.
The *Reported text* column is the exact error string the Arm Controller reports.

Startup Errors
--------------

The Arm Controller could not bring up a hardware interface while booting, so it will not accept control commands.

-   Check the cabling.
-   Power-cycle the Arm Controller.
-   If the error persists, submit a support ticket.

.. list-table::
    :header-rows: 1
    :widths: 5 18 32 45

    *   -   Code
        -   Name
        -   Reported text
        -   What it means
    *   -   1
        -   Ethernet Init Failed
        -   ``Controller's Ethernet manager failed to initialize``
        -   The Arm Controller's Ethernet manager failed to initialize at boot.
    *   -   2
        -   CAN Init Failed
        -   ``Controller's CAN interface failed to initialize``
        -   The Arm Controller could not bring up the CAN bus to the arm at boot. While in this state it only answers handshake and log requests, so you may also see code 12.

Controller-to-Arm Communication and Motor Faults
------------------------------------------------

The Arm Controller could not exchange messages with a joint motor over the CAN bus, or a motor reported an error.
These almost always point to the controller-to-arm cabling, the cabling inside the arm, or an individual motor.
The log names the specific motor, and motors are 0-indexed (so ``motor 0`` is the first joint).

-   Trace the cable to that motor and reseat its connectors on both ends, then power-cycle the Arm Controller.
-   If the error persists, replace that motor's cable (see the cable replacement guides in :doc:`/service`).
-   If a specific joint keeps failing after reseating and replacing its cable, submit a support ticket.

.. list-table::
    :header-rows: 1
    :widths: 5 18 32 45

    *   -   Code
        -   Name
        -   Reported text
        -   What it means
    *   -   3
        -   Joint Command Failed
        -   ``Controller's CAN interface failed to send a message``
        -   The Arm Controller failed to send a command to a motor over the CAN bus.
    *   -   4
        -   Joint Feedback Failed
        -   ``Controller's CAN interface failed to receive a message``
        -   The Arm Controller stopped receiving feedback from a motor (reported after several consecutive losses).
    *   -   5
        -   Joint Clear Error Failed
        -   ``Joint clear error command failed``
        -   The command to clear a motor's error did not succeed.
    *   -   6
        -   Joint Enable Failed
        -   ``Joint enable command failed``
        -   The command to enable a motor did not succeed.
    *   -   7
        -   Joint Disable Failed
        -   ``Joint disable command failed``
        -   The command to disable a motor did not succeed.
    *   -   8
        -   Joint Set Home Failed
        -   ``Joint home calibration command failed``
        -   The home calibration command for a motor did not succeed. See :doc:`/service` for the homing procedure.
    *   -   9
        -   Joint Disabled Unexpectedly
        -   ``Joint disabled unexpectedly``
        -   A motor reported that it disabled itself; the Arm Controller sets it to idle. Check the log for the motor's own error code.

Thermal Protection
------------------

.. list-table::
    :header-rows: 1
    :widths: 5 18 32 45

    *   -   Code
        -   Name
        -   Reported text
        -   What it means and what to do
    *   -   10
        -   Joint Overheated
        -   ``Joint overheated``
        -   A motor's driver or rotor temperature exceeded its limit, and the Arm Controller set the arm to idle to protect it. Power off the Arm Controller and let the joint cool down. For sustained high-effort tasks, reduce the payload or duty cycle, or improve cooling.

Driver and Firmware Protocol Errors
-----------------------------------

The Arm Controller received a command it does not recognize.
This most often means the driver and firmware versions are mismatched, but it can also happen if another device on the network sends unrecognized packets to the Arm Controller.

-   Make sure the driver and Arm Controller firmware share the same minor version (see :doc:`/getting_started/firmware_upgrade`).
-   Make sure no other device is sending traffic to the Arm Controller.

.. list-table::
    :header-rows: 1
    :widths: 5 18 32 45

    *   -   Code
        -   Name
        -   Reported text
        -   What it means
    *   -   11
        -   Invalid Mode
        -   ``Invalid mode command received``
        -   An unrecognized mode value was received.
    *   -   12
        -   Invalid Robot Command
        -   ``Invalid robot command indicator received``
        -   An unrecognized command indicator was received. Can also appear if any command other than handshake or log retrieval is sent after a failed initialization (see code 2).
    *   -   13
        -   Invalid Configuration Address
        -   ``Invalid configuration address``
        -   An unrecognized configuration address was received.

Command and Input Errors
------------------------

The Arm Controller rejected an input you sent, usually because of a scripting issue.

-   Read the log; it names the joint and the offending value.
-   Correct the input in your script using the per-code guidance below.

.. list-table::
    :header-rows: 1
    :widths: 5 18 32 45

    *   -   Code
        -   Name
        -   Reported text
        -   What it means and what to do
    *   -   14
        -   Robot Input Mode Mismatch
        -   ``Robot input with modes different than configured modes received``
        -   The modes in your robot input do not match the configured modes. Set the joints to the intended modes before sending inputs, and make every input match the configured mode.
    *   -   15
        -   Joint Limit Exceeded
        -   ``Joint limit exceeded``
        -   A joint's position, velocity, or effort went outside its configured limit (plus tolerance), and the Arm Controller set the arm to idle. The log states which joint, which limit, the allowed range, and the reported value. Keep inputs within the limits, see :ref:`getting_started/configuration:joint limits`. Note that velocities and efforts can spike near singular configurations in Cartesian space.
    *   -   16
        -   Robot Input Infinite
        -   ``Robot input with infinite values received``
        -   An input contained a non-finite value (NaN or infinity); the Arm Controller rejected it and kept the previous inputs. Check your scripting for uninitialized values or division by zero, and avoid commanding near singular configurations in Cartesian space.

.. note::

    **Singular configurations.** A singular configuration is an arm pose where joints align such that the arm loses the ability to move its end effector in some direction (for example with the elbow fully extended).
    Near a singularity, small Cartesian motions demand very large joint velocities and efforts, which can trip codes 15 and 16.
    When operating in Cartesian space, plan paths that stay clear of these poses, or switch to joint-space control through them.
