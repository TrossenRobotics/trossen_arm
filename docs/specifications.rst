==============
Specifications
==============

This page contains specifications for the Trossen Arms.

WidowX AI
=========

Overall Specifications
----------------------

.. list-table::
  :align: center
  :header-rows: 1

  * - Specification
    - Value
  * - Degrees of Freedom
    - 6
  * - Payload Capacity
    - 1.5 kg
  * - Weight
    - 4kg
  * - Reach
    - 0.769m
  * - Nominal Voltage
    - 24 V
  * - Peak Current
    - 15 A
  * - Communication
    - UDP over Ethernet

Joint Limits
------------

.. list-table::
  :align: center
  :header-rows: 1

  * - Joint
    - Min Position [rad (deg)]
    - Max Position [rad (deg)]
    - Velocity [rad/s (deg/s)]
    - Effort [N*m]
  * - Joint 1
    - -3.054 (-175)
    - 3.054 (175)
    - 3.14 (180)
    - 27
  * - Joint 2
    - 0 (0)
    - 3.14 (180)
    - 3.14 (180)
    - 27
  * - Joint 3
    - 0 (0)
    - 4.712 (270)
    - 3.14 (180)
    - 27
  * - Joint 4
    - -1.57 (-90)
    - 1.57 (90)
    - 3.14 (180)
    - 7
  * - Joint 5
    - -1.57 (-90)
    - 1.57 (90)
    - 3.14 (180)
    - 7
  * - Joint 6
    - -3.14 (-180)
    - 3.14 (180)
    - 3.14 (180)
    - 7

.. list-table::
  :align: center
  :header-rows: 1

  * - Specification
    - Value
  * - Max Gripper Opening [m]
    - 0.088
  * - Max Gripper Force [N]
    - 400

Workspace
---------

.. image:: specifications/images/wxai_workspace.png
   :alt: WidowX AI Workspace
   :align: center
