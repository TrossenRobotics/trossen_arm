========================
Motion and Teleoperation
========================

Issues related to how the arm moves during operation and teleoperation.

Stiff Leader Arm
================

If the leader arm or gripper feels stiff or resistant to back-driving during teleoperation, the joint friction compensation may be tuned conservatively for your particular arm or application.
Each arm ships with calibrated defaults that work for most applications, but they can be fine-tuned to your preference.

In most cases, the parameter to focus on is :member:`trossen_arm::JointCharacteristic::friction_constant_term`.
Increasing it uniformly reduces resistance across all velocities and efforts, and is usually enough on its own to resolve a stiff-feeling arm.
To tune it:

1.  Put the arm in gravity compensation mode so all external efforts are zero.
2.  Working one joint at a time from gripper to base, increase ``friction_constant_term`` until the joint moves freely.
    Back off if the joint starts to drift on its own without being touched.

The other friction parameters (``friction_coulomb_coef``, ``friction_viscous_coef``, and ``friction_transition_velocity``) are available for finer adjustments, but most stiffness issues are solved with the constant term alone.

To make this iterative process easier, the repository ships an interactive tuning tool at `scripts/tuning.py <https://github.com/TrossenRobotics/trossen_arm/blob/main/scripts/tuning.py>`_ that lets you adjust each joint's characteristics live from the terminal and immediately feel the result on the arm.
Run it with ``uv run scripts/tuning.py [--ip 192.168.1.2]``.
See `scripts/README.md <https://github.com/TrossenRobotics/trossen_arm/blob/main/scripts/README.md>`_ for details.

For the full friction model, parameter ranges, and the complete tuning guideline, see :ref:`getting_started/configuration:friction_transition_velocities, friction_constant_terms, friction_coulomb_coefs, and friction_viscous_coefs`.
