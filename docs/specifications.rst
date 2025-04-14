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
  * - Joint 0
    - -3.054 (-175)
    - 3.054 (175)
    - 3.375 (193)
    - 27
  * - Joint 1
    - 0 (0)
    - 3.14 (180)
    - 3.375 (193)
    - 27
  * - Joint 2
    - 0 (0)
    - 4.712 (270)
    - 3.375 (193)
    - 27
  * - Joint 3
    - -1.57 (-90)
    - 1.57 (90)
    - 7.000 (400)
    - 7
  * - Joint 4
    - -1.57 (-90)
    - 1.57 (90)
    - 7.000 (400)
    - 7
  * - Joint 5
    - -3.14 (-180)
    - 3.14 (180)
    - 7.000 (400)
    - 7

.. list-table::
  :align: center
  :header-rows: 1

  * - Specification
    - Value
  * - Max Finger Displacement [m]
    - 0.044
  * - Max Gripping Force [N]
    - :math:`\text{torque}_{6, \max} / \text{pitch raidus} = 3.0/0.00875 = 342.857`

.. note::

  These effort limits are nominal values. The actual limits are computed
  with the following formula:

  .. math::

    \text{effort}_\max = \frac{\text{effort}_\text{max_nominal}}{\text{effort_correction}}

  where :math:`\text{effort_nominal}` is the nominal effort limit of a joint,
  :math:`\text{effort_correction}` is the effort correction factor of this joint
  , and :math:`\text{effort}_\max` is the actual effort limit of this joint.

  The effort corrections can be retrieved with
  :func:`trossen_arm::TrossenArmDriver::get_effort_corrections`.

Workspace
---------

.. image:: specifications/images/wxai_workspace.png
   :alt: WidowX AI Workspace
   :align: center

Denavit-Hartenberg parameters
-----------------------------

Classic
^^^^^^^

This set of parameters follows the `classic Denavit-Hartenberg convention`_.

.. _`classic Denavit-Hartenberg convention`:
  https://en.wikipedia.org/wiki/Denavit%E2%80%93Hartenberg_parameters#Denavit%E2%80%93Hartenberg_convention

.. list-table::
  :align: center
  :header-rows: 1

  * - Transformation
    - :math:`d` [m]
    - :math:`\theta` [rad]
    - :math:`a` [m]
    - :math:`\alpha` [rad]
  * - :math:`^\text{base}T_0`
    - 0.1035
    - :math:`\theta_0`
    - 0.02
    - :math:`-\pi/2`
  * - :math:`^0T_1`
    - 0.0
    - :math:`\theta_1+\pi`
    - 0.264
    - :math:`\pi`
  * - :math:`^1T_2`
    - 0.0
    - :math:`\theta_2-\pi/2-\arctan(0.245/0.06)`
    - :math:`\sqrt{0.245^2+0.06^2}`
    - 0.0
  * - :math:`^2T_3`
    - 0.0
    - :math:`\theta_3-\arctan(0.06/0.245)`
    - 0.06775
    - :math:`\pi/2`
  * - :math:`^3T_4`
    - 0.0
    - :math:`\theta_4+\pi/2`
    - 0.0
    - :math:`\pi/2`
  * - :math:`^4T_5`
    - 0.17095
    - :math:`\theta_5`
    - 0.0
    - :math:`\pi/2`
  * - :math:`^5T_\text{gripper}`
    - 0.0
    - :math:`\pi/2`
    - 0.0
    - 0.0

Modified
^^^^^^^^

This set of parameters follows the `modified Denavit-Hartenberg convention`_.

.. _`modified Denavit-Hartenberg convention`:
  https://en.wikipedia.org/wiki/Denavit%E2%80%93Hartenberg_parameters#Modified_DH_parameters

.. list-table::
  :align: center
  :header-rows: 1

  * - Transformation
    - :math:`\alpha` [rad]
    - :math:`a` [m]
    - :math:`\theta` [rad]
    - :math:`d` [m]
  * - :math:`^\text{base}T_0`
    - 0.0
    - 0.0
    - :math:`\theta_0`
    - 0.1035
  * - :math:`^0T_1`
    - :math:`-\pi/2`
    - 0.02
    - :math:`\theta_1+\pi`
    - 0.0
  * - :math:`^1T_2`
    - :math:`\pi`
    - 0.264
    - :math:`\theta_2-\pi/2-\arctan(0.245/0.06)`
    - 0.0
  * - :math:`^2T_3`
    - 0.0
    - :math:`\sqrt{0.245^2+0.06^2}`
    - :math:`\theta_3-\arctan(0.06/0.245)`
    - 0.0
  * - :math:`^3T_4`
    - :math:`\pi/2`
    - 0.06775
    - :math:`\theta_4+\pi/2`
    - 0.0
  * - :math:`^4T_5`
    - :math:`\pi/2`
    - 0.0
    - :math:`\theta_5`
    - 0.17095
  * - :math:`^5T_\text{gripper}`
    - :math:`\pi/2`
    - 0.0
    - :math:`\pi/2`
    - 0.0
