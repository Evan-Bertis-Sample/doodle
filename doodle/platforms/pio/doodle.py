# pio/build.py

import os
from enum import Enum
import argparse


def get_pio_ini(directory: str):
    # expects a directory
    # returns the pio.ini file in the directory
    for file in os.listdir(directory):
        if file == "platformIO.ini":
            return file

    return None

def build(project_path : str, project_name : str, build_dir : str):
    # literally copy-paste doodle into pio
    # this is a placeholder
    print(f"Building project {project_name} at {project_path} using platformIO")