#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

#this is CMakeLists.txt for buffer_unittests
cmake_minimum_required(VERSION 2.8.11)

compileAsC11()
find_package(UUID REQUIRED)
set(theseTestsName uniqueid_unittests)

if(WIN32)
    set(uniqueid_unittests_test_file ${theseTestsName}_win32.cpp)
else()
    set(uniqueid_unittests_test_file ${theseTestsName}_linux.cpp)
endif()

set(${theseTestsName}_cpp_files
${uniqueid_unittests_test_file}
)

set(${theseTestsName}_c_files
${UNIQUEID_C_FILE}
)

set(${theseTestsName}_h_files
)

build_test_artifacts(${theseTestsName} ON)

if(WIN32)
    target_link_libraries(${theseTestsName}_exe rpcrt4.lib)
    target_link_libraries(${theseTestsName}_dll rpcrt4.lib)
else()
    target_link_libraries(${theseTestsName}_exe ${LIBUUID_LIBRARIES})
endif()
