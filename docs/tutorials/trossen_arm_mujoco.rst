==================
Trossen Arm MuJoCo
==================

.. video:: trossen_arm_mujoco/stationary_ai_pick_place.mp4
    :class: hidden

.. video:: trossen_arm_mujoco/sim_to_real.mp4
    :class: hidden

.. raw:: html

    <style>.hidden { display: none; }</style>
    <div style="display: flex; justify-content: center; gap: 20px; flex-wrap: wrap; align-items: flex-start;">
        <div style="text-align: center; flex: 1; max-width: 48%;">
            <video autoplay loop muted playsinline style="width: 100%; height: 300px; object-fit: contain; background: #000;">
                <source src="../_images/stationary_ai_pick_place.mp4" type="video/mp4">
            </video>
            <p><em>Stationary AI Pick-and-Place Demo</em></p>
        </div>
        <div style="text-align: center; flex: 1; max-width: 48%;">
            <video autoplay loop muted playsinline style="width: 100%; height: 300px; object-fit: contain; background: #000;">
                <source src="../_images/sim_to_real.mp4" type="video/mp4">
            </video>
            <p><em>Sim-to-Real Transfer Demo</em></p>
        </div>
    </div>

Overview
========

This repository provides MuJoCo simulation environments for Trossen AI robotic arms.
It includes comprehensive robot models, inverse kinematics-based control, and tools for policy execution, data collection, and visualization.

What This Repository Offers
----------------------------

* MuJoCo XML models for Trossen AI robots:

    * WidowX AI (single arm manipulator)
    * Stationary AI (dual-arm stationary platform)
    * Mobile AI (dual-arm mobile manipulator)

* Differential inverse kinematics controller for Cartesian end-effector control
* Pick-and-place and target following demonstration scripts
* Motion capture and joint-controlled simulation environments
* Data collection and visualization tools for imitation learning

Tested Environment
------------------

* Ubuntu 22.04
* MuJoCo 3.2.3
* Python 3.10+

Installation
============

#. Clone the repository:

    .. code-block:: bash

        git clone https://github.com/TrossenRobotics/trossen_arm_mujoco.git ~/trossen_arm_mujoco


#. It is recommended to create a virtual environment before installing dependencies.
   Create a Conda environment with Python 3.10 or above.

    .. code-block:: bash

        conda create --name trossen_mujoco_env python=3.10
        conda activate trossen_mujoco_env

#. Install the package and required dependencies using:

    .. code-block:: bash

        cd ~/trossen_arm_mujoco
        pip install -e .

#. To verify the installation, run:

    .. code-block:: bash

        python3 trossen_arm_mujoco/scripts/wxai_pick_place.py

    If the simulation window appears with the robot performing pick-and-place, the setup was successful.

    In case of any issues, please refer to the `Troubleshooting`_ section.

Assets
======

All robot models are located in :guilabel:`trossen_arm_mujoco/assets/`.
The assets folder contains all robot models organized by robot type, along with mesh files and scene configurations.

Directory Structure
-------------------

.. code-block:: text

    trossen_arm_mujoco/assets/
    ├── meshes/          # STL files for robot components
    ├── wxai/
    │   ├── wxai_base.xml              # WidowX AI base model
    │   ├── wxai_follower.xml          # WidowX AI follower arm with camera
    │   ├── scene.xml                  # Basic visualization scene
    │   ├── scene_wxai_pick_place.xml  # Pick-and-place scene
    │   └── scene_wxai_follow_target.xml # Target following scene
    ├── stationary_ai/
    │   ├── stationary_ai.xml          # Dual-arm stationary platform
    │   ├── stationary_ai_mocap.xml    # Mocap-enabled model with weld constraints
    │   ├── scene.xml                  # Basic visualization scene
    │   ├── scene_stationary_ai_pick_place.xml # Pick-and-place with handoff
    │   ├── scene_joint.xml            # Joint-controlled setup
    │   └── scene_mocap.xml            # Motion capture-controlled setup
    └── mobile_ai/
        ├── mobile_ai.xml              # Mobile base + dual-arm
        ├── scene.xml                  # Basic visualization scene
        └── scene_mobile_ai_pick_place.xml # Mobile pick-and-place

Asset Details
-------------

**WidowX AI** - Single-arm manipulator:

* Base model (``wxai_base.xml``): 6-DOF arm
* Follower model (``wxai_follower.xml``): 6-DOF arm with camera

**Stationary AI** - Dual-arm stationary platform:

* Dual WXAI arms on shared base

**Mobile AI** - Mobile manipulator:

* Differential drive mobile base
* Dual WXAI arms

Motion Capture vs Joint-Controlled Environments
------------------------------------------------

**Motion Capture** (``stationary_ai/scene_mocap.xml``):

* Uses predefined mocap bodies that move the robot arms based on scripted end effector movements
* Useful for defining task trajectories and generating reference motions

**Joint Control** (``stationary_ai/scene_joint.xml``):

* Uses position controllers for each joint, similar to real-world robot setup
* Used for replaying recorded trajectories with realistic joint-level control
* Enables clean simulation visuals without mocap bodies visible in rendered output

All models are derived from URDF descriptions in `TrossenRobotics/trossen_arm_description <https://github.com/TrossenRobotics/trossen_arm_description>`_. See individual ``README.md`` files in asset folders for detailed URDF→MJCF derivation steps.

MuJoCo IK Demo Scripts
======================

These scripts demonstrate inverse kinematics-based control for manipulation tasks.

Pick-and-Place Demonstrations
------------------------------

**WidowX AI** - Single arm pick-and-place:

.. code-block:: bash

    python3 trossen_arm_mujoco/scripts/wxai_pick_place.py

**Stationary AI** - Dual-arm pick-and-place with handoff:

.. code-block:: bash

    python3 trossen_arm_mujoco/scripts/stationary_ai_pick_place.py

**Mobile AI** - Mobile base navigation + dual-arm manipulation:

.. code-block:: bash

    python3 trossen_arm_mujoco/scripts/mobile_ai_pick_place.py

Target Following Demo
----------------------

Real-time end-effector tracking using differential IK:

.. code-block:: bash

    python3 trossen_arm_mujoco/scripts/wxai_follow_target.py

**Mouse Controls:**

* Double-click to select/highlight the target cube
* Ctrl + Left-click to rotate the view
* Ctrl + Right-click to move the target cube

The robot will track the target cube position in real-time.

Controller API
==============

The differential inverse kinematics controller (``Controller`` in :guilabel:`controller.py`) provides Cartesian end-effector control for all Trossen AI robots.

Key Features
------------

* Damped least squares differential IK for smooth motion
* Gripper control with open/close commands
* Support for all robot types (WidowX AI, Stationary AI, Mobile AI)
* Position-only or full 6D pose control

Basic Usage
-----------

.. code-block:: python

    from trossen_arm_mujoco.src.controller import Controller, RobotType

    # Initialize controller
    robot = Controller(
        model=mujoco_model,
        data=mujoco_data,
        robot_type=RobotType.WXAI,  # or RobotType.STATIONARY_AI, RobotType.MOBILE_AI
        arm_joint_names=["joint_1", "joint_2", ...],
        gripper_joint_names=["left_carriage_joint"],
        ik_scale=1.0,
        ik_damping=0.03,
    )

    # Command end-effector pose
    error = robot.set_ee_pose(
        target_position=np.array([0.3, 0.0, 0.2]),
        target_orientation=np.array([1.0, 0.0, 0.0, 0.0]),  # [w, x, y, z]
        position_only=False,
    )

    # Gripper control
    robot.open_gripper()
    robot.close_gripper()

Controller Parameters
---------------------

* ``ik_scale``: Scaling factor for IK velocity (default: 1.0)
* ``ik_damping``: Damping factor for singularity avoidance (default: 0.03)
* ``position_only``: When True, only controls position, orientation remains free

Data Collection
===============

Modules
-------

The :guilabel:`trossen_arm_mujoco` folder contains all Python modules necessary for running simulations, executing policies, recording episodes, and visualizing results.

Simulations
-----------

#. :guilabel:`ee_sim_env.py`

    * Loads ``stationary_ai/scene_mocap.xml`` (motion capture-based control).
    * The arms move by following the positions commanded to the mocap bodies.
    * Used for generating scripted policies that control the robot's arms in predefined ways.

#. :guilabel:`sim_env.py`

    * Loads ``stationary_ai/scene_joint.xml`` (position-controlled joints).
    * Uses joint controllers instead of mocap bodies.
    * Replays joint trajectories from :guilabel:`ee_sim_env.py`, enabling clean simulation visuals without mocap bodies visible in the rendered output.

Scripted Policy Execution
-------------------------

#. :guilabel:`scripted_policy.py`

    * Defines pre-scripted movements for the robot arms to perform tasks like picking up objects.
    * Uses the motion capture bodies to generate smooth movement trajectories.
    * In the current setup, a policy is designed to pick up a red block, with randomized block positions in the environment.

How the Data Collection Works
=============================

The data collection process simulates robot behavior in two stages: a mocap-driven recording phase followed by a clean replay phase for observation.
This pipeline allows you to define robot movements in Cartesian space, capture the corresponding joint trajectories, and then collect realistic sensor data without contaminating it with mocap artifacts.

Motion Capture Bodies
---------------------

Motion capture (mocap) bodies are dummy rigid bodies welded to the final link ``link_6`` of each robot arm.
This design enables intuitive motion definition and automatic inverse kinematics resolution:

* **Welding Behavior**:
    The mocap body is rigidly attached to ``link_6`` using a weld constraint.
    As the mocap body moves, the simulator ensures that the robot’s end-effector follows it.

* **Cartesian Control**:
    Instead of manually commanding joint angles, you move the mocap body in 3D space ``x, y, z`` using a scripted policy.
    The arm's joints are automatically adjusted to follow.

* **Joint State Recording**:
    As the end-effector tracks the mocap body, the simulation records the joint configurations required at each timestep.
    These are saved as the action trajectory.

Replay in Joint-Controlled Environment
--------------------------------------

The recorded joint trajectories are later replayed in a second scene where:

* The mocap bodies are removed (e.g., in ``stationary_ai/scene_joint.xml``).
* The arm is directly controlled using joint position commands.
* Observations are collected without the mocap artifacts.

During replay:

* Camera feeds from multiple viewpoints are captured.
* Joint state feedback is logged.
* Rewards and other metadata are recorded.

Step-by-Step Process
--------------------

#. Run :guilabel:`record_sim_episodes.py`

    #. Launch the mocap-driven simulation :guilabel:`ee_sim_env.py`.
    #. Execute a scripted Cartesian policy.
    #. Save the resulting joint position trajectory.
    #. Replay the trajectory immediately in a clean joint-controlled simulation :guilabel:`sim_env.py` to collect observations.

      * Camera feeds from 4 different viewpoints
      * Joint states (actual positions during execution)
      * Actions (input joint positions)
      * Reward values indicating success or failure

    To generate and save simulation episodes, use:

.. code-block:: bash

    python trossen_arm_mujoco/scripts/record_sim_episodes.py \
        --task_name sim_transfer_cube \
        --data_dir sim_transfer_cube \
        --num_episodes 5 \
        --onscreen_render

    Arguments:

    - ``--task_name``: Name of the task to execute (default: sim_transfer_cube).
    - ``--num_episodes``: Number of episodes to generate (default: 1).
    - ``--data_dir``: Directory where episodes will be saved (required).
    - ``--root_dir``: Root directory prefix for locating ``data_dir``. Default: ``~/.trossen/mujoco/data/``
    - ``--episode_len``: Number of simulation steps per episode (default: 1000).
    - ``--onscreen_render``: Enables on-screen rendering. Default: False (set to True to enable).
    - ``--inject_noise``: Adds noise to actions for variability. Default: False (set to True to enable).
    - ``--cam_names``: Comma-separated list of camera names for image collection (default: all available cameras).

    .. note::

        * The ``--task_name`` argument is used to load the corresponding configuration from :guilabel:`constants.py`.
        * You can extend ``SIM_TASK_CONFIGS`` in :guilabel:`constants.py` to support new task configurations.
        * All parameters loaded from :guilabel:`constants.py` can be individually overridden via command-line arguments.

#. Save the Data

    All observations and metadata are saved in .hdf5 format, with one file per episode:

    .. code-block:: bash

        ~/.trossen/mujoco/data/sim_transfer_cube/episode_0.hdf5
        ~/.trossen/mujoco/data/sim_transfer_cube/episode_1.hdf5

    Check the dataset structure in the `Dataset Structure`_ section for details on the saved data.

#. Visualizing the Data

    Use the :guilabel:`visualize_eps.py` script to convert episodes into videos:

    .. code-block:: bash

        python trossen_arm_mujoco/scripts/visualize_eps.py \
            --data_dir sim_transfer_cube \
            --output_dir videos \
            --fps 50

    Arguments:

    - ``--data_dir``: Directory containing :guilabel:`.hdf5` files (required), relative to ``--root_dir`` if provided.
    - ``--root_dir``: Root path prefix for locating ``data_dir``. Default: ``~/.trossen/mujoco/data/``
    - ``--output_dir``: Subdirectory inside ``data_dir`` where generated :guilabel:`.mp4` videos will be saved. Default: ``videos``
    - ``--fps``: Frames per second for the generated videos. Default: `50`

    .. note::

        If you do not specify ``--root_dir``, videos will be saved to ``~/.trossen/mujoco/data/<data_dir>/<output_dir>``.
        You can customize the output path by changing ``--root_dir``, ``--data_dir``, or ``--output_dir`` as needed.

#. Sim-to-real

    To deploy the episode on real hardware, run:

    .. code-block:: bash

        python trossen_arm_mujoco/scripts/replay_episode_real.py \
            --data_dir sim_transfer_cube \
            --episode_idx 0 \
            --fps 10 \
            --left_ip 192.168.1.5 \
            --right_ip 192.168.1.4

    This script:

        * Loads the selected joint trajectory (.hdf5)
        * Sends joint commands to real arms at the specified IP addresses
        * Logs the error between commanded vs actual joint positions
        * Returns both arms to home and sleep positions after execution

    Arguments:

    - ``--data_dir``: Directory containing `.hdf5` files (required), relative to ``--root_dir`` if provided.
    - ``--root_dir``: Root directory prefix for locating ``data_dir``. Default: `~/.trossen/mujoco/data/`
    - ``--episode_idx``: Index of the episode to replay. Default: `0`.
    - ``--fps``: Playback frame rate (Hz). Controls the action replay speed. Default: `10`.
    - ``--left_ip``: IP address of the left Trossen arm. Default: `192.168.1.5`.
    - ``--right_ip``: IP address of the right Trossen arm. Default: `192.168.1.4`.

Dataset Structure
=================

We use the `HDF5 <https://docs.h5py.org/en/stable/index.html>`_ format to store the recorded data, which is efficient for large datasets and allows for easy access to specific parts of the data.

Root Attributes
---------------

* ``sim`` A boolean attribute indicating whether the data was collected in simulation (``True``) or on real hardware (``False``).

* ``observations`` :guilabel:`group`: Contains all the observations recorded during the simulation.

    * ``images`` :guilabel:`subgroup`: Stores image data from multiple cameras.

        * Each camera has its own dataset named after the camera (e.g., ``cam_name``).
        * Dataset shape: ``(max_timesteps, 480, 640, 3)``, where:

            * ``max_timesteps``: Number of timesteps in the episode.
            * ``480, 640, 3``: Image dimensions (height, width, RGB channels).

        * Data type: ``uint8`` (8-bit unsigned integers for pixel values).
        * Chunked storage: ``(1, 480, 640, 3)`` for efficient access to individual timesteps.

    * ``qpos``: Joint positions of the robot arms in :guilabel:`radians` and gripper positions in :guilabel:`meters`.

        * Shape: ``(max_timesteps, 16)``, where:

            * ``max_timesteps``: Number of timesteps in the episode.
            * ``16``: Number of joints (8 per arm: 6 revolute in radians + 2 prismatic in meters).

    * ``qvel``: Joint velocities of the robot arms in :guilabel:`radians/s` and gripper velocities in :guilabel:`meters/s`.

        * Shape: ``(max_timesteps, 16)``.

* ``action``: Contains the commanded joint positions (in :guilabel:`radians`) and gripper positions (in :guilabel:`meters`) for the robot arms.

    * Shape: ``(max_timesteps, 16)``, where:

        * ``max_timesteps``: Number of timesteps in the episode.
        * ``16``: Number of control dimensions (8 per arm: 6 revolute joints in radians + 2 prismatic joints in meters).

Additional Data
---------------

* Any additional data in ``data_dict`` is stored as separate datasets under the root group.

    * Each dataset is named after the corresponding key in ``data_dict``.
    * The data is written using ``root[name][...] = array``.

This structure ensures efficient storage and retrieval of simulation data, supporting tasks like visualization, analysis, and sim-to-real transfer.

Customization
=============

Modifying Tasks
---------------

To create a custom task, modify :guilabel:`ee_sim_env.py` and define a new subclass of `TrossenAIStationaryEETask` this will be used for running the scripted policy.
Implement the following methods:

- ``initialize_episode(self, physics)``: Sets up the initial environment state, including robot and object positions.
- ``get_env_state(self, physics)``: Defines the data to be recorded as observations from the environment.
- ``get_reward(self, physics)``: Implements the reward function to determine task success criteria.

.. code-block:: python

    class CustomTask(TrossenAIStationaryEETask):
        def initialize_episode(self, physics):
            # Set up the initial state of the environment
            pass

        def get_env_state(self, physics):
            # Define the observations to be recorded
            pass

        def get_reward(self, physics):
            # Implement the reward function
            pass

Example:

.. code-block:: python

    class TransferCubeTask(TrossenAIStationaryEETask):
        def initialize_episode(self, physics):
            # Set up the initial state of the environment
            pass

        def get_env_state(self, physics):
            # Define the observations to be recorded
            pass

        def get_reward(self, physics):
            # Implement the reward function
            pass

.. code-block:: python

    def initialize_episode(self, physics: Physics) -> None:
        """
        Set up the simulation environment at the start of an episode.

        :param physics: The simulation physics engine.
        """
        self.initialize_robots(physics)
        # randomize box position
        cube_pose = sample_box_pose()
        box_start_idx = physics.model.name2id("red_box_joint", "joint")
        np.copyto(physics.data.qpos[box_start_idx : box_start_idx + 7], cube_pose)

        super().initialize_episode(physics)

Here, ``sample_box_pose()`` is a function that generates a random pose for the red box.
Then we get the joint index of the red box and set its position using `np.copyto()`.
The ``initialize_robots()`` method is called to set the initial positions of the robot arms.
The ``super().initialize_episode(physics)`` call ensures that the base class's initialization logic is executed, setting up the environment correctly.


.. code-block:: python

    @staticmethod
    def get_env_state(physics: Physics) -> np.ndarray:
        """
        Retrieve the environment state specific to this task.

        :param physics: The simulation physics engine.
        :return: The state of the environment.
        """
        env_state = physics.data.qpos.copy()[16:]
        return env_state

The ``get_env_state()`` method retrieves the environment state, which includes the joint positions of the red box.
The ``physics.data.qpos.copy()[16:]`` line extracts the joint positions starting from index 16, which corresponds to the red box's joint positions.
`physics.data.qpos` is a numpy array that contains the positions of all joints in the simulation.
Each arm has 6 revolute joints and 2 prismatic joints for gripper.
Therefore the first 16 indices are occupied by the 2 robot arms.
The rest are the joint states of the red box which is a free joint.


.. code-block:: python

    def get_reward(self, physics: Physics) -> int:
        """
        Compute the reward based on the cube's interaction with the robot and the environment.

        :param physics: The simulation physics engine.
        :return: The computed reward.
        """
        # return whether left gripper is holding the box
        all_contact_pairs = []
        for i_contact in range(physics.data.ncon):
            id_geom_1 = physics.data.contact[i_contact].geom1
            id_geom_2 = physics.data.contact[i_contact].geom2
            name_geom_1 = physics.model.id2name(id_geom_1, "geom")
            name_geom_2 = physics.model.id2name(id_geom_2, "geom")
            contact_pair = (name_geom_1, name_geom_2)
            all_contact_pairs.append(contact_pair)
        touch_left_gripper = (
            "red_box",
            "left/gripper_follower_left",
        ) in all_contact_pairs
        touch_right_gripper = (
            "red_box",
            "right/gripper_follower_left",
        ) in all_contact_pairs
        touch_table = ("red_box", "table") in all_contact_pairs

        reward = 0
        if touch_right_gripper:
            reward = 1
        if touch_right_gripper and not touch_table:  # lifted
            reward = 2
        if touch_left_gripper:  # attempted transfer
            reward = 3
        if touch_left_gripper and not touch_table:  # successful transfer
            reward = 4
        return reward

The ``get_reward()`` method computes the reward based on the interactions between the robot arms and the red box.
It checks for contact pairs between the red box and the left and right grippers, as well as the table.
The reward is assigned based on the following conditions:
- If the right gripper touches the red box, the reward is 1.
- If the right gripper touches the red box and it is not touching the table, the reward is 2 (indicating that the box is lifted).
- If the left gripper touches the red box, the reward is 3 (indicating an attempted transfer).
- If the left gripper touches the red box and it is not touching the table, the reward is 4 (indicating a successful transfer).


Similarly we will also have to modify the :guilabel:`sim_env.py` file to add the new task this will be used for running the joint controlled simulation.
Similar to the :guilabel:`ee_sim_env.py` file, we will have to implement the following methods:

- ``initialize_episode(self, physics)``: Sets up the initial environment state, including robot and object positions.
- ``get_env_state(self, physics)``: Defines the data to be recorded as observations from the environment.
- ``get_reward(self, physics)``: Implements the reward function to determine task success criteria.

.. code-block:: python

    class CustomTask(TrossenAIStationaryTask):
        def initialize_episode(self, physics):
            # Set up the initial state of the environment
            pass

        def get_env_state(self, physics):
            # Define the observations to be recorded
            pass

        def get_reward(self, physics):
            # Implement the reward function
            pass

Example:

.. code-block:: python

    class TransferCubeTask(TrossenAIStationaryTask):
        def initialize_episode(self, physics):
            # Set up the initial state of the environment
            pass

        def get_env_state(self, physics):
            # Define the observations to be recorded
            pass

        def get_reward(self, physics):
            # Implement the reward function
            pass

.. code-block:: python

    def initialize_episode(self, physics: Physics) -> None:
        """
        Initializes the episode, resetting the robot's pose and cube position.

        :param physics: The MuJoCo physics simulation instance.
        """
        # TODO Notice: this function does not randomize the env configuration. Instead, set
        # BOX_POSE from outside reset qpos, control and box position
        with physics.reset_context():
            physics.named.data.qpos[:16] = START_ARM_POSE
            assert BOX_POSE[0] is not None
            physics.named.data.qpos[-7:] = BOX_POSE[0]

        super().initialize_episode(physics)

The ``initialize_episode()`` method sets the initial state of the environment, including the robot arms and the red box.
The ``physics.named.data.qpos[:16] = START_ARM_POSE`` line sets the initial joint positions of the robot arms, while ``physics.named.data.qpos[-7:] = BOX_POSE[0]`` sets the position of the red box.
We store the randomized box position in the ``BOX_POSE`` variable, which is passed to the ``initialize_episode()`` method.
So that we can correctly set the position of the red box in the :guilabel:`sim_env.py` file whihc is used for running the joint controlled simulation.

.. code-block:: python

    @staticmethod
    def get_env_state(physics: Physics) -> np.ndarray:
        """
        Retrieves the environment state, including joint positions and box position.

        :param physics: The MuJoCo physics simulation instance.
        :return: The environment state as a numpy array.
        """
        env_state = physics.data.qpos.copy()[16:]
        return env_state

The ``get_env_state()`` method remain the same as in the :guilabel:`ee_sim_env.py` file, retrieving the joint positions of the red box.
You can change this to your liking if you want to add more information to the environment state.

.. code-block:: python

    def get_reward(self, physics: Physics) -> int:
        """
        Computes the reward based on whether the cube has been transferred successfully.

        :param physics: The MuJoCo physics simulation instance.
        :return: The computed reward which is whether left gripper is holding the box
        """
        all_contact_pairs = []
        for i_contact in range(physics.data.ncon):
            id_geom_1 = physics.data.contact[i_contact].geom1
            id_geom_2 = physics.data.contact[i_contact].geom2
            name_geom_1 = physics.model.id2name(id_geom_1, "geom")
            name_geom_2 = physics.model.id2name(id_geom_2, "geom")
            contact_pair = (name_geom_1, name_geom_2)
            all_contact_pairs.append(contact_pair)

        touch_left_gripper = (
            "red_box",
            "left/gripper_follower_left",
        ) in all_contact_pairs
        touch_right_gripper = (
            "red_box",
            "right/gripper_follower_left",
        ) in all_contact_pairs
        touch_table = ("red_box", "table") in all_contact_pairs

        reward = 0
        if touch_right_gripper:
            reward = 1
        # lifted
        if touch_right_gripper and not touch_table:
            reward = 2
        # attempted transfer
        if touch_left_gripper:
            reward = 3
        # successful transfer
        if touch_left_gripper and not touch_table:
            reward = 4
        return reward

The ``get_reward()`` method also remains the same as in the :guilabel:`ee_sim_env.py` file, computing the reward based on the interactions between the robot arms and the red box.
You can change this to your liking if you want to add more information to the environment state.

We see that ``get_env_state()`` and ``get_reward()`` methods are the same in both files, but we have to implement them in both files because they are used in different contexts.
This is because the :guilabel:`ee_sim_env.py` file is used for running the scripted policy, while the :guilabel:`sim_env.py` file is used for running the joint controlled simulation.
This allows us to have different implementations of the same methods in different contexts, which is useful for customizing the behavior of the robot arms in different scenarios.

The ``initialize_episode()`` method is different in both as we randomize the box position in the :guilabel:`ee_sim_env.py` file, while in the :guilabel:`sim_env.py` file we set the box position to the value used in the :guilabel:`ee_sim_env.py` file.


Changing Policy Behavior
------------------------

To define new behavior for the robotic arms, modify :guilabel:`scripted_policy.py`.
Update the trajectory generation logic in ``PickAndTransferPolicy.generate_trajectory()`` or create a new class of your own.

Each movement step in the trajectory is defined by:

- ``t``: The time step at which the movement occurs.
- ``xyz``: The target position of the end effector in 3D space.
- ``quat``: The target orientation of the end effector, represented as a quaternion.
- ``gripper``: The target gripper finger position (0 to 0.044, where 0 is closed and 0.044 is fully open).


Example:

.. code-block:: python

    def generate_trajectory(self, ts_first: TimeStep):
        self.left_trajectory = [
            {"t": 0, "xyz": [0, 0, 0.4], "quat": [1, 0, 0, 0], "gripper": 0},
            {"t": 100, "xyz": [0.1, 0, 0.3], "quat": [1, 0, 0, 0], "gripper": 0.044}
        ]

We define some fixed and dynamic waypoints as follows:

.. code-block:: python

    init_mocap_pose_right = ts_first.observation["mocap_pose_right"]
    init_mocap_pose_left = ts_first.observation["mocap_pose_left"]

    box_info = np.array(ts_first.observation["env_state"])
    box_xyz = box_info[:3]
    print(f"Generate trajectory for {box_xyz=}")

    gripper_pick_quat = Quaternion(init_mocap_pose_right[3:])
    gripper_pick_quat = gripper_pick_quat * Quaternion(
        axis=[0.0, 1.0, 0.0], degrees=-45
    )

    meet_left_quat = Quaternion(axis=[1.0, 0.0, 0.0], degrees=90)

    meet_xyz = np.array([0.0, 0.0, 0.3])


Here we define the initial pose of the right and left grippers using the ``ts_first.observation["mocap_pose_right"]`` and ``ts_first.observation["mocap_pose_left"]`` values.
We also define the box position using the ``ts_first.observation["env_state"]`` value, which contains the joint positions of the red box.
We then define the quaternion for the gripper pick pose and the meet pose using the ``Quaternion`` class from the ``trossen_arm_mujoco.utils`` module.


Adding New Environment Setups
-----------------------------

The simulation uses XML files stored in the :guilabel:`assets/` directory. To introduce a new environment setup:

1. Create a new XML configuration file in :guilabel:`assets/` with the desired object placements and constraints.
2. Modify :guilabel:`sim_env.py` to load the new environment by specifying the new XML file.
3. Update the scripted policies in :guilabel:`scripted_policy.py` to accommodate new task goals and constraints.

Troubleshooting
===============

If you encounter Mesa Loader or ``mujoco.FatalError: gladLoadGL error`` issues, use the following command:

.. code-block:: bash

    export LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libstdc++.so.6

Related Links
=============

- `Trossen Robotics <https://www.trossenrobotics.com/>`_
- `Trossen Arm Documentation <https://docs.trossenrobotics.com/trossen_arm/>`_
- `Trossen Arm Description (URDF) <https://github.com/TrossenRobotics/trossen_arm_description>`_
- `MuJoCo Documentation <https://mujoco.readthedocs.io/>`_
- `MuJoCo Python Bindings <https://mujoco.readthedocs.io/en/stable/python.html>`_
