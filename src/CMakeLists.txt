include(tools)

assertVariableSet(projectName espMockName safeCstringName safeCstringVersion TOP_LEVEL)

include(FindGit)
find_package(Git)
include(FetchContent)

FetchContent_Declare(
  ${safeCstringName}
  GIT_REPOSITORY https://github.com/essenius/safe-cstring
  GIT_TAG        v${safeCstringVersion}
)
FetchContent_MakeAvailable(${safeCstringName})

add_library(${projectName} "")

# Add the include folder of the dependencies
foreach (entry ${CMAKE_PREFIX_PATH})
    list(APPEND includeFolders ${entry}/include)
endforeach() 

# We directly add the safe-cstring include folder (it's header only)
list(APPEND includeFolders ${${safeCStringName}_SOURCE_DIR}/src)

set(myHeaders Configuration.h)
set(mySources Configuration.cpp)

target_sources (${projectName} PUBLIC ${myHeaders} PRIVATE ${mySources})
target_link_libraries(${projectName} PUBLIC ${espMockName})

target_include_directories(${projectName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${includeFolders} )
install(TARGETS ${projectName} DESTINATION lib)
install(FILES ${myHeaders} DESTINATION include) 
