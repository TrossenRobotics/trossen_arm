=============================
Trossen AI Data Collection UI
=============================

.. image:: trossen_ai_data_collection_ui/images/trossen_ai_gui.png
   :width: 80%
   :alt: Trossen AI Data Collection UI
   :align: center

Overview
========

The **Trossen AI Data Collection UI** is a Python-based graphical user interface (GUI) designed for seamless and efficient robotic data collection.
It allows users to easily manage robot configurations, record tasks, and streamline data collection with real-time feedback, camera views, task management, and progress tracking.
This documentation provides a comprehensive guide to setting up, installing, and using the Trossen AI Data Collection UI, including all its features and functionalities.

Pre-Installation Setup
======================

Before installing the application, complete the following setup steps:

#. Install Miniconda:

   Download and install Miniconda, a minimal installer for Conda. It simplifies package management and deployment of Python environments.
   `Download Miniconda <https://www.anaconda.com/docs/getting-started/miniconda/install#basic-install-instructionsl>`_

#. Create a Virtual Environment:

   Use Miniconda to create and activate a virtual environment to ensure a clean setup for the application:

   .. code-block:: bash

      conda create -n trossen_ai_data_collection_ui_env python=3.10 -y
      conda activate trossen_ai_data_collection_ui_env

#. Verify Python Version:

   Ensure that Python 3.10 is activated in the environment by running:

   .. code-block:: bash

      python --version

   You should see ``Python 3.10.x`` as the output.

Installation
============

Once the pre-installation setup is complete, install the **Trossen AI Data Collection UI Application**.

#.  Install build dependencies on Linux:

    .. code-block:: bash

        sudo apt-get install -y \
            build-essential \
            cmake \
            libavcodec-dev \
            libavdevice-dev \
            libavfilter-dev \
            libavformat-dev \
            libavutil-dev \
            libswresample-dev \
            libswscale-dev \
            pkg-config \
            python3-dev

    For other systems, see: `Compiling PyAV <https://pyav.org/docs/develop/overview/installation.html#bring-your-own-ffmpeg>`_.

#.  Install the application:

    .. code-block:: bash

        pip install trossen_ai_data_collection_ui

    .. note::

        The command above requires sudo privileges and may prompt you for your password.

Post-Installation
=================

After the installation process, run the post-installation script to complete the setup:

The post-installation script sets up additional configurations, including:

#. Cloning and installing required dependencies for **Interbotix/lerobot**.
#. Resolving common issues with OpenCV and video encoding libraries.
#. Creating a desktop icon for easy access to the application.

Run the following command to complete the post-installation setup:

.. code-block:: bash

    trossen_ai_data_collection_ui_post_install

Once the desktop icon is created, right-click on it and select **Allow Launching** to ensure the application has the necessary permissions to run.

Launching the Application
=========================

Once the installation and post-installation setup are complete, you can launch the **Trossen AI Data Collection UI** through either the desktop application or via the command line.

#. Desktop Application

    After installation, a desktop shortcut named **Trossen AI Data Collection UI** will be available on your desktop.
    Simply click the shortcut to launch the application.

#. Command Line

    Alternatively, you can run the application directly from the terminal:

    .. code-block:: bash

        trossen_ai_data_collection_ui

Configuring the Robots
======================

The Trossen AI Data Collection UI provides a user-friendly interface for configuring robot settings such as **camera serial numbers** and **arm IP addresses**.
To configure the robot, follow these steps:

#. Launch the application and click on ``Edit`` in the top-left menu. Then select ``Robot Configuration``.
#. In the ``Robot Configuration`` window, you will be able to modify the YAML file that contains all robot-specific settings.
#. Update the relevant fields such as camera serial numbers and arm IP addresses as needed.

We support three robot configurations ``trossen_ai_stationary``, ``trossen_ai_mobile``, and ``trossen_ai_solo``.

An example configuration for the stationary robot is shown below:

.. code-block:: yaml

    trossen_ai_stationary:

        max_relative_target: null

        min_time_to_move_multiplier: 3.0

        camera_interface: 'opencv'

        leader_arms:
            right:
                ip: '192.168.1.3'
                model: 'V0_LEADER'
            left:
                ip: '192.168.1.2'
                model: 'V0_LEADER'

        follower_arms:
            right:
                ip: '192.168.1.5'
                model: 'V0_FOLLOWER'
            left:
                ip: '192.168.1.4'
                model: 'V0_FOLLOWER'

        cameras:
            cam_high:
                serial_number: 000123456789
                width: 640
                height: 480
                fps: 30
            cam_low:
                serial_number: 000123456789
                width: 640
                height: 480
                fps: 30
            cam_right_wrist:
                serial_number: 000123456789
                width: 640
                height: 480
                fps: 30
            cam_left_wrist:
                serial_number: 000123456789
                width: 640
                height: 480
                fps: 30

- ``max_relative_target``: Limits the magnitude of the relative positional target vector for safety purposes.
  Set this to a positive scalar to have the same value for all motors.
  The magnitude defines the maximum distance (in radians for rotational joints and meters for linear joints) that the end-effector can be commanded to move in a single command.
  When you feel more confident with teleoperation or running the policy, you can extend this safety limit and even remove it by setting it to ``null``.

- ``min_time_to_move_multiplier``:  Multiplier for computing minimum time (in seconds) for the arm to reach a target position.
  The final goal time is computed as: min_time_to_move = multiplier / fps.
  A smaller multiplier results in faster (but potentially jerky) motion.
  A larger multiplier results in smoother motion but with increased lag.
  A recommended starting value is 3.0.

- ``camera_interface``: Set this according to the camera interface you want to use.

  - ``opencv`` is the default and recommended option.
  - ``intel_realsense`` can be used if you have Intel RealSense cameras connected to the system.

- ``leader_arms``: Contains the IP addresses and models of the leader arms.

  - ``ip``: Update the IP addresses to match those assigned to your leader arms.
  - ``model``: The currently supported leader arm model is ``V0_LEADER``.

- ``follower_arms``: Contains the IP addresses and models of the follower arms.

  - ``ip``: Update the IP addresses to match those assigned to your follower arms.
  - ``model``: The currently supported follower arm model is ``V0_FOLLOWER``.

  .. note::

    Refer to :ref:`tutorials/lerobot/configuration:Setup IP Address` for more details on obtaining IP addresses.

- ``cameras``: The ``cameras`` section defines the configuration for each camera used in the system.
  Each camera entry (such as ``cam_high``, ``cam_low``, ``cam_right_wrist``, and ``cam_left_wrist``) includes:

    - ``serial_number``: The unique identifier for the camera device.
      For ``intel_realsense`` cameras, use the actual device serial number (e.g., 123456789).
      For ``opencv`` cameras, specify the camera index (e.g., 0, 1, 2).
    - ``width``: The width of the camera image in pixels.
    - ``height``: The height of the camera image in pixels.
    - ``fps``: The desired frames per second for capturing images.

  .. note::

    - Do not change the camera names (e.g., ``cam_high``, ``cam_low``, etc.) as they are referenced in the application for creating predefined robot layouts.
    - Ensure that the specified FPS is supported by the camera hardware.
    - Larger resolutions may require more processing power and could impact the overall system performance.

Configuring the Tasks
=====================

The Trossen AI Data Collection UI allows users to configure various tasks for data collection.
To configure tasks, follow these steps:

#. Launch the application and click on ``Edit`` in the top-left menu.
   Then select ``Task Configuration``.
#. In the ``Task Configuration`` window, you will be able to modify the YAML file that contains all task-specific settings.
#. Update the relevant fields such as task names, parameters, and other configurations as needed.

An example configuration for tasks is shown below:

.. code-block:: yaml

    - task_name: "trossen_ai_stationary_dummy"
      robot_model: "trossen_ai_stationary"
      task_description: "A dummy task for the Trossen AI Stationary robot."
      episode_length_s: 10
      warmup_time_s: 5
      reset_time_s: 5
      hf_user: "YourUser"
      fps: 30
      push_to_hub: false
      play_sounds: true
      disable_active_ui_updates: false
      save_interval: 1
      operators:
      - name: "YourOperator0"
        email: "youroperatoremail0@example.com"
      - name: "YourOperator1"
        email: "youroperatoremail1@example.com"

- ``task_name``: Name of the task. This should be unique for each task.
  This will also set the name of the dataset.
- ``robot_model``: The robot model associated with the task.
  This should match one of the robot configurations defined in the robot configuration YAML.
- ``task_description``: A brief description of the task.
- ``episode_length_s``: Duration of each episode in seconds.
- ``warmup_time_s``: Time in seconds to wait before starting the episode.
- ``reset_time_s``: Time in seconds to wait after the episode ends before resetting.
- ``hf_user``: Your Hugging Face username.
  This is used if you plan to push data to the Hugging Face Hub.
- ``fps``: Frames per second for data collection.
- ``push_to_hub``: Boolean flag indicating whether to push the collected data to the Hugging Face Hub.
- ``play_sounds``: Boolean flag indicating whether to play sounds during the task.
- ``disable_active_ui_updates``: Boolean flag to disable active UI updates during the task.
- ``save_interval``: Interval in episodes at which to encode images to video and save data to disk.
  For example, if set to 5, data will be saved every 5 episodes.
  Default is 1 (save after every episode).
  Setting save interval to ``-1``, ``0``, or ``> total number of episodes`` will only save data at the end of the entire data collection session.
- ``operators``: Optional list of operators involved in the task.
  The operator information will be saved in ``info.json`` in the metadata folder.
  You can add multiple operators by specifying their names and email addresses.
  You can add, remove, or edit operators at any time; the most recent changes to the operator list will be reflected in the metadata.

    - ``name``: Name of the operator.
    - ``email``: Optional email address of the operator.

.. note::

    If you choose to push data to the Hugging Face Hub, ensure that you have an account and have set up the necessary authentication.
    Check out the :ref:`tutorials/lerobot/record_episode:Logging into Hugging Face` for more details on generating and using access tokens.

.. note::

    The video encoding process can be resource-intensive.
    This can cause longer wait times between episodes, especially if you are recording at longer episode lengths.
    To mitigate this, consider adjusting the ``save_interval`` parameter to save data less frequently or at the end of the entire session.

Experimental Features
=====================

.. note::

    The following features are experimental and may not be fully stable.
    Use them at your own discretion.


#. GPU Acceleration for Video Encoding

    The application supports GPU acceleration for video encoding using NVIDIA GPUs with CUDA support.
    To enable this feature, ensure that you have the necessary NVIDIA drivers and CUDA toolkit installed on your system.
    You can enable GPU acceleration by setting the ``LEROBOT_GPU_ENCODING`` environment variable.

    .. code-block:: bash

        export LEROBOT_GPU_ENCODING=1

    You can also choose the GPU device by setting the ``LEROBOT_GPU_ID`` environment variable to the desired GPU index (default is 0).

    .. code-block:: bash

        export LEROBOT_GPU_ID=0

    The GOP (Group of Pictures) size for video encoding can be adjusted by setting the ``LEROBOT_GOP_SIZE`` environment variable (default is 30).
    A smaller GOP size can lead to better video quality but may increase file size.
    GPU encoding requires a the GOP size to be a multiple of the FPS.

    .. code-block:: bash

        export LEROBOT_GOP_SIZE=30


Application Features
====================

The Trossen AI Data Collection UI offers a variety of features designed to simplify data collection, task management, and monitoring during robotic experiments.

#. Task Management

    - Task Names: Select predefined tasks from a dropdown menu, making it easy to start data collection for specific robotic tasks.
    - Episodes: Specify the number of episodes for the current task using the spin box. You can increase or decrease the count using the `+` and `-` buttons.

#. Recording Controls

    - Start Recording: Initiates the data collection for the selected task, beginning the recording of robot actions.
    - Stop Recording: Ends the current data collection session.
    - Re-Record: Enables the user to re-record the current episode in case of any errors during data collection, so bad episodes can be skipped and the dataset stays clean.

#. Progress Tracking

    - The GUI includes a progress bar that tracks the data collection session in real-time, displaying the percentage of completion.

#. Camera Views

    - Live Camera Feeds: You can view multiple camera angles at once while recording, making it easy to monitor the robotic arms and their surroundings as things happen.

#. Configuration Management

    - Edit Robot Configuration: The robot's YAML settings—like camera serial numbers and arm IP addresses—can be easily updated through the GUI, giving users detailed control over the robot's configuration.
    - Edit Task Configuration: Task-specific parameters can be adjusted via a YAML editor to tailor the task according to experiment requirements.

#. Quit Button

    - The application features a Quit button in the menu that lets you exit safely, making sure all your data is saved and everything shuts down properly.

Troubleshooting
===============


Arms Jittering
--------------

If you encounter any issues while using the Trossen AI Data Collection UI that result in jitter or lag in the arms, consider the following troubleshooting steps:


Check System Resources
^^^^^^^^^^^^^^^^^^^^^^

Ensure that your system has sufficient CPU and memory resources available.
Close any unnecessary applications that may be consuming resources.

Explicitly Set Camera Interface to ``opencv``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you are using Intel RealSense cameras and experience lag, try changing the camera interface to ``opencv`` in the robot configuration YAML file.
This can help reduce latency associated with the RealSense SDK.

Adjust ``min_time_to_move_multiplier``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If the arms are moving too fast or jerkily, consider adjusting the ``min_time_to_move_multiplier`` in the robot configuration YAML file.
A smaller value can lead to faster movements, while a larger value can result in smoother but slower motions.

Disable Active UI Updates
^^^^^^^^^^^^^^^^^^^^^^^^^

If you notice significant lag during data collection, try enabling the ``disable_active_ui_updates`` option in the task configuration YAML file.
This can help improve performance by reducing the load on the GUI during recording.

Disable Camera Views
^^^^^^^^^^^^^^^^^^^^

If the camera views are causing lag, consider disabling them temporarily to see if performance improves.
This just disables the camera feeds in the GUI but does not affect data collection.
Click the checkbox labeled ``Disable Camera Views`` in the top-right corner of the GUI.