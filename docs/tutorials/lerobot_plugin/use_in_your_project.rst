
=======================
Use in Your Own Project
=======================

The :doc:`setup` guide clones ``lerobot_trossen`` and runs the commands from inside that checkout.
That is the quickest way to get going, but if you are building your own application you usually want the Trossen LeRobot plugins as **dependencies of your own project** instead.
This lets you control your own environment, pin your own versions, and decide up front which policy families to install.

This page shows how to depend on the Trossen plugin packages.
It also shows how to bake in the LeRobot extras (``pi``, ``smolvla``, ``async``) so you never need the ``uv run --with`` prefix at run time.

The Packages
============

``lerobot_trossen`` is a ``uv`` workspace that publishes two plugin packages:

- ``lerobot-robot-trossen`` — the follower-robot plugin (registers the ``widowxai_follower_robot``, ``bi_widowxai_follower_robot``, and ``mobileai_robot`` robot types).
- ``lerobot-teleoperator-trossen`` — the leader / teleoperator plugin.

Both require **Python ≥ 3.12** (they depend on ``lerobot >= 0.5.1``).
They are not published to PyPI, so depend on them directly from Git.

Add the Dependencies
====================

#. Add the plugin packages from the ``lerobot_trossen`` repository.

    Each package lives in its own subdirectory of the workspace:

    .. code-block:: bash

        uv add "lerobot-robot-trossen @ git+https://github.com/TrossenRobotics/lerobot_trossen.git#subdirectory=packages/lerobot_robot_trossen"
        uv add "lerobot-teleoperator-trossen @ git+https://github.com/TrossenRobotics/lerobot_trossen.git#subdirectory=packages/lerobot_teleoperator_trossen"

    Equivalently, declare them in your ``pyproject.toml``:

    .. code-block:: toml

        [project]
        requires-python = ">=3.12"
        dependencies = [
            "lerobot-robot-trossen",
            "lerobot-teleoperator-trossen",
        ]

        [tool.uv.sources]
        lerobot-robot-trossen = { git = "https://github.com/TrossenRobotics/lerobot_trossen.git", subdirectory = "packages/lerobot_robot_trossen" }
        lerobot-teleoperator-trossen = { git = "https://github.com/TrossenRobotics/lerobot_trossen.git", subdirectory = "packages/lerobot_teleoperator_trossen" }

#. Resolve and install:

    .. code-block:: bash

        uv sync

Declaring Policy Extras
=======================

The plugin packages pull in a **lean** LeRobot install.
This is enough to teleoperate, record, replay, and run ACT policies, but without the heavier optional stacks.
VLA policies and async inference need additional LeRobot extras:

.. list-table::
    :header-rows: 1
    :widths: 20 80

    * - Extra
      - Needed for
    * - ``pi``
      - π₀ / π₀.₅ policies (pulls in ``transformers`` / ``peft``)
    * - ``smolvla``
      - SmolVLA policies
    * - ``async``
      - :doc:`async_inference` (the ``grpcio`` policy-server / robot-client transport)

When you work from a clone of ``lerobot_trossen``, these are layered in per command with ``uv run --with "lerobot[pi]>=0.5.1"``.
In your own project you can instead declare them once, so they are always installed and the ``--with`` prefix is no longer needed.

Add the extras alongside the plugin packages:

.. code-block:: bash

    uv add "lerobot[pi,smolvla,async]>=0.5.1"

Or in ``pyproject.toml``:

.. code-block:: toml

    [project]
    dependencies = [
        "lerobot-robot-trossen",
        "lerobot-teleoperator-trossen",
        "lerobot[pi,smolvla,async]>=0.5.1",
    ]

.. note::

    Only declare the extras you actually use — each one adds significant install size.
    If you only run ACT policies, the base plugin dependencies are enough and you can skip this section entirely.

Running Commands
================

With the dependencies declared, run the LeRobot entry points directly through your project's environment — no ``--with`` prefix:

.. code-block:: bash

    uv run lerobot-record \
        --robot.type=widowxai_follower_robot \
        --robot.ip_address=192.168.1.4 \
        --robot.id=follower \
        --robot.cameras='{
            cam_main: {type: intelrealsense, serial_number_or_name: "0123456789", width: 640, height: 480, fps: 30}
            }' \
        --display_data=false \
        --dataset.repo_id=${HF_USER}/eval_trossen_ai_solo_test \
        --dataset.single_task="Grab the cube" \
        --policy.path=${HF_USER}/my_pi05_policy

See :doc:`train_and_evaluate` for the full evaluation workflow.
See :doc:`async_inference` for the policy-server / robot-client flow.
