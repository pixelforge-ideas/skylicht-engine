include_directories(
	${SKYLICHT_ENGINE_PROJECT_DIR}/Skylicht/System
	${SKYLICHT_ENGINE_PROJECT_DIR}/Skylicht/Engine
	${SKYLICHT_ENGINE_PROJECT_DIR}/Irrlicht/Include
	${SKYLICHT_ENGINE_PROJECT_DIR}/ThirdParty/freetype2/include
	${SKYLICHT_ENGINE_PROJECT_DIR}/ThirdParty/kdtree
	${SKYLICHT_ENGINE_PROJECT_DIR}/Skylicht/Audio
)

file(GLOB_RECURSE skylicht_source 
	./*.cpp 
	./*.hpp 
	./*.c 
	./*.h)
setup_project_group("${skylicht_source}" ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Engine ${ENGINE_SHARED_OR_STATIC_LIB} ${skylicht_source})

if (BUILD_SHARED_LIBS)
	set_target_properties(Engine PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS true)
	add_definitions(-DSKYLICHT_EXPORTS)
endif()

target_precompiled_header(Engine ./pch.cpp ${skylicht_source})

set_target_properties(Engine PROPERTIES VERSION ${SKYLICHT_VERSION})

target_link_libraries(Engine Irrlicht System)

if (BUILD_SKYLICHT_AUDIO)
target_link_libraries(Engine Audio)
endif()

if (INSTALL_LIBS)
install(TARGETS Engine
			EXPORT EngineTargets
			RUNTIME DESTINATION ${SKYLICHT_RUNTIME_INSTALL_DIR}
			LIBRARY DESTINATION ${SKYLICHT_LIBRARY_INSTALL_DIR}
			ARCHIVE DESTINATION ${SKYLICHT_ARCHIVE_INSTALL_DIR})

install (DIRECTORY ${SKYLICHT_ENGINE_PROJECT_DIR}/Skylicht/Engine
	DESTINATION ${SKYLICHT_INCLUDE_INSTALL_DIR}/Skylicht
	FILES_MATCHING PATTERN "*.h*")

install(EXPORT EngineTargets
	FILE EngineTargets.cmake
	NAMESPACE Skylicht::
	DESTINATION ${SKYLICHT_TARGET_INSTALL_DIR}
)
endif()