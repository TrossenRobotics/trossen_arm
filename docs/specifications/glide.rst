=====
Glide
=====

.. attention::

  Specifications on this page are preliminary and subject to change.

Overview
========

The Glide is a passive teleoperation device that an operator moves to control a follower arm.
The handle has a trigger, buttons, a joystick, and haptic feedback.

.. list-table::
    :align: center

    * - .. image:: images/glide_hero.webp
              :align: center
              :alt: Glide teleoperation device
              :width: 320px

      - .. table::
            :align: center

            +-------------------------------------------------------------+
            | **Glide**                                                   |
            +====================+========================================+
            | Degrees of Freedom | 6 + Trigger                            |
            +--------------------+----------------------------------------+
            | Held weight        | About 280 g, with gravity compensation |
            +--------------------+----------------------------------------+
            | Nominal Voltage    | 24 V                                   |
            +--------------------+----------------------------------------+
            | Encoder resolution | 14-bit                                 |
            +--------------------+----------------------------------------+
            | Communication      | Ethernet                               |
            +--------------------+----------------------------------------+

Handle I/O
==========

.. list-table::
  :align: center
  :header-rows: 1

  * - Feature
    - Value
  * - Trigger
    - Position, 0 to configured max position
  * - Joystick
    - 2-axis, 0 to 4095 per axis
  * - Buttons
    - 4, each with an LED (off, solid, or breathing)
  * - Haptic feedback
    - Vibration, intensity 0 to 255

Controller Base
===============

The controller base has three M12 connectors with different functions.

.. list-table::
  :align: center
  :header-rows: 1

  * - Coding
    - Carries
  * - X code
    - Network
  * - A code
    - GPIO
  * - L code
    - Power

The base also has the Glide's power switch, a status LED, and a small screen that can display and manage the Glide's state and configuration.
