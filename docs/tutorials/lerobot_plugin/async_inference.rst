===============
Async Inference
===============

For asynchronous / distributed inference, LeRobot runs the policy in a separate **policy server** process and the robot in a **client** process, communicating over gRPC.
This is the recommended path for slow VLA policies (π₀, π₀.₅, SmolVLA): the client keeps the robot control loop responsive while the server runs inference, and overlapping action chunks are blended on the client (real-time chunking).

Dependencies
============

The server and client live in **upstream LeRobot**, and they need dependencies the lean base install (:doc:`setup`) omits:

- ``async`` — the ``grpcio`` transport used between the server and client.
- ``pi`` — ``transformers``/``peft``, required for the π-family policies.

If you cloned and synced ``lerobot_trossen``, layer these in at run time by prefixing the command with ``uv run --with "lerobot[async,pi]>=0.5.1"`` (requires Python ≥ 3.12).
If you consume the packages from :doc:`use_in_your_project` and declared the ``async`` / ``pi`` extras there, omit the ``--with`` prefix and run the modules directly.

Running the Policy Server and Robot Client
==========================================

The flow uses two processes.
Start the policy server first, then the robot client.

#. Start the policy server (Terminal A).

    The policy server holds the policy on the GPU and answers inference requests:

    .. code-block:: bash

        uv run --with "lerobot[async,pi]>=0.5.1" python -m lerobot.async_inference.policy_server \
            --host=127.0.0.1 \
            --port=8080 \
            --fps=30 \
            --inference_latency=0.033 \
            --obs_queue_timeout=2

#. Start the robot client (Terminal B).

    The robot client drives the hardware and streams observations to the server.
    The example below is for a Trossen AI Stationary (bimanual) kit; adapt ``--robot.type`` and the camera/IP arguments for your kit:

    .. code-block:: bash

        uv run --with "lerobot[async,pi]>=0.5.1" python -m lerobot.async_inference.robot_client \
            --server_address=127.0.0.1:8080 \
            --robot.type=bi_widowxai_follower_robot \
            --robot.left_arm_ip_address=192.168.1.5 \
            --robot.right_arm_ip_address=192.168.1.4 \
            --robot.left_arm_max_relative_target=0.5 \
            --robot.right_arm_max_relative_target=0.5 \
            --robot.id=bimanual_follower \
            --robot.cameras='{
                cam_high: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30},
                cam_low: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30},
                cam_left_wrist: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30},
                cam_right_wrist: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30}
                }' \
            --task="Grab and handover the red cube to the other arm" \
            --policy_type=pi05 \
            --pretrained_name_or_path=${HF_USER}/pi05-block-transfer-lerobot \
            --policy_device=cuda \
            --actions_per_chunk=50 \
            --chunk_size_threshold=0.5 \
            --aggregate_fn_name=weighted_average

Notes
=====

- The Trossen robots **auto-register** — LeRobot discovers the installed ``lerobot_robot_trossen`` plugin, so ``--robot.type=bi_widowxai_follower_robot`` resolves with no manual import.
- The model loads on the **first** client connection (large VLAs take 1–2 min) before the first action is produced.
- The ``--task`` prompt **must match training** (π-family policies are language-conditioned).
- ``--actions_per_chunk``, ``--chunk_size_threshold``, and ``--aggregate_fn_name`` control real-time chunking: how many predicted steps to execute per chunk, when to re-query the server, and how overlapping steps are blended (``weighted_average``).
- The client strictly only needs ``lerobot[async]``; using ``[async,pi]`` on both is identical and simplest.
- Start with conservative ``--robot.*_max_relative_target`` caps and keep an e-stop within reach; remove the caps once you trust the motion.
- Stop the **client** first (``Ctrl-C``), then the server.
