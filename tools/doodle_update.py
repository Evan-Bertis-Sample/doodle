# doodle_update.py

from tools.tool_utils import DoodleToolUtil
import subprocess

def main(args):
    doodle_root = DoodleToolUtil.get_doodle_parent_dir()

    # now we can run a git pull command, but first ask the user if they want to
    print("Attempting to update Doodle")
    print("This will run a git pull command in the Doodle directory")
    print("Specifically, this will run 'git pull' in the following directory:")
    print(doodle_root)

    # ask the user if they want to continue
    while True:
        user_input = input("Continue? (y/n): ")
        if user_input.lower() == "y":
            break
        elif user_input.lower() == "n":
            print("Exiting")
            return
        else:
            print("Invalid input")

    # run the git pull command
    print("Running git pull")
    try:
        subprocess.run(["git", "pull"], cwd=doodle_root, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error running git pull: {e}")
        return
