# build_util.py

import os
import subprocess
import inifile

from tools.tool_utils import DoodleToolUtil

from tools.build.platform import (
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
    def execute_cli_command(command: str):
        subprocess.run(command, shell=True)

    @staticmethod
    def build_platform_from_ini(ini_file_path: str) -> DoodleBuildPlatform:
        ini_file = inifile.IniFile(ini_file_path)
        # in the build section, there should be a type and build_src key
        build_section = ini_file.get_section("build")

        if build_section is None:
            return None

        build_src = build_section.get("build_src")
        build_type = DoodleToolUtil.get_doodle_module_type_from_str(
            build_section.get("type")
        )

        if build_type is None:
            return None

        if build_src is None or build_src == "":
            # default to doodle.py or doodle.cmake
            if build_type == DoodleBuildPlatformType.CUSTOM_BUILD:
                build_src = "doodle.py"
            else:
                build_src = "doodle.cmake"

        # now we should get the supported modules, platform name, version and main
        platform_section = ini_file.get_section("platform")
        if platform_section is None:
            return None

        platform_name = platform_section.get("platform")
        platform_version = platform_section.get("version") or "N/A"
        platform_main = platform_section.get("main") or "main.c"
        platform_modules_raw = platform_section.get("modules") or ""
        platform_modules = platform_modules_raw.split(",")
        platform_modules = [
            DoodleToolUtil.get_doodle_module_type_from_str(m) for m in platform_modules
        ]
        # remove any None values
        platform_modules = [m for m in platform_modules if m is not None]

        return DoodleBuildPlatform(
            DoodleBuildPlatformBuildInfo(build_src, build_type),
            DoodleBuildPlatformInfo(
                platform_name, platform_version, platform_main, platform_modules
            ),
        )

    @staticmethod
    def get_platforms() -> dict[str, DoodleBuildPlatform]:
        platforms = {}
        for platform in os.listdir(DoodleToolUtil.get_doodle_dir() / "platforms"):
            platform_ini = DoodleToolUtil.get_doodle_dir() / "platforms" / platform / "doodle_platform.ini"
            if not platform_ini.exists():
                continue

            platform = DoodleBuildUtil.build_platform_from_ini(platform_ini)
            platforms[platform.name] = platform
        return platforms
