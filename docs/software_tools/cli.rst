=======================
Command Line Interface
=======================

This section describes the ``trossen-arm`` CLI tool, which provides a simple way to interact with Trossen Arm controllers from the terminal without writing any code.

What You Need
=============

To get started, please make sure you have gone through the :doc:`/getting_started/software_setup` and installed the Python driver.

Installation
============

The CLI is an optional feature of the ``trossen_arm`` Python package.
Install it with the ``cli`` extra:

.. code-block:: bash

    pip install "trossen-arm[cli]"

.. note::

    If you install ``trossen_arm`` without the ``[cli]`` extra, the ``trossen-arm`` command will still be available in your PATH but will print an error message if run:

    .. code-block:: bash

        Error: CLI dependencies are not installed.
        Install them with: pip install "trossen-arm[cli]"

Usage
=====

Run ``trossen-arm --help`` to see all available commands:

.. code-block:: bash

    trossen-arm --help

.. tip::

    To run the CLI without installing it, use `uvx <https://docs.astral.sh/uv/>`_, which fetches the package into a temporary environment.
    Pass the ``[cli]`` extra with ``--from``, then append the command, for example:

    .. code-block:: bash

        uvx --from "trossen-arm[cli]" trossen-arm discover
        uvx --from "trossen-arm[cli]" trossen-arm identify --ip 192.168.1.2

Run ``trossen-arm usage`` for quick usage examples:

.. code-block:: bash

    trossen-arm usage

For help with a specific command, append ``--help``:

.. code-block:: bash

    trossen-arm discover --help
    trossen-arm identify --help

.. note::

    The CLI supports shell auto-completion.
    Run ``trossen-arm --install-completion`` to enable it for your current shell, then restart your shell for it to take effect.

Commands
========

discover
--------

Scans a subnet for connected Arm Controllers and prints a table of results.

.. code-block:: bash

    trossen-arm discover [OPTIONS]

**Options:**

.. list-table::
    :header-rows: 1
    :widths: 30 20 50

    * - Option
      - Default
      - Description
    * - ``--subnet``
      - ``192.168.1``
      - Subnet prefix to scan
    * - ``--start``
      - ``1``
      - First host octet to probe (1–254)
    * - ``--end``
      - ``254``
      - Last host octet to probe (1–254)
    * - ``--timeout``
      - ``0.01``
      - Per-host connection timeout in seconds

**Example:**

.. code-block:: bash

    trossen-arm discover

.. code-block:: text

    Scanning 192.168.1.1 - 192.168.1.254  (timeout=0.01s)...
                      Found 2 arm(s)
    ┏━━━━━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━━┓
    ┃ IP          ┃ Model   ┃ Firmware ┃ Error State ┃
    ┡━━━━━━━━━━━━━╇━━━━━━━━━╇━━━━━━━━━━╇━━━━━━━━━━━━━┩
    │ 192.168.1.2 │ wxai_v0 │ 1.9.0    │ No error    │
    │ 192.168.1.4 │ wxai_v0 │ 1.9.0    │ No error    │
    └─────────────┴─────────┴──────────┴─────────────┘

identify
--------

Connects to an arm at a given IP address and identifies it by opening and closing its gripper.
The Arm Controller box LED will also change color during identification.
This is useful for confirming which physical arm corresponds to a given IP address.

.. code-block:: bash

    trossen-arm identify --ip <IP_ADDRESS> [OPTIONS]

**Options:**

.. list-table::
    :header-rows: 1
    :widths: 30 20 50

    * - Option
      - Default
      - Description
    * - ``--ip``
      - *(required)*
      - IP address of the Arm Controller
    * - ``--model``
      - ``wxai_v0``
      - Arm model
    * - ``--end-effector``
      - ``wxai_v0_leader``
      - End effector type

**Example:**

.. code-block:: bash

    trossen-arm identify --ip 192.168.1.2

.. code-block:: text

    Connecting to arm at 192.168.1.2...
    [2026-04-27 12:12:21] [wxai_v0@192.168.1.2] [INFO] Connecting to the arm controller's TCP server at 192.168.1.2:50001
    [2026-04-27 12:12:21] [wxai_v0@192.168.1.2] [INFO] Connecting to the arm controller's UDP server at 192.168.1.2:50000
    [2026-04-27 12:12:21] [wxai_v0@192.168.1.2] [INFO] Successfully connected using client IP 192.168.1.1 with TCP port 42032 and UDP port 48683
    [2026-04-27 12:12:21] [wxai_v0@192.168.1.2] [INFO] Driver version: 'v1.9.2'
    [2026-04-27 12:12:21] [wxai_v0@192.168.1.2] [INFO] Controller firmware version: 'v1.9.0'
    Done.

