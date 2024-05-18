function(FetchContent_MakeAvailable_With_Check targetName)
    FetchContent_MakeAvailable(${targetName})
    if(NOT ${targetName}_POPULATED)
        message(FATAL_ERROR "Could not make ${targetName} available")
    endif()
    set(${targetName}_POPULATED ${${targetName}_POPULATED} PARENT_SCOPE)
    set(${targetName}_SOURCE_DIR ${${targetName}_SOURCE_DIR} PARENT_SCOPE)
    set(${targetName}_BINARY_DIR ${${targetName}_BINARY_DIR} PARENT_SCOPE)
endfunction()

function(assertVariableSet)
    foreach(argument ${ARGN})
        if(NOT DEFINED ${argument})
            message(FATAL_ERROR "Variable '${argument}' was not set")
        endif()
    endforeach()
endfunction()