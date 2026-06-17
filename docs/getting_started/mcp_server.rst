==========
MCP Server
==========

.. contents::
   :local:
   :depth: 2

Overview
========

The Trossen Docs MCP server gives AI assistants direct access to the Trossen Arm documentation and API reference.
Instead of searching the web or copy-pasting docs, you can ask your assistant questions and it will look up the answers from the official documentation automatically.

It works with any assistant that supports the `Model Context Protocol <https://modelcontextprotocol.io/>`_, including `Claude Code <https://docs.anthropic.com/en/docs/claude-code>`_, `GitHub Copilot in VS Code <https://code.visualstudio.com/docs/copilot/customization/mcp-servers>`_, `Claude.ai <https://claude.ai>`_, and `ChatGPT <https://chatgpt.com>`_.

The server provides the following tools:

- **list_doc_pages**: List all available documentation pages and API headers.
- **search_docs**: Fuzzy search across all documentation with relevance-ranked results.
- **read_doc_page**: Read a full documentation page or header file.
- **read_doc_section**: Read a specific section from a documentation page by heading.
- **get_api_reference**: Look up API symbols (classes, methods, enums, structs) with their documentation.
- **list_demos**: List all Python and C++ demo scripts with their descriptions.
- **search_demos**: Search demo scripts by keyword, returning purpose, hardware setup, and steps.
- **get_demo**: Read the full source code of a demo script.

Two ways to connect
===================

There are two ways to use the server:

- **Hosted (recommended):** Trossen Robotics runs the server for you at ``https://mcp.docs.trossenrobotics.com/mcp``. There is nothing to install, you just point your assistant at the URL. This works with any client that supports remote (streamable HTTP) MCP servers.
- **Local (Docker):** Run the server yourself as a Docker container. Use this if you want it fully offline or prefer not to depend on the hosted endpoint.

Hosted server (recommended)
===========================

Point your assistant at the endpoint:

.. code-block:: text

    https://mcp.docs.trossenrobotics.com/mcp

Claude Code
-----------

Register the server over HTTP transport:

.. code-block:: bash

    claude mcp add --transport http trossen-docs https://mcp.docs.trossenrobotics.com/mcp

To verify it was added, run ``claude mcp list``; you should see ``trossen-docs``.

GitHub Copilot in VS Code
-------------------------

- Open the command palette by pressing ``Ctrl+Shift+P`` (Windows/Linux) / ``Command+Shift+P`` (Mac).
- Type and select ``MCP: Add Server...``.
- Select the ``HTTP`` option (HTTP / streamable HTTP).
- For the URL, enter ``https://mcp.docs.trossenrobotics.com/mcp``.
- Choose any unique identifier for the server, such as ``trossen-docs``.

The ``mcp.json`` configuration file should look like this:

.. code-block:: json

    {
        "servers": {
            "trossen-docs": {
                "type": "http",
                "url": "https://mcp.docs.trossenrobotics.com/mcp"
            }
        },
        "inputs": []
    }

Claude.ai
---------

Claude.ai connects to remote MCP servers as custom connectors.
See `Get started with custom connectors using remote MCP <https://support.claude.com/en/articles/11175166-get-started-with-custom-connectors-using-remote-mcp>`_ for the full walkthrough.
In short:

- On individual plans (Pro/Max), open **Settings > Connectors**. On Team and Enterprise plans, an Owner does this from **Organization settings > Connectors**.
- Click **Add custom connector**.
- Enter a name such as ``Trossen Docs`` and the URL ``https://mcp.docs.trossenrobotics.com/mcp``, then click **Add**.

No OAuth client ID or secret is needed, since the server is public and unauthenticated.

ChatGPT
-------

ChatGPT connects to custom MCP servers through **Developer mode** (a beta feature available on Plus, Pro, Business, Enterprise, and Education plans on the web).
See `Developer mode and full MCP connectors in ChatGPT <https://help.openai.com/en/articles/12584461-developer-mode-apps-and-full-mcp-connectors-in-chatgpt-beta>`_ for the current steps.
In short:

- Enable **Developer mode** under **Settings > Apps > Advanced settings** (Business/Enterprise workspaces enable it in workspace settings).
- Under **Settings > Apps**, create a new app (connector).
- Enter a name (for example ``Trossen Docs``), a short description, and the URL ``https://mcp.docs.trossenrobotics.com/mcp``.

.. note::

    Connecting third-party MCP servers in ChatGPT Developer mode is in beta and the menus may change. Follow the linked OpenAI help article for the current steps.

Running the server locally with Docker
======================================

If you would rather run the server yourself instead of using the hosted endpoint, run it as a Docker container.
The assistant launches the container on demand and talks to it over stdio.

Prerequisites
-------------

- `Docker <https://docs.docker.com/get-docker/>`_ installed and running
- An AI coding assistant with MCP support installed, such as

  - `Claude Code <https://docs.anthropic.com/en/docs/claude-code>`_
  - `GitHub Copilot in VS Code <https://code.visualstudio.com/docs/copilot/customization/mcp-servers>`_

Get the image
-------------

Pull the pre-built image from GitHub Container Registry:

.. code-block:: bash

    docker pull ghcr.io/trossenrobotics/trossen_arm/docs-mcp-server

Or build it from source:

#. Clone the repository:

    .. code-block:: bash

        git clone https://github.com/TrossenRobotics/trossen_arm.git
        cd trossen_arm

#. Build the Docker image:

    .. code-block:: bash

        docker build -f docs/mcp_server/Dockerfile -t ghcr.io/trossenrobotics/trossen_arm/docs-mcp-server .

Adding the local server to Claude Code
--------------------------------------

Run the following command to register the MCP server with Claude Code:

.. code-block:: bash

    claude mcp add trossen-docs -- docker run -i --rm ghcr.io/trossenrobotics/trossen_arm/docs-mcp-server

This configures Claude Code to launch the server automatically when needed using stdio transport.

.. note::

    If you built the image from source without the suggested tag, replace ``ghcr.io/trossenrobotics/trossen_arm/docs-mcp-server`` with your local image name in the commands above.

To verify the server was added:

.. code-block:: bash

    claude mcp list

You should see ``trossen-docs`` in the output.

Adding the local server to GitHub Copilot in VS Code
----------------------------------------------------

- Open the command palette by pressing ``Ctrl+Shift+P`` (Windows/Linux) / ``Command+Shift+P`` (Mac).
- Type and select ``MCP: Add Server...``.
- Type and select ``Command (stdio)``.
- For the command to run, enter:

  .. code-block:: bash

      docker run -i --rm ghcr.io/trossenrobotics/trossen_arm/docs-mcp-server

- You can choose any unique identifier for the server, such as ``trossen-docs``.

To verify the server was added:

- Open the command palette by pressing ``Ctrl+Shift+P`` (Windows/Linux) / ``Command+Shift+P`` (Mac).
- Type and select ``MCP: List Servers``.
- You should be able to see and manage the status of ``trossen-docs``.

The ``mcp.json`` configuration file should look like this:

.. code-block:: json

    {
        "servers": {
            "trossen-docs": {
                "type": "stdio",
                "command": "docker",
                "args": [
                    "run",
                    "-i",
                    "--rm",
                    "ghcr.io/trossenrobotics/trossen_arm/docs-mcp-server"
                ]
            }
        },
        "inputs": []
    }

Usage
=====

Once the server is connected, your assistant automatically uses its tools when you ask questions about the Trossen Arm.
No special syntax is needed.

Example prompts:

- "How do I set up the Trossen Arm for the first time?"
- "What are the parameters for the configure method?"
- "Show me the ROS 2 bringup instructions"
- "What does the Mode enum look like?"
- "Why is my arm jittering during data collection?"
- "Show me how to do cartesian position control"
- "How do I open and close the gripper?"

Behind the scenes, the assistant calls the MCP server's tools to search the docs, read relevant pages, and look up API symbols to answer your question.

Removing the server
===================

- **Claude Code:** run ``claude mcp remove trossen-docs``.
- **GitHub Copilot in VS Code:** open the command palette, select ``MCP: List Servers``, then remove ``trossen-docs``.
- **Claude.ai / ChatGPT:** remove the connector from the same Connectors (Apps) settings page where you added it.
