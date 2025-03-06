==============
Hardware Setup
==============

This section walks you through the hardware setup for a new Trossen Arm.

What You Need
-------------

To get started, you will need the following:

-   Trossen Arm
-   Trossen Arm Controller
-   24V Power Supply
-   Linux PC
-   Ethernet Switch (Optional, Managed)

Mounting the Arm
----------------

The Trossen Arm comes pre-assembled, so you will only need to mount it to a sturdy surface.
The dimensions of the mounting holes at the base are shown below.

.. image:: images/mounting_holes.png
    :alt: Mounting Holes
    :align: center
    :width: 50%

Connecting the Cables
---------------------

After securing the arm, the next step is to connect the cables.
The following diagram shows the connections between the hardware components.

.. image:: images/overview.png
    :alt: Overview
    :align: center
    :width: 80%

The closeup pictures for each component are provided below.

.. list-table::
    :width: 100%
    :class: borderless
    :align: center

    *   -   .. image:: images/closeup_arm.png
                :alt: Arm
                :align: center

        -   .. image:: images/closeup_controller_one_socket.jpeg
                :alt: Controller Side with One Socket
                :align: center

        -   .. image:: images/closeup_controller_two_socket.jpeg
                :alt: Controller Side with Two Sockets
                :align: center

    *   -   .. image:: images/closeup_switch.jpeg
                :alt: Switch
                :align: center

        -   .. image:: images/closeup_pc.jpeg
                :alt: PC
                :align: center

        -

.. note::

    If your application calls for the use of multiple arms, we recommend using a managed switch like the NETGEAR GS305E.
    Purchase links: [`NETGEAR`_] [`Amazon`_]

.. _NETGEAR: https://www.netgear.com/business/wired/switches/plus/gs305e/
.. _Amazon: https://www.amazon.com/NETGEAR-5-Port-Gigabit-Ethernet-Managed/dp/B07PJ7XZ7X

.. note::

    If only one arm is being used, the Arm Controller can connect directly to the PC via the provided Ethernet cable.
    In this case, the Ethernet switch is not needed.

.. warning::

    Make sure the Arm Controller is OFF before connecting any cables to it.

.. warning::

    Please connect only the devices required by your application to the switch .
    Low-quality connections can degrade the performance of the arm.

What's Next?
------------

Now that you have completed the hardware setup, you can move on to the :doc:`/getting_started/software_setup` section to install the necessary software.
