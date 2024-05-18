include(tools)

assertVariableSet(projectName espMockName TOP_LEVEL)

include(FindGit)
find_package(Git)
include(FetchContent)

FetchContent_Declare(
  safe-cstring
  GIT_REPOSITORY https://github.com/essenius/safe-cstring
  GIT_TAG        main
)
FetchContent_MakeAvailable(safe-cstring)

add_library(${projectName} "")

# Add the include folder of the dependencies
foreach (entry ${CMAKE_PREFIX_PATH})
    list(APPEND includeFolders ${entry}/include)
endforeach() 

# show all variables
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

  # We directly add the dependency include folder (it's header only)
list(APPEND includeFolders ${safe-cstring_SOURCE_DIR}/src)

set(myHeaders Configuration.h)
set(mySources Configuration.cpp)

target_sources (${projectName} PUBLIC ${myHeaders} PRIVATE ${mySources})
target_link_libraries(${projectName} PUBLIC ${espMockName})

target_include_directories(${projectName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${includeFolders} )
install(TARGETS ${projectName} DESTINATION lib)
install(FILES ${myHeaders} DESTINATION include) 
