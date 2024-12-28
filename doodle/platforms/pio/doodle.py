import os
import shutil
import subprocess

from tools.tool_utils import DoodleToolUtil

MAIN_FILE_PATH = os.path.join(os.path.dirname(__file__), "pio_main.c")


def build(project_src_path: str, project_name: str, build_output_dir: str):
    """
    Builds a PlatformIO project from a Doodle project.

    :param project_src_path: Path to the source Doodle project.
    :param project_name: Name of the project.
    :param build_output_dir: Directory where the PlatformIO project will be built.
    """
    print(f"Building project {project_name} at {project_src_path} using PlatformIO")

    # Check for a platformio.ini file in the source project directory
    platformio_ini_path = os.path.join(project_src_path, "platformio.ini")
    if not os.path.exists(platformio_ini_path):
        print("Error: No platformio.ini file found in the project directory")
        return

    print("Building PlatformIO project from Doodle project at", project_src_path)

    # Create or clean the build output directory
    os.makedirs(build_output_dir, exist_ok=True)
    for root, dirs, files in os.walk(build_output_dir):
        for f in files:
            os.remove(os.path.join(root, f))
        for d in dirs:
            shutil.rmtree(os.path.join(root, d))

    # Copy the platformio.ini into the build output directory
    build_platformio_ini_path = os.path.join(build_output_dir, "platformio.ini")
    shutil.copyfile(platformio_ini_path, build_platformio_ini_path)

    # Create required subdirectories
    os.makedirs(os.path.join(build_output_dir, "src"), exist_ok=True)
    os.makedirs(os.path.join(build_output_dir, "include"), exist_ok=True)
    os.makedirs(os.path.join(build_output_dir, "lib"), exist_ok=True)

    # Copy over src/include/lib from the source project if present
    src_path = os.path.join(project_src_path, "src")
    include_path = os.path.join(project_src_path, "include")
    lib_path = os.path.join(project_src_path, "lib")

    if os.path.exists(src_path):
        shutil.copytree(src_path, os.path.join(build_output_dir, "src"), dirs_exist_ok=True)
    if os.path.exists(include_path):
        shutil.copytree(include_path, os.path.join(build_output_dir, "include"), dirs_exist_ok=True)
    if os.path.exists(lib_path):
        shutil.copytree(lib_path, os.path.join(build_output_dir, "lib"), dirs_exist_ok=True)

    # Copy Doodle core/platform files
    doodle_core_dir_path = os.path.join(DoodleToolUtil.get_doodle_dir(), "core")
    doodle_pio_platform_dir_path = os.path.join(DoodleToolUtil.get_doodle_platforms_dir(), "pio")
    doodle_dest_dir_path = os.path.join(build_output_dir, "include", "doodle")

    if not os.path.exists(doodle_core_dir_path):
        print("Error: Doodle core directory not found")
        return
    if not os.path.exists(doodle_pio_platform_dir_path):
        print("Error: Doodle pio platform directory not found")
        return

    shutil.copytree(doodle_core_dir_path, os.path.join(doodle_dest_dir_path, "core"), dirs_exist_ok=True)
    doodle_pio_project_dir_path = os.path.join(doodle_dest_dir_path, "platforms", "pio")
    shutil.copytree(doodle_pio_platform_dir_path, doodle_pio_project_dir_path, dirs_exist_ok=True)

    # Remove the default pio_main.c (we'll replace it with our own)
    default_main_path = os.path.join(doodle_pio_project_dir_path, "pio_main.c")
    if os.path.exists(default_main_path):
        os.remove(default_main_path)

    # Remove Python files and __pycache__ directories
    for root, dirs, files in os.walk(doodle_pio_project_dir_path):
        for f in files:
            if f.endswith(".py"):
                os.remove(os.path.join(root, f))
        for d in dirs:
            if d == "__pycache__":
                shutil.rmtree(os.path.join(root, d))

    # Copy our main file to the src directory
    main_c_file_path = os.path.join(build_output_dir, "src", "main.cpp")
    shutil.copyfile(MAIN_FILE_PATH, main_c_file_path)

    # Run `pio run` in the build output directory
    build_cmd = "pio run"
    old_cwd = os.getcwd()
    os.chdir(build_output_dir)
    os.system(build_cmd)
    os.chdir(old_cwd)

    print("Build complete")


def run(project_src_path: str, project_name: str, build_output_dir: str):
    """
    Runs a PlatformIO project built from a Doodle project.

    :param project_src_path: Path to the source Doodle project.
    :param project_name: Name of the project.
    :param build_output_dir: Directory where the PlatformIO project resides.
    """
    print(f"Running project {project_name} at {project_src_path} using PlatformIO")
    # Placeholder for run logic
