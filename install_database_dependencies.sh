#!/bin/bash

# Detect the operating system
OS=$(uname -s)

echo "Detected OS: $OS"

if [[ "$OS" == "Darwin" ]]; then
    # macOS Installation (using Homebrew)
    echo "Installing database client libraries on macOS using Homebrew..."

    # Check if Homebrew is installed
    if ! command -v brew &>/dev/null; then
        echo "Homebrew is not installed. Installing Homebrew first..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi

    echo "Updating Homebrew..."
    brew update

    echo "Installing PostgreSQL client library..."
    brew install libpq

    echo "Installing SQLite library..."
    brew install sqlite

    echo "Installing MariaDB client library..."
    brew install mariadb-connector-c

    echo "Installing JSONCPP client library..."
    brew install jsoncpp

    echo "Installing Template engine for modern C++ library..."
    brew install inja


    echo "Database client libraries installed successfully on macOS!"

elif [[ "$OS" == "Linux" ]]; then
    # Linux Installation (Debian/Ubuntu-based)
    if command -v apt &>/dev/null; then
        echo "Installing database client libraries on Linux using apt..."

        echo "Updating package lists..."
        sudo apt update

        echo "Installing PostgreSQL client library..."
        sudo apt install -y libpq-dev

        echo "Installing SQLite library..."
        sudo apt install -y libsqlite3-dev

        echo "Installing MariaDB client library..."
        sudo apt install -y libmariadb-dev

        echo "Database client libraries installed successfully on Linux!"

    else
        echo "Unsupported package manager. Please install dependencies manually."
        exit 1
    fi

else
    echo "Unsupported operating system: $OS"
    exit 1
fi

# Verify installations
echo "Verifying installations..."

# Check PostgreSQL client library
if [[ "$OS" == "Darwin" ]] && [[ -f "$(brew --prefix libpq)/lib/libpq.dylib" ]]; then
    echo "PostgreSQL client library is installed."
elif [[ "$OS" == "Linux" ]] && [[ -f "/usr/lib/x86_64-linux-gnu/libpq.so" ]]; then
    echo "PostgreSQL client library is installed."
else
    echo "PostgreSQL client library installation failed."
fi

# Check SQLite library
if command -v sqlite3 &>/dev/null; then
    echo "SQLite library is installed: $(sqlite3 --version)"
else
    echo "SQLite library installation failed."
fi

# Check MariaDB client library
if [[ "$OS" == "Darwin" ]] && [[ -f "$(brew --prefix mariadb-connector-c)/lib/libmariadb.dylib" ]]; then
    echo "MariaDB client library is installed."
elif [[ "$OS" == "Linux" ]] && [[ -f "/usr/lib/x86_64-linux-gnu/libmariadb.so" ]]; then
    echo "MariaDB client library is installed."
else
    echo "MariaDB client library installation failed."
fi

echo "All installations complete!"