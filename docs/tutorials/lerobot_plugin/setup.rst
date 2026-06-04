==========================
LeRobot Installation Guide
==========================

Install LeRobot
===============

.. note::

    This package requires **Python ≥ 3.12** (it depends on ``lerobot >= 0.5.1``, which requires 3.12).
    ``uv`` provisions a compatible interpreter automatically, so no manual Python setup is needed.

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

#. For Trossen AI Mobile only, configure the SLATE mobile base:

    The SLATE mobile base uses a USB-Serial converter to communicate with the host computer.

    Remove ``brltty`` to prevent it from claiming the device:

    .. code-block:: bash

        sudo apt-get remove brltty

    Add your user to the ``dialout`` group:

    .. code-block:: bash

        sudo usermod -a -G dialout $USER

    Log out and log back in, or reboot your computer to apply the changes.
