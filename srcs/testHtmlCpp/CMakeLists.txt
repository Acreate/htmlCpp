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

set( cmake_definitions_out "${CMAKE_CURRENT_LIST_DIR}/src/${cmake_auto_sub_dir_name}/macro/cmake_to_c_cpp_header_env.h" )

# # 合并源码
list( APPEND ${SRC_LIST} ${SRC_UI} ${cmake_definitions_out} )

add_executable( ${PROJECT_NAME}
	${SRC_LIST}
)

get_target_property( project_type_property ${PROJECT_NAME} TYPE )

if( "EXECUTABLE" STREQUAL ${project_type_property} )
	set( project_type_property_is_exe true )
else()
	set( project_type_property_is_exe false )
endif()

set(Project_Restources_Dir "${CMAKE_CURRENT_LIST_DIR}/res/writeFile")
copy_dir_path( ${PROJECT_NAME} "${Project_Restources_Dir}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/writeFile/" )


# 设置变量
set( Cmake_Project_Name "${PROJECT_NAME}" )
set( Cmake_Project_Binary_Dir "${CMAKE_BINARY_DIR}" )
set( Cmake_Project_Binary_Dir "${PROJECT_BINARY_DIR}" )
set( Cmake_Project_Binary_File_Name "$<TARGET_FILE:tgt>" )

set( cmake_definitions_org "${cmake_definitions_tmp}/cmake_in/cmake_to_c_cpp_header_env.h.in" )
configure_file( "${cmake_definitions_org}" "${cmake_definitions_out}" )

target_link_libraries( ${PROJECT_NAME}
	HtmlTools
)
set_target_properties( ${PROJECT_NAME} PROPERTIES
	WIN32_EXECUTABLE FALSE
)
