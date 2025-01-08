# doodle_index.py
# regenerates the doodle c_cpp_properties.json file to allow for intellisense to work

import os
import shutil
import json
from tools.tool_utils import DoodleToolUtil

CPP_PROPERTIES_PATH = ".vscode/c_cpp_properties.json"

def create_base_cpp_properties_object():
    doodle_includes = []
    doodle_includes.append(DoodleToolUtil.get_doodle_parent_dir())

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
    parser.add_argument(
        "project_dir",
        help="The directory of the project",
    )


def main(args):
    print("Regenerating c_cpp_properties.json file")

    cpp_properties_file = os.path.join(args.project_dir, CPP_PROPERTIES_PATH)
    os.makedirs(os.path.dirname(cpp_properties_file), exist_ok=True)

    with open(cpp_properties_file, "w") as f:
        properties = None
        try:
            properties = json.load(f)
        except Exception as e:
            print("Error: Could not load c_cpp_properties.json file")
            print(e)

            # now query the user if they just want to overwrite the file
            while (True):
                choice = input("Do you want to overwrite the file? [y/n]: ")
                if choice.lower() == "y":
                    break
                elif choice.lower() == "n":
                    return
                else:
                    print("Invalid choice")

        if properties == None:
            properties = create_base_cpp_properties_object()
        else:
            additional_properties = create_base_cpp_properties_object()
            if "configurations" in properties:
                properties["configurations"].extend(additional_properties["configurations"])
            else:
                properties["configurations"] = additional_properties["configurations"]

        f.write(json.dumps(properties, indent=4))
        
    print("Regenerated c_cpp_properties.json file, which should allow for intellisense to work")
    print("Please refresh VSCode to see the changes, if you don't see them immediately")





