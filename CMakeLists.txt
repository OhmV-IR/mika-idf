# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

#list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/components/esp-adf/components")
add_definitions(-DLV_CONF_INCLUDE_SIMPLE=1)
include_directories("${CMAKE_CURRENT_LIST_DIR}/main")

add_compile_options("-DIS_ENABLED(config_macro)=_IS_ENABLED(config_macro)")
add_compile_options("-D_IS_ENABLED(value)=__is_enabled(__ARG_PLACEHOLDER_##value)")
add_compile_options("-D__ARG_PLACEHOLDER_1=0,")
add_compile_options("-D__is_enabled(arg1_or_junk)=__take_secondarg(arg1_or_junk 1, 0)")
add_compile_options("-D__take_secondarg(arg1, arg2, ...)=arg2")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(mika-idf)