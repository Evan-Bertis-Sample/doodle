# tool_utils.py

import os

class DoodleToolUtil:
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