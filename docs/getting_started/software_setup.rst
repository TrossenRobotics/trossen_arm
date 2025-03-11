==============
Software Setup
==============

This section describes how to set up the software environment.

What You Need
=============

To get started, please make sure you have gone through the :doc:`/getting_started/hardware_setup`.

Ethernet Setup
==============

The Trossen Arm Controller communicates with the Linux PC via Ethernet.
As a result, the following network configurations are required on the Linux PC.

-   IPv4 Method: Manual
-   IP Address: any unused IP addresses under the same subnet as the Arm Controller, e.g., ``192.168.1.1``
-   Subnet Mask: the same subnet mask used by the Arm Controller, e.g., ``255.255.255.0``
-   Other settings can be left as default

.. note::

    Per factory settings, the Arm Controller uses

    -   IPv4 method: Manual
    -   IP address: ``192.168.1.2``
    -   Subnet mask ``255.255.255.0``
    -   Gateway: ``192.168.1.1``
    -   DNS: ``8.8.8.8``

Installing the Drivers
======================

A Trossen Arm can be controlled using either the C++ or Python driver.

Python
------

To install and use the Python driver, follow these steps.

1.  Set up a virtual environment (optional)

    Some of the popular package and environment management tools are listed below.
    Please setup one that suits your needs.

    -   `venv <https://docs.python.org/3/library/venv.html>`_
    -   `virtualenv <https://virtualenv.pypa.io/en/latest/installation.html>`_
    -   `conda <https://docs.conda.io/projects/conda/en/stable/user-guide/getting-started.html>`_

2.  Install the driver

    Depending on the nature your application, you can install the driver in a virtual environment or the system environment.

    -   Use virtual environment if the application is in pure Python
    -   Use system environment if the application is for ROS

    .. code-block:: bash

        pip install trossen-arm

3.  Run a script

    .. code-block:: bash

        python3 my_script.py

C++
---

To install and use the C++ driver, follow these steps.

1.  Clone the repository

    .. code-block:: bash

        git clone https://github.com/TrossenRobotics/libtrossen_arm.git

2.  Install the driver

    .. code-block:: bash

        cd libtrossen_arm
        mkdir build
        cd build
        cmake .. [-DCMAKE_INSTALL_PREFIX=/path/to/install]
        make install

    If the ``CMAKE_INSTALL_PREFIX`` argument is specified, the driver will be installed in locally.

    -   static library and cmake configurations: ``/path/to/install/lib``
    -   header files: ``/path/to/install/include``.

    Otherwise, the driver will be installed in the default locations.

    -   static library and cmake configurations: ``/usr/local/lib``
    -   header files: ``/usr/local/include``.

3.  Build a script

    Here is the file structure of a minimal CMake project.

    .. code-block:: bash

        my_project/
        ├── CMakeLists.txt
        └── my_script.cpp

    The `CMakeLists.txt` file should have the following content.

    .. code-block:: cmake

        cmake_minimum_required(VERSION 3.10)

        # Set the project name and version
        project(my_project VERSION 1.0)

        # Specify the C++ standard
        set(CMAKE_CXX_STANDARD 17)
        set(CMAKE_CXX_STANDARD_REQUIRED True)

        # Find the libtrossen_arm package
        find_package(libtrossen_arm REQUIRED)

        # Add the executable for the test
        add_executable(my_script my_script.cpp)

        # Link the libtrossen_arm library to the executable
        target_link_libraries(my_script PRIVATE libtrossen_arm)

        # Install the executable
        install(TARGETS my_script DESTINATION bin)

    Build the project.

    .. code-block:: bash

        cd my_project
        mkdir build
        cd build
        cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install
        make install

    Run the executable.

    .. code-block:: bash

        ./path/to/install/bin/my_script

Software Upgrade
================

A controller firmware is compatible with a driver if their major and minor versions match exactly.
For example, the driver v1.3.x is compatible with the controller firmware v1.3.x.

Typically a controller comes with the firmware compatible with the latest driver as of the manufacturing date.
As a result, when a new driver is released, the controller firmware may need to be updated.

To upgrade, follow these steps.

1.  Install the Teensy Loader CLI if not already installed

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

2.  Flash the firmware

    Connect the controller to the Linux PC with a USB to micro-USB cable.

    Download the firmware at :doc:`/downloads`.

    Unzip and flash the firmware using the following commands.

    .. code:: bash

        # Unzip the firmware
        unzip firmware-wxai_v0.zip

        # Flash the firmware
        teensy_loader_cli --mcu=TEENSY41 -s firmware-wxai_v0.hex

.. warning::

    Upgrading the firmware will reset all configurations to the factory settings.

    If you have made any customized configurations, we assume you are already familiar with :doc:`/getting_started/configuration` using the driver.

    A typical workflow is given below.

    1.  Use the driver compatible with the current firmware to backup all customized configurations.
    2.  Update the driver to the latest version.
    3.  Flash the latest firmware.
    4.  Set the customized configurations using the latest driver.

What's Next
===========

After setting up the software, let's :doc:`configure </getting_started/configuration>` the arm for your specific application.
