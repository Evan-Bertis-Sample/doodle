from enum import Enum
from typing import List
import os

from tools.tool_utils import DoodleModuleType, DoodleToolUtil
import subprocess
import sys

DOODLE_BUILD_BASH_SCRIPT = "doodle_build/doodle_run.sh"


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
    def get_build_dir(platform_name: str, project_name: str):
        work_dir = DoodleToolUtil.get_doodle_work_dir()
        build_dir = os.path.join(work_dir, "build", project_name, platform_name)
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
        build_dir = DoodleBuildPlatform.get_build_dir(platform_name, project_name)

        rel_project_dir = os.path.relpath(project_dir, DoodleToolUtil.get_doodle_parent_dir())
        print(f"Building project {project_name} in {project_dir} with platform {platform_name}")
        print("Rel project dir: ", rel_project_dir)

        # replace backslashes with forward slashes for CMake
        rel_project_dir = rel_project_dir.replace("\\", "/")

        # 4. Run CMake configure
        cmake_command = [
            "cmake",
            "-G", "Unix Makefiles",
            "-S", ".",  # Assume the current directory is the root CMakeLists.txt
            "-B", build_dir,
            f"-DPLATFORM_NAME={platform_name}",
            f"-DPROJECT={project_name}",
            f"-DPROJECT_CMAKE_DIR={rel_project_dir}",
            f"-DPLATFORM_MAIN_FILE={self.platform_info.main}",
        ]
        try:
            subprocess.run(cmake_command, check=True)
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
            subprocess.run(build_command, check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error building project: {e}")
            return

        print(f"Build complete. The executable should be in {build_dir}")
        print("Running the executable...")
        print("-----------------------------------\n")

        # 6. Run the generated executable. 
        #    On Windows, you may have a .exe. On Unix-like systems, you often won't.
        #    Adjust accordingly if your platform doesn't use ".exe".
        exe_path = os.path.join(build_dir, f"{project_name}.exe")
        print(f"Running executable: {exe_path}")
        try:
            subprocess.run([exe_path], check=True)
        except FileNotFoundError:
            print(f"Executable not found: {exe_path}")
        except subprocess.CalledProcessError as e:
            print(f"Error running executable: {e}")

    def __build_platform_custom(self, project_path: str, project_name: str):
        # there should be a build script in the platform directory
        # called doodle.py, which should be a python script which
        # builds the project

        # the build script expects:
        # <PROJECT_NAME> [platform_name] [project_dir]
        command = (
            f"python {self.path}/doodle.py {project_name} {self.name} {project_path}"
        )
