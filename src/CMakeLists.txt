cmake_minimum_required(VERSION 2.8)

project(HeatMapGenerator)

aux_source_directory(. SRC_LIST)
 
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})
 
add_executable(${PROJECT_NAME} WIN32 ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})