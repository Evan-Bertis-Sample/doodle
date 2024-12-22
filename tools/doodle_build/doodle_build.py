# doodle_build.py

import os
from enum import Enum
import argparse
import subprocess

from tools.build.build_util import DoodleBuildUtil

DOODLE_BUILD_BASH_SCRIPT = "doodle_run.sh"



def register_subparser(subparser):
    subparser.add_argument("project_name", type=str, help="the name of the project")
    subparser.add_argument("platform_name", type=str, help="the name of the platform")
    subparser.add_argument("project_dir", type=str, help="the directory of the project")


def main(args):
    platform_name = args.platform_name
    project_name = args.project_name
    project_dir = args.project_dir

    # get the platforms
    platforms = DoodleBuildUtil.get_platforms()

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
