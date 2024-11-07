#!/bin/bash

# Set versions for each library
OATPP_VERSION="1.3.0-latest"
OATPP_SQLITE_VERSION="1.3.0-latest"
OATPP_SWAGGER_VERSION="1.3.0-latest"

# Detect platform
OS_TYPE="$(uname -s)"
echo "Detected OS: $OS_TYPE"

# Detect the number of CPU cores for faster builds
if [ "$OS_TYPE" == "Darwin" ]; then
  NUM_CORES=$(sysctl -n hw.ncpu)
elif [ "$OS_TYPE" == "Linux" ]; then
  NUM_CORES=$(nproc)
else
  echo "Unsupported OS: $OS_TYPE"
  exit 1
fi

echo "Using $NUM_CORES cores for building."

# Function to install dependencies on macOS
install_macos_dependencies() {
  echo "Installing dependencies with Homebrew for macOS..."

  # Check and install each dependency only if it's not already installed
  brew list nlohmann-json &>/dev/null || brew install nlohmann-json
  brew list aws-sdk-cpp &>/dev/null || brew install aws-sdk-cpp
  brew list sqlite3 &>/dev/null || brew install sqlite3
  brew list cmake &>/dev/null || brew install cmake
}

# Function to install dependencies on Linux
install_linux_dependencies() {
  echo "Updating package list and installing dependencies for Linux..."

  # Check and install each dependency only if it's not already installed
  dpkg -s git &>/dev/null || sudo apt-get install -y git
  dpkg -s cmake &>/dev/null || sudo apt-get install -y cmake
  dpkg -s libsqlite3-dev &>/dev/null || sudo apt-get install -y libsqlite3-dev
  dpkg -s libjsoncpp-dev &>/dev/null || sudo apt-get install -y libjsoncpp-dev
  dpkg -s awscli &>/dev/null || sudo apt-get install -y awscli
}

# Select installation function based on OS
case "$OS_TYPE" in
  Darwin*) install_macos_dependencies ;;
  Linux*) install_linux_dependencies ;;
esac

# Set the installation directory for Oat++ libraries
OATPP_INSTALL_DIR="/usr/local/oatpp"
mkdir -p "$OATPP_INSTALL_DIR"

# Function to clone, build, and install an Oat++ library if not already installed
install_oatpp_library() {
  REPO_URL=$1
  VERSION_TAG=$2
  LIB_NAME=$3

  # Check if the library is already installed
  if [ -d "$OATPP_INSTALL_DIR/lib/cmake/$LIB_NAME" ]; then
    echo "$LIB_NAME is already installed at $OATPP_INSTALL_DIR. Skipping."
    return
  fi

  echo "Cloning $LIB_NAME (version $VERSION_TAG)..."
  git clone --branch "$VERSION_TAG" --depth 1 "$REPO_URL" "$LIB_NAME"
  cd "$LIB_NAME" || exit 1

  echo "Building $LIB_NAME using $NUM_CORES cores..."
  mkdir build && cd build || exit 1
  cmake -DCMAKE_INSTALL_PREFIX="$OATPP_INSTALL_DIR" ..
  cmake --build . --config Release -- -j"$NUM_CORES"
  sudo cmake --install .

  echo "$LIB_NAME installed to $OATPP_INSTALL_DIR/lib/cmake/$LIB_NAME"
  cd ../../
  rm -rf "$LIB_NAME"
}

# Install Oat++ core library if not already installed
install_oatpp_library "https://github.com/oatpp/oatpp.git" "$OATPP_VERSION" "oatpp"

# Install Oat++ SQLite module if not already installed
install_oatpp_library "https://github.com/oatpp/oatpp-sqlite.git" "$OATPP_SQLITE_VERSION" "oatpp-sqlite"

# Install Oat++ Swagger module if not already installed
install_oatpp_library "https://github.com/oatpp/oatpp-swagger.git" "$OATPP_SWAGGER_VERSION" "oatpp-swagger"

# Environment setup for CMake
echo "Setting up environment variables for CMake..."
export CMAKE_PREFIX_PATH="$OATPP_INSTALL_DIR"
export PKG_CONFIG_PATH="$OATPP_INSTALL_DIR/lib/pkgconfig"

echo "Oat++ and dependencies are installed. You may now configure your CMake project with the correct paths."