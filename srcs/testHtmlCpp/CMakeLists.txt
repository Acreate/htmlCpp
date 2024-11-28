get_current_dir_name( prject_name ${CMAKE_CURRENT_SOURCE_DIR} )
message( "============ ${prject_name}" )
message( "name =" ${prject_name} )
project( ${prject_name} VERSION 0.1 LANGUAGES CXX )
message( "============ ${CURRENT_FOLDER}" )

m_set_env( "${Project_Run_bin}" "${Project_Install_bin}" )

file( GLOB_RECURSE SRC_LIST
	"*.h"
	"*.c"
	"*.cpp"
	"*.cxx"
	"*.hpp"
)

include_directories( ${HtmlTools_include_dir} )

# # 合并源码
list( APPEND ${SRC_LIST} ${SRC_UI}  )

add_executable( ${PROJECT_NAME}
	${SRC_LIST}
)
set(Project_Restources_Dir "${CMAKE_CURRENT_LIST_DIR}/res/writeFile")
copy_dir_path( ${PROJECT_NAME} "${Project_Restources_Dir}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/writeFile/" )


target_link_libraries( ${PROJECT_NAME}
	HtmlTools
)
set_target_properties( ${PROJECT_NAME} PROPERTIES
	WIN32_EXECUTABLE FALSE
)
