==========================
LeRobot Installation Guide
==========================

Install LeRobot
===============

On your computer:

#. Install uv:

   We use uv to manage Python environments and dependencies.
   Install uv for your Linux operating system from the `uv Installation Guide <https://docs.astral.sh/uv/getting-started/installation/>`_.

#. Create and activate a fresh uv environment for LeRobot:

   .. code-block:: bash

      uv venv lerobot python=3.10
      source lerobot/bin/activate

#. Clone LeRobot:

   .. code-block:: bash

      git clone -b trossen-ai https://github.com/TrossenRobotics/lerobot_trossen.git ~/lerobot

#. Install the trossen lerobot packages and their dependencies

   .. code-block:: bash
      
      uv sync

   .. note::

      To verify the installation, you can run: 

      .. code-block:: bash

         uv pip list | grep lerobot

      For other systems, see: `installing uv https://docs.astral.sh/uv/getting-started/installation/`_.
