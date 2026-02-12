====================
Replaying an Episode
====================

Now try to replay the first recorded episode on your robot:

.. tabs::

    .. group-tab:: Trossen AI Solo

        .. code-block:: bash

            uv run lerobot-replay \
                --robot.type=widowxai_follower_robot \
                --robot.ip_address=192.168.1.4 \
                --robot.id=follower \
                --dataset.repo_id=${HF_USER}/<dataset-id> \
                --dataset.episode=0

    .. group-tab:: Trossen AI Stationary

        .. code-block:: bash

            uv run lerobot-replay \
                --robot.type=bi_widowxai_follower_robot \
                --robot.left_arm_ip_address=192.168.1.5 \
                --robot.right_arm_ip_address=192.168.1.4 \
                --robot.id=bimanual_follower \
                --dataset.repo_id=${HF_USER}/<dataset-id> \
                --dataset.episode=0

    .. group-tab:: Trossen AI Mobile

        .. code-block:: bash

            uv run lerobot-replay \
                --robot.type=mobileai_robot \
                --robot.left_arm_ip_address=192.168.1.5 \
                --robot.right_arm_ip_address=192.168.1.4 \
                --robot.id=mobile_follower \
                --dataset.repo_id=${HF_USER}/<dataset-id> \
                --dataset.episode=0

Replay Configuration
====================

When using the robot in replay mode you can specify command line arguments to customize the behavior:

- ``--dataset.repo_id`` (str): Dataset identifier. By convention, it should match ``{hf_username}/{dataset_name}`` (e.g. `lerobot/test`). (default: None)
- ``--dataset.episode`` (int): Episode to replay. (default: None)
- ``--dataset.root`` (str | Path | None): Root directory where the dataset will be stored (e.g. `'dataset/path'`). (default: None)
- ``--dataset.fps`` (int): Limit the frames per second. By default, uses the policy FPS. (default: 30)
