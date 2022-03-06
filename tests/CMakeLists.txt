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

gtest_discover_tests(unit_tests)
