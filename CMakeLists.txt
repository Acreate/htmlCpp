CMAKE_MINIMUM_REQUIRED( VERSION 3.19 )

SET( CMAKE_C_STANDARD 99 )
SET( CMAKE_C_STANDARD_REQUIRED ON )
SET( CMAKE_C_VISIBILITY_PRESET hidden )

SET( CMAKE_CXX_STANDARD 17 )
SET( CMAKE_CXX_STANDARD_REQUIRED ON )
SET( CMAKE_CXX_VISIBILITY_PRESET hidden )

FIND_PACKAGE( CmakeFuncs REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/srcs/cmake/CmakeFuncs/" )

get_current_dir_name( prject_name ${CMAKE_CURRENT_SOURCE_DIR} )
message( "============ ${prject_name}" )
message( "name =" ${prject_name} )
project( ${prject_name} VERSION 0.1 LANGUAGES CXX )
message( "============ ${CURRENT_FOLDER}" )

INCLUDE( GenerateExportHeader )
INCLUDE( GNUInstallDirs )

SET( Project_Run_bin "${CMAKE_HOME_DIRECTORY}/builder/${CMAKE_BUILD_TYPE}_${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_CXX_COMPILER_ID}/" )
SET( Project_Install_bin "${Project_Run_bin}" )
get_current_dir_name( current_dir_name ${CMAKE_CURRENT_SOURCE_DIR} )

# # 根部定义的模板位置
set( cmake_definitions_tmp ${CMAKE_HOME_DIRECTORY}/srcs/cmake/temp )
set( cmake_auto_sub_dir_name auto_generate_files )

# 创建测试写入目录
set( Cache_Path_Dir "${Project_Run_bin}write_cache/" )
file( MAKE_DIRECTORY  ${Cache_Path_Dir} )

m_set_env( "${Project_Run_bin}" "${Project_Install_bin}" )

add_compile_options( /utf-8 )

get_path_cmake_dir_path( out_path_list ${CMAKE_CURRENT_SOURCE_DIR}/srcs/HtmlTools/ "CMakeLists.txt" )

get_path_cmake_dir_path( out_path_list ${CMAKE_CURRENT_SOURCE_DIR}/srcs/testHtmlCpp/ "CMakeLists.txt" )

foreach( out_dir ${out_path_list} )
	STRING( REPLACE ${CMAKE_SOURCE_DIR}/ "./" out_path ${out_dir} )
	MESSAGE( STATUS "正在添加路径 :\t" ${out_dir} )
	ADD_SUBDIRECTORY( ${out_dir} )
endforeach()
