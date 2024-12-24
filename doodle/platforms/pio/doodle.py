# pio/build.py

import os
import shutil

MAIN_FILE = "pio_main.c"

def get_pio_project_dir(output_dir : str):
    return os.path.join(output_dir, "pio_project")

def build(project_path : str, project_name : str, output_dir : str):
    # literally copy-paste doodle into pio
    # this is a placeholder
    print(f"Building project {project_name} at {project_path} using platformIO")

    # lets see if there is a PlatformIO project ini file in the project directory
    # if not, we need to alert the user
    platformio_ini = os.path.join(project_path, "platformio.ini")

    if not os.path.exists(platformio_ini):
        print("Error: No platformio.ini file found in the project directory")
        return
    

    # create the project directory
    pio_project_dir = get_pio_project_dir(output_dir)
    os.makedirs(pio_project_dir, exist_ok=True)

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
        shutil.copytree(src_dir, os.path.join(pio_project_dir, "src"), dirs_exist_ok=True)
    
    if os.path.exists(include_dir):
        shutil.copytree(include_dir, os.path.join(pio_project_dir, "include"), dirs_exist_ok=True)

    if os.path.exists(lib_dir):
        shutil.copytree(lib_dir, os.path.join(pio_project_dir, "lib"), dirs_exist_ok=True)

    # now for all of the remaining files in the project directory, it should be safe to just copy them into source
    for root, dirs, files in os.walk(project_path):
        for file in files:
            if file == "platformio.ini":
                continue

            # copy the file
            shutil.copyfile(os.path.join(root, file), os.path.join(pio_project_dir, "src", file))

        for dir in dirs:
            if dir == "src" or dir == "include" or dir == "lib":
                continue

            # copy the directory
            shutil.copytree(os.path.join(root, dir), os.path.join(pio_project_dir, "src", dir), dirs_exist_ok=True)

    # now copy the main file to the src directory
    main_file = os.path.join(pio_project_dir, "src", "main.c")
    shutil.copyfile(MAIN_FILE, main_file)
    
    # now run the platformio build command
    os.system(f"platformio run -d {pio_project_dir}")

    print("Build complete")


def run(project_path : str, project_name : str, output_dir : str):
    # literally copy-paste doodle into pio
    # this is a placeholder
    print(f"Running project {project_name} at {project_path} using platformIO")