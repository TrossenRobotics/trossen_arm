==============
Software Setup
==============

This section prepares the host PC, the computer that runs your control software and communicates with the Arm Controller.

.. contents::
    :local:

What You Need
=============

To get started, please make sure you have gone through the :doc:`/getting_started/hardware_setup`.

PC Network Setup
================

The Trossen Arm Controller communicates with the Linux PC via Ethernet.
As a result, the following network configurations are required on the Linux PC.

-   IPv4 Method: Manual
-   IP Address: Any unused IP addresses under the same subnet as the Arm Controller, e.g., ``192.168.1.1``
-   Subnet Mask: The same subnet mask used by the Arm Controller, e.g., ``255.255.255.0``
-   Other settings (Gateway, DNS, etc.) can be left as default

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

Supported Platforms
-------------------

Below is a list of compatible platforms for both the C++ and Python driver:

.. list-table::
    :align: center
    :header-rows: 1
    :class: centered-table

    * -
      - Ubuntu 20.04
      - Ubuntu 22.04
      - Ubuntu 24.04
      - MacOS 14
      - MacOS 15
    * - **x86_64**
      - ✅
      - ✅
      - ✅
      -
      -
    * - **arm64/aarch64**
      - ✅
      - ✅
      - ✅
      - ✅
      - ✅

Build System and Dependencies
-----------------------------

The Trossen Arm Linux C++ driver is distributed from a build on Ubuntu 20.04 that links to glibc 2.31.
It is provided as a static library and is statically linked to the following libraries:

.. list-table::
    :align: center
    :header-rows: 1
    :class: centered-table

    * - Dependency
      - Version
    * - Eigen3
      - 3.4.0
    * - spdlog
      - 1.15.3
    * - yaml-cpp
      - 0.8.0
    * - tinyxml2
      - 9.0.0
    * - console_bridge
      - 1.0.1
    * - urdfdom_headers
      - 1.0.5
    * - urdfdom
      - 4.0.0
    * - pinocchio
      - 3.4.0
    * - Boost
      - 1.71.0

Python wheels are built for Python 3.10 ~ Python 3.13 on the supported platforms.

Python
------

To install and use the Python driver, follow these steps.

#.  Set up a virtual environment (optional)

    Some of the popular package and environment management tools are listed below.
    Please setup one that suits your needs.

    -   `uv <https://docs.astral.sh/uv/>`_
    -   `venv <https://docs.python.org/3/library/venv.html>`_
    -   `virtualenv <https://virtualenv.pypa.io/en/latest/>`_
    -   `conda <https://docs.conda.io/projects/conda/en/stable/user-guide/getting-started.html>`_

#.  Install the driver

    .. code-block:: bash

        pip install trossen-arm

    If you are using `uv <https://docs.astral.sh/uv/>`_, add it to your project instead:

    .. code-block:: bash

        uv add trossen-arm

#.  Run a script

    .. code-block:: bash

        python3 my_script.py

C++
---

To install and use the C++ driver, follow these steps.

#.  Clone the repository

    .. code-block:: bash

        git clone https://github.com/TrossenRobotics/trossen_arm.git

#.  Install the driver

    .. code-block:: bash

        cd trossen_arm
        mkdir build
        cd build
        cmake .. [-DCMAKE_INSTALL_PREFIX=/path/to/install]
        make install

    If the ``CMAKE_INSTALL_PREFIX`` argument is specified, the driver will be installed locally.

    -   static library and cmake configurations: ``/path/to/install/lib``
    -   header files: ``/path/to/install/include``.

    Otherwise, the driver will be installed in the default locations.

    -   static library and cmake configurations: ``/usr/local/lib``
    -   header files: ``/usr/local/include``.

#.  Build a script

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

Arm Network Setup
=================

As stated above, the Arm Controller uses the following factory network settings:

-   IPv4 method: Manual
-   IP address: ``192.168.1.2``
-   Subnet mask ``255.255.255.0``
-   Gateway: ``192.168.1.1``
-   DNS: ``8.8.8.8``

You may want to change the Arm Controller's IP address to fit your application, for example to support multiple arms or to assign a static IP on your local network.
To change the IP address, follow the :ref:`getting_started/demo_scripts:`set_manual_ip`_` demo.

.. tip::

    If you have a kit and are using a Trossen-supported framework like :doc:`LeRobot</tutorials/lerobot>`, the expected IP addresses are:

    .. list-table::
        :align: center

        * - Leader Left
          - ``192.168.1.3``
        * - Leader Right
          - ``192.168.1.2``
        * - Follower Left
          - ``192.168.1.5``
        * - Follower Right
          - ``192.168.1.4``

Video Overview
==============

.. youtube:: XoD0Zj9VfFk
    :align: center

What's Next
===========

After setting up the software, let's :doc:`configure </getting_started/configuration>` the arm for your specific application.
If your driver and Arm Controller firmware versions do not match, see :doc:`firmware_upgrade` first.
