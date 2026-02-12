=================================
Touchscreen Setup and Mapping
=================================

Overview
========

This guide covers the setup, configuration, and troubleshooting of touchscreen displays for use with the Trossen AI Data Collection UI.
Proper touchscreen configuration ensures accurate input mapping and optimal user experience when operating the robot control interface.

Hardware Setup
==============

#. Connect the touchscreen display to your system:

   - Connect the video cable (HDMI, DisplayPort, etc.) to the appropriate port
   - Connect the USB cable for touch input (if separate from video connection)
   - Ensure all connections are secure

#. Power on the touchscreen display

#. Verify the display is detected by the system:

   .. code-block:: bash

       xrandr --query | grep " connected"

   You should see your touchscreen display listed among the connected outputs.

Software Setup
==============

Install Required Packages
--------------------------

Install the necessary tools for touchscreen configuration:

.. code-block:: bash

    sudo apt-get update
    sudo apt-get install xinput x11-xserver-utils

These packages provide:

- ``xinput``: Tool for configuring and testing input devices
- ``x11-xserver-utils``: Utilities including ``xrandr`` for display management

Verify Touchscreen Detection
-----------------------------

Check that the touchscreen input device is properly detected:

.. code-block:: bash

    xinput list

Look for your touchscreen device in the output. Common names include:

- ``TSTP MTouch``
- ``TouchScreen``
- ``Touchscreen``
- Device names containing your manufacturer name

The output will show entries like:

.. code-block:: text

    ⎜   ↳ TSTP MTouch                              id=28   [slave  pointer  (2)]

Note the device ID number (e.g., ``id=28``).

.. important::

    Make sure to identify the **pointer** device, not the keyboard entry.
    Some touchscreens register multiple input types.

Touchscreen Mapping
===================

Identify Display Outputs
-------------------------

List all connected displays to find the correct output name:

.. code-block:: bash

    xrandr --query | grep " connected"

Example output:

.. code-block:: text

    HDMI-1-0 connected 1920x1080+0+0 (normal left inverted right x axis y axis) 527mm x 296mm
    DP-1 connected primary 1920x1080+1920+0 (normal left inverted right x axis y axis) 509mm x 286mm

In this example, the display outputs are ``HDMI-1-0`` and ``DP-1``.

Map Touchscreen to Display
---------------------------

Use ``xinput`` to map the touchscreen input to the correct display output:

.. code-block:: bash

    xinput map-to-output <device_id> <display_name>

Replace:

- ``<device_id>``: The touchscreen device ID from ``xinput list`` (e.g., ``28``)
- ``<display_name>``: The display output name from ``xrandr`` (e.g., ``HDMI-1-0``)

Example:

.. code-block:: bash

    xinput map-to-output 28 HDMI-1-0
