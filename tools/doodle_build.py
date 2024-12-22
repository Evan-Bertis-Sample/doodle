# doodle_build.py

import os
from enum import Enum
import argparse
import subprocess
import inifile

DOODLE_ROOT = "doodle"
DOODLE_PLATFORM_DIR = DOODLE_ROOT + "/platforms"
DOODLE_BUILD_BASH_SCRIPT = "doodle_run.sh"
BUILD_OUTPUT_DIR = "build"


class DoodleBuildPlatformType(Enum):
    STANDARD_BUILD = 1  # for just using the method desscribed in the platform
    CUSTOM_BUILD = 2  # for using a custom build script, using python


class DoodleBuildPlatform:
    def __init__(self, name: str, path: str, type: DoodleBuildPlatformType):
        self.name = name
        self.path = path
        self.type = type

    def get_output_dir(self, project_name: str):
        # get where this file was executed from
        current_dir = os.getcwd()
        return (
            current_dir
            + "/"
            + DOODLE_ROOT
            + "/"
            + project_name
            + "/"
            + BUILD_OUTPUT_DIR
            + "/"
            + self.name
        )

    def build_platform(self, project_path: str, project_name: str):
        if self.type == DoodleBuildPlatformType.STANDARD_BUILD:
            self.__build_platform_standard(project_path, project_name)
        elif self.type == DoodleBuildPlatformType.CUSTOM_BUILD:
            self.__build_platform_custom(project_path, project_name)

    def __build_platform_standard(self, project_path: str, project_name: str):
        # the standard build method is to just run the build script
        # the build script expects:
        # <PROJECT_NAME> <PLATFORM_NAME> <PROJECT_DIR> <OUTPUT_DIR>

        

    

    def __build_platform_custom(self, project_path: str, project_name: str):
        # there should be a build script in the platform directory
        # called doodle.py, which should be a python script which
        # builds the project

        # the build script expects:
        # <PROJECT_NAME> [platform_name] [project_dir]
        command = (
            f"python {self.path}/doodle.py {project_name} {self.name} {project_path}"
        )




def get_platforms():
    platforms = []
    for platform in os.listdir(DOODLE_PLATFORM_DIR):
        platform_path = DOODLE_PLATFORM_DIR + "/" + platform
        if os.path.isdir(platform_path):
            if os.path.exists(platform_path + "/doodle.py"):
                platforms.append(
                    DoodleBuildPlatform(
                        platform, platform_path, DoodleBuildPlatformType.CUSTOM_BUILD
                    )
                )
            else:
                platforms.append(
                    DoodleBuildPlatform(
                        platform, platform_path, DoodleBuildPlatformType.STANDARD_BUILD
                    )
                )

    return platforms


def register_subparser(subparser):
    subparser.add_argument("project_name", type=str, help="the name of the project")
    subparser.add_argument("platform_name", type=str, help="the name of the platform")
    subparser.add_argument("project_dir", type=str, help="the directory of the project")


def main(args):
    platform_name = args.platform_name
    project_name = args.project_name
    project_dir = args.project_dir

    # get the platforms
    platforms = get_platforms()

    print("Found platforms:")
    for p in platforms:
        print("  " + p.name + " (" + p.type.name + ")")

    print("")
    # find the platform
    platform = None
    for p in platforms:
        if p.name == args.platform_name:
            platform = p
            break

    if platform is None:
        print("Platform not found")
        return

    print(
        f"Building project {args.project_name} with platform {args.platform_name} using {platform.type.name} method"
    )

    platform.build_platform(args.project_dir, args.project_name)
