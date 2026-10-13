==========
WidowX Pro
==========

.. attention::

  Specifications on this page are preliminary and subject to change.

Overview
========

See :doc:`arms` to compare the WidowX arms.

.. list-table::
    :align: center

    * - .. image:: images/wxpro_hero.webp
              :align: center
              :alt: WidowX Pro arm
              :width: 250px

      - .. table::
            :align: center

            +----------------------------------+
            | **WidowX Pro**                   |
            +====================+=============+
            | Degrees of Freedom | 6 + Gripper |
            +--------------------+-------------+
            | Payload Capacity   | 4 kg        |
            +--------------------+-------------+
            | Weight             | 6 kg        |
            +--------------------+-------------+
            | Reach              | 0.790 m     |
            +--------------------+-------------+
            | Gripper Stroke     | 90 mm       |
            +--------------------+-------------+
            | Nominal Voltage    | 24 V        |
            +--------------------+-------------+
            | Peak Current       | 15 A        |
            +--------------------+-------------+
            | Communication      | Ethernet    |
            +--------------------+-------------+

Joint Limits
============

.. list-table::
  :align: center
  :header-rows: 1

  * - Joint
    - Min Position [rad (deg)]
    - Max Position [rad (deg)]
    - Velocity [rad/s (deg/s)]
    - Effort [N*m]
  * - Joint 0
    - -2.8798 (-165)
    - 2.8798 (165)
    - 6.2832 (360)
    - 27
  * - Joint 1
    - 0 (0)
    - 3.5779 (205)
    - 6.2832 (360)
    - 97
  * - Joint 2
    - 0 (0)
    - 3.1416 (180)
    - 6.2832 (360)
    - 27
  * - Joint 3
    - -1.5708 (-90)
    - 1.5708 (90)
    - 9.4248 (540)
    - 7
  * - Joint 4
    - -1.5708 (-90)
    - 1.5708 (90)
    - 9.4248 (540)
    - 7
  * - Joint 5
    - -2.0071 (-115)
    - 2.0071 (115)
    - 9.4248 (540)
    - 7

.. note::

  These effort limits are nominal values.
  The actual limits are computed with the following formula:

  .. math::

    \text{effort}_\max = \frac{\text{effort}_\text{max_nominal}}{\text{effort_correction}}

  where :math:`\text{effort_nominal}` is the nominal effort limit of a joint, :math:`\text{effort_correction}` is the effort correction factor of this joint, and :math:`\text{effort}_\max` is the actual effort limit of this joint.

  The effort corrections can be retrieved with :func:`trossen_arm::TrossenArmDriver::get_effort_corrections`.

Controller Base
===============

The controller base has three M12 connectors with different functions.

.. list-table::
  :align: center
  :header-rows: 1

  * - Coding
    - Carries
  * - X Code
    - Network
  * - A Code
    - GPIO
  * - L Code
    - Power

The base also has the arm's power switch, a status LED, and a small screen that can display and manage the arm's state and configuration.
