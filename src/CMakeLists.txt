cmake_minimum_required(VERSION 2.8)

project(HeatMapGenerator)

FILE(GLOB SRCS "*.cpp")

FILE(GLOB HEADERS "*.h")
 
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})
 
add_executable(${PROJECT_NAME} WIN32 ${SRCS} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})