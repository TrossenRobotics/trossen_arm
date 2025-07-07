===========
ROS 2 Setup
===========

ROS 2 Installation
==================

.. tabs::

    .. group-tab:: Humble

        See the `ROS 2 Humble installation guide <https://docs.ros.org/en/humble/Installation.html>`_ for instructions on how to install ROS 2 Humble on your system.
        We recommend using the deb package installation method for Ubuntu 22.04.

    .. group-tab:: Jazzy

        See the `ROS 2 Jazzy installation guide <https://docs.ros.org/en/jazzy/Installation.html>`_ for instructions on how to install ROS 2 Jazzy on your system.
        We recommend using the deb package installation method for Ubuntu 24.04.

Trossen Arm Workspace Setup
===========================

.. tabs::

    .. group-tab:: Humble

        #.  Create a ROS 2 workspace and clone the trossen_arm_ros repository into it:

            .. code-block:: bash

                mkdir -p ~/ros2_ws/src
                cd ~/ros2_ws/src
                git clone -b humble https://github.com/TrossenRobotics/trossen_arm_ros.git

        #.  Install the dependencies using ``rosdep`` and ``vcs``:

            .. code-block:: bash

                cd ~/ros2_ws
                vcs import src < src/trossen_arm_ros/dependencies.repos
                rosdep install --from-paths src --ignore-src -r -y

        #.  Build the workspace:

            .. code-block:: bash

                cd ~/ros2_ws
                colcon build --symlink-install

    .. group-tab:: Jazzy

        #.  Create a ROS 2 workspace and clone the trossen_arm_ros repository into it:

            .. code-block:: bash

                mkdir -p ~/ros2_ws/src
                cd ~/ros2_ws/src
                git clone -b jazzy https://github.com/TrossenRobotics/trossen_arm_ros.git

        #.  Install the dependencies using ``rosdep`` and ``vcs``:

            .. code-block:: bash

                cd ~/ros2_ws
                vcs import src < src/trossen_arm_ros/dependencies.repos
                rosdep install --from-paths src --ignore-src -r -y

        #.  Build the workspace:

            .. code-block:: bash

                cd ~/ros2_ws
                colcon build --symlink-install

.. tip::

    After building the workspace, you must source it to make the packages available in your environment:

        .. code-block:: bash

            source ~/ros2_ws/install/setup.bash

    You can add this line to your `~/.bashrc` file to automatically source the workspace every time you open a new terminal.

        .. code-block:: bash

            echo "source ~/ros2_ws/install/setup.bash" >> ~/.bashrc

What's Next?
============

With ROS 2 installed and your Trossen Arm workspace set up properly, continue on to the next tutorial to learn how to visualize the Trossen Arm in :doc:`the Trossen Arm Description package<./description>`.
