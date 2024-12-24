# tool_utils.py

import os
import enum
import subprocess


class DoodleModuleType(enum.Enum):
    INPUT = 1
    OUTPUT = 2
    RENDERER = 3
    AUDIO = 4
    FILESYSTEM = 5


class DoodleToolUtil:
    @staticmethod
    def get_doodle_module_types_str_list():
        return [e.name for e in DoodleModuleType]

    @staticmethod
    def get_doodle_dir():
        # get where this file was executed
        current_dir = os.getcwd()
        return current_dir + os.path.sep + "doodle"

    @staticmethod
    def get_doodle_tool_dir():
        current_dir = os.getcwd()
        return current_dir + os.path.sep + "tools"

    @staticmethod
    def get_doodle_work_dir():
        # get where this file was executed
        current_dir = os.getcwd()
        return current_dir + os.path.sep + ".doodle"
    
    @staticmethod
    def get_doodle_parent_dir():
        # get where this file was executed
        current_dir = os.getcwd()
        return current_dir

    @staticmethod
    def get_doodle_module_type_str(module_type: DoodleModuleType):
        return module_type.name

    @staticmethod
    def get_doodle_module_type_from_str(module_type_str: str):
        if module_type_str == None or module_type_str == "":
            return None

        list_of_types = DoodleToolUtil.get_doodle_module_types_str_list()
        for t in list_of_types:
            if t.lower() == module_type_str.lower():
                return DoodleModuleType[t]

        return None

    @staticmethod
    def execute_bash_script(script_path: str, args: list):
        # check if the script exists
        if not os.path.exists(script_path):
            print(f"Error: The script {script_path} does not exist")
            return
        
        print(f"Executing script: {script_path}")
        print(f"Arguments: {args}")

        # execute the script
        os.exec([script_path] + args, shell=True)


    @staticmethod
    def get_doodle_platforms_dir():
        return DoodleToolUtil.get_doodle_dir() + os.path.sep + "platforms"