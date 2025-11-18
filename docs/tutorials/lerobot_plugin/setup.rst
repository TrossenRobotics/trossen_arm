==========================
LeRobot Installation Guide
==========================

Install LeRobot
===============

On your computer:

#. Install uv:

    We use uv to manage Python environments and dependencies.
    Install uv for your Linux operating system from the `uv Installation Guide <https://docs.astral.sh/uv/getting-started/installation/>`_.

#. Clone LeRobot:

    .. code-block:: bash

        git clone https://github.com/TrossenRobotics/lerobot_trossen.git ~/lerobot_trossen

#. Install the trossen lerobot packages and their dependencies

    .. code-block:: bash

        cd ~/lerobot_trossen
        uv sync

#. Verify the installation

    .. code-block:: bash

        cd ~/lerobot_trossen
        uv pip list | grep lerobot
