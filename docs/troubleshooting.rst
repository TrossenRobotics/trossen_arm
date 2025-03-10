===============
Troubleshooting
===============

This section explains the common issues while using the Trossen Arm and the corresponding solutions.

Expected Error Handling Behavior
--------------------------------

Depending on when the error occurs, the Trossen Arm exhibits different behaviors.

.. list-table::
    :width: 600px
    :widths: 40 60
    :header-rows: 1
    :align: center

    *   -   Timing of Error
        -   Expected Behavior
    *   -   At startup
        -   -   The controller LED flashes red
            -   LEDs on the joints are solid red
            -   Joints don't output any torque
    *   -   During operation
        -   -   The controller LED is solid red
            -   LEDs on the joints are solid green
            -   The arm joints are brake-on
            -   The gripper closes with a safe force

Specific Error States
---------------------

The next step is to identify and resolve the specific error state.

After carrying out the proposed solution, please do one of the following.

-   restart the controller (recommended)
-   clear the error at the driver's ``configure(...)`` call

If the problem persists, please submit a support ticket at `Trossen Robotics Support <https://www.trossenrobotics.com/support>`_.

Select the error code from the list below to view the description and solution:

.. contents::
    :local:

0: None
^^^^^^^

**Description:** No error.

**Solution:** No action needed.

1: UDP Init Failed
^^^^^^^^^^^^^^^^^^

**Description:** Controller's UDP interface failed to initialize.

**Solution:** Check the network connection.

2: CAN Init Failed
^^^^^^^^^^^^^^^^^^

**Description:** Controller's CAN interface failed to initialize.

**Solution:** Check the controller to arm connection.

3: Joint Command Failed
^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Controller's CAN interface failed to send a message.

**Solution:** Check the controller to arm connection.

4: Joint Feedback Failed
^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Controller's CAN interface failed to receive a message.

**Solution:** Check the controller to arm connection.

5: Joint Enable Failed
^^^^^^^^^^^^^^^^^^^^^^

**Description:** Joint enable command failed.

**Solution:** Check the controller to arm connection.

6: Joint Disable Failed
^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Joint disable command failed.

**Solution:** Check the controller to arm connection.

7: Joint Set Home Failed
^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Joint home calibration command failed.

**Solution:** Check the controller to arm connection.

8: Joint Disabled Unexpectedly
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Joint disabled unexpectedly.

**Solution:** Check the controller to arm connection.

9: Joint Overheated
^^^^^^^^^^^^^^^^^^^

**Description:** Joint overheated.

**Solution:** Turn off the controller to cool down the joint.

10: Invalid Mode
^^^^^^^^^^^^^^^^

**Description:** Invalid mode command received.

**Solution:** Check that the driver version matches the controller firmware version.

11: Invalid Robot Command
^^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Invalid robot command indicator received.

**Solution:** Check that the driver version matches the controller firmware version.

12: Invalid Robot Command Size
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Robot command with unexpected size received.

**Solution:** Check that the driver version matches the controller firmware version.

13: Invalid Configuration Address
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Invalid configuration address.

**Solution:** Check that the driver version matches the controller firmware version.

14: Invalid Pending Command
^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Invalid pending command.

**Solution:** Flash the controller firmware of a different version.

15: Robot Input Mode Mismatch
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Robot input with modes different than configured modes received.

**Solution:** Verify that the sent joint inputs match the configured modes.

16: Robot Input Discontinuous
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Description:** Discontinuous robot input received.

**Solution:** Check that the joint inputs are continuous and are sent at higher than 300 Hz.
For more details, please refer to :ref:`getting_started/configuration:continuity_factors`.
