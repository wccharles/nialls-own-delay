# build gtest framework
add_subdirectory(googletest)

# ##############################################################################
# Include directories relative to test  #
# ##############################################################################

set(TEST_FILES ${PROJECT_SOURCE_DIR}/tests/unittests/AudioProcessor_gTest.cpp)

set(TEST_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/tests/googletest/include)

# ##############################################################################
# Statically linked libraries  #
# ##############################################################################

set(TEST_LINK_LIBRARIES gtest gtest_main)

add_executable(unit_tests ${TEST_FILES})

target_compile_definitions(unit_tests PRIVATE JUCE_UNIT_TESTS=0 JUCE_USE_CURL=0
                                              JUCE_WEB_BROWSER=0)

target_include_directories(unit_tests PRIVATE ${TEST_INCLUDE_DIRS})

target_link_libraries(unit_tests PRIVATE ${TEST_LINK_LIBRARIES}
                                         ${JUCE_LINK_LIBRARIES} ModDelay)

# Make an Xcode Scheme for the test executable so we can run tests in the IDE
set_target_properties(unit_tests PROPERTIES XCODE_GENERATE_SCHEME ON)

# Organize the test source in the Tests/ folder in the IDE
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}/unittests
  PREFIX ""
  FILES ${TEST_FILES})

gtest_discover_tests(unit_tests)
