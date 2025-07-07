==============
MoveIt Configs
==============

The Trossen Arm MoveIt configuration package provides the necessary files to control the Trossen Arm using `MoveIt <https://moveit.picknik.ai/main/index.html>`_.

Two different hardware interfaces are supported:

-   **Mock Hardware**: For testing and development purposes, simulating the arm's behavior.
-   **Real Hardware**: For controlling the actual Trossen Arm hardware.

These interfaces can be swapped by changing the ``ros2_control_hardware_type`` launch argument when launching the bringup package.

Usage
=====

We will first cover how to launch MoveIt with a mocked Trossen Arm hardware interface and associated controllers.

.. code-block:: bash

    source ~/ros2_ws/install/setup.bash
    ros2 launch trossen_arm_moveit moveit.launch.py robot_model:=wxai ros2_control_hardware_type:=mock_components

This will start the MoveIt ``move_group`` node, which is responsible for planning and executing motions for the Trossen Arm.
You can read more about what you can do with the node on `the move_group node documentation page <https://moveit.picknik.ai/main/doc/concepts/move_group.html>`_.
You can see the topics, services, and actions that are available by running:

.. code-block:: bash

    ros2 node info /move_group
    /move_group
    Subscribers:
        /parameter_events: rcl_interfaces/msg/ParameterEvent
        /trajectory_execution_event: std_msgs/msg/String
    Publishers:
        /display_contacts: visualization_msgs/msg/MarkerArray
        /display_planned_path: moveit_msgs/msg/DisplayTrajectory
        /motion_plan_request: moveit_msgs/msg/MotionPlanRequest
        /parameter_events: rcl_interfaces/msg/ParameterEvent
        /rosout: rcl_interfaces/msg/Log
    Service Servers:
        /apply_planning_scene: moveit_msgs/srv/ApplyPlanningScene
        /check_state_validity: moveit_msgs/srv/GetStateValidity
        /clear_octomap: std_srvs/srv/Empty
        /compute_cartesian_path: moveit_msgs/srv/GetCartesianPath
        /compute_fk: moveit_msgs/srv/GetPositionFK
        /compute_ik: moveit_msgs/srv/GetPositionIK
        /get_planner_params: moveit_msgs/srv/GetPlannerParams
        /load_map: moveit_msgs/srv/LoadMap
        /move_group/describe_parameters: rcl_interfaces/srv/DescribeParameters
        /move_group/get_parameter_types: rcl_interfaces/srv/GetParameterTypes
        /move_group/get_parameters: rcl_interfaces/srv/GetParameters
        /move_group/list_parameters: rcl_interfaces/srv/ListParameters
        /move_group/set_parameters: rcl_interfaces/srv/SetParameters
        /move_group/set_parameters_atomically: rcl_interfaces/srv/SetParametersAtomically
        /plan_kinematic_path: moveit_msgs/srv/GetMotionPlan
        /query_planner_interface: moveit_msgs/srv/QueryPlannerInterfaces
        /save_map: moveit_msgs/srv/SaveMap
        /set_planner_params: moveit_msgs/srv/SetPlannerParams
    Service Clients:

    Action Servers:
        /execute_trajectory: moveit_msgs/action/ExecuteTrajectory
        /move_action: moveit_msgs/action/MoveGroup
    Action Clients:

You will also see that RViz has been launched with the Trossen Arm model displayed along with MoveIt's MotionPlanning display and panel.
These new components allow you to plan, execute, and visualize motions for the Trossen Arm.

.. image:: images/moveit_rviz.png
    :width: 80%
    :align: center

Let's start using it by planning a motion to the ``upright`` configuration.

#.  In the MotionPlanning panel, select the following:

    -   **Planning Group**: ``arm``
    -   **Start State**: ``<current>``
    -   **Goal State**: ``upright``

    You will notice that orange "Goal State Query" robot model update to reflect the ``upright`` configuration.

    .. image:: images/upright.png
        :width: 80%
        :align: center

#.  Click the **Plan** button to generate a motion plan to the ``upright`` configuration.
    If planning is successful, you will see a translucent robot move from the current configuration to the ``upright`` configuration.

    .. image:: images/plan.png
        :width: 80%
        :align: center

#.  Click the **Execute** button to execute the planned motion.
    The robot should move to the ``upright`` configuration.

    .. image:: images/execute.png
        :width: 80%
        :align: center

#.  You can also use the sphere, arrow, and ring markers to interactively set the goal state.
    Click on the sphere marker and drag it to a new position to change the goal state.
    Then click **Plan** and **Execute** to see the robot move to the new position.

    .. image:: images/interactive_goal.png
        :width: 80%
        :align: center

You can now proceed to launch MoveIt with your real Trossen Arm hardware.
Run the command below to launch MoveIt and repeat the steps above to plan and execute motions with the real hardware.

.. code-block:: bash

    source ~/ros2_ws/install/setup.bash
    ros2 launch trossen_arm_moveit moveit.launch.py robot_model:=wxai ros2_control_hardware_type:=real

To further customize the moveit launch file at runtime, refer to the table below, or run the command below

.. code-block:: console

    ros2 launch trossen_arm_moveit moveit.launch.py --show-args

.. csv-table::
    :file: /_data/trossen_arm_moveit.csv
    :header-rows: 1
    :widths: 20, 60, 20, 20
