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

The Arm Controller is off or has not finished booting.

-   Confirm it is powered on and its status LED is solid green or blue (not red).

Arm Controller Not Connected to the Network
-------------------------------------------

The Arm Controller is not reachable on the network.

-   Confirm the Ethernet cable is firmly seated on both ends.
-   Inspect the M12 Ethernet connector for bent pins or other damage.

Misconfigured Arm Controller IP Address
---------------------------------------

The IP address in the driver's configuration does not match the address the Arm Controller is actually using.

-   Confirm the configured IP matches the Arm Controller's address.
-   Discover controllers on your network with the ``trossen-arm`` CLI (see :doc:`/software_tools/cli`):

    .. code-block:: bash

        trossen-arm discover

-   As a fallback, scan the subnet with nmap, for example:

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

.. _`host network driver in Docker`: https://docs.docker.com/engine/network/drivers/host/

Incompatible Driver Version
---------------------------

The driver and Arm Controller firmware have different minor versions.

-   Update so the driver and firmware share the same minor version (see :doc:`/getting_started/firmware_upgrade`).

Another Driver Owning the Arm Controller
----------------------------------------

A previous driver was configured but never cleaned up, so it still owns the Arm Controller and blocks new connections.

-   Make sure no other driver instance is still connected.

Resource Limitations
--------------------

The host machine is low on CPU, memory, or network bandwidth, so the driver cannot maintain a stable connection.

-   Free up resources on the host.

Outdated Arm Controller Firmware
--------------------------------

Arm Controller firmware 1.9.2 and older can hang after the driver is killed, and stays unresponsive until it is power-cycled.

-   Update the firmware to version 1.9.3 or later.

Outdated Driver
---------------

Drivers 1.9.0 and older hang forever when the Arm Controller is unresponsive or owned by another driver.

-   Update the driver to version 1.9.1 or later, which raises an exception instead of hanging.
