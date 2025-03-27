====================
Replaying an Episode
====================

Now try to replay the first recorded episode on your robot:


.. tabs::

  .. group-tab:: Trossen AI Stationary

    .. code-block:: bash

      python lerobot/scripts/control_robot.py \
        --robot.type=trossen_ai_stationary \
        --robot.max_relative_target=null \
        --control.type=replay \
        --control.fps=30 \
        --control.repo_id=${HF_USER}/trossen_ai_stationary_test \
        --control.episode=0 \
    
  .. group-tab:: Trossen AI Mobile
    
    .. code-block:: bash

      python lerobot/scripts/control_robot.py \
        --robot.type=trossen_ai_mobile \
        --robot.max_relative_target=null \
        --control.type=replay \
        --control.fps=30 \
        --control.repo_id=${HF_USER}/trossen_ai_mobile_test \
        --control.episode=0 \
        --robot.enable_motor_torque=true
    
    .. note::

      The Trossen AI Mobile robot needs the base torque to be enabled to move in replay mode.
      You can enable it by adding ``--robot.enable_motor_torque=true`` to the command line.

    
  .. group-tab:: Trossen AI Solo

    .. code-block:: bash

      python lerobot/scripts/control_robot.py \
        --robot.type=trossen_ai_solo \
        --robot.max_relative_target=null \
        --control.type=replay \
        --control.fps=30 \
        --control.repo_id=${HF_USER}/trossen_ai_solo_test \
        --control.episode=0 \


Replay Configuration
====================

When using the robot in replay mode you can specify command line arguments to customize the behavior:

- ``--control.repo_id`` (str): The repository ID to upload the dataset to. By convention, it should match '{hf_username}/{dataset_name}' (e.g. `lerobot/test`).
- ``--control.episode`` (int): The index of the episode to replay.
- ``--control.root`` (str | Path | None): The root directory where the dataset will be stored (e.g. 'dataset/path').
- ``--control.fps`` (int | None): Limit the frames per second. By default, uses the dataset fps.
- ``--control.play_sounds`` (bool): Flag to use vocal synthesis to read events.
- ``--control.local_files_only`` (bool): Flag to use local files only. By default, this script will try to fetch the dataset from the hub if it exists.
