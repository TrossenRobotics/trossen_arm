==============
MoveIt Configs
==============

The Trossen Arm MoveIt configuration package provides the necessary files to control the Trossen Arm using `MoveIt<https://moveit.picknik.ai/main/index.html>`_.

Two different hardware interfaces are supported:

-   **Mock Hardware**: For testing and development purposes, simulating the arm's behavior.
-   **Real Hardware**: For controlling the actual Trossen Arm hardware.

These interfaces can be swapped by changing the ``ros2_control_hardware_type`` launch argument when launching the bringup package.

Usage
=====

We