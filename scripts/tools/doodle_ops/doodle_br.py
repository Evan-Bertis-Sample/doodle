# doodle_br.py

import shutil
from tools.doodle_ops.build_util import DoodleBuildUtil
from tools.doodle_ops.ops_common import DoodleOpsCommon

def register_subparser(subparser):
    DoodleOpsCommon.register_subparser(subparser)
    # add a debug flag to the build command
    subparser.add_argument("--debug", action="store_true", help="Build in debug mode")

def main(args):
    args = DoodleOpsCommon.clean_args(args)
    platform = DoodleOpsCommon.find_platform_protocol(args.platform_name)

    if platform is None:
        print("Platform not found")
        return

    print(
        f"Building project {args.project_name} with platform {args.platform_name} using {platform.build_info.build_type.name} method"
    )

    debug_mode = args.debug
    platform.build_platform(args.project_dir, args.project_name, debug_mode)

    print("")

    # get the width of the terminal
    terminal_width = shutil.get_terminal_size().columns

    print("=" * terminal_width)
    print("")

    print(
        f"Running project {args.project_name} with platform {args.platform_name} using {platform.build_info.build_type.name} method"
    )

    platform.run_platform(args.project_dir, args.project_name)