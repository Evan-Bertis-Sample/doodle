# tool_utils.py

import os
import enum

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
        return current_dir / "doodle"

    @staticmethod
    def get_doodle_work_dir():
        # get where this file was executed
        current_dir = os.getcwd()
        return current_dir / ".doodle"

    @staticmethod
    def get_doodle_module_type_str(module_type: DoodleModuleType):
        return module_type.name

    @staticmethod
    def get_doodle_module_type_from_str(module_type_str: str):
        list_of_types = DoodleToolUtil.get_doodle_module_types_str_list()
        for t in list_of_types:
            if t.lower() == module_type_str.lower():
                return DoodleModuleType[t]
