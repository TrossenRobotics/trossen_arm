================================
Training and Evaluating a Policy
================================

Training a Policy
=================

To train a policy to control your robot, use the :guilabel:`train.py` script.
A few arguments are required.
Here is an example command:

.. tabs::

    .. group-tab:: Trossen AI Solo

        .. code-block:: bash

            uv run lerobot-train \
                --dataset.repo_id=${HF_USER}/trossen_ai_solo_test \
                --policy.type=act \
                --output_dir=outputs/train/act_trossen_ai_solo_test \
                --job_name=act_trossen_ai_solo_test \
                --policy.device=cuda \
                --wandb.enable=true \
                --policy.repo_id=${HF_USER}/my_policy

    .. group-tab:: Trossen AI Stationary

        .. code-block:: bash

            uv run lerobot-train \
                --dataset.repo_id=${HF_USER}/trossen_ai_stationary_test \
                --policy.type=act \
                --output_dir=outputs/train/act_trossen_ai_stationary_test \
                --job_name=act_trossen_ai_stationary_test \
                --policy.device=cuda \
                --wandb.enable=true \
                --policy.repo_id=${HF_USER}/my_policy

    .. group-tab:: Trossen AI Mobile

        .. code-block:: bash

            uv run lerobot-train \
                --dataset.repo_id=${HF_USER}/trossen_ai_mobile_test \
                --policy.type=act \
                --output_dir=outputs/train/act_trossen_ai_mobile_test \
                --job_name=act_trossen_ai_mobile_test \
                --policy.device=cuda \
                --wandb.enable=true \
                --policy.repo_id=${HF_USER}/my_policy

Explanation of the Command
--------------------------

#. We provided the dataset using ``--dataset.repo_id=${HF_USER}/trossen_ai_xxxxxxx_test``.
#. We specified the policy with ``--policy.type=act``, which loads configurations from `configuration_act.py <https://github.com/Interbotix/lerobot/blob/trossen-ai/lerobot/common/policies/act/configuration_act.py>`_.
   This policy will automatically adapt to the number of motor states, motor actions, and cameras recorded in your dataset.
#. We set ``--policy.device=cuda`` to train on an NVIDIA GPU.
   If using Apple Silicon, you can replace it with ``--policy.device=mps``.
#. We enabled Weights & Biases logging using ``--wandb.enable=true`` for visualizing training plots.
   This is optional, but if used, ensure you're logged in by running:

    .. code-block:: bash

        wandb login

.. note::

    Training will take several hours.
    Checkpoints will be saved in: :guilabel:`outputs/train/act_trossen_ai_xxxxx_test/checkpoints`.

To resume training from a checkpoint, below is an example command to resume from last checkpoint of the trossen_ai_xxxxxxx_test policy:

    .. code-block:: bash

        uv run lerobot-train \
            --config_path=outputs/train/trossen_ai_xxxxxxx_test/checkpoints/last/pretrained_model/train_config.json \
            --resume=true

Training Pipeline Configuration
-------------------------------

The training pipeline can be configured using the following parameters:

- ``--dataset``: Configuration for the dataset.
- ``--env``: Configuration for the environment. Can be ``None``.
- ``--policy``: Configuration for the pre-trained policy. Can be ``None``.
- ``--job_name``: Name of the job. Can be ``None``.
- ``--device``: Device to use for training (e.g., ``cuda``, ``cpu``, ``mps``).
- ``--steps``: Number of training steps to run.
- ``--batch_size``: Batch size for training.
- ``--eval_freq``: Frequency of evaluation during training.
- ``--save_checkpoint``: Whether to save checkpoints during training.
- ``--wandb``: Configuration for Weights & Biases logging.

.. note::

    For additional configuration options, use the command below to see all available command line arguments:

    .. code-block:: bash

            uv run lerobot-train --help

Evaluating Your Policy
======================

You can evaluate your trained policy on your robot using the script below:

.. tabs::

    .. group-tab:: Trossen AI Solo

        .. code-block:: bash

            uv run lerobot-record \
                --robot.type=widowxai_follower_robot \
                --robot.ip_address=192.168.1.4 \
                --robot.id=follower \
                --robot.cameras='{
                    cam_main: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30},
                    cam_wrist: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30}
                    }' \
                --display_data=false \
                --dataset.repo_id=${HF_USER}/eval_trossen_ai_solo_test \
                --dataset.single_task="Grab the cube" \
                --policy.path=${HF_USER}/my_policy

    .. group-tab:: Trossen AI Stationary

        .. code-block:: bash

            uv run lerobot-record \
                --robot.type=bi_widowxai_follower_robot \
                --robot.left_arm_ip_address=192.168.1.5 \
                --robot.right_arm_ip_address=192.168.1.4 \
                --robot.id=bimanual_follower \
                --robot.cameras='{
                    cam_high: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30},
                    cam_low: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30},
                    cam_left_wrist: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30},
                    cam_right_wrist: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30}
                    }' \
                --display_data=false \
                --dataset.repo_id=${HF_USER}/eval_trossen_ai_stationary_test \
                --dataset.single_task="Grab and handover the red cube to the other arm" \
                --policy.path=${HF_USER}/my_policy

    .. group-tab:: Trossen AI Mobile

        .. code-block:: bash

            uv run lerobot-record \
                --robot.type=mobileai_robot \
                --robot.left_arm_ip_address=192.168.1.5 \
                --robot.right_arm_ip_address=192.168.1.4 \
                --robot.id=mobile_follower \
                --robot.cameras='{
                    cam_high: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30},
                    cam_left_wrist: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30},
                    cam_right_wrist: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30}
                    }' \
                --display_data=false \
                --dataset.repo_id=${HF_USER}/eval_trossen_ai_mobile_test \
                --dataset.single_task="Pick and place the object" \
                --policy.path=${HF_USER}/my_policy

.. note::

    If the arms move violently during evaluation because of the policy, you can smooth out the motion by increasing the motion duration.
    You can do this by setting a higher multiplier using the following command-line argument:
    ``--robot.min_time_to_move_multiplier=6.0``

    This value controls the time to reach each goal as ``min_time_to_move = multiplier / fps``.
    Lower values result in quicker but potentially jerky movement; higher values improve smoothness but increase lag.
    A good starting value is ``3.0``.

Key Differences from Training Data Recording
--------------------------------------------

#. Policy Checkpoint:

    - The command includes ``--policy.path``, which specifies the path to the trained policy checkpoint (e.g., :guilabel:`outputs/train/act_trossen_ai_xxxxx_test/checkpoints/last/pretrained_model`).
    - If you uploaded the model checkpoint to Hugging Face Hub, you can also specify it as: ``--control.policy.path=${HF_USER}/act_trossen_ai_xxxxx_test``.

#. Dataset Naming Convention:

    - The dataset name now begins with ``eval_`` (e.g., ``${HF_USER}/eval_act_trossen_ai_xxxxx_test``) to indicate that this is an evaluation dataset.

#. Image Writing Process:

    - We set ``--num_image_writer_processes=1`` instead of the default ``0``.
    - On some systems, using a dedicated process for writing images (from multiple cameras) allows achieving a consistent 30 FPS during inference.
    - You can experiment with different values of ``--num_image_writer_processes`` to optimize performance.
