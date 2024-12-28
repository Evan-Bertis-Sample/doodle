from enum import Enum
from typing import List
import os
import sys

from tools.tool_utils import DoodleModuleType, DoodleToolUtil
import subprocess
import importlib

class DoodleBuildPlatformType(Enum):
    CMAKE_BUILD = 1  # for just using the method desscribed in the platform
    PYTHON_BUILD = 2  # for using a custom build script, using python


class DoodleBuildPlatformBuildInfo:
    def __init__(self, build_src: str, build_type: DoodleBuildPlatformType):
        self.build_src = build_src
        self.build_type = build_type


class DoodleBuildPlatformInfo:
    def __init__(
        self, name: str, version: str, main: str, modules: List[DoodleModuleType]
    ):
        self.name = name
        self.version = version
        self.main = main
        self.modules = modules


class DoodleBuildPlatform:
    @staticmethod
    def get_build_type_from_str(build_type_str: str) -> DoodleBuildPlatformType:
        type_names = [e.name for e in DoodleBuildPlatformType]
        for t in type_names:
            if build_type_str.lower() in t.lower():
                return DoodleBuildPlatformType[t]

        return None
    
    @staticmethod
    def get_build_dir(platform_name: str, project_root_dir: str):
        work_dir = DoodleToolUtil.get_doodle_work_dir(project_root_dir)
        build_dir = os.path.join(work_dir, "build", platform_name)
        # if it doesn't exist, create it
        os.makedirs(build_dir, exist_ok=True)
        return build_dir

    def __init__(
        self,
        build_info: DoodleBuildPlatformBuildInfo,
        platform_info: DoodleBuildPlatformInfo,
    ):
        self.build_info = build_info
        self.platform_info = platform_info

    def build_platform(self, project_path: str, project_name: str):
        if self.build_info.build_type == DoodleBuildPlatformType.CMAKE_BUILD:
            self.__build_platform_standard(project_path, project_name)
        elif self.build_info.build_type == DoodleBuildPlatformType.PYTHON_BUILD:
            self.__build_platform_custom(project_path, project_name)

    def __build_platform_standard(self, project_path: str, project_name: str):
        print("Doodle Build System (PYTHON BUILD)")

        # 1. Figure out platform_name (default = "native")
        platform_name = self.platform_info.name if self.platform_info.name else "native"
        if not self.platform_info.name:
            print(f"No platform specified, using default: {platform_name}")

        # 2. Figure out the project directory
        # Check if the project directory actually exists
        project_dir = os.path.abspath(project_path)
        if not os.path.exists(project_dir):
            print(f"Error: Project directory does not exist: {project_dir}")
            return

        # 3. Figure out build directory
        build_dir = DoodleBuildPlatform.get_build_dir(platform_name, project_dir)

        rel_project_dir = os.path.relpath(project_dir, DoodleToolUtil.get_doodle_parent_dir())
        print(f"Building project {project_name} in {project_dir} with platform {platform_name}")
        print("Relative project dir: ", rel_project_dir)

        # replace backslashes with forward slashes for CMake
        rel_project_dir = rel_project_dir.replace("\\", "/")

        work_dir = DoodleToolUtil.get_doodle_parent_dir()

        rel_build_src = os.path.join(
            DoodleToolUtil.get_doodle_platforms_dir(),
            platform_name,
            self.build_info.build_src
        )

        # 4. Run CMake configure
        cmake_command = [
            "cmake",
            "-G", "Unix Makefiles",
            "-S", ".",  # Assume the current directory is the root CMakeLists.txt
            "-B", build_dir,
            f"-DPLATFORM_NAME={platform_name}",
            f"-DPROJECT={project_name}",   
            f"-DPROJECT_CMAKE_DIR={rel_project_dir}",
            f"-DPLATFORM_CMAKE_FILE={rel_build_src}",
            f"-DPLATFORM_MAIN_FILE={self.platform_info.main}",
        ]
        try:
            subprocess.run(cmake_command, check=True, cwd=work_dir)
        except subprocess.CalledProcessError as e:
            print(f"Error configuring with CMake: {e}")
            return

        # 5. Run the build for the requested project target
        build_command = [
            "cmake",
            "--build", build_dir,
            "--target", project_name  
        ]
        try:
            subprocess.run(build_command, check=True, cwd=work_dir)
        except subprocess.CalledProcessError as e:
            print(f"Error building project: {e}")
            return

        print(f"Build complete. The executable should be in {build_dir}")
        print("Running the executable...")
        print("-----------------------------------\n")

        # # 6. Run the generated executable. 
        # #    On Windows, you may have a .exe. On Unix-like systems, you often won't.
        # #    Adjust accordingly if your platform doesn't use ".exe".
        # exe_path = os.path.join(build_dir, f"{project_name}.exe")
        # print(f"Running executable: {exe_path}")
        # try:
        #     subprocess.run([exe_path], check=True)
        # except FileNotFoundError:
        #     print(f"Executable not found: {exe_path}")
        # except subprocess.CalledProcessError as e:
        #     print(f"Error running executable: {e}")

    def __build_platform_custom(self, project_path: str, project_name: str):
        # we should pass in the project path, project name, and project root dir, and the output dir
        # to the script
        # the script must include a "build" function that takes in these parameters
        # and a "run" function that runs/uploads the project
        print("Doodle Build System (CUSTOM BUILD)")
        print("Building project with custom build script")
        build_dir = DoodleBuildPlatform.get_build_dir(self.platform_info.name, project_path)
        # script should be at
        # <doodle_platforms_dir>/<platform_name>/doodle.py
        build_script = os.path.join(
            DoodleToolUtil.get_doodle_platforms_dir(),
            self.platform_info.name,
            self.build_info.build_src,
        )

        # Load the external Python file as a module
        spec = importlib.util.spec_from_file_location("custom_build_script", build_script)
        if spec is None:
            print(f"Error: Unable to create import spec for {build_script}")
            return

        module = importlib.util.module_from_spec(spec)
        try:
            spec.loader.exec_module(module)
        except Exception as e:
            print(f"Error: Failed to load or execute {build_script}:\n{e}")
            return

        # The external script should define build(...) and run(...)
        # Adjust the parameter list to match your script's function signatures.
        if not hasattr(module, "build"):
            print("Error: The custom build script does not define a build(...) function.")
            return

        if not hasattr(module, "run"):
            print("Warning: The custom build script does not define a run(...) function.")

        # Call build(...)
        try:
            module.build(
                project_path,
                project_name,
                build_dir
            )
        except Exception as e:
            print(f"Error: 'build' function in {build_script} raised an exception:\n{e}")
            return

        # Call run(...)
        # If run() isn't required or might not exist, you can make this optional:
        if hasattr(module, "run"):
            try:
                module.run(
                    project_path,
                    project_name,
                    build_dir
                )
            except Exception as e:
                print(f"Error: 'run' function in {build_script} raised an exception:\n{e}")
        else:
            print("Skipping run(...) since it's not defined in the custom build script.")

        print("Custom build process complete.")
        

