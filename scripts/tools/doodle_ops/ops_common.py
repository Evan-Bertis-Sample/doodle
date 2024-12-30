# ops_common.py
# Common operations for doodle operation tools, such as building and running

import os
import argparse
from tools.doodle_ops.build_util import DoodleBuildUtil

class DoodleOpsCommon:
    @staticmethod
    def register_subparser(subparser : argparse.ArgumentParser):
        # requires the platform name and directory
        # optionally, you can specify the project name, but that only affects the name of the output file
        # by default, the project name is the same as the directory name
        subparser.add_argument("platform_name", type=str, help="The name of the platform to build")
        subparser.add_argument("project_dir", type=str, help="The directory of the project to build")
        subparser.add_argument("--project_name", type=str, help="The name of the project to build")

    @staticmethod
    def clean_args(args):
        if args.project_name is None:
            if args.project_dir == ".":
                # if the project name is not specified, use the directory name
                cwd = os.getcwd()
                args.project_name = os.path.basename(cwd)
            else:
                args.project_name = args.project_dir

        return args

    @staticmethod
    def find_platform_protocol(platform_name : str):
        platforms = DoodleBuildUtil.get_platforms()

        print("Found platforms:")
        for name, platform in platforms.items():
            print(f"{name} ({platform.build_info.build_type.name})")

        print("")

        platform_name = platform_name.lower()
        platform = platforms[platform_name]
        print(f"Selected platform: {platform_name}")
        return platform