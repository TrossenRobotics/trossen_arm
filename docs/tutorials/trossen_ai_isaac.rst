================
Trossen AI Isaac
================

Overview
========

This package provides NVIDIA Isaac Sim and Isaac Lab integration for Trossen AI robotic arms.
It includes USD robot models, inverse kinematics-based task examples, and Isaac Lab tasks for reinforcement learning and imitation learning.

Features
--------

* Isaac Sim USD models for Trossen AI robots:

    * WidowX AI (single arm base, follower, leader left, leader right)
    * Stationary AI (dual-arm stationary platform)
    * Mobile AI (dual-arm mobile manipulator)

* Robot bringup utilities for quick model visualization and testing
* Differential inverse kinematics controller for Cartesian end-effector control
* Example scripts for pick-and-place and target following tasks
* Isaac Lab tasks for reinforcement learning (e.g., reach, lift, cabinet)
* Teleoperation interface for imitation learning data collection

Tested Environment
------------------

* Ubuntu 22.04
* Isaac Sim 5.1.0
* Isaac Lab 2.3.0
* NVIDIA GeForce RTX 5090

Installation
============

Prerequisites
-------------

Install Isaac Lab 2.3.0 following the `official installation guide <https://isaac-sim.github.io/IsaacLab/release/2.3.0/source/setup/installation/index.html>`_.
This will also install Isaac Sim 5.1.0.

.. note::

    Recommended Installation Method: Binary + Source (binary download for Isaac Sim + source via git for Isaac Lab).
    This differs from the official recommended method.

Clone Repository
----------------

.. code-block:: bash

    git clone https://github.com/TrossenRobotics/trossen_ai_isaac.git
    cd trossen_ai_isaac

Install Trossen AI Extension
----------------------------

Install the extension for Isaac Lab:

.. code-block:: bash

    ~/IsaacLab/isaaclab.sh -p -m pip install -e source/trossen_ai_isaac

Verify the environments are registered:

.. code-block:: bash

    ~/IsaacLab/isaaclab.sh -p scripts/tools/list_envs.py

You should see output similar to:

.. code-block:: text

    | 1  | Isaac-Open-Drawer-WXAI-v0      | isaaclab.envs:ManagerBasedRLEnv | trossen_ai_isaac.tasks.manager_based.manipulation.wxai.cabinet.joint_pos_env_cfg:WXAICabinetEnvCfg           |
    | 2  | Isaac-Open-Drawer-WXAI-Play-v0 | isaaclab.envs:ManagerBasedRLEnv | trossen_ai_isaac.tasks.manager_based.manipulation.wxai.cabinet.joint_pos_env_cfg:WXAICabinetEnvCfg_PLAY      |
    | 3  | Isaac-Lift-Cube-WXAI-v0        | isaaclab.envs:ManagerBasedRLEnv | trossen_ai_isaac.tasks.manager_based.manipulation.wxai.lift.config.joint_pos_env_cfg:WXAICubeLiftEnvCfg      |
    | 4  | Isaac-Lift-Cube-WXAI-Play-v0   | isaaclab.envs:ManagerBasedRLEnv | trossen_ai_isaac.tasks.manager_based.manipulation.wxai.lift.config.joint_pos_env_cfg:WXAICubeLiftEnvCfg_PLAY |
    | 5  | Isaac-Lift-Cube-WXAI-IK-Rel-v0 | isaaclab.envs:ManagerBasedRLEnv | trossen_ai_isaac.tasks.manager_based.manipulation.wxai.lift.config.ik_rel_env_cfg:WXAICubeLiftEnvCfg         |
    | 6  | Isaac-Lift-Cube-WXAI-IK-Abs-v0 | isaaclab.envs:ManagerBasedRLEnv | trossen_ai_isaac.tasks.manager_based.manipulation.wxai.lift.config.ik_abs_env_cfg:WXAICubeLiftEnvCfg         |
    | 7  | Isaac-Reach-WXAI-v0            | isaaclab.envs:ManagerBasedRLEnv | trossen_ai_isaac.tasks.manager_based.manipulation.wxai.reach.config.joint_pos_env_cfg:WXAIReachEnvCfg        |
    | 8  | Isaac-Reach-WXAI-Play-v0       | isaaclab.envs:ManagerBasedRLEnv | trossen_ai_isaac.tasks.manager_based.manipulation.wxai.reach.config.joint_pos_env_cfg:WXAIReachEnvCfg_PLAY   |
    | 9  | Isaac-Reach-WXAI-IK-Rel-v0     | isaaclab.envs:ManagerBasedRLEnv | trossen_ai_isaac.tasks.manager_based.manipulation.wxai.reach.config.ik_rel_env_cfg:WXAIReachEnvCfg           |
    | 10 | Isaac-Reach-WXAI-IK-Abs-v0     | isaaclab.envs:ManagerBasedRLEnv | trossen_ai_isaac.tasks.manager_based.manipulation.wxai.reach.config.ik_abs_env_cfg:WXAIReachEnvCfg           |

Robot Assets
============

All robot USD models are located in :guilabel:`assets/robots/`:

.. code-block:: text

    assets/robots/
    ├── mobile_ai/
    │   └── mobile_ai.usd
    ├── stationary_ai/
    │   └── stationary_ai.usd
    └── wxai/
        ├── wxai_base.usd
        ├── wxai_follower.usd
        ├── wxai_leader_left.usd
        └── wxai_leader_right.usd

Asset Generation
----------------

All USD files are generated from URDF descriptions in `TrossenRobotics/trossen_arm_description <https://github.com/TrossenRobotics/trossen_arm_description>`_.
See :guilabel:`assets/robots/asset_generation.md` for detailed generation instructions.

Isaac Sim Demo Scripts
======================

.. note::

    Commands below assume Isaac Sim is installed at :guilabel:`~/isaacsim/`. Adjust the path if your installation directory differs.

Robot Bringup
-------------

Load and visualize any robot model:

.. code-block:: bash

    ~/isaacsim/isaac-sim.sh scripts/robot_bringup.py [robot_name]

Supported robots:

* ``wxai_base`` (default)
* ``wxai_follower``
* ``wxai_leader_left``
* ``wxai_leader_right``
* ``stationary_ai``
* ``mobile_ai``

Pick and Place Demo
-------------------

Run pick and place demonstrations for different robot configurations.

**WidowX AI Pick and Place**

.. video:: trossen_ai_isaac/wxai_pick_place_demo.mp4
    :align: center
    :nocontrols:
    :autoplay:
    :playsinline:
    :muted:
    :loop:
    :width: 80%
    :caption: WidowX AI Pick and Place Demo

.. code-block:: bash

    ~/isaacsim/python.sh scripts/wxai_pick_place.py

**Stationary AI Pick and Place**

.. video:: trossen_ai_isaac/stationary_ai_pick_place_demo.mp4
    :align: center
    :nocontrols:
    :autoplay:
    :playsinline:
    :muted:
    :loop:
    :width: 80%
    :caption: Stationary AI Pick and Place Demo

.. code-block:: bash

    ~/isaacsim/python.sh scripts/stationary_ai_pick_place.py

**Mobile AI Pick and Place**

.. video:: trossen_ai_isaac/mobile_ai_pick_place_demo.mp4
    :align: center
    :nocontrols:
    :autoplay:
    :playsinline:
    :muted:
    :loop:
    :width: 80%
    :caption: Mobile AI Pick and Place Demo

.. code-block:: bash

    ~/isaacsim/python.sh scripts/mobile_ai_pick_place.py

Follow Target Demo
------------------

Real-time end-effector tracking using differential IK.

.. video:: trossen_ai_isaac/wxai_follow_target_demo.mp4
    :align: center
    :nocontrols:
    :autoplay:
    :playsinline:
    :muted:
    :loop:
    :width: 80%
    :caption: WidowX AI Follow Target Demo

.. code-block:: bash

    ~/isaacsim/python.sh scripts/wxai_follow_target.py

Isaac Lab Demo Tasks
====================

.. note::

    Commands below assume Isaac Lab is installed at :guilabel:`~/IsaacLab/`. Adjust the path if your installation directory differs.

Available Tasks
---------------

* ``Isaac-Reach-WXAI-v0`` - Move end-effector to target pose using joint position control
* ``Isaac-Reach-WXAI-IK-Rel-v0`` - Reach task with relative IK delta actions
* ``Isaac-Reach-WXAI-IK-Abs-v0`` - Reach task with absolute IK pose actions
* ``Isaac-Lift-Cube-WXAI-v0`` - Pick up a cube and lift it to a target height
* ``Isaac-Open-Drawer-WXAI-v0`` - Open a cabinet drawer by grasping and pulling

Reinforcement Learning
----------------------

Train a policy using RSL-RL PPO:

.. code-block:: bash

    ~/IsaacLab/isaaclab.sh -p scripts/reinforcement_learning/rsl_rl/train.py \
        --task Isaac-Reach-WXAI-v0 \
        --num_envs 32 \
        --max_iterations 4000 \
        --headless

Training Options
~~~~~~~~~~~~~~~~

* ``--num_envs 32``: Number of parallel environments (adjust based on GPU memory)
* ``--max_iterations 4000``: Number of iterations steps (adjust as per training tasks)
* ``--headless``: Run without GUI for faster training

Training logs and checkpoints are saved to :guilabel:`logs/rsl_rl/<task>/<timestamp>/`.

Resume Training
~~~~~~~~~~~~~~~

Resume training from a checkpoint:

.. code-block:: bash

    ~/IsaacLab/isaaclab.sh -p scripts/reinforcement_learning/rsl_rl/train.py \
        --task Isaac-Reach-WXAI-v0 \
        --num_envs 32 \
        --headless \
        --resume \
        --load_run <timestamp> \
        --checkpoint <model>.pt

Run Trained Policy
~~~~~~~~~~~~~~~~~~

Run a trained policy:

.. code-block:: bash

    ~/IsaacLab/isaaclab.sh -p scripts/reinforcement_learning/rsl_rl/play.py \
        --task Isaac-Reach-WXAI-v0 \
        --num_envs 16 \
        --checkpoint logs/rsl_rl/<task>/<timestamp>/<model>.pt

Imitation Learning
------------------

Teleoperation for Data Collection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Use teleoperation to collect demonstration data:

.. code-block:: bash

    ~/IsaacLab/isaaclab.sh -p scripts/teleoperation/teleop_se3_agent.py \
        --task Isaac-Reach-WXAI-IK-Rel-v0 \
        --teleop_device keyboard

Teleop Device Options
~~~~~~~~~~~~~~~~~~~~~

* ``keyboard``
* ``spacemouse``
* ``gamepad``

Controller API
==============

The :guilabel:`TrossenAIController` class provides a unified interface for controlling all Trossen AI robots.

Key Features
------------

* Differential inverse kinematics for Cartesian end-effector control
* Gripper control with open/close commands
* Support for all robot types (WidowX AI, Stationary AI, Mobile AI)

Basic Usage
-----------

.. code-block:: python

    from controller import RobotType, TrossenAIController

    # Initialize controller
    robot = TrossenAIController(
        robot_path="/World/wxai_robot",
        robot_type=RobotType.WXAI,
        arm_dof_indices=[0, 1, 2, 3, 4, 5],
        gripper_dof_index=6,
        default_dof_positions=[0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.044, 0.044],
    )

    # Command end-effector pose
    robot.set_end_effector_pose(
        position=np.array([0.3, 0.0, 0.2]),
        orientation=np.array([0.7071, 0.0, 0.7071, 0.0]),  # [w, x, y, z]
    )

    # Gripper control
    robot.open_gripper()
    robot.close_gripper()

    # Reset to default pose
    robot.reset_to_default_pose()

Controller Initialization Parameters
------------------------------------

* ``robot_path``: Path to the robot in the simulation scene
* ``robot_type``: Type of robot (``RobotType.WXAI``, ``RobotType.STATIONARY_AI``, ``RobotType.MOBILE_AI``)
* ``arm_dof_indices``: Indices of the arm degrees of freedom
* ``gripper_dof_index``: Index of the gripper degree of freedom
* ``default_dof_positions``: Default joint positions for reset

End-Effector Control
--------------------

The ``set_end_effector_pose()`` method uses differential inverse kinematics to compute joint commands:

.. code-block:: python

    robot.set_end_effector_pose(
        position=np.array([x, y, z]),      # Target position in meters
        orientation=np.array([w, x, y, z]) # Target orientation as quaternion
    )

Gripper Control
---------------

Control the gripper with simple open/close commands:

.. code-block:: python

    robot.open_gripper()   # Fully open gripper
    robot.close_gripper()  # Fully close gripper

Related Links
=============

* `Trossen Robotics <https://www.trossenrobotics.com/>`_
* `Trossen Arm Documentation <https://docs.trossenrobotics.com/trossen_arm/>`_
* `Trossen Arm Description (URDF) <https://github.com/TrossenRobotics/trossen_arm_description>`_
* `NVIDIA Isaac Sim <https://developer.nvidia.com/isaac-sim>`_
* `NVIDIA Isaac Lab <https://isaac-sim.github.io/IsaacLab>`_
* `RSL-RL <https://github.com/leggedrobotics/rsl_rl>`_
