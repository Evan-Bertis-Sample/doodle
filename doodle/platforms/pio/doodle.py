# pio/build.py

import os
from enum import Enum
import argparse

def get_pio_ini(directory : str):
    # expects a directory
    # returns the pio.ini file in the directory
    for file in os.listdir(directory):
        if file == "platformIO.ini":
            return file

    return None

def main():
    # expects <PROJECT_NAME> [platform_name] [project_dir]
    parser = argparse.ArgumentParser(description='Build a project')
    parser.add_argument('project_name', type=str, help='the name of the project')
    parser.add_argument('platform_name', type=str, help='the name of the platform')
    parser.add_argument('project_dir', type=str, help='the directory of the project')
    args = parser.parse_args()

    # we are essentially going to build a PlatformIO project, then compile it
    # the project directory is the directory of the project
    ini_file = get_pio_ini(args.project_dir)





if __name__ == "__main__":
    main()