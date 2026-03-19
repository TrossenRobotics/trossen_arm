# Copyright 2026 Trossen Robotics
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#
#    * Neither the name of the copyright holder nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES LOSS OF USE, DATA, OR PROFITS OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

"""Trossen Documentation MCP Server.

Provides tools for searching and reading the Trossen documentation,
including RST guides, tutorials, and C++ API headers.
"""

import importlib.util
import os
import re
from pathlib import Path

from mcp.server.fastmcp import FastMCP

DOCS_DIR = Path(os.environ.get("DOCS_DIR", Path(__file__).parent.parent))
INCLUDE_DIR = Path(
    os.environ.get(
        "INCLUDE_DIR", Path(__file__).parent.parent.parent / "include"
    )
)
DEMOS_DIR = Path(
    os.environ.get(
        "DEMOS_DIR", Path(__file__).parent.parent.parent / "demos"
    )
)


def _find_python_stubs() -> Path:
    """Auto-discover the trossen_arm ``.pyi`` stub from the installed package."""
    env_path = os.environ.get("PYTHON_STUBS")
    if env_path:
        return Path(env_path)
    spec = importlib.util.find_spec("trossen_arm")
    if spec and spec.submodule_search_locations:
        stub = (
            Path(spec.submodule_search_locations[0]) / "trossen_arm.pyi"
        )
        if stub.is_file():
            return stub
    return Path("trossen_arm.pyi")  # fallback


PYTHON_STUBS = _find_python_stubs()

TRANSPORT_MODE = os.environ.get("TRANSPORT_MODE", "stdio")
TRANSPORT_HOST = os.environ.get("TRANSPORT_HOST", "127.0.0.1")
TRANSPORT_PORT = int(os.environ.get("TRANSPORT_PORT", "8080"))

mcp = FastMCP(
    "trossen-docs",
    instructions=(
        "Trossen Arm documentation server. Use this to look up information"
        " about Trossen Arm robots including hardware setup, software"
        " configuration, tutorials (ROS 2, LeRobot, OpenPI, Isaac Sim,"
        " MuJoCo), API reference, specifications, troubleshooting, and demo"
        " scripts (Python and C++). The Python and C++ APIs are equivalent"
        " - the Python trossen_arm package is a direct binding of the C++"
        " libtrossen_arm library with identical classes, methods, enums,"
        " and behavior. When a user asks about the Python API, use"
        " get_api_reference which searches both the C++ headers and the"
        " Python type stubs. Python demos are in demos/python/ and C++"
        " demos are in demos/cpp/ with matching filenames for the same"
        " functionality."
    ),
    host=TRANSPORT_HOST,
    port=TRANSPORT_PORT,
)


_EXCLUDED_DIRS = {
    "build", "api", "doxygen", "_build", "__pycache__", "mcp_server",
}


def _collect_files(
    base_dir: Path, extensions: set[str]
) -> list[Path]:
    """
    Collect all files with given extensions under *base_dir*.

    :param base_dir: Root directory to search recursively.
    :param extensions: Set of file suffixes to include (e.g. ``{".rst"}``).
    :returns: Sorted list of matching :class:`~pathlib.Path` objects,
        excluding generated/build directories.
    """
    files = []
    if not base_dir.is_dir():
        return files
    for path in sorted(base_dir.rglob("*")):
        if path.is_file() and path.suffix in extensions:
            # Skip generated/build directories
            rel_parts = path.relative_to(base_dir).parts
            if not any(part in _EXCLUDED_DIRS for part in rel_parts):
                files.append(path)
    return files


def _extract_rst_title(content: str) -> str | None:
    """
    Extract the first RST title from *content*.

    :param content: Raw RST file content.
    :returns: The title string, or ``None`` if no title is found.
    """
    lines = content.splitlines()
    for i, line in enumerate(lines):
        if (
            i + 1 < len(lines)
            and re.match(r"^[=\-~^\"]+$", lines[i + 1])
        ):
            if len(lines[i + 1]) >= len(line.rstrip()) and line.strip():
                return line.strip()
        if i > 0 and re.match(r"^[=\-~^\"]+$", lines[i - 1]):
            if len(lines[i - 1]) >= len(line.rstrip()) and line.strip():
                return line.strip()
    return None


def _split_into_sections(content: str) -> list[dict]:
    """
    Split RST content into sections by heading.

    :param content: Raw RST file content.
    :returns: List of dicts with ``"title"`` and ``"content"`` keys.
    """
    lines = content.splitlines()
    sections = []
    current_title = "(introduction)"
    current_lines: list[str] = []
    i = 0
    while i < len(lines):
        # Underline-style heading: title line followed by === or ---
        if (
            i + 1 < len(lines)
            and lines[i].strip()
            and re.match(r"^[=\-~^\"]+$", lines[i + 1])
            and len(lines[i + 1]) >= len(lines[i].rstrip())
        ):
            if current_lines:
                sections.append({
                    "title": current_title,
                    "content": "\n".join(current_lines),
                })
            current_title = lines[i].strip()
            current_lines = []
            i += 2
            continue
        # Overline-style heading: === then title then ===
        if (
            i + 2 < len(lines)
            and re.match(r"^[=\-~^\"]+$", lines[i])
            and lines[i + 1].strip()
            and re.match(r"^[=\-~^\"]+$", lines[i + 2])
        ):
            if current_lines:
                sections.append({
                    "title": current_title,
                    "content": "\n".join(current_lines),
                })
            current_title = lines[i + 1].strip()
            current_lines = []
            i += 3
            continue
        current_lines.append(lines[i])
        i += 1
    if current_lines:
        sections.append({
            "title": current_title,
            "content": "\n".join(current_lines),
        })
    return sections


@mcp.tool()
def list_doc_pages() -> str:
    """
    List all available documentation pages and API headers.

    Returns a structured list of all RST documentation pages and C++ header files with their titles
    where available. Use this to discover what documentation is available before searching or
    reading specific pages.
    """
    output_lines = ["## Documentation Pages\n"]
    for path in _collect_files(DOCS_DIR, {".rst"}):
        rel = path.relative_to(DOCS_DIR)
        try:
            content = path.read_text()
            title = _extract_rst_title(content)
        except Exception:
            title = None
        if title:
            output_lines.append(f"- `{rel}` — {title}")
        else:
            output_lines.append(f"- `{rel}`")

    output_lines.append("\n## C++ API Headers\n")
    for path in _collect_files(INCLUDE_DIR, {".hpp", ".h"}):
        rel = path.relative_to(INCLUDE_DIR)
        output_lines.append(f"- `{rel}`")

    output_lines.append("\n## Python API Type Stubs\n")
    if PYTHON_STUBS.is_file():
        output_lines.append(
            "- `trossen_arm.pyi` — Python type stubs with docstrings"
            " (equivalent to the C++ API)"
        )

    return "\n".join(output_lines)


_STOP_WORDS = frozenset({
    "a", "an", "the", "is", "are", "was", "were", "be", "been",
    "being", "have", "has", "had", "do", "does", "did", "will",
    "would", "could", "should", "may", "might", "shall", "can",
    "need", "must", "to", "of", "in", "for", "on", "with", "at",
    "by", "from", "as", "into", "about", "between", "through",
    "during", "before", "after", "and", "but", "or", "nor", "not",
    "so", "yet", "both", "either", "it", "its", "this", "that",
    "these", "those", "what", "which", "who", "how", "where", "when",
    "why", "if", "then", "than", "i", "me", "my", "we", "our",
    "you", "your", "he", "she", "they",
})


def _tokenize(text: str) -> list[str]:
    """
    Split text into lowercase alphanumeric tokens, filtering stop words.

    :param text: The input text to tokenize.
    :returns: List of filtered tokens, or the unfiltered list if all
        tokens were stop words.
    """
    tokens = re.findall(r"[a-z0-9_]+", text.lower())
    filtered = [t for t in tokens if t not in _STOP_WORDS]
    return filtered if filtered else tokens


def _score_match(
    tokens: list[str],
    text_lower: str,
    text_tokens: set[str],
) -> float:
    """
    Score how well a set of query tokens match a piece of text.

    Scoring considers:

    * Fraction of query tokens present in the text
    * Bonus for exact phrase match
    * Bonus for token proximity (tokens appearing near each other)

    :param tokens: Query tokens to match against.
    :param text_lower: Lowercased text to search within.
    :param text_tokens: Set of tokens extracted from *text_lower*.
    :returns: A relevance score (0.0 means no match).
    """
    if not tokens:
        return 0.0

    # Base score: fraction of query tokens found
    matched = sum(1 for t in tokens if t in text_tokens)
    score = matched / len(tokens)

    # Must match at least half the tokens to be considered
    if score < 0.5:
        return 0.0

    # Bonus for exact phrase match
    phrase = " ".join(tokens)
    if phrase in text_lower:
        score += 0.5

    # Proximity bonus: check if tokens appear within a short window
    if len(tokens) > 1 and matched == len(tokens):
        words = text_lower.split()
        positions: list[list[int]] = []
        for token in tokens:
            positions.append(
                [i for i, w in enumerate(words) if token in w]
            )
        if all(positions):
            min_span = float("inf")
            for start_pos in positions[0]:
                for end_pos in positions[-1]:
                    span = abs(end_pos - start_pos)
                    min_span = min(min_span, span)
            if min_span < len(tokens) * 3:
                score += 0.3

    return score


def _extract_context_lines(
    lines: list[str],
    tokens: list[str],
    max_lines: int = 6,
) -> list[str]:
    """
    Extract lines matching any query token, with surrounding context.

    :param lines: Source lines to search.
    :param tokens: Query tokens to match.
    :param max_lines: Maximum number of context lines to return.
    :returns: List of matching lines with one line of context on each side.
    """
    context_lines: list[str] = []
    seen_indices: set[int] = set()
    for j, line in enumerate(lines):
        line_lower = line.lower()
        if any(t in line_lower for t in tokens):
            start = max(0, j - 1)
            end = min(len(lines), j + 2)
            for k in range(start, end):
                if k not in seen_indices:
                    seen_indices.add(k)
                    context_lines.append(lines[k])
        if len(context_lines) >= max_lines:
            break
    return context_lines


@mcp.tool()
def search_docs(query: str, include_headers: bool = False) -> str:
    """
    Search the Trossen Arm documentation for a keyword or phrase.

    Searches across all RST documentation files and optionally C++ headers. Uses fuzzy token-based
    matching so multi-word queries like "data collection UI features" match documents containing
    those words even if the exact phrase doesn't appear. Results are ranked by relevance.

    :param query: The search term or phrase to find (case-insensitive). Multi-word queries match
        documents containing most/all words.
    :param include_headers: If ``True``, also search C++ header files in ``include/``.
    :returns: Formatted search results ranked by relevance, or a "no results" message.
    """
    tokens = _tokenize(query)
    if not tokens:
        return "Please provide a search query."

    scored_results: list[tuple[float, str]] = []

    # Search RST files with section-level granularity
    for path in _collect_files(DOCS_DIR, {".rst"}):
        rel = str(path.relative_to(DOCS_DIR))
        try:
            content = path.read_text()
        except Exception:
            continue

        content_lower = content.lower()
        content_tokens = set(_tokenize(content_lower))

        # Quick check: at least half the query tokens present?
        matched_count = sum(1 for t in tokens if t in content_tokens)
        if matched_count < max(1, len(tokens) // 2):
            continue

        sections = _split_into_sections(content)
        matching_sections: list[tuple[float, str]] = []
        for section in sections:
            section_lower = section["content"].lower()
            section_tokens = set(_tokenize(section_lower))
            score = _score_match(tokens, section_lower, section_tokens)
            if score > 0:
                lines = section["content"].splitlines()
                context_lines = _extract_context_lines(lines, tokens)
                preview = "\n".join(context_lines[:6])
                matching_sections.append(
                    (score, f"### {section['title']}\n```\n{preview}\n```")
                )

        if matching_sections:
            matching_sections.sort(key=lambda x: x[0], reverse=True)
            page_title = _extract_rst_title(content) or rel
            best_score = matching_sections[0][0]
            section_text = "\n\n".join(
                s[1] for s in matching_sections[:3]
            )
            result = f"## {page_title} (`{rel}`)\n\n{section_text}"
            scored_results.append((best_score, result))

    # Search C++ headers
    if include_headers:
        for path in _collect_files(INCLUDE_DIR, {".hpp", ".h"}):
            rel = str(path.relative_to(INCLUDE_DIR))
            try:
                content = path.read_text()
            except Exception:
                continue
            content_lower = content.lower()
            content_tokens = set(_tokenize(content_lower))
            score = _score_match(tokens, content_lower, content_tokens)
            if score > 0:
                lines = content.splitlines()
                context_lines = _extract_context_lines(
                    lines, tokens, max_lines=10,
                )
                if context_lines:
                    preview = "\n".join(context_lines[:10])
                    scored_results.append(
                        (
                            score,
                            f"## C++ Header: `{rel}`\n"
                            f"```cpp\n{preview}\n```",
                        )
                    )

    if not scored_results:
        return f"No results found for '{query}'."

    # Sort by relevance score descending
    scored_results.sort(key=lambda x: x[0], reverse=True)
    results = [r[1] for r in scored_results[:10]]

    return (
        f"Found {len(scored_results)} result(s) for '{query}':\n\n"
        + "\n\n---\n\n".join(results)
    )


@mcp.tool()
def read_doc_page(path: str) -> str:
    """
    Read a documentation page or C++ header file.

    :param path: Path relative to ``docs/`` for RST files (e.g.
        ``'getting_started/software_setup.rst'``) or relative to ``include/`` for headers (e.g.
        ``'libtrossen_arm/trossen_arm.hpp'``). Use :func:`list_doc_pages` to discover available
        paths.
    :returns: The file contents, or an error message if not found.
    """
    # Try docs dir first
    full_path = DOCS_DIR / path
    if full_path.is_file():
        return full_path.read_text()

    # Try include dir
    full_path = INCLUDE_DIR / path
    if full_path.is_file():
        return full_path.read_text()

    return (
        f"File not found: '{path}'. "
        "Use list_doc_pages() to see available files. "
        "Paths should be relative to docs/ or include/."
    )


@mcp.tool()
def read_doc_section(path: str, section_title: str) -> str:
    """
    Read a specific section from a documentation page.

    Useful for reading just the relevant part of a long page without fetching the entire document.

    :param path: Path relative to ``docs/`` (e.g. ``'troubleshooting.rst'``).
    :param section_title: The heading text of the section to read (case-insensitive).
    :returns: The section content, or an error message listing available sections.
    """
    full_path = DOCS_DIR / path
    if not full_path.is_file():
        return f"File not found: '{path}'."

    content = full_path.read_text()
    sections = _split_into_sections(content)
    section_lower = section_title.lower()

    for section in sections:
        if section["title"].lower() == section_lower:
            return f"## {section['title']}\n\n{section['content']}"

    available = [
        s["title"] for s in sections if s["title"] != "(introduction)"
    ]
    return (
        f"Section '{section_title}' not found in '{path}'.\n"
        f"Available sections: {', '.join(available)}"
    )


@mcp.tool()
def get_api_reference(symbol: str, language: str = "both") -> str:
    """
    Look up an API symbol in the C++ headers and/or Python type stubs.

    The Python API is a direct binding of the C++ library — classes, methods, enums, and behavior
    are identical. Use ``language="python"`` for Python-specific signatures and docstrings,
    ``"cpp"`` for C++ declarations, or ``"both"`` (default).

    :param symbol: The symbol name to look up (e.g. ``'TrossenArmDriver'``, ``'Mode'``,
        ``'configure'``).
    :param language: Which API to search: ``"cpp"``, ``"python"``, or ``"both"`` (default).
    :returns: Formatted matches with source snippets, or a "not found" message.
    """
    results: list[str] = []

    # Search C++ headers
    if language in ("cpp", "both"):
        for path in _collect_files(INCLUDE_DIR, {".hpp", ".h"}):
            rel = str(path.relative_to(INCLUDE_DIR))
            try:
                lines = path.read_text().splitlines()
            except Exception:
                continue

            for i, line in enumerate(lines):
                if symbol in line:
                    # Grab doxygen comment block above
                    start = i
                    while start > 0 and (
                        lines[start - 1].strip().startswith("///")
                        or lines[start - 1].strip().startswith("*")
                        or lines[start - 1].strip().startswith("/**")
                    ):
                        start -= 1
                    end = min(len(lines), i + 5)
                    snippet = "\n".join(lines[start:end])
                    results.append(
                        f"### C++: `{rel}` (line {i + 1})\n"
                        f"```cpp\n{snippet}\n```"
                    )

    # Search Python type stubs
    if language in ("python", "both") and PYTHON_STUBS.is_file():
        try:
            lines = PYTHON_STUBS.read_text().splitlines()
        except Exception:
            lines = []

        for i, line in enumerate(lines):
            if symbol in line:
                # Grab decorator/def context above
                start = i
                while start > 0 and (
                    lines[start - 1].strip().startswith("@")
                    or lines[start - 1].strip().startswith('"""')
                    or lines[start - 1].strip().startswith("#")
                ):
                    start -= 1

                # Expand forward to capture docstrings
                end = i + 1
                in_docstring = False
                while end < len(lines):
                    stripped = lines[end].strip()
                    if (
                        stripped.startswith('"""')
                        and not in_docstring
                    ):
                        in_docstring = True
                    elif stripped.endswith('"""') and in_docstring:
                        end += 1
                        break
                    elif (
                        not in_docstring
                        and stripped
                        and not stripped.startswith('"""')
                    ):
                        if stripped.startswith(
                            ("def ", "class ", "@")
                        ):
                            break
                    end += 1
                end = min(end, i + 25)

                snippet = "\n".join(lines[start:end])
                results.append(
                    f"### Python: `trossen_arm.pyi` (line {i + 1})\n"
                    f"```python\n{snippet}\n```"
                )

    if not results:
        return (
            f"Symbol '{symbol}' not found in API headers or type stubs."
        )

    return (
        f"Found {len(results)} match(es) for '{symbol}':\n\n"
        + "\n\n".join(results[:15])
    )


def _extract_demo_metadata(content: str) -> dict:
    """
    Extract purpose, hardware setup, and steps from demo header comments.

    Handles both Python (``#`` comments) and C++ (``//`` comments) demo headers. Scans past blank
    lines between comment blocks (e.g. between the license header and the purpose/steps section).

    :param content: Full demo file content.
    :returns: Dict with optional keys ``"purpose"``, ``"hardware_setup"``, and ``"steps"``.
    """
    metadata: dict[str, str] = {}
    current_key = None
    current_lines: list[str] = []

    # Determine comment prefix from file content
    comment_prefix = "//" if content.lstrip().startswith("//") else "#"

    for line in content.splitlines():
        stripped = line.strip()

        # Allow blank lines within the header region
        if not stripped:
            continue

        # Stop at the first non-comment, non-blank line
        if not stripped.startswith(comment_prefix):
            break

        # Strip comment prefix and leading whitespace
        text = stripped[len(comment_prefix):].strip()

        if text.lower().startswith("purpose:"):
            if current_key:
                metadata[current_key] = "\n".join(
                    current_lines
                ).strip()
            current_key = "purpose"
            current_lines = [text[len("purpose:"):].strip()]
        elif text.lower().startswith("hardware setup:"):
            if current_key:
                metadata[current_key] = "\n".join(
                    current_lines
                ).strip()
            current_key = "hardware_setup"
            current_lines = [text[len("hardware setup:"):].strip()]
        elif re.match(
            r"the (script|program) does the following:", text.lower()
        ):
            if current_key:
                metadata[current_key] = "\n".join(
                    current_lines
                ).strip()
            current_key = "steps"
            current_lines = []
        elif current_key and text:
            current_lines.append(text)

    if current_key and current_key not in metadata:
        metadata[current_key] = "\n".join(current_lines).strip()

    return metadata


@mcp.tool()
def list_demos(language: str = "both") -> str:
    """
    List available demo scripts with their descriptions.

    The demos show how to use the Trossen Arm driver for common tasks like movement, cartesian
    control, configuration, gravity compensation, and more.

    :param language: Filter by language: ``"python"``, ``"cpp"``, or ``"both"`` (default).
    :returns: Formatted list of demos with purpose descriptions.
    """
    output_lines: list[str] = []
    lang_dirs = []
    if language in ("python", "both"):
        lang_dirs.append(("Python", DEMOS_DIR / "python", {".py"}))
    if language in ("cpp", "both"):
        lang_dirs.append(("C++", DEMOS_DIR / "cpp", {".cpp"}))

    for lang_name, lang_dir, extensions in lang_dirs:
        output_lines.append(f"## {lang_name} Demos\n")
        for path in _collect_files(lang_dir, extensions):
            rel = path.relative_to(DEMOS_DIR)
            try:
                content = path.read_text()
                meta = _extract_demo_metadata(content)
                purpose = meta.get("purpose", "")
            except Exception:
                purpose = ""
            if purpose:
                output_lines.append(f"- `{rel}` — {purpose}")
            else:
                output_lines.append(f"- `{rel}`")
        output_lines.append("")

    return "\n".join(output_lines) if output_lines else "No demos found."


@mcp.tool()
def get_demo(path: str) -> str:
    """
    Read a demo script with its full source code.

    Returns the complete demo file contents. Each demo includes header comments explaining its
    purpose, required hardware setup, and step-by-step description.

    :param path: Path relative to ``demos/`` (e.g. ``'python/simple_move.py'`` or
        ``'cpp/cartesian_position.cpp'``). Use :func:`list_demos` to discover available demos.
    :returns: The full demo source code, or an error message if not found.
    """
    full_path = DEMOS_DIR / path
    if not full_path.is_file():
        return (
            f"Demo not found: '{path}'. "
            "Use list_demos() to see available demos. "
            "Paths should be relative to demos/"
            " (e.g. 'python/simple_move.py')."
        )
    return full_path.read_text()


@mcp.tool()
def search_demos(query: str) -> str:
    """
    Search demo scripts by keyword.

    Searches across all Python and C++ demo files including their header comments (purpose,
    hardware setup, steps) and source code. Results are ranked by relevance.

    :param query: The search term or phrase (e.g. ``"cartesian"``, ``"gripper"``,
        ``"teleoperation"``).
    :returns: Formatted search results with demo metadata, or a "no results" message.
    """
    tokens = _tokenize(query)
    if not tokens:
        return "Please provide a search query."

    scored_results: list[tuple[float, str]] = []

    for path in (
        _collect_files(DEMOS_DIR / "python", {".py"})
        + _collect_files(DEMOS_DIR / "cpp", {".cpp"})
    ):
        rel = str(path.relative_to(DEMOS_DIR))
        try:
            content = path.read_text()
        except Exception:
            continue

        content_lower = content.lower()
        content_tokens = set(_tokenize(content_lower))
        score = _score_match(tokens, content_lower, content_tokens)
        if score > 0:
            meta = _extract_demo_metadata(content)
            purpose = meta.get("purpose", "")
            steps = meta.get("steps", "")
            hardware = meta.get("hardware_setup", "")
            summary_parts = []
            if purpose:
                summary_parts.append(f"**Purpose:** {purpose}")
            if hardware:
                summary_parts.append(f"**Hardware:** {hardware}")
            if steps:
                summary_parts.append(f"**Steps:**\n{steps}")
            summary = (
                "\n".join(summary_parts)
                if summary_parts
                else "(no description)"
            )
            scored_results.append(
                (score, f"## `{rel}`\n\n{summary}")
            )

    if not scored_results:
        return f"No demos found for '{query}'."

    scored_results.sort(key=lambda x: x[0], reverse=True)
    results = [r[1] for r in scored_results[:10]]

    return (
        f"Found {len(scored_results)} demo(s) for '{query}':\n\n"
        + "\n\n---\n\n".join(results)
    )


def main():
    """Entry point - runs the MCP server in stdio or HTTP mode."""
    if TRANSPORT_MODE == "http":
        mcp.run(transport="streamable-http")
    else:
        mcp.run(transport="stdio")


if __name__ == "__main__":
    main()
