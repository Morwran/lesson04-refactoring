cmake_minimum_required(VERSION 3.16)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(refactoring_good_test refactoring_good.cpp)

set_target_properties(refactoring_good_test PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(
	refactoring_good_test 
	GTest::gtest_main
	GTest::gtest	
	${GTEST_LIBRARIES} 
	pthread
)