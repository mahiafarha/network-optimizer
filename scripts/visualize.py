import sys
import os
import subprocess
from pathlib import Path
import shutil

def find_dot_binary():
    # 1. Check if dot is already in PATH
    dot_cmd = shutil.which("dot")
    if dot_cmd:
        return dot_cmd

    # 2. Check standard Windows installation directories
    candidate_paths = [
        r"C:\Program Files\Graphviz\bin\dot.exe",
        r"C:\Program Files (x86)\Graphviz\bin\dot.exe",
        os.path.expandvars(r"%LOCALAPPDATA%\Programs\Graphviz\bin\dot.exe"),
        r"C:\msys64\ucrt64\bin\dot.exe",
        r"C:\msys64\mingw64\bin\dot.exe"
    ]

    for p in candidate_paths:
        if os.path.exists(p):
            return p

    return None

def render_graph(dot_filename: str = "network_route.dot", output_format: str = "png") -> None:
    dot_path = Path(dot_filename)
    if not dot_path.is_file():
        print(f"Error: Could not find '{dot_filename}'. Run the C++ optimizer first!")
        return

    dot_bin = find_dot_binary()
    if not dot_bin:
        print("Error: Could not locate 'dot.exe'.")
        return

    out_file = dot_path.with_suffix(f".{output_format}")
    cmd = [dot_bin, f"-T{output_format}", str(dot_path), "-o", str(out_file)]

    try:
        subprocess.run(cmd, check=True)
        print(f"Success! Rendered graph to: {out_file.resolve()}")
    except subprocess.CalledProcessError as e:
        print(f"Graphviz rendering failed with error: {e}")

if __name__ == "__main__":
    target = sys.argv[1] if len(sys.argv) > 1 else "network_route.dot"
    render_graph(target)