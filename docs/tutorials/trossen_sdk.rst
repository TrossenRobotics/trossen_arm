===========
Trossen SDK
===========

.. important::

    The Trossen SDK has its own documentation site: `docs.trossenrobotics.com/trossen_sdk <https://docs.trossenrobotics.com/trossen_sdk/>`_.
    This page is a short summary.

Overview
========

The Trossen SDK is a C++ SDK for recording robot demonstrations.
One JSON config file describes the hardware and the session, and any setting can be overridden from the command line.
Each episode is recorded to its own TrossenMCAP file.
To train a policy, convert the files to a LeRobot dataset.

.. mermaid::

    flowchart LR
        A[Arms, cameras,<br/>mobile base] --> B[TrossenMCAP<br/>one .mcap per episode]
        B --> C[Conversion script]
        C --> D[LeRobot dataset]

Supported Hardware
==================

.. list-table::
    :header-rows: 1
    :widths: 30 25 45

    * - Hardware
      - Type string
      - Notes
    * - :doc:`Trossen arm </specifications>`
      - ``trossen_arm``
      - Leader and follower
    * - Stereolabs ZED camera
      - ``zed_camera``
      - Color and depth
    * - RealSense camera
      - ``realsense_camera``
      - Color and depth
    * - USB camera
      - ``opencv_camera``
      - Any V4L2-compatible camera
    * - SLATE mobile base
      - ``slate_base``
      - Differential drive, with odometry

Documentation
=============

* `Installation <https://docs.trossenrobotics.com/trossen_sdk/installation.html>`_
* `Recording <https://docs.trossenrobotics.com/trossen_sdk/record.html>`_
* `Configuration reference <https://docs.trossenrobotics.com/trossen_sdk/configuration.html>`_
* `Visualizing episodes <https://docs.trossenrobotics.com/trossen_sdk/visualize.html>`_
* `Replaying episodes <https://docs.trossenrobotics.com/trossen_sdk/replay.html>`_
* `Converting to LeRobot <https://docs.trossenrobotics.com/trossen_sdk/convert.html>`_
* `Source on GitHub <https://github.com/TrossenRobotics/trossen_sdk>`_
