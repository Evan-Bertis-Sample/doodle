# pio/build.py

import os
import shutil
import subprocess

from tools.tool_utils import DoodleToolUtil

MAIN_FILE = os.path.join(os.path.dirname(__file__), "pio_main.c")



def build(project_path: str, project_name: str, output_dir: str):
    # literally copy-paste doodle into pio
    # this is a placeholder
    print(f"Building project {project_name} at {project_path} using platformIO")

    # lets see if there is a PlatformIO project ini file in the project directory
    # if not, we need to alert the user
    platformio_ini = os.path.join(project_path, "platformio.ini")
    

    if not os.path.exists(platformio_ini):
        print("Error: No platformio.ini file found in the project directory")
        return
    
    print("Building PlatformIO project from doodle project at ", project_path)

    # create the project directory
    pio_project_dir = output_dir
    os.makedirs(pio_project_dir, exist_ok=True)

    # erase everything in the project directory to make sure it is clean
    for root, dirs, files in os.walk(pio_project_dir):
        for file in files:
            os.remove(os.path.join(root, file))

        for dir in dirs:
            shutil.rmtree(os.path.join(root, dir))


    # copy the platformio.ini file to the project directory
    pio_platformio_ini = os.path.join(pio_project_dir, "platformio.ini")
    shutil.copyfile(platformio_ini, pio_platformio_ini)

    # create all the necessary directories in the project directory
    os.makedirs(os.path.join(pio_project_dir, "src"), exist_ok=True)
    os.makedirs(os.path.join(pio_project_dir, "include"), exist_ok=True)
    os.makedirs(os.path.join(pio_project_dir, "lib"), exist_ok=True)

    # if there is an include/src directory in the project directory, copy it to the pio project directory
    src_dir = os.path.join(project_path, "src")
    include_dir = os.path.join(project_path, "include")
    lib_dir = os.path.join(project_path, "lib")

    if os.path.exists(src_dir):
        shutil.copytree(
            src_dir, os.path.join(pio_project_dir, "src"), dirs_exist_ok=True
        )

    if os.path.exists(include_dir):
        shutil.copytree(
            include_dir, os.path.join(pio_project_dir, "include"), dirs_exist_ok=True
        )

    if os.path.exists(lib_dir):
        shutil.copytree(
            lib_dir, os.path.join(pio_project_dir, "lib"), dirs_exist_ok=True
        )

    # now lets copy over doodle files into the lib directory
    doodle_core_dir = os.path.join(DoodleToolUtil.get_doodle_dir(), "core")
    doodle_pio_platform_dir = os.path.join(DoodleToolUtil.get_doodle_platforms_dir(), "pio")
    doodle_dest_dir = os.path.join(pio_project_dir, "include", "doodle")


    if not os.path.exists(doodle_core_dir):
        print("Error: Doodle core directory not found")
        return
    
    if not os.path.exists(doodle_pio_platform_dir):
        print("Error: Doodle pio platform directory not found")
        return
    
    shutil.copytree(
        doodle_core_dir, os.path.join(doodle_dest_dir, "core"), dirs_exist_ok=True
    )

    shutil.copytree(
        doodle_pio_platform_dir, os.path.join(doodle_dest_dir, "platforms", "pio"), dirs_exist_ok=True
    )

    # now copy the main file to the src directory
    main_file = os.path.join(pio_project_dir, "src", "main.c")
    shutil.copyfile(MAIN_FILE, main_file)

    # now run the platformio build command
    # change the directory to the project directory
    command = "pio run"

    old_dir = os.getcwd()
    os.chdir(pio_project_dir)
    os.system(command)
    os.chdir(old_dir)

    print("Build complete")


def run(project_path: str, project_name: str, output_dir: str):
    # literally copy-paste doodle into pio
    # this is a placeholder
    print(f"Running project {project_name} at {project_path} using platformIO")
