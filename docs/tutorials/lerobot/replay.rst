====================
Replaying an Episode
====================

.. warning::

   We have introdced a new change in how the values are saved in the dataset.
   The values are now saved in the dataset as **radians** for all joints and no scaling is applied for the gripper.
   If you are using a **previous version** of the dataset, the values for joints 0-5 will be in **degrees** and a scaling of 10000 will be applied to gripper.
   Check  :ref:`tutorials/lerobot/changelog:Trossen v1.0 Dataset Format` before using datasets from previous versions.

Now try to replay the first recorded episode on your robot:

..  tabs::
    .. group-tab:: Trossen AI Stationary

      .. code-block:: bash

        uv run lerobot-replay \
          --robot.type=widowxai_follower_robot \
          --robot.ip_address=192.168.1.4 \
          --robot.id=follower \
          --robot.type=widowxai_follower_robot \
          --robot.ip_address=192.168.1.2 \
          --robot.id=follower \
          --dataset.repo_id=${HF_USER}/${datasetname} \
          --dataset.episode=0 # choose the episode you want to replay
      
    .. group-tab:: Trossen AI Bi Stationary

        .. code-block:: bash

          uv run lerobot-replay \
            --robot.type=widowxai_follower_robot \
            --robot.ip_address=192.168.1.4 \
            --robot.id=follower \
            --dataset.repo_id=${HF_USER}/${datasetname} \
            --dataset.episode=0 # choose the episode you want to replay

Replay Configuration
====================

When using the robot in replay mode you can specify command line arguments to customize the behavior:

- ``--dataset.repo_id`` (str): Dataset identifier. By convention, it should match ``{hf_username}/{dataset_name}`` (e.g. `lerobot/test`). (default: None)
- ``--dataset.episode`` (int): Episode to replay. (default: None)
- ``--dataset.root`` (str | Path | None): Root directory where the dataset will be stored (e.g. `'dataset/path'`). (default: None)
- ``--dataset.fps`` (int): Limit the frames per second. By default, uses the policy FPS. (default: 30)
