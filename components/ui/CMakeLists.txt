file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/src/*.c")

idf_component_register(SRCS ${SOURCE_FILES} INCLUDE_DIRS 
	"${CMAKE_CURRENT_LIST_DIR}/src/"
	"${CMAKE_CURRENT_LIST_DIR}/src/screens"
	"${CMAKE_CURRENT_LIST_DIR}" 
	"${CMAKE_CURRENT_LIST_DIR}/src/components"
	"${CMAKE_CURRENT_LIST_DIR}/src/images"
REQUIRES lvgl
PRIV_REQUIRES lvgl
)

component_compile_options(-fmax-include-depth=10000)
