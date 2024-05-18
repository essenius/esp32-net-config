include(FindGit)
find_package(Git)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.12.1
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    INSTALL_COMMAND ""
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(${projectTestName} "")

set(mySources ConfigurationTest.cpp )

target_sources (${projectTestName} PRIVATE ${mySources})
target_include_directories(${projectName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries(${projectTestName} ${projectName} gtest_main)

add_test(NAME ${projectTestName} COMMAND ${projectTestName})