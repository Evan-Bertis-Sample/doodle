# build_util.py

import os
import subprocess
import configparser

from tools.tool_utils import DoodleToolUtil

from tools.doodle_build.platform import (
    DoodleBuildPlatform,
    DoodleBuildPlatformType,
    DoodleBuildPlatformBuildInfo,
    DoodleBuildPlatformInfo,
)
from tools.tool_utils import DoodleToolUtil


class DoodleBuildUtil:
    @staticmethod
    def get_build_output_dir(platform_name: str, project_name: str):
        work_dir = DoodleBuildUtil.get_doodle_work_dir()
        return work_dir / "build" / project_name / platform_name
    @staticmethod
    def build_platform_from_ini(ini_file_path: str) -> DoodleBuildPlatform:
        ini_file = configparser.ConfigParser()
        ini_file.read(ini_file_path)
        # in the build section, there should be a type and build_src key
        if not ini_file.has_section("build"):
            return None

        build_src = ini_file.get("build", "build_src")
        build_type = DoodleBuildPlatform.get_build_type_from_str(
            ini_file.get("build", "type")
        )

        if build_type is None:
            return None

        if build_src is None or build_src == "":
            # default to doodle.py or doodle.cmake
            if build_type == DoodleBuildPlatformType.PYTHON_BUILD:
                build_src = "doodle.py"
            else:
                build_src = "doodle.cmake"

        # now we should get the supported modules, platform name, version and main
        if not ini_file.has_section("platform"):
            return None
        
        # get the direcotory name of the ini file, which should be the platform name
        platform_name = os.path.basename(os.path.dirname(ini_file_path))
        platform_version = ini_file.get("platform", "version")
        platform_main = ini_file.get("platform", "main")
        platform_modules_raw = ini_file.get("platform", "modules")
        platform_modules = platform_modules_raw.split(",")
        platform_modules = [
            DoodleToolUtil.get_doodle_module_type_from_str(m) for m in platform_modules
        ]
        # remove any None values
        platform_modules = [m for m in platform_modules if m is not None]

        if platform_name is None or platform_name == "":
            return None
    

        return DoodleBuildPlatform(
            DoodleBuildPlatformBuildInfo(build_src, build_type),
            DoodleBuildPlatformInfo(
                platform_name, platform_version, platform_main, platform_modules
            ),
        )

    @staticmethod
    def get_platforms() -> dict[str, DoodleBuildPlatform]:
        platforms = {}
        for platform in os.listdir(DoodleToolUtil.get_doodle_dir() + os.path.sep + "platforms"):
            platform_ini = os.path.sep.join([DoodleToolUtil.get_doodle_dir(), "platforms", platform, "doodle_platform.ini"])
            if not os.path.exists(platform_ini):
                continue

            platform = DoodleBuildUtil.build_platform_from_ini(platform_ini)

            if platform is None:
                continue

            platforms[platform.platform_info.name] = platform
        return platforms
