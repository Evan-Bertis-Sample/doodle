#!/usr/bin/env bash
#
# install_doodle.sh
#
# This script installs a "doodle" command so you can run doodle.py from anywhere.

# 1) The path to your doodle.py. Adjust to wherever it resides:
DOODLE_PY_PATH="$(pwd)/doodle.py"

# 2) Decide where you want to install the symlink or wrapper script.
#    Typically on macOS/Linux, /usr/local/bin is in $PATH.
INSTALL_DIR="/usr/local/bin"

# 3) Detect OS
OS="$(uname -s 2>/dev/null || echo Windows)"

echo "Detected OS: ${OS}"

# Check that doodle.py exists
if [ ! -f "${DOODLE_PY_PATH}" ]; then
  echo "ERROR: Cannot find doodle.py at: ${DOODLE_PY_PATH}"
  exit 1
fi

if [[ "${OS}" == "Darwin" || "${OS}" == "Linux" ]]; then
  echo "Installing on macOS/Linux..."

  # Ensure doodle.py is executable
  chmod +x "${DOODLE_PY_PATH}"

  # If /usr/local/bin/doodle already exists, remove it (or back it up).
  if [ -L "${INSTALL_DIR}/doodle" ] || [ -f "${INSTALL_DIR}/doodle" ]; then
    echo "Removing existing ${INSTALL_DIR}/doodle"
    sudo rm -f "${INSTALL_DIR}/doodle"
  fi

  # Create the symlink
  echo "Creating symlink: ${INSTALL_DIR}/doodle -> ${DOODLE_PY_PATH}"
  sudo ln -s "${DOODLE_PY_PATH}" "${INSTALL_DIR}/doodle"

  echo "Installation complete. You should now be able to run 'doodle' anywhere."

elif [[ "${OS}" =~ ^CYGWIN || "${OS}" =~ ^MINGW || "${OS}" == "Windows" ]]; then
  echo "Installing on Windows..."

  # On Windows, creating a symlink in the standard system PATH is less common
  # (and can require admin privileges). Instead, we can generate a .cmd wrapper
  # if the user has a directory in PATH for local scripts. E.g. %USERPROFILE%\bin

  # 1) Where can we place the wrapper? We’ll pick something like %USERPROFILE%\bin.
  #    If that folder doesn't exist or isn't in PATH, user must manually add it.
  USER_BIN_WIN="$HOME/bin"   # For Git Bash, Cygwin, or MINGW, $HOME points to user dir

  if [ ! -d "$USER_BIN_WIN" ]; then
    echo "Creating directory: $USER_BIN_WIN"
    mkdir -p "$USER_BIN_WIN"
  fi

  cat <<EOF > "$USER_BIN_WIN/doodle.cmd"
@echo off
REM This wrapper script calls Python on doodle.py
python "$(cygpath -w "${DOODLE_PY_PATH}")" %*
EOF

  echo "Created $USER_BIN_WIN/doodle.cmd"
  echo ""
  echo "Next steps (Windows user):"
  echo "  1) Ensure $USER_BIN_WIN is in your PATH."
  echo "  2) Then you can run 'doodle' from any command prompt."
  echo ""
  echo "Installation complete."

else
  echo "ERROR: Unrecognized OS: ${OS}"
  echo "Please edit this script to handle your environment."
  exit 1
fi
