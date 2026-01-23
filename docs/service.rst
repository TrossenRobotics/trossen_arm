=======================
Service and Maintenance
=======================

This section provides information on how to service and maintain your Trossen Arm hardware.

Cable Replacement Guides
========================

See the `cable replacement guides`_ for detailed instructions on how to replace the cables in the Trossen Arm.

.. _cable replacement guides: https://drive.google.com/drive/folders/1fTkOV6DC5rlOQEOLTlptDM7j4ATTRVNL

Cable routing diagrams can be seen below:

.. image:: service/images/cables_side.png
    :align: center
    :width: 800px

.. image:: service/images/cables_top.png
    :align: center
    :width: 800px

Motor Replacement Guides
========================

See the `motor replacement guides`_ for detailed instructions on how to replace the motors in the Trossen Arm.

.. _motor replacement guides: https://drive.google.com/drive/folders/1XYWOUI-m5p2t7TWM-cbzQznoVFy23upe?usp=drive_link

Arm Homing
==========

If a motor is replaced or if the arm loses its position for any reason, it is necessary to home the arm.
The process for homing the arm is as follows:

#.  Power off the arm.
#.  Remove the gripper fingers or paddles from the gripper carriages.
#.  Install the homing jigs on the base and wrist rotate motors.

    .. list-table::
        :align: center
        :header-rows: 1

        * - Base Motor Homing Jig
          - Wrist Rotate Motor Homing Jig
        * - .. image:: service/images/base_motor_homing_jig.jpg
              :align: center
              :width: 300px
          - .. image:: service/images/wrist_rotate_motor_homing_jig.jpg
              :align: center
              :width: 300px

#.  Close the gripper carriages such that they are both in contact with the retainer bearing housing.

    .. image:: service/images/gripper_carriages_closed.jpg
        :align: center
        :width: 600px

#.  Power on the arm.
#.  Download, unzip, and run the :download:`Trossen Arm Homing script </_downloads/trossen_arm_homing.zip>`.
#.  Follow the instructions in the script to home the arm.
#.  Power off the arm.
#.  Remove the homing jigs from the base and wrist rotate motors.
#.  Reinstall the gripper fingers or paddles on the gripper carriages.
#.  Your arm is now homed and ready for use!

Encoder Hardware test
=====================

Occasionally, the encoder in the gripper motor malfunctions and reads incorrect values.
This can cause issues including but not limited to:

-   Gripper position of a follower does not match the leader gripper position during teleoperation.
-   Position limit exceeded error: ``[ERROR] [Motor Interface] Joint 6 position limit exceeded: expected in range [-0.004000, 0.044000], motor reported x.xxxxxx. Setting to idle.``

Please follow the steps below to test the encoder.

#.  Download, unzip, and run the :download:`Encoder Test Script<_downloads/encoder_test.zip>`.
#.  Follow the instructions in the script to test the encoder.
#.  If the encoder is malfunctioning, please contact us at https://www.trossenrobotics.com/support.
