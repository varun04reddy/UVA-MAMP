# mamp


```
cmake_minimum_required(VERSION 3.12)
project(OpenSimCustomPlugin)

find_package(OpenSim REQUIRED)

set(SOURCES
    MyCustomPlugin.cpp
)

set(HEADERS
    MyCustomPlugin.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${OpenSim_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenSim_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME MyCustomPlugin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${OpenSim_INSTALL_PLUGINDIR})

```
