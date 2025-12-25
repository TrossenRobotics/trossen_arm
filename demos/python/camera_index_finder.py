#!/usr/bin/env python3
"""
Script to detect available camera indexes and save sample frames.
This helps identify which camera corresponds to which index.
"""


import cv2
import os
from pathlib import Path
from threading import Thread
import time
from rich.console import Console
from rich.table import Table
from rich.panel import Panel
from rich.progress import Progress, SpinnerColumn, TextColumn

console = Console()


def try_camera_with_timeout(index, output_path, timeout=3.0):
    """Try to capture from a camera with timeout"""
    result = {'success': False, 'info': None, 'error': None}
    
    def capture():
        try:
            cap = cv2.VideoCapture(index)
            if not cap.isOpened():
                result['error'] = 'Failed to open'
                return
            
            # Drop first 10 frames to let camera initialize
            for _ in range(10):
                cap.read()
            
            ret, frame = cap.read()
            if ret and frame is not None:
                width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
                height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
                fps = cap.get(cv2.CAP_PROP_FPS)
                
                output_file = output_path / f"camera_{index}.jpg"
                cv2.imwrite(str(output_file), frame)
                
                result['success'] = True
                result['info'] = {
                    'width': width,
                    'height': height,
                    'fps': fps,
                    'file': str(output_file)
                }
            else:
                result['error'] = 'Failed to read frame'
            
            cap.release()
        except Exception as e:
            result['error'] = str(e)
    
    thread = Thread(target=capture)
    thread.daemon = True
    thread.start()
    thread.join(timeout=timeout)
    
    if thread.is_alive():
        return {'success': False, 'info': None, 'error': 'Timeout'}
    
    return result


def find_camera_indexes(max_index=10, output_folder="camera_frames", timeout=3.0):
    """
    Find all available camera indexes and save a frame from each.
    
    Args:
        max_index: Maximum camera index to check (default: 10)
        output_folder: Folder to save the captured frames
        timeout: Timeout in seconds for each camera (default: 3.0)
    """
    # Create output folder if it doesn't exist
    output_path = Path(output_folder)
    output_path.mkdir(parents=True, exist_ok=True)

    console.print(Panel.fit(f"[bold cyan]Scanning for cameras[/bold cyan] (indexes 0 to {max_index})", border_style="cyan"))

    available_cameras = []
    camera_infos = []

    # Suppress OpenCV warnings
    os.environ["OPENCV_LOG_LEVEL"] = "SILENT"

    with Progress(SpinnerColumn(), TextColumn("[progress.description]{task.description}"), console=console) as progress:
        task = progress.add_task("[cyan]Checking cameras...", total=max_index + 1)
        for index in range(max_index + 1):
            progress.update(task, description=f"[cyan]Checking camera {index}...")
            result = try_camera_with_timeout(index, output_path, timeout)
            if result['success']:
                info = result['info']
                available_cameras.append(index)
                camera_infos.append({
                    'index': index,
                    'width': info['width'],
                    'height': info['height'],
                    'fps': info['fps'],
                    'file': info['file']
                })
                progress.console.print(f"[green]✓ Camera {index}: Found[/green] [dim]({info['width']}x{info['height']} @ {info['fps']:.2f} FPS)[/dim]")
            elif result['error'] == 'Timeout':
                progress.console.print(f"[yellow]✗ Camera {index}: Timeout (skipped)[/yellow]")
            elif index < 5:
                progress.console.print(f"[red]✗ Camera {index}: {result['error']}[/red]")
            progress.advance(task)

    console.print("\n[bold]Summary:[/bold]")
    if camera_infos:
        table = Table(title="Detected Cameras", show_header=True, header_style="bold magenta")
        table.add_column("Index", style="cyan", justify="center")
        table.add_column("Resolution", style="yellow")
        table.add_column("FPS", style="green")
        table.add_column("Saved Frame", style="blue")
        for info in camera_infos:
            table.add_row(
                str(info['index']),
                f"{info['width']}x{info['height']}",
                f"{info['fps']:.2f}",
                info['file']
            )
        console.print(table)
        console.print(f"[bold green]✓ Found {len(available_cameras)} available camera(s): {available_cameras}[/bold green]")
        console.print(f"[bold blue]📁 Frames saved to: {output_path.absolute()}[/bold blue]")
    else:
        console.print("[bold red]⚠ No cameras detected![/bold red]")
        console.print("[yellow]Make sure your cameras are connected and not in use by another application.[/yellow]")

    return available_cameras


if __name__ == "__main__":
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Find available camera indexes and save sample frames"
    )
    parser.add_argument(
        "--max-index",
        type=int,
        default=10,
        help="Maximum camera index to check (default: 10)"
    )
    parser.add_argument(
        "--output",
        type=str,
        default="camera_frames",
        help="Output folder for captured frames (default: camera_frames)"
    )
    
    args = parser.parse_args()
    
    cameras = find_camera_indexes(
        max_index=args.max_index,
        output_folder=args.output
    )
    
    # Exit with success code only if cameras were found
    exit(0 if len(cameras) > 0 else 1)
    parser.add_argument(
        "--timeout",
        type=float,
        default=3.0,
        help="Timeout in seconds for each camera check (default: 3.0)"
    )
    
    args = parser.parse_args()
    
    cameras = find_camera_indexes(
        max_index=args.max_index,
        output_folder=args.output,
        timeout=args.timeout
    )
