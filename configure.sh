#! /bin/sh

### Constant definitions ######################################################

# Project name
readonly PRJ_NAME="TraceLog"

# External build directory
readonly BUILD_DIR="build"

# Save the current path, to be able to exit in the same directory
readonly CURRENT_DIR=$(pwd)

# Installation query phrase
readonly QUERY_INSTALL="Do you wish to install project '${PRJ_NAME}' (requires root privileges)?"

### Global Variables ##########################################################

# Make all command result
GLOB_MAKE_ALL_RESULT=-1

### Functions #################################################################

# Create the CMake build environment
makeEnv()
{
    printf "%s\n" "-- Create the build directory ${BUILD_DIR}..."
    # External Build: outside of the project structure
    mkdir -p "${BUILD_DIR}"
    cd "${BUILD_DIR}"

    printf "%s\n" "-- Generate build environment from CMake input..."
    # Create the makefile based on the CMakeLists.txt up-dir
    cmake -G "Unix Makefiles" ..
}

# Build project
buildProject()
{
    # Make the targets
    make all
    GLOB_MAKE_ALL_RESULT=${?}
}

# Project installation
installProject()
{
    printf "%s\n" "-- '${PRJ_NAME}' will be installed..."
    # Install project files, this requires Root privileges
    sudo make install
}

# Query for installation and proceed if agreed
installQuery()
{
    while true; do
        read -p "-- ${QUERY_INSTALL} (y/n) " yn
        case ${yn} in
            [Yy]* ) installProject; break;;
            [Nn]* ) break;;
            * ) printf "%s\n" "-- Invalid input, use Y(es) or N(o)";;
        esac
    done
}

# Called before exit
cleanUp()
{
    # Change the directory back
    cd  "${CURRENT_DIR}"
}

# Script body
main()
{
    printf "%s\n" "Configure project '${PRJ_NAME}'"
    makeEnv
    buildProject
    if [ ${GLOB_MAKE_ALL_RESULT} -eq 0 ]; then
        installQuery
    else
        printf "Building '%s' failed!\n" "${PRJ_NAME}"
    fi
    cleanUp
}

# Call main function with all CLI arguments
main "${@}"
