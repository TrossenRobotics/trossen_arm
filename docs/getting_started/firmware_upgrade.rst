================
Firmware Upgrade
================

An Arm Controller firmware is compatible with a driver if their major and minor versions match exactly.
For example, the driver ``v1.3.x`` is compatible with the Arm Controller firmware ``v1.3.x``.

Typically an Arm Controller comes with the firmware compatible with the latest driver as of the manufacturing date.
As a result, when a new driver is released, the Arm Controller firmware may need to be updated.

To upgrade, follow these steps.

#.  Install the Teensy Loader CLI if not already installed

    .. code:: bash

        # Install the dependencies
        sudo apt update
        sudo apt install build-essential libusb-dev

        # Clone the repository
        git clone https://github.com/PaulStoffregen/teensy_loader_cli.git

        # Compile the source code
        cd teensy_loader_cli
        make

        # Copy the executable to the system path
        sudo cp teensy_loader_cli /usr/local/bin

        # Configure the udev rules
        sudo wget -O /etc/udev/rules.d/00-teensy.rules https://www.pjrc.com/teensy/00-teensy.rules

#.  Flash the firmware

    IMPORTANT! Use the driver compatible with the current firmware to backup all configurations as in :ref:`getting_started/demo_scripts:`configuration_in_yaml`_`.

    Connect the Arm Controller to the Linux PC with a USB to micro-USB cable.

    Download the new firmware at :doc:`/downloads`.

    Unzip and flash the new firmware using the following commands.

    .. code:: bash

        # Unzip the firmware
        unzip firmware-wxai_v0.zip

        # Flash the firmware
        teensy_loader_cli --mcu=TEENSY41 -s firmware-wxai_v0.hex

.. warning::

    The expected behavior of :doc:`/getting_started/configuration` through a firmware upgrade is as follows.

    -   Upgrade, e.g., from ``v1.8.x`` to ``v1.9.x``

        -   Fields compatible with the new firmware will be retained.
        -   Fields added in the new firmware will be set to default values.

    -   Downgrade, e.g., from ``v1.9.x`` to ``v1.8.x``

        -   STRONGLY NOT RECOMMENDED
        -   All fields will be reset to default values.
        -   Default values of arm-specific fields calibrated at manufacturing, e.g., :class:`trossen_arm::JointCharacteristic`, will be replaced by one-size-fits-all values leading to degraded performance.

What's Next
===========

With the firmware updated, continue to :doc:`configuration`.
