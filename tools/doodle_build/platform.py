
from enum import Enum
from typing import List

from tools.tool_utils import DoodleModuleType

class DoodleBuildPlatformType(Enum):
    STANDARD_BUILD = 1  # for just using the method desscribed in the platform
    CUSTOM_BUILD = 2  # for using a custom build script, using python


class DoodleBuildPlatformBuildInfo:
    def __init__(self, build_src: str, build_type: DoodleBuildPlatformType):
        self.build_src = build_src
        self.build_type = build_type

class DoodleBuildPlatformInfo:
    def __init__(self, name: str, version: str, main: str, modules: List[DoodleModuleType]):
        self.name = name
        self.version = version
        self.main = main
        self.modules = modules

class DoodleBuildPlatform:
    def __init__(self, build_info: DoodleBuildPlatformBuildInfo, platform_info: DoodleBuildPlatformInfo):
        self.build_info = build_info
        self.platform_info = platform_info

    def build_platform(self, project_path: str, project_name: str):
        if self.build_info.type == DoodleBuildPlatformType.STANDARD_BUILD:
            self.__build_platform_standard(project_path, project_name)
        elif self.build_info.type == DoodleBuildPlatformType.CUSTOM_BUILD:
            self.__build_platform_custom(project_path, project_name)

    def __build_platform_standard(self, project_path: str, project_name: str):
        # the standard build method is to just run the build script
        # the build script expects:
        # <PROJECT_NAME> <PLATFORM_NAME> <PROJECT_DIR> <OUTPUT_DIR>
        pass
        

    
    def __build_platform_custom(self, project_path: str, project_name: str):
        # there should be a build script in the platform directory
        # called doodle.py, which should be a python script which
        # builds the project

        # the build script expects:
        # <PROJECT_NAME> [platform_name] [project_dir]
        command = (
            f"python {self.path}/doodle.py {project_name} {self.name} {project_path}"
        )