# Motion and Motor Performance  Laboratory

A collection of my work with the UVA Motion and Motor Performance Laboratory. My biggest work was implementing the findings from the "The gait deviation index: A new comprehensive index of gait pathology" research paper written by Michael H. Schwartz and Adam Rozumalski by creating a custom C++ OpenSim plugin that is able to create a standardized gait deviation index for our rodent data. 

Research Paper: 
https://www.sciencedirect.com/science/article/abs/pii/S0966636208001136

To use the custom C++ plugin: 
Build the required system based on the following CMakeLists file
```
cmake_minimum_required(VERSION 3.12)
project(OpenSimCustomPlugin)

find_package(OpenSim REQUIRED)

set(SOURCES
    CustomGaitAnalysisPlugin.cpp
)

set(HEADERS
    CustomGaitAnalysisPlugin.h
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
Build the plugin using VS Code: 
```
msbuild CustomGaitAnalysisPlugin.sln /p:Configuration=Release
```
Once the plugin is built, then you can add it to your OpenSim directory, find the OpenSim.ini and open it in a text editor, then navigate to the plugin section, finally add the follwowing line

```
CustomGaitAnalysisPlugin = plugins/CustomGaitAnalysisPlugin
```

Save the changes, and run Opensim
