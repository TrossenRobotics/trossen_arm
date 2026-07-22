========================
Motion and Teleoperation
========================

Issues related to how the arm moves during operation and teleoperation.

Stiff Leader Arm
================

If the leader arm or gripper feels stiff or resistant to back-driving during teleoperation, its joint friction compensation may be tuned too conservatively for your arm or application.
Each arm ships with calibrated defaults that work for most applications, but they can be fine-tuned to your preference.

The repository ships an interactive tuning tool, `scripts/tuning.py <https://github.com/TrossenRobotics/trossen_arm/blob/main/scripts/tuning.py>`_, that lets you adjust each joint's characteristics live from the terminal and feel the result on the arm immediately.
Run it with:

.. code-block:: bash

    uv run scripts/tuning.py [--ip 192.168.1.2]

See `scripts/README.md <https://github.com/TrossenRobotics/trossen_arm/blob/main/scripts/README.md>`_ for usage.

The parameter to focus on is :member:`trossen_arm::JointCharacteristic::friction_constant_term`.
Increasing it uniformly reduces resistance across all velocities and efforts, and is usually enough on its own to resolve a stiff-feeling arm.
Working one joint at a time from gripper to base, increase it until the joint moves freely, backing off if the joint starts to drift on its own.
The other friction parameters (``friction_coulomb_coef``, ``friction_viscous_coef``, and ``friction_transition_velocity``) are available for finer adjustments.

For the full friction model, parameter ranges, and the complete tuning guideline, see :ref:`getting_started/configuration:friction_transition_velocities, friction_constant_terms, friction_coulomb_coefs, and friction_viscous_coefs`.
