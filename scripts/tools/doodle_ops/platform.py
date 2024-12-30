from enum import Enum
from typing import List
import os
import sys
import shutil

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

    def build_platform(self, project_path: str, project_name: str, debug_mode: bool):
        if self.build_info.build_type == DoodleBuildPlatformType.CMAKE_BUILD:
            self.__build_platform_cmake(project_path, project_name, debug_mode)
        elif self.build_info.build_type == DoodleBuildPlatformType.PYTHON_BUILD:
            self.__build_platform_custom(project_path, project_name, debug_mode)

    def run_platform(self, project_path: str, project_name: str):
        if self.build_info.build_type == DoodleBuildPlatformType.CMAKE_BUILD:
            self.__run_platform_cmake(project_path, project_name)
        elif self.build_info.build_type == DoodleBuildPlatformType.PYTHON_BUILD:
            self.__run_platform_custom(project_path, project_name)

    def clean_platform(self, project_path: str, project_name: str):
        # find the build directory and remove it
        build_dir = DoodleBuildPlatform.get_build_dir(self.platform_info.name, project_path)
        if os.path.exists(build_dir):
            print(f"Cleaning build directory {build_dir}")
            try:
                subprocess.run(["rm", "-rf", build_dir], check=True)
            except subprocess.CalledProcessError as e:
                print(f"Error cleaning build directory: {e}")
                return
        else:
            print(f"Build directory {build_dir} does not exist, nothing to clean)")

    def __build_platform_cmake(self, project_path: str, project_name: str, debug_mode: bool):
        # print("Doodle Build System (PYTHON BUILD)")

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
        # print(f"Building project {project_name} in {project_dir} with platform {platform_name}")
        # print("Relative project dir: ", rel_project_dir)

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
            # debug mode
            f"-DCMAKE_BUILD_TYPE={'Debug' if debug_mode else 'Release'}",
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
            cli_width = shutil.get_terminal_size().columns

            print("")
            print("=" * cli_width)
            print("")

            print(f"Error building project: {e}")
            return

        print(f"Build complete. The executable should be in {build_dir}")

    def __run_platform_cmake(self, project_path: str, project_name: str):
        # print("Doodle Build System (PYTHON BUILD)")

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
        # print(f"Running project {project_name} in {project_dir} with platform {platform_name}")
        # print("Relative project dir: ", rel_project_dir)

        work_dir = DoodleToolUtil.get_doodle_parent_dir()

        # 4. Run the project
        run_command = [
            os.path.join(build_dir, project_name)
        ]
        try:
            subprocess.run(run_command, check=True, cwd=work_dir)
        except subprocess.CalledProcessError as e:
            cli_width = shutil.get_terminal_size().columns

            print("")
            print("=" * cli_width)
            print("")

            print(f"Error running project: {e}")
            return


    def __get_custom_module(build_script_path : str):
        # Load the external Python file as a module
        spec = importlib.util.spec_from_file_location("custom_build_script", build_script_path)
        if spec is None:
            print(f"Error: Unable to create import spec for {build_script_path}")
            return None

        module = importlib.util.module_from_spec(spec)
        try:
            spec.loader.exec_module(module)
        except Exception as e:
            print(f"Error: Failed to load or execute {build_script_path}:\n{e}")
            return None
        
        return module
        

    def __build_platform_custom(self, project_path: str, project_name: str, debug_mode: bool):
        # print("Building project with custom build script")
        build_dir = DoodleBuildPlatform.get_build_dir(self.platform_info.name, project_path)
        # script should be at
        # <doodle_platforms_dir>/<platform_name>/doodle.py
        build_script = os.path.join(
            DoodleToolUtil.get_doodle_platforms_dir(),
            self.platform_info.name,
            self.build_info.build_src,
        )

        if not os.path.exists(build_script):
            print(f"Error: Build script not found at {build_script}")
            return
        
        module = DoodleBuildPlatform.__get_custom_module(build_script)

        # Call build(...)
        try:
            module.build(
                project_path,
                project_name,
                build_dir,
                debug_mode
            )
        except Exception as e:
            print(f"Error: 'build' function in {build_script} raised an exception:\n{e}")
            return
        

    def __run_platform_custom(self, project_path: str, project_name: str):
        # we should pass in the project path, project name, and project root dir, and the output dir
        # to the script
        # the script must include a "build" function that takes in these parameters
        # and a "run" function that runs/uploads the project
        # print("Doodle Build System (CUSTOM BUILD)")
        # print("Running project with custom build script")
        build_dir = DoodleBuildPlatform.get_build_dir(self.platform_info.name, project_path)
        # script should be at
        # <doodle_platforms_dir>/<platform_name>/doodle.py
        build_script = os.path.join(
            DoodleToolUtil.get_doodle_platforms_dir(),
            self.platform_info.name,
            self.build_info.build_src,
        )

        if not os.path.exists(build_script):
            print(f"Error: Build script not found at {build_script}")
            return
        
        module = DoodleBuildPlatform.__get_custom_module(build_script)

        # Call run(...)
        try:
            module.run(
                project_path,
                project_name,
                build_dir
            )
        except Exception as e:
            print(f"Error: 'run' function in {build_script} raised an exception:\n{e}")
            return
        
    def __hash__(self):
        return hash((self.build_info, self.platform_info))
    
    def __str__(self):
        return f"Platform: {self.platform_info.name} ({self.build_info.build_type.name})"
        

