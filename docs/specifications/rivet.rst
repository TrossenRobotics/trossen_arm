=====
Rivet
=====

.. attention::

  Specifications on this page are preliminary and subject to change.

Overview
========

The Rivet is a mobile system with two :doc:`WidowX Core <wxcore>` or :doc:`WidowX Pro <wxpro>` arms on a lift, a three-wheel swerve base, and an onboard computer.

.. list-table::
    :align: center

    * - .. image:: images/rivet_hero.webp
              :align: center
              :alt: Rivet mobile platform
              :width: 260px

      - .. table::
            :align: center

            +----------------------------------------------------------------------------------------+
            | **Rivet**                                                                              |
            +=========================+==============================================================+
            | Drive                   | Three-wheel swerve                                           |
            +-------------------------+--------------------------------------------------------------+
            | Weight                  | 65 kg without arms                                           |
            +-------------------------+--------------------------------------------------------------+
            | Dimensions              | 0.660 x 0.658 x 1.535 m (W x L x H) without arms             |
            +-------------------------+--------------------------------------------------------------+
            | Max Translational Speed | 1.6 m/s                                                      |
            +-------------------------+--------------------------------------------------------------+
            | Max Rotational Speed    | 4.5 rad/s                                                    |
            +-------------------------+--------------------------------------------------------------+
            | Lift Travel             | 0.865 m                                                      |
            +-------------------------+--------------------------------------------------------------+
            | Max Lift Speed          | 0.3 m/s                                                      |
            +-------------------------+--------------------------------------------------------------+
            | Arm Spacing             | 0.55 m center-to-center                                      |
            +-------------------------+--------------------------------------------------------------+
            | Battery                 | 24 V, 60 Ah LiFePO4; user-swappable                          |
            +-------------------------+--------------------------------------------------------------+
            | Compute                 | `NVIDIA Jetson AGX Orin`_ 64 GB                              |
            +-------------------------+--------------------------------------------------------------+
            | Display                 | 13.3" touchscreen                                            |
            +-------------------------+--------------------------------------------------------------+
            | Cameras                 | `ZED X Mini`_ (scene); `ZED X Nano`_ (wrists)                |
            +-------------------------+--------------------------------------------------------------+
            | User Power Breakouts    | VBat (24 V nominal); 12 V                                    |
            +-------------------------+--------------------------------------------------------------+
            | User Ports              | USB-C, RJ45 Ethernet                                         |
            +-------------------------+--------------------------------------------------------------+
            | Safety                  | E-stop button; perimeter safety bumper (both trigger E-stop) |
            +-------------------------+--------------------------------------------------------------+

.. _NVIDIA Jetson AGX Orin: https://www.nvidia.com/en-us/autonomous-machines/embedded-systems/jetson-orin/
.. _ZED X Mini: https://www.stereolabs.com/products/zed-x
.. _ZED X Nano: https://www.stereolabs.com/products/zed-x-nano

Workspace
=========

The shaded areas are where each arm's gripper fingertips can reach with the base parked, swept over the full lift travel.
Reach is measured horizontally from the base rotation axis (the joint 0 axis) to the fingertips.
All dimensions are in meters.

.. tabs::

    .. group-tab:: WidowX Core

        .. tabs::

            .. group-tab:: Top

                .. image:: images/rivet_core_workspace_top.webp
                    :align: center
                    :alt: Rivet with WidowX Core arms, workspace top view
                    :class: workspace-image

            .. group-tab:: Side

                .. image:: images/rivet_core_workspace_side.webp
                    :align: center
                    :alt: Rivet with WidowX Core arms, workspace side view of the left arm
                    :class: workspace-image

            .. group-tab:: Front

                .. image:: images/rivet_core_workspace_front.webp
                    :align: center
                    :alt: Rivet with WidowX Core arms, workspace front view
                    :class: workspace-image

    .. group-tab:: WidowX Pro

        .. tabs::

            .. group-tab:: Top

                .. image:: images/rivet_pro_workspace_top.webp
                    :align: center
                    :alt: Rivet with WidowX Pro arms, workspace top view
                    :class: workspace-image

            .. group-tab:: Side

                .. image:: images/rivet_pro_workspace_side.webp
                    :align: center
                    :alt: Rivet with WidowX Pro arms, workspace side view of the left arm
                    :class: workspace-image

            .. group-tab:: Front

                .. image:: images/rivet_pro_workspace_front.webp
                    :align: center
                    :alt: Rivet with WidowX Pro arms, workspace front view
                    :class: workspace-image
