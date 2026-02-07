===============
Troubleshooting
===============

This section explains the common issues while using the Trossen Arm and the corresponding solutions.

.. contents::
    :local:
    :backlinks: none
    :depth: 3

Connection Issues
=================

Common connection errors are listed below.

-   ``[CRITICAL] Failed to connect to the arm controller's TCP server at 192.168.1.2:50001 within 22.556093953 seconds``
-   ``[CRITICAL] The major and minor versions of the driver and controller firmware must match.``
-   ``[CRITICAL] Failed to receive initial joint outputs``
-   ``[CRITICAL] Failed to read UDP message due to Success``
-   ``[CRITICAL] Failed to send UDP message to 192.168.1.2:50000 due to Network is unreachable``
-   ``[CRITICAL] Failed to read TCP message from 192.168.1.2:50001 due to Resource temporarily unavailable``
-   ``[CRITICAL] TCP connection closed unexpectedly``
-   ``[WARNING] Failed to connect to the arm controller's TCP server at 192.168.1.2:50001 due to Connection timed out``
-   ``[WARNING] Failed to connect to the arm controller's TCP server at 192.168.1.2:50001 due to Connection refused``
-   ``[WARNING] Among the last 5000 UDP messages expected from 192.168.1.2:50000, 4756 messages were lost``
-   Driver hangs forever

Common causes and associated solutions to these issues are listed below.

-   Controller Not Powered On

    If the controller is not powered on, the driver will not be able to connect to it.

    To resolve this, check that the controller is powered on and the status LED is solid green.

-   Controller Not Connected to the Network

    If the controller is not connected to the network, the driver will not be able to connect to it.

    To resolve this, check that the controller is connected to the network and the network cable is plugged in on both ends.

-   Misconfigured Controller IP Address

    If the controller's IP address is not set correctly when configuring the driver, the driver will not be able to connect to it.

    To resolve this, check that the controller's IP address is set correctly in the driver's configuration.

    .. tip::

        You can use nmap to search for the controller's IP address on the network.
        For example, if the controller's IP address is in the ``192.168.1.X`` subnet, you can run the following command to scan that subnet for devices:

        .. code-block:: bash

            nmap -sn 192.168.1.0/24

-   Misconfigured Network Settings

    The driver and controller must be on the same network to communicate with each other.

    -   If there are multiple network interfaces such as Ethernet and Wi-Fi, check that the one connected to the controller is configured with an IP address in the same subnet as the controller's IP address.
    -   If the driver is running on a virtual machine or a docker container, check that the traffic is directly routed to the subnet that the controller is on, e.g., `host network driver in Docker`_ and `bridged network driver in VirtualBox`_.
    -   If there is a firewall enabled on the host machine, check that TCP and UDP traffic are allowed.

.. _`host network driver in Docker`: https://docs.docker.com/network/host/
.. _`bridged network driver in VirtualBox`: https://www.virtualbox.org/manual/ch06.html#network_bridged

-   Incompatible Driver Version

    If the driver version is not compatible with the controller firmware version, the driver will not be able to connect to it.
    Driver and firmware versions are compatible if they have the same minor version number.

    See the guide on updating the controller firmware in :ref:`getting_started/software_setup:Software Upgrade` for more information.

-   Another driver owning the Controller

    If a driver is configured without being cleaned up or deconstructed, other instances of the driver will not be able to connect to the controller.

    To resolve this, check that there are no other instances of the driver owning the controller.

-   Resource Limitations

    If the host machine is running out of resources such as CPU, memory, or network bandwidth, the driver may not be able to maintain a stable connection to the controller.

    To resolve this, check that the host machine has sufficient resources.

-   Outdated Controller Firmware

    If the driver is killed in exceptional circumstances, controller firmware older than 1.9.2 inclusively is known to hang and don't respond to the driver until it's power cycled.

    To resolve this, update the firmware to version 1.9.3 or later.

-   Outdated Driver

    Drivers older than 1.9.0 inclusively will hang forever in cases like controller being unresponsive or controller owned by another driver.

    To resolve this, update the driver to version 1.9.1 or later, which will throw an exception instead of hanging forever.

Error Codes
===========

Expected Error Handling Behavior
--------------------------------

Depending on when the error occurs, the Trossen Arm exhibits different behaviors.

.. list-table::
    :width: 600px
    :widths: 40 60
    :header-rows: 1
    :align: center

    *   -   Timing of Error
        -   Expected Behavior
    *   -   At startup
        -   -   The controller LED flashes red
            -   LEDs on the joints are solid red
            -   Joints don't output any torque
    *   -   During operation
        -   -   The controller LED is solid red
            -   LEDs on the joints are solid green
            -   The arm joints are brake-on
            -   The gripper closes with a safe force

Specific Error States
---------------------

The next step is to identify and resolve the specific error state.

After carrying out the proposed solution, please do one of the following.

-   restart the controller (recommended)
-   clear the error at the driver's ``configure(...)`` call

If the problem persists, please submit a support ticket at `Trossen Robotics Support <https://www.trossenrobotics.com/support>`_.

Select the error code from the list below to view the description and solution:

.. contents::
    :local:

0: None
^^^^^^^

**Description:** No error.

**Solution:** No action needed.

1: Ethernet Init Failed
^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Controller's Ethernet manager failed to initialize.

**Solution:** Check the network connection.

2: CAN Init Failed
^^^^^^^^^^^^^^^^^^

**Description:** Controller's CAN interface failed to initialize.

**Solution:** Check the controller to arm connection.

3: Joint Command Failed
^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Controller's CAN interface failed to send a message.

**Solution:** Check the controller to arm connection.

4: Joint Feedback Failed
^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Controller's CAN interface failed to receive a message.

**Solution:** Check the controller to arm connection.

5: Joint Clear Error Failed
^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Joint clear error command failed.

**Solution:** Check the controller to arm connection.

6: Joint Enable Failed
^^^^^^^^^^^^^^^^^^^^^^

**Description:** Joint enable command failed.

**Solution:** Check the controller to arm connection.

7: Joint Disable Failed
^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Joint disable command failed.

**Solution:** Check the controller to arm connection.

8: Joint Set Home Failed
^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Joint home calibration command failed.

**Solution:** Check the controller to arm connection.

9: Joint Disabled Unexpectedly
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Joint disabled unexpectedly.

**Solution:** Check the controller to arm connection.

10: Joint Overheated
^^^^^^^^^^^^^^^^^^^^

**Description:** Joint overheated.

**Solution:** Turn off the controller to cool down the joint.

11: Invalid Mode
^^^^^^^^^^^^^^^^

**Description:** Invalid mode command received.

**Solution:** Check that the driver version matches the controller firmware version.

12: Invalid Robot Command
^^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Invalid robot command indicator received.

**Solution:** Check that the driver version matches the controller firmware version.

13: Invalid Configuration Address
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Invalid configuration address.

**Solution:** Check that the driver version matches the controller firmware version.

14: Robot Input Mode Mismatch
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Robot input with modes different than configured modes received.

**Solution:** Verify that the sent joint inputs match the configured modes.

15: Joint Limit Exceeded
^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Joint limit exceeded.

**Solution:** Check that the joint inputs are within the joint limits.
Please refer to :ref:`getting_started/configuration:joint limits` for how the joint limits work.

16: Robot Input Infinite
^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Robot input with infinite values received.

**Solution:** Check that the joint inputs are finite.
Possible causes are:

- Incorrect scripting logic
- The robot is close to a singular configuration while operating in Cartesian space
