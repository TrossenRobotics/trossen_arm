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
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# /// script
# requires-python = ">=3.10"
# dependencies = [
#     "readchar>=4.0",
#     "rich>=13.0",
#     "trossen_arm>=1.9",
# ]
# ///

"""
Joint Characteristic Tuning

Run with:
    uv run tuning.py [--ip 192.168.1.2]

Setup:  Arrow keys to navigate, Left/Right to change selections, Enter to confirm.
Tuning: Arrow keys to navigate the grid, w/s to nudge value, W/S for 10x nudge,
        +/- to double/halve increment, r to refresh, q or Esc to quit.
"""

from __future__ import annotations

import argparse
import logging
import os
import termios

import readchar
import trossen_arm
from rich.console import Console
from rich.layout import Layout
from rich.panel import Panel
from rich.table import Table
from rich.text import Text


def configure_logging(model: trossen_arm.Model, serv_ip: str) -> None:
    """Suppress trossen_arm log output to the terminal."""
    for name in (
        trossen_arm.TrossenArmDriver.get_default_logger_name(),
        trossen_arm.TrossenArmDriver.get_logger_name(model, serv_ip),
    ):
        logger = logging.getLogger(name)
        logger.handlers.clear()
        logger.addHandler(logging.NullHandler())
        logger.propagate = False


# Constants

CHARACTERISTICS = (
    "friction_constant_term",
    "friction_coulomb_coef",
    "friction_viscous_coef",
    "friction_transition_velocity",
    "position_offset",
    "effort_correction",
)

CHAR_INFO: dict[str, tuple[str, str]] = {
    "effort_correction":            ("Effort Correction", "motor effort / Nm or N"),
    "friction_transition_velocity": ("Fric Trans Vel",    "rad/s | m/s"),
    "friction_constant_term":       ("Fric Const Term",   "Nm | N"),
    "friction_coulomb_coef":        ("Fric Coulomb",      "Nm/Nm | N/N"),
    "friction_viscous_coef":        ("Fric Viscous",      "Nm/(rad/s) | N/(m/s)"),
    "position_offset":              ("Pos Offset",        "rad | m"),
}

END_EFFECTORS = (
    ("WXAI V0 Base",     trossen_arm.StandardEndEffector.wxai_v0_base),
    ("WXAI V0 Leader",   trossen_arm.StandardEndEffector.wxai_v0_leader),
    ("WXAI V0 Follower", trossen_arm.StandardEndEffector.wxai_v0_follower),
    ("No Gripper",       trossen_arm.StandardEndEffector.no_gripper),
)

DEFAULT_INCREMENT = 1e-3

console = Console()


def clamp(val: float, lo: float, hi: float) -> float:
    return max(lo, min(hi, val))


def read_key() -> str:
    """Read a keypress; raises KeyboardInterrupt on terminal errors."""
    try:
        return readchar.readkey()
    except (termios.error, OSError):
        raise KeyboardInterrupt


# Setup screen


def render_setup(
    ee_idx: int, ip_str: str, clear_error: bool, field: int, error_msg: str
) -> Panel:
    """Build the Rich renderable for the setup screen."""
    lines = Text()
    lines.append("Trossen Arm — Joint Characteristic Finetuner\n\n", style="bold cyan")

    fields = [
        ("End Effector", f"◂ {END_EFFECTORS[ee_idx][0]} ▸"),
        ("Robot IP", ip_str),
        ("Clear Error", "Yes" if clear_error else "No"),
    ]

    for i, (label, value) in enumerate(fields):
        if i == field:
            lines.append(f"  ▸ {label}:  ", style="bold")
            lines.append(f" {value} ", style="reverse bold")
        else:
            lines.append(f"    {label}:  ", style="dim")
            lines.append(value)
        lines.append("\n")

    lines.append("\n")
    if field == 3:
        lines.append("  ▸ ", style="bold")
        lines.append(" [ Connect ] ", style="reverse bold green")
    else:
        lines.append("    [ Connect ]", style="dim")
    lines.append("\n")

    if error_msg:
        lines.append(f"\n  {error_msg}", style="bold red")

    lines.append("\n\n")
    lines.append(
        "↑↓ navigate  ←→ change  Enter confirm  Type when IP focused", style="dim"
    )

    return Panel(lines, title="Setup", border_style="blue", expand=False, width=60)


def setup_phase(cli_ip: str | None = None) -> tuple[trossen_arm.TrossenArmDriver, int]:
    """Run the interactive setup. Returns (driver, num_joints)."""
    ee_idx = 1
    ip_str = cli_ip or "192.168.1.2"
    clear_error = False
    field = 0  # 0=ee, 1=ip, 2=clear_error, 3=connect
    error_msg = ""

    while True:
        console.clear()
        console.print(render_setup(ee_idx, ip_str, clear_error, field, error_msg))

        key = read_key()

        # Navigation
        if key in (readchar.key.DOWN, "\t"):
            field = (field + 1) % 4
        elif key == readchar.key.UP:
            field = (field - 1) % 4
        elif key in (readchar.key.ENTER, readchar.key.CR, readchar.key.LF):
            if field == 3:
                # Connect
                error_msg = ""
                console.clear()
                console.print("[bold yellow]Connecting…[/bold yellow]")
                try:
                    configure_logging(trossen_arm.Model.wxai_v0, ip_str)
                    driver = trossen_arm.TrossenArmDriver()
                    driver.configure(
                        trossen_arm.Model.wxai_v0,
                        END_EFFECTORS[ee_idx][1],
                        ip_str,
                        clear_error,
                    )
                    num_joints = driver.get_num_joints()
                    return driver, num_joints
                except Exception as exc:
                    error_msg = str(exc)
            else:
                field = (field + 1) % 4
        # Field-specific input
        elif field == 0:
            if key == readchar.key.LEFT:
                ee_idx = (ee_idx - 1) % len(END_EFFECTORS)
            elif key == readchar.key.RIGHT:
                ee_idx = (ee_idx + 1) % len(END_EFFECTORS)
        elif field == 1:
            if key == readchar.key.BACKSPACE:
                ip_str = ip_str[:-1]
            elif len(key) == 1 and 32 <= ord(key) <= 126:
                ip_str += key
        elif field == 2:
            if key in (readchar.key.LEFT, readchar.key.RIGHT, " "):
                clear_error = not clear_error


# Tuning screen


def build_sidebar(
    joint_idx: int,
    char_idx: int,
    increment: float,
    cur_val: float,
    char_name: str,
    orig_val: float | None = None,
) -> Panel:
    """Build the sidebar panel."""
    label, unit = CHAR_INFO[char_name]
    t = Text()

    t.append("   Joint:  ", style="bold")
    t.append(f"Joint {joint_idx}\n", style="bold cyan")
    t.append("   Characteristic:\n", style="bold")
    t.append(f"     {label}\n\n", style="bold cyan")
    t.append("   Increment: ", style="bold")
    t.append(f"{increment:.2e}\n", style="bold cyan")

    t.append("   ─────────────────────────\n", style="dim")
    t.append(f"   {label}:\n", style="bold cyan")
    t.append(f"   Current:  {cur_val:+.8f}\n", style="bold white")
    if orig_val is not None:
        delta = cur_val - orig_val
        delta_style = "green" if delta > 0 else "red" if delta < 0 else "dim"
        t.append(f"   Original: {orig_val:+.8f}\n", style="dim")
        t.append(f"   Δ delta:  {delta:+.8f}\n", style=delta_style)
    t.append(f"   ({unit})\n\n", style="dim")
    t.append("   Tip: start small (1e-3),\n", style="dim")
    t.append("   double with + until\n", style="dim")
    t.append("   behavior changes.\n", style="dim")

    return Panel(t, title="Controls", border_style="green", width=40)


def build_table(chars, joint_idx: int, char_idx: int, orig_chars=None) -> Panel:
    """Build the characteristics table."""
    table = Table(show_header=True, header_style="bold", expand=True, show_lines=False)
    table.add_column("Joint", style="bold", width=8)
    for c in CHARACTERISTICS:
        table.add_column(CHAR_INFO[c][0], justify="right", width=14)

    for i, jc in enumerate(chars):
        sel = i == joint_idx
        row: list[str] = [f"{'→' if sel else ' '} Joint {i}"]
        for ci, c in enumerate(CHARACTERISTICS):
            v = getattr(jc, c)
            cell = f"{v:+.6f}"
            changed = False
            if orig_chars is not None:
                ov = getattr(orig_chars[i], c)
                if abs(v - ov) > 1e-12:
                    changed = True
                    cell += f" ({v - ov:+.2e})"
            if sel and ci == char_idx:
                cell = f"[bold yellow]{cell}[/bold yellow]"
            elif changed:
                cell = f"[cyan]{cell}[/cyan]"
            row.append(cell)
        table.add_row(*row, style="bold white on dark_green" if sel else "")

    return Panel(
        table,
        title="All Joint Characteristics  [dim](cyan = changed)[/dim]",
        border_style="blue",
    )


FOOTER_ITEMS = (
    ("←→", "characteristic"),
    ("↑↓", "joint"),
    ("w/s", "nudge"),
    ("W/S", "10x"),
    ("+/-", "increment"),
    ("r", "refresh"),
    ("q/Esc", "quit"),
)


def build_footer() -> Text:
    t = Text()
    for key, desc in FOOTER_ITEMS:
        t.append(f" {key}", style="bold")
        t.append(f" {desc} ", style="dim")
    return t


def tuning_phase(driver: trossen_arm.TrossenArmDriver, num_joints: int) -> None:
    """Main tuning loop."""
    joint_idx = 0
    char_idx = CHARACTERISTICS.index("friction_constant_term")
    increment = DEFAULT_INCREMENT

    def apply_joint_mode():
        modes = [trossen_arm.Mode.idle] * num_joints
        modes[joint_idx] = trossen_arm.Mode.external_effort
        driver.set_joint_modes(modes)
        driver.set_joint_external_effort(joint_idx, 0.0, False)

    # Snapshot original characteristics for comparison
    orig_chars = driver.get_joint_characteristics()

    apply_joint_mode()

    def apply_delta(delta: float):
        chars = driver.get_joint_characteristics()
        jc = chars[joint_idx]
        char_name = CHARACTERISTICS[char_idx]
        old_val = getattr(jc, char_name)
        new_val = old_val + delta
        if char_name == "effort_correction":
            new_val = clamp(new_val, 0.2, 5.0)
        elif char_name == "friction_transition_velocity":
            new_val = max(1e-12, new_val)
        setattr(jc, char_name, new_val)
        chars[joint_idx] = jc
        driver.set_joint_characteristics(chars)

    def render():
        chars = driver.get_joint_characteristics()
        jc = chars[joint_idx]
        char_name = CHARACTERISTICS[char_idx]
        cur_val = getattr(jc, char_name)

        orig_val = getattr(orig_chars[joint_idx], char_name)
        layout = Layout()
        layout.split_row(
            Layout(
                build_sidebar(
                    joint_idx,
                    char_idx,
                    increment,
                    cur_val,
                    char_name,
                    orig_val,
                ),
                name="sidebar",
                size=42,
            ),
            Layout(build_table(chars, joint_idx, char_idx, orig_chars), name="table"),
        )

        group = Layout()
        group.split_column(
            Layout(
                Panel(
                    "[bold cyan]Trossen Arm — Joint Characteristic Finetuner[/bold cyan]",
                    border_style="cyan",
                ),
                size=3,
            ),
            Layout(layout, name="body"),
            Layout(build_footer(), size=1),
        )
        return group

    while True:
        console.clear()
        console.print(render())

        key = read_key()

        if key in ("q", readchar.key.ESC):
            return

        if key == "r":
            continue

        # Navigate grid: arrows move joint (row) and characteristic (column)
        if key == readchar.key.UP:
            joint_idx = (joint_idx - 1) % num_joints
            apply_joint_mode()
        elif key == readchar.key.DOWN:
            joint_idx = (joint_idx + 1) % num_joints
            apply_joint_mode()
        elif key == readchar.key.LEFT:
            char_idx = (char_idx - 1) % len(CHARACTERISTICS)
        elif key == readchar.key.RIGHT:
            char_idx = (char_idx + 1) % len(CHARACTERISTICS)

        # Nudge selected value
        elif key == "w":
            apply_delta(increment)
        elif key == "s":
            apply_delta(-increment)
        elif key == "W":
            apply_delta(increment * 10)
        elif key == "S":
            apply_delta(-increment * 10)

        # Adjust increment
        elif key in ("+", "="):
            increment *= 2
        elif key == "-":
            increment = max(1e-12, increment / 2)


# Entry point


def main() -> None:
    parser = argparse.ArgumentParser(description="Joint Characteristic Finetuning TUI")
    parser.add_argument("--ip", type=str, default=None, help="Robot IP address")
    args = parser.parse_args()

    driver = None
    console.set_alt_screen(True)
    try:
        driver, num_joints = setup_phase(cli_ip=args.ip)
        # Check terminal size and warn if too small
        cols, rows = os.get_terminal_size()
        min_cols, min_rows = 160, 30
        if cols < min_cols or rows < min_rows:
            console.clear()
            console.print(
                f"[bold yellow]Terminal is {cols}x{rows} — "
                f"recommend at least {min_cols}x{min_rows} "
                f"to see the full table.[/bold yellow]\n"
                "[dim]Maximize your terminal or reduce font size, "
                "then press any key to continue…[/dim]"
            )
            read_key()
        tuning_phase(driver, num_joints)
    except KeyboardInterrupt:
        pass
    finally:
        console.set_alt_screen(False)
        if driver is not None:
            try:
                driver.cleanup()
            except Exception:
                pass
        console.print("[dim]Goodbye.[/dim]")


if __name__ == "__main__":
    main()
