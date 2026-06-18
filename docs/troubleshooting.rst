===============
Troubleshooting
===============

This section covers common issues while using the Trossen Arm and their solutions.

.. tip::

    Seeing a ``[CRITICAL]`` or ``[WARNING]`` message about connecting to the Arm Controller?
    Start with the `Quick Checks`_ below, then see :doc:`troubleshooting/connection` for the specific error.

Quick Checks
============

Most issues come down to power, cabling, network, or a version mismatch.
Before digging into the detailed guides, run through these checks:

#.  **Power.** The Arm Controller is powered on (status LED solid green or blue, not red) and the joint motor LEDs have turned green, indicating the motors are enabled.
    Motor LEDs that are off mean the arm is unpowered; motor LEDs stuck red mean the motors never finished enabling.
    See `LED Status`_.
#.  **Cabling.** The Ethernet cable is firmly seated on both ends, and the controller-to-arm cable is connected.
#.  **Network reachability.** The host and Arm Controller are on the same subnet.
    Confirm the Arm Controller is discoverable:

    .. code-block:: bash

        trossen-arm discover

    See :doc:`/software_tools/cli` for details.
#.  **Version match.** The driver and Arm Controller firmware share the same minor version.
    See :doc:`/getting_started/firmware_upgrade`.
#.  **Single owner.** No other driver instance is currently connected to the Arm Controller.

If the arm has faulted (solid or flashing red LED), see :doc:`troubleshooting/errors`.

LED Status
==========

The Arm Controller and joint LEDs are the fastest way to read the arm's state.

.. list-table::
    :widths: 20 25 55
    :header-rows: 1
    :align: center

    *   -   LED
        -   State
        -   Meaning
    *   -   Arm Controller
        -   Solid green
        -   Powered on and healthy, in idle mode.
    *   -   Arm Controller
        -   Solid blue
        -   Powered on and healthy, in an operating mode other than idle.
    *   -   Arm Controller
        -   Flashing red
        -   Error at startup. Joints output no torque and the joint motor LEDs are solid red. See :doc:`troubleshooting/errors`.
    *   -   Arm Controller
        -   Solid red
        -   Error during operation. The arm joints are brake-on and the gripper closes with a safe force; the joint motor LEDs stay green. See :doc:`troubleshooting/errors`.
    *   -   Joint motor
        -   Off
        -   The arm is powered off.
    *   -   Joint motor
        -   Red
        -   Motors are disabled. This is normal briefly during boot-up, before the Arm Controller enables them. If it persists, the motors never finished enabling.
    *   -   Joint motor
        -   Green
        -   Motors are enabled and operating normally.

See :doc:`troubleshooting/errors` for the full list of error codes and how to clear them.

.. toctree::
    :hidden:
    :maxdepth: 1
    :caption: Contents:

    troubleshooting/connection.rst
    troubleshooting/errors.rst
    troubleshooting/motion.rst
