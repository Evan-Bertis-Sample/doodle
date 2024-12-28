# doodle_run.py

from tools.doodle_ops.build_util import DoodleBuildUtil
from tools.doodle_ops.ops_common import DoodleOpsCommon

def register_subparser(subparser):
    DoodleOpsCommon.register_subparser(subparser)


def main(args):
    args = DoodleOpsCommon.clean_args(args)

    platform = DoodleOpsCommon.find_platform_protocol(args.platform_name)

    if platform is None:
        print("Platform not found")
        return

    print(
        f"Building project {args.project_name} with platform {args.platform_name} using {platform.build_info.build_type.name} method"
    )

    platform.run_platform(args.project_dir, args.project_name)
