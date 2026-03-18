==========
MCP Server
==========

.. contents::
   :local:
   :depth: 2

Overview
========

The Trossen Arm MCP (Model Context Protocol) server gives AI coding assistants like `Claude Code <https://docs.anthropic.com/en/docs/claude-code>`_ direct access to the Trossen Arm documentation and API reference.
Instead of searching the web or copy-pasting docs, you can ask your assistant questions and it will look up the answers from the official documentation automatically.

The server provides the following tools:

- **list_doc_pages** — List all available documentation pages and API headers.
- **search_docs** — Fuzzy search across all documentation with relevance-ranked results.
- **read_doc_page** — Read a full documentation page or header file.
- **read_doc_section** — Read a specific section from a documentation page by heading.
- **get_api_reference** — Look up API symbols (classes, methods, enums, structs) with their documentation.
- **list_demos** — List all Python and C++ demo scripts with their descriptions.
- **search_demos** — Search demo scripts by keyword, returning purpose, hardware setup, and steps.
- **get_demo** — Read the full source code of a demo script.

Prerequisites
=============

- `Docker <https://docs.docker.com/get-docker/>`_ installed and running
- `Claude Code <https://docs.anthropic.com/en/docs/claude-code>`_ installed

Installation
============

Using a Pre-built Docker Image
------------------------------

Pull the image from GitHub Container Registry:

.. code-block:: bash

    docker pull ghcr.io/trossenrobotics/trossen_arm/docs-mcp-server

Building from Source
--------------------

If you prefer to build the image yourself:

#. Clone the repository:

    .. code-block:: bash

        git clone https://github.com/TrossenRobotics/trossen_arm.git
        cd trossen_arm

#. Build the Docker image:

    .. code-block:: bash

        docker build -f docs/mcp_server/Dockerfile -t trossen-arm-docs-mcp .

Adding the Server to Claude Code
=================================

Run the following command to register the MCP server with Claude Code:

.. code-block:: bash

    claude mcp add trossen-arm-docs -- docker run -i --rm ghcr.io/trossenrobotics/trossen_arm/docs-mcp-server

This configures Claude Code to launch the server automatically when needed using stdio transport.

.. note::

    If you built the image from source, replace ``ghcr.io/trossenrobotics/trossen_arm/docs-mcp-server`` with your local image name in the command above.

To verify the server was added:

.. code-block:: bash

    claude mcp list

You should see ``trossen-arm-docs`` in the output.

Usage
=====

Once installed, Claude Code will automatically use the MCP server tools when you ask questions about the Trossen Arm.
No special syntax is needed.

Example prompts:

- "How do I set up the Trossen Arm for the first time?"
- "What are the parameters for the configure method?"
- "Show me the ROS 2 bringup instructions"
- "What does the Mode enum look like?"
- "Why is my arm jittering during data collection?"
- "Show me how to do cartesian position control"
- "How do I open and close the gripper?"

Behind the scenes, Claude Code calls the MCP server's tools to search the docs, read relevant pages, and look up API symbols to answer your question.

Removing the Server
===================

To remove the MCP server from Claude Code:

.. code-block:: bash

    claude mcp remove trossen-arm-docs
