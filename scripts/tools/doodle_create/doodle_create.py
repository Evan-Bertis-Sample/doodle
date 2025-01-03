# doodle_create.py

import os
from tools.tool_utils import DoodleToolUtil
import shutil
import json

TEMPLATE_MAP = {
    "project": "project_template",
    "platform": "platform_template",
}

def create_cpp_properties_object():
    # basically just add absolute paths to the include directories
    # that way vscode can find the headers
    doodle_includes = []
    doodle_includes.append(DoodleToolUtil.get_doodle_dir())

    obj = {}
    obj["configurations"] = [
        {
            "name": "doodle",
            "includePath": doodle_includes,
            "defines": [],
        }
    ]
    return obj


def register_subparser(parser):
    # type arg, required, is it a project or a platform?
    parser.add_argument(
        "type",
        help="The type of the module to create",
        choices=["project", "platform"],
    )
    # name arg, required, name of the module
    parser.add_argument("name", help="The name of the item to create")

    # if the type is project, add a path argument
    parser.add_argument(
        "--path",
        help="The path to the project",
        default=".",
    )

def main(args):
    type = args.type
    name = args.name

    path = ""
    if type == "project":
        path = args.path
    else:
        path = DoodleToolUtil.get_doodle_platforms_dir()

    path = path + os.path.sep + name

    # now copy the template to the new location
    template_name = TEMPLATE_MAP.get(type, None)
    if template_name == None:
        print(f"Error: No template found for type {type}")
        return
    
    # now copy the template to the new location
    # the path is relative to the location of this file
    template_path = os.path.join(os.path.dirname(__file__), template_name)
    if not os.path.exists(template_path):
        print(f"Error: The template {template_path} does not exist")
        return
    
    # copy the template to the new location
    print(f"Copying template from {template_path} to {path}")
    os.makedirs(path, exist_ok=True)

    # print all the files in the template
    for root, dirs, files in os.walk(template_path):
        for file in files:
            print(f"Copying file: {file}")

        for dir in dirs:
            print(f"Copying directory: {dir}")

    # now copy all the files and directories to the new location
    shutil.copytree(template_path, path, dirs_exist_ok=True)

    # now write the cpp_properties.json file
    cpp_properties_path = os.path.join(path, ".vscode")
    os.makedirs(cpp_properties_path, exist_ok=True)
    cpp_properties_file = os.path.join(cpp_properties_path, "c_cpp_properties.json")
    cpp_properties_object = create_cpp_properties_object()
    with open(cpp_properties_file, "w") as f:
        f.write(json.dumps(cpp_properties_object, indent=4))
    



