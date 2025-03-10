=============
Configuration
=============

This section walks you through the configurations that can be set on a Trossen AI arm.
Properly configuring the arm for your application is crucial to ensure the arm operates as expected.

What You Need
=============

To get started, please make sure you have gone through the :doc:`software_setup`.

Overview
========

Depending on

- when the change takes effect
- whether the changed configuration is reset to default at the next boot

the configurations are divided into four categories as given in the following table.

.. list-table::
    :width: 100%
    :class: borderless
    :align: center
    :header-rows: 1
    :stub-columns: 1

    *   -
        -   Immediately Applied
        -   Applied at Next Boot
    *   -   Remain Unchanged After Reboot
        -   -   joint_characteristics
            -   effort_corrections
            -   friction_transition_velocities
            -   friction_constant_terms
            -   friction_coulomb_coefs
            -   friction_viscous_coefs
            -   continuity_factors
        -   -   ip_method
            -   manual_ip
            -   dns
            -   gateway
            -   subnet
    *   -   Reset to Default After Reboot
        -   -   end_effector
            -   gripper_force_limit_scaling_factor
        -   -   factory_reset_flag

The driver provides methods to get and set these configurations.
An example of a configuration script is given here.

.. tabs::

    .. code-tab:: c++

        // Include the header files
        #include "libtrossen_arm/trossen_arm.hpp"

        int main(int argc, char** argv)
        {
          // Create a driver object
          trossen_arm::TrossenArmDriver driver;

          // Configure the driver
          // This configuration is mandatory, including
          // - model of the arm
          // - end effector properties
          // - IP address of the arm
          // - whether to clear the existing error state if any
          driver.configure(...);

          // Get/set some configurations if needed
          // Here xxx can be
          // - factory_reset_flag
          // - ip_method
          // - manual_ip
          // - dns
          // - gateway
          // - subnet
          // - joint_characteristics
          // - effort_corrections
          // - friction_transition_velocities
          // - friction_constant_terms
          // - friction_coulomb_coefs
          // - friction_viscous_coefs
          // - continuity_factors
          // - end_effector
          // - gripper_force_limit_scaling_factor
          auto xxx = driver.get_xxx(...);
          driver.set_xxx(...);
        }

    .. code-tab:: py

        # Import the driver
        import trossen_arm

        if __name__ == "__main__":
            # Create a driver object
            driver = trossen_arm.TrossenArmDriver()

            # Configure the driver
            # This configuration is mandatory, including
            # - model of the arm
            # - end effector properties
            # - IP address of the arm
            # - whether to clear the existing error state if any
            driver.configure(...)

            # Get/set some configurations if needed
            # Here xxx can be
            # - factory_reset_flag
            # - ip_method
            # - manual_ip
            # - dns
            # - gateway
            # - subnet
            # - joint_characteristics
            # - effort_corrections
            # - friction_transition_velocities
            # - friction_constant_terms
            # - friction_coulomb_coefs
            # - friction_viscous_coefs
            # - continuity_factors
            # - end_effector
            # - gripper_force_limit_scaling_factor
            xxx = driver.get_xxx(...)
            driver.set_xxx(...)

.. tip::

    We provide methods to exchange persistent configurations via a YAML file.
    Check out the :ref:`getting_started/demo_scripts:`configuration_in_yaml`_` demo for more details.

How They Work?
==============

Here is a breakdown of how the configurations affect the behavior of the arm.

.. contents::
    :local:
    :depth: 2

factory_reset_flag
------------------

If the factory reset flag is set to true, all configurations are reset to their factory default values at the next boot.

Default value: ``false``

Ethernet Configuration
----------------------

At startup, the arm controller tries to connect to the network.
The procedure is as follows.

.. mermaid::
    :align: center

    flowchart LR
        A(Power on) --> B{ip_method?}
        B -->|dhcp| C(Acquire IP from DHCP server)
        B -->|manual| D(Set up ethernet according to the configurations)
        C --> E{success?}
        E -->|yes| F(Set up ethernet as DHCP server directs)
        E -->|no| D

ip_method
^^^^^^^^^

The IP method can be set to ``dhcp`` or ``manual``.

Default value: ``manual``

.. note::

    If the IP method is set to ``dhcp``, we expect a DHCP server to be present in the network.
    It can be a router or a computer with a DHCP server running.

manual_ip, dns, gateway, subnet
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If the IP method is set to ``manual``, the manual IP address, DNS, gateway, and subnet are used.

Default values:

- manual_ip: ``192.168.1.2``
- dns: ``8.8.8.8``
- gateway: ``192.168.1.1``
- subnet: ``255.255.255.0``

Joint Characteristics
---------------------

The joint characteristics affect the behavior of each joint.

effort_corrections
^^^^^^^^^^^^^^^^^^

The effort corrections map the motors' effort unit to the standard unit, i.e., Nm and N.

To give an example, in external effort mode, the command sent to the motor is given by the following expression.

.. math::

    \text{effort}_\text{motor} = \text{effort_correction} \times \left( \text{external_effort}_\text{desired} + \text{effort}_\text{compensation} \right)

Vice versa, the effort returned by the driver is given by the following expression.

.. math::

    \text{external_effort} = \frac{\text{effort}_\text{motor}}{\text{effort_correction}} - \text{effort}_\text{compensation}

Default values are arm specific and calibrated at the factory.

friction_transition_velocities, friction_constant_terms, friction_coulomb_coefs, and friction_viscous_coefs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

We model a joint friction as a function of velocity and effort of three components: Coulomb, viscous, and constant.

- The Coulomb friction is proportional to the magnitude of the effort.
- The viscous friction is proportional to the velocity.
- The constant friction is independent of the velocity and effort.

To deal with the discontinuity when the direction of the velocity changes, we use a linear transition characterized by the transition velocity.

The resulting compensation effort is given below, where :math:`\text{effort}_\text{inverse_dynamics}` is the effort computed by inverse dynamics.

.. math::

    \text{effort}_\text{friction} &= \text{constant_term} \\
    &+ \text{coulomb_coef} \times \lvert \text{effort}_\text{inverse_dynamics} \rvert \\
    &+ \text{viscous_coef} \times \lvert \text{velocity} \rvert \\
    \text{effort}_\text{compensation} &= \text{effort}_\text{inverse_dynamics} \\
    &+ \begin{cases}
        + \text{effort}_\text{friction} & \text{if } \text{velocity} \gt \text{transition_velocity} \\
        - \text{effort}_\text{friction} & \text{if } \text{velocity} \lt -\text{transition_velocity} \\
        + \text{effort}_\text{friction} \times \frac{\text{velocity}}{\text{transition_velocity}} & \text{otherwise}
    \end{cases}

Each controller-arm pair comes with calibrated effort corrections and friction parameters as defaults.
They should work decently for most applications.
However, you can always fine-tune them according to personal preferences.

Here is a guideline to tune the effort corrections and friction parameters.

1.  Put the arm in gravity compensation, i.e., all external efforts are zero
2.  Tune the joints one by one from gripper to base

    -   Increase the ``effort_correction`` if the links onwards are pulled down by gravity
    -   Move the joint at low velocity and increase the ``friction_coulomb_coef`` if the resistance is stronger when the joint is compensating for gravity than in a balanced position
    -   Move the joint at varying velocities and increase the ``friction_viscous_coef`` if there's more resistance at higher velocity
    -   Increase the ``friction_constant_term`` to uniformly reduce the resistance up til the joint starts moving spontaneously
    -   Increase the ``friction_transition_velocity`` if quiet operation and large stiction is preferred over reduced stiction with high-frequency oscillations

Default values are arm specific and calibrated at the factory.

.. warning::

    Since these configurations are arm specific, mixed usage of controller and arm with different serial numbers may cause deterioration in performance.

continuity_factors
^^^^^^^^^^^^^^^^^^

The arm controller uses this factor to determine whether the commanded joint position is continuous.
If not, the controller enter the :ref:`troubleshooting:16: robot input discontinuous` error state.

Default value: ``2.0``

.. tip::

    If you run into the :ref:`troubleshooting:16: robot input discontinuous` error state, please

    - check that the frequency of the control loop is at least 300 Hz if the built-in interpolator is disabled, i.e., the ``goal_time`` is zero
    - otherwise, check that the ``goal_time`` is long enough compared to looping time so that the joint can feasibly reach ``goal_position`` within ``goal_time``
    - finally, if you're confident that there's no implementation mistake in the script, increase the continuity factor with discretion

End Effector
------------

The :class:`trossen_arm::EndEffectorProperties` allow the usage of different end effectors.
It's important to match the end effector properties with the actual end effector attached to the arm.
Otherwise, the controller won't be able to properly compensate for the end effector's weight and inertia.

.. tip::

    The commonly used :class:`trossen_arm::StandardEndEffector` are provided in the driver.

Link Properties
^^^^^^^^^^^^^^^

The :class:`trossen_arm::LinkProperties` members of the end effector define the three links of an end effector.

- plam: the whole end effector excluding the fingers
- left finger: the finger on the left side
- right finger: the finger on the right side

where the mass, inertia, origin_xyz, and origin_rpy values follow the `URDF convention <https://wiki.ros.org/urdf/XML/link>`_.
And the left and right sides are defined with respect to the arm's perspective, i.e., observing from the base to the end effector when the joints are in home positions.

Finger Offsets
^^^^^^^^^^^^^^

The offsets of the left and right fingers are required.

- offset_finger_left: the offset from the palm center to the left carriage center in m with the fingers closed
- offset_finger_right: the offset from the palm center to the right carriage center in m with the fingers closed

Gripper Force Limit Scaling Factor
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This scaling factor scales the force limit of the gripper.
The force limit is given by the following expression where :math:`\text{max_force}` is the maximum force of the gripper given in :doc:`/specifications`.

.. math::

    \text{actual_max_force} = \text{t_max_factor} \times \text{max_force}

Default value: ``0.5``

What's Next?
============

Now that the arm is configured, an assorted collection of :doc:`/getting_started/demo_scripts` is available to help you get started with controlling the arm.
