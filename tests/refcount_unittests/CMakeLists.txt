#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

#this is CMakeLists.txt for map_unittests
cmake_minimum_required(VERSION 2.8.11)

compileAsC11()
set(theseTestsName refcount_unittests)

set(${theseTestsName}_cpp_files
${theseTestsName}.cpp
)

set(${theseTestsName}_c_files
	some_refcount_impl.c
)

set(${theseTestsName}_h_files
	some_refcount_impl.h
)

build_test_artifacts(${theseTestsName} ON)