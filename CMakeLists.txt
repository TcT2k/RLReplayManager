cmake_minimum_required(VERSION 2.8)

project(RLReplayManager)

SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "3")
SET(CPACK_PACKAGE_VERSION_PATCH "1")

find_package(wxWidgets 3.0 COMPONENTS core base adv xml qa REQUIRED)

include( ${wxWidgets_USE_FILE} )

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	
	# Enable PDB creation for release builds
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

set(SRCS ${SRCS}
	App.cpp
	ManagerFrame.cpp
	ManagerFrame.h
	RLReplayManager.fbp
	RLRMFrames.cpp
	RLRMFrames.h
	Replay.h
	Replay.cpp
	ReplayProvider.h
	ReplayProvider.cpp
	TransferManager.h
	TransferManager.cpp
	)
	
if(WIN32)
configure_file(VersionInfo.rc.in
	${${PROJECT_NAME}_BINARY_DIR}/VersionInfo.rc)
	set(SRCS ${SRCS}
		appicon.rc
		${${PROJECT_NAME}_BINARY_DIR}/VersionInfo.rc
	)
	
endif()

# Prepare version information
configure_file(VersionInfo.h.in
	${${PROJECT_NAME}_BINARY_DIR}/VersionInfo.h)

add_subdirectory(thirdparty)
include(${CURLCPP_BINARY_DIR}/curlcppConfig.cmake)

include_directories(
	${${PROJECT_NAME}_BINARY_DIR}
	${CURL_INCLUDE_DIRS}
	${CURLCPP_INCLUDE_DIRS}
	thirdparty/wxJSON/include
)

if (WIN32)
	add_definitions(-D_WINSOCKAPI_)
endif()

add_executable(RLReplayManager WIN32 MACOSX_BUNDLE ${SRCS})
target_link_libraries(RLReplayManager ${wxWidgets_LIBRARIES} curlcpp wxJSON)

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS                "-Wall -std=c++11")
endif()

set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ".")
if (APPLE)
	install(TARGETS RLReplayManager BUNDLE DESTINATION ${CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION})
	set(CPACK_SYSTEM_NAME Mac)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/RLReplayManager.plist.in
	)
else()
	install(TARGETS RLReplayManager RUNTIME DESTINATION ${CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION})
	install(FILES RLReplayManager.cfg README.md LICENSE DESTINATION ${CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION})
endif()
include(InstallRequiredSystemLibraries)

SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_GENERATOR "ZIP")
include(CPack)
