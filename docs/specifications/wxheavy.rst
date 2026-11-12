============
WidowX Heavy
============

.. attention::

  Specifications on this page are preliminary and subject to change.

Overview
========

See :doc:`arms` to compare the WidowX arms.

.. list-table::
    :align: center

    * - .. image:: images/wxheavy_hero.webp
              :align: center
              :alt: WidowX Heavy arm
              :width: 270px

      - .. table::
            :align: center

            +----------------------------------+
            | **WidowX Heavy**                 |
            +====================+=============+
            | Degrees of Freedom | 6 + Gripper |
            +--------------------+-------------+
            | Payload Capacity   | 6 kg        |
            +--------------------+-------------+
            | Weight             | 8 kg        |
            +--------------------+-------------+
            | Reach              | 1.13 m      |
            +--------------------+-------------+
            | Gripper Stroke     | 90 mm       |
            +--------------------+-------------+
            | Nominal Voltage    | 24 V        |
            +--------------------+-------------+
            | Peak Current       | TBD         |
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
    - 97
  * - Joint 1
    - 0 (0)
    - 3.8397 (220)
    - 6.2832 (360)
    - 194
  * - Joint 2
    - 0 (0)
    - 3.7525 (215)
    - 6.2832 (360)
    - 97
  * - Joint 3
    - -1.5708 (-90)
    - 1.5708 (90)
    - 6.2832 (360)
    - 27
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

Workspace
=========

The shaded area is where the gripper fingertips can reach.
Reach is measured horizontally from the base rotation axis (the joint 0 axis) to the fingertips.
All dimensions are in meters.

.. tabs::

    .. group-tab:: Top

        .. image:: images/wxheavy_workspace_top.webp
            :align: center
            :alt: WidowX Heavy workspace, top view
            :class: workspace-image

    .. group-tab:: Side

        .. image:: images/wxheavy_workspace_side.webp
            :align: center
            :alt: WidowX Heavy workspace, side view
            :class: workspace-image

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
