include_directories(
	${SKYLICHT_ENGINE_PROJECT_DIR}/Skylicht/Components
	${SKYLICHT_ENGINE_PROJECT_DIR}/Irrlicht/Include
	${SKYLICHT_ENGINE_PROJECT_DIR}/Skylicht/System
	${SKYLICHT_ENGINE_PROJECT_DIR}/Skylicht/Engine
)

file(GLOB_RECURSE skylicht_components 
	./*.cpp 
	./*.hpp 
	./*.c 
	./*.h)
setup_project_group("${skylicht_components}" ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Components ${ENGINE_SHARED_OR_STATIC_LIB}
	${skylicht_components}
)

if (BUILD_SHARED_LIBS)
	set_target_properties(Components PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS true)
	add_definitions(-DCOMPONENTS_EXPORTS)
endif()

target_precompiled_header(Components ./pch.cpp ${skylicht_components})

set_target_properties(Components PROPERTIES VERSION ${SKYLICHT_VERSION})

target_link_libraries(Components Engine System)

if (INSTALL_LIBS)
install(TARGETS Components
			EXPORT ComponentsTargets
			RUNTIME DESTINATION ${SKYLICHT_RUNTIME_INSTALL_DIR}
			LIBRARY DESTINATION ${SKYLICHT_LIBRARY_INSTALL_DIR}
			ARCHIVE DESTINATION ${SKYLICHT_ARCHIVE_INSTALL_DIR})

install (DIRECTORY ${SKYLICHT_ENGINE_PROJECT_DIR}/Skylicht/Components
	DESTINATION ${SKYLICHT_INCLUDE_INSTALL_DIR}/Skylicht
	FILES_MATCHING PATTERN "*.h*")

install(EXPORT ComponentsTargets
	FILE ComponentsTargets.cmake
	NAMESPACE Skylicht::
	DESTINATION ${SKYLICHT_TARGET_INSTALL_DIR}
)
endif()