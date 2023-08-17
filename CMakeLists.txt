cmake_minimum_required(VERSION 3.10)
project(arena_sdk_opencv_example)

# Find OpenCV
find_package(OpenCV REQUIRED)

set(ARENA_ROOT $ENV{HOME}/localInstalls/ArenaSDK_Linux_x64)

#set includes
include_directories(
    ${ARENA_ROOT}/include/Arena
    ${ARENA_ROOT}/include/Save
    ${ARENA_ROOT}/include/GenTL
    ${ARENA_ROOT}/GenICam/library/CPP/include
    )


find_library(ARENA_LIBS 
    NAMES arena save gentl
    HINTS ${ARENA_ROOT}/lib64)

find_library(GENICAM_LIBS
    NAMES GCBase_gcc54_v3_3_LUCID log4cpp_gcc54_v3_3_LUCID MathParser_gcc54_v3_3_LUCID XmlParser_gcc54_v3_3_LUCID GenApi_gcc54_v3_3_LUCID Log_gcc54_v3_3_LUCID NodeMapData_gcc54_v3_3_LUCID
    HINTS ${ARENA_ROOT}/GenICam/library/lib/Linux64_x64)


#------------------------------------------------------------------
# Add executable
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.cpp
    )


# Link OpenCV
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    ${ARENA_LIBS}
    ${GENICAM_LIBS}
    ${OpenCV_LIBS}
    )
#------------------------------------------------------------------

