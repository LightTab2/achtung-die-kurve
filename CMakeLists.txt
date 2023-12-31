cmake_minimum_required(VERSION 3.21)

project("qt-template"
        LANGUAGES CXX)

# Find all needed libraries
include(cmake/Modules.cmake)
foreach(library IN LISTS Modules)
    find_package(${library} REQUIRED)
endforeach()

foreach(library IN LISTS QtModules)
    find_package(Qt6 REQUIRED COMPONENTS ${library})
endforeach()

# Setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

qt_standard_project_setup()

include(cmake/StandardSettings.cmake)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if (UNIX)
    add_compile_options("$<$<CONFIG:DEBUG>:-D_DEBUG>")    #this will allow to use same _DEBUG macro available in both Unix as well as Windows - MSCV environment. Easy to put Debug specific code
endif (UNIX)

# Setup alternative names
if(${PROJECT_NAME}_USE_ALT_NAMES)
    string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWERCASE)
    string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE)
else()
    set(PROJECT_NAME_LOWERCASE ${PROJECT_NAME})
    set(PROJECT_NAME_UPPERCASE ${PROJECT_NAME})
endif()

# Prevent building in the source directory
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n")
endif()

# Find all headers and implementation files
include(cmake/SourcesAndHeaders.cmake)

# Create library, setup header and source files
if(${PROJECT_NAME}_BUILD_EXECUTABLE)
    if (APPLE)
        # Check for icon
        if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icon/icon.icns")
            qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${sources} "${CMAKE_CURRENT_SOURCE_DIR}/icon/icon.icns")
        else()
            qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${sources})
        endif()
    elseif(WIN32)
        # Check for icon
        if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icon/icon.rc" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icon/icon.ico")
            qt_add_executable(${PROJECT_NAME} WIN32 ${sources} "${CMAKE_CURRENT_SOURCE_DIR}/icon/icon.rc")
        else()
            qt_add_executable(${PROJECT_NAME} WIN32 ${sources})
        endif()
    else()
        # Icons added at deployment
        qt_add_executable(${PROJECT_NAME} ${sources})
    endif()
    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
    qt_add_library(${PROJECT_NAME}_LIB ${headers} ${sources})
    set_target_properties(${PROJECT_NAME}_LIB
                          PROPERTIES
                          ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
                          LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
                          OUTPUT_NAME ${PROJECT_NAME})
    target_compile_features(${PROJECT_NAME}_LIB PUBLIC cxx_std_20)
else()
    qt_add_library(${PROJECT_NAME}
                ${headers}
                ${sources})
    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
endif()

if (NOT ${PROJECT_NAME}_BUILD_EXECUTABLE)
    set_target_properties(${PROJECT_NAME}
                          PROPERTIES
                          ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
                          LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
                          RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
endif()

# Set the project standard and warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME})

# Model project dependencies
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# Allow usage of header files in the `src` directory, but only for utilities
target_include_directories(${PROJECT_NAME}
                           PUBLIC
                           $<INSTALL_INTERFACE:include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           ${CMAKE_INCLUDE_PATH} 
                           PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_directories(${PROJECT_NAME}
                        PRIVATE
                        ${CMAKE_LIBRARY_PATH})

if(${PROJECT_NAME}_BUILD_EXECUTABLE)
    target_include_directories(${PROJECT_NAME}_LIB
                               PUBLIC
                               $<INSTALL_INTERFACE:include>
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                               ${CMAKE_INCLUDE_PATH} 
                               PRIVATE
                               ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_directories(${PROJECT_NAME}_LIB
                            PRIVATE
                            ${CMAKE_LIBRARY_PATH})
endif()

# Link libraries
foreach(library IN LISTS Modules)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${library})
    if(${PROJECT_NAME}_BUILD_EXECUTABLE)
        target_link_libraries(${PROJECT_NAME}_LIB PRIVATE ${library})
    endif()
endforeach()

# Link Qt libraries
foreach(library IN LISTS QtModules)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::${library})
    if(${PROJECT_NAME}_BUILD_EXECUTABLE)
        target_link_libraries(${PROJECT_NAME}_LIB PRIVATE Qt6::${library})
    endif()
endforeach()

# Resources
include(cmake/Resources.cmake)
qt_add_resources(${PROJECT_NAME} "resources"
    PREFIX "/resource"
    FILES ${Resources})

# Provide alias to library for
if(${PROJECT_NAME}_BUILD_EXECUTABLE)
    add_executable(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
else()
    add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
endif()

# Generate export header if specified
if(${PROJECT_NAME}_GENERATE_EXPORT_HEADER)
    include(GenerateExportHeader)
    generate_export_header(${PROJECT_NAME})
endif()

# Deployment
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
                          MACOSX_BUNDLE TRUE)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/icon/icon.icns")
        set(MACOSX_BUNDLE_ICON_FILE icon.icns)
        set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/icon/icon.icns" PROPERTIES
                                    MACOSX_PACKAGE_LOCATION "Resources")
    endif()
elseif(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
                          WIN32_EXECUTABLE TRUE)
endif()

# Install target
if(APPLE)
    install(TARGETS ${PROJECT_NAME}
            CONFIGURATIONS Release
            BUNDLE DESTINATION .)
    set(EXECUTABLE_PATH "./$<TARGET_FILE_NAME:${PROJECT_NAME}>.app")
    set(CMAKE_INSTALL_PREFIX "./install")
    message("exe path: ${EXECUTABLE_PATH}")
else()
    install(TARGETS ${PROJECT_NAME}
            CONFIGURATIONS Release
            RUNTIME DESTINATION .)
    if(UNIX)
        set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/install")
    else()
        set(CMAKE_INSTALL_PREFIX "install")
    endif()
    set(EXECUTABLE_PATH "$<TARGET_FILE:${PROJECT_NAME}>")
endif()

qt_generate_deploy_script(TARGET ${PROJECT_NAME}
                          OUTPUT_SCRIPT DEPLOY_SCRIPT
                          CONTENT "
                          qt_deploy_runtime_dependencies(EXECUTABLE \"${EXECUTABLE_PATH}\"
                          BIN_DIR .
                          VERBOSE)")

install(SCRIPT ${DEPLOY_SCRIPT})

# Unit testing setup
enable_testing(true)
add_subdirectory(test)
