=================
Connection Issues
=================

This page covers errors the driver reports when it cannot reach or stay connected to the Arm Controller.

.. tip::

    If you have not already, run through the :ref:`troubleshooting:Quick Checks` first.

Symptoms
========

Connection problems usually fall into one of the categories below.
Find the message closest to what you see, then work through the linked causes.

Cannot connect at all
---------------------

Example messages:

-   ``Failed to connect to the arm controller's TCP server at 192.168.1.2:50001 within 22.556093953 seconds``
-   ``Failed to connect to the arm controller's TCP server at 192.168.1.2:50001 due to Connection timed out``
-   ``Failed to connect to the arm controller's TCP server at 192.168.1.2:50001 due to Connection refused``

Likely causes:

-   `Arm Controller Not Powered On`_
-   `Arm Controller Not Connected to the Network`_
-   `Misconfigured Arm Controller IP Address`_
-   `Misconfigured Network Settings`_

Connects, then drops or loses data
----------------------------------

Example messages:

-   ``Failed to receive initial joint outputs``
-   ``Failed to read UDP message due to Success``
-   ``Failed to send UDP message to 192.168.1.2:50000 due to Network is unreachable``
-   ``Failed to read TCP message from 192.168.1.2:50001 due to Resource temporarily unavailable``
-   ``TCP connection closed unexpectedly``
-   ``Among the last 5000 UDP messages expected from 192.168.1.2:50000, 4756 messages were lost``

Likely causes:

-   `Misconfigured Network Settings`_
-   `Resource Limitations`_
-   `Another Driver Owning the Arm Controller`_

Version mismatch
----------------

Example message:

-   ``The major and minor versions of the driver and controller firmware must match.``

Likely cause:

-   `Incompatible Driver Version`_

Driver hangs forever
--------------------

No error is ever raised.

Likely causes:

-   `Outdated Arm Controller Firmware`_
-   `Outdated Driver`_

Causes and Solutions
====================

The causes below are ordered roughly from most to least common.

Arm Controller Not Powered On
-----------------------------

Check that the Arm Controller is powered on and its status LED is solid green or blue (not red).

Arm Controller Not Connected to the Network
-------------------------------------------

Check that the Arm Controller is on the network and the cable is firmly seated on both ends.
Inspect the M12 Ethernet connector for bent pins or other damage.

Misconfigured Arm Controller IP Address
---------------------------------------

Check that the IP address in the driver's configuration matches the address the Arm Controller is actually using.

.. tip::

    Use the ``trossen-arm`` :doc:`/software_tools/cli` to find Arm Controllers on your network:

    .. code-block:: bash

        trossen-arm discover

    As a fallback, you can scan a subnet with nmap, for example if the Arm Controller's IP address is in the ``192.168.1.X`` subnet:

    .. code-block:: bash

        nmap -sn 192.168.1.0/24

Misconfigured Network Settings
------------------------------

The driver and Arm Controller must be on the same network to communicate with each other.

-   If there are multiple network interfaces such as Ethernet and Wi-Fi, check that the one connected to the Arm Controller is configured with an IP address in the same subnet as the Arm Controller's IP address.
    Additionally, check that multiple interfaces are not configured to use the same subnet, which can cause routing issues.
-   If the driver is running on a virtual machine or a docker container, check that the traffic is directly routed to the subnet that the Arm Controller is on, e.g., `host network driver in Docker`_.
-   If there is a firewall enabled on the host machine, check that TCP and UDP traffic are allowed.

.. tip::

    When running the driver inside Docker, start the container with ``--network=host`` if using the Docker CLI, or ``network_mode: "host"`` if using Docker Compose.
    The default bridge network does not forward the controller's UDP state stream, so the driver connects over TCP but then fails to receive state.

.. _`host network driver in Docker`: https://docs.docker.com/network/host/

Incompatible Driver Version
---------------------------

The driver and Arm Controller firmware must share the same minor version number to be compatible.

See the guide on updating the Arm Controller firmware in :doc:`/getting_started/firmware_upgrade` for more information.

Another Driver Owning the Arm Controller
----------------------------------------

A driver that was configured but never cleaned up or destroyed keeps ownership of the Arm Controller, which blocks other instances from connecting.
Make sure no other driver instance is still connected.

Resource Limitations
--------------------

If the host machine runs low on CPU, memory, or network bandwidth, the driver may not maintain a stable connection.
Check that the host has sufficient free resources.

Outdated Arm Controller Firmware
--------------------------------

If the driver is killed in exceptional circumstances, Arm Controller firmware older than 1.9.2 inclusively is known to hang and not respond to the driver until it is power cycled.

To resolve this, update the firmware to version 1.9.3 or later.

Outdated Driver
---------------

Drivers older than 1.9.0 inclusively will hang forever in cases like the Arm Controller being unresponsive or the Arm Controller being owned by another driver.

To resolve this, update the driver to version 1.9.1 or later, which will throw an exception instead of hanging forever.
