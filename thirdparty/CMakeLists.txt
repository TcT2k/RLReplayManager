macro(set_cache_variable VAR_NAME VAR_DESCRIPTION)
    set(${VAR_NAME} ${${VAR_NAME}} CACHE INTERNAL ${VAR_DESCRIPTION})
    message(STATUS "Set ${VAR_NAME} to ${${VAR_NAME}}.")
endmacro()

# Curl configuration

find_package(CURL)
if(CURL_FOUND)
    set(USE_SYSTEM_CURL ON CACHE BOOL "Use the system curl for faster builds")
endif()

if(USE_SYSTEM_CURL)
    find_package(CURL)
endif()
if(NOT USE_SYSTEM_CURL OR NOT CURL_FOUND)
    message(STATUS "Not using system Curl, using built-in curl project instead.")
    option(BUILD_CURL_TESTS "Set to ON to build cURL tests." OFF)
    add_subdirectory(curl)
    set(CURL_FOUND TRUE)
    set(CURL_LIBRARIES libcurl)
    set(CURL_INCLUDE_DIRS
        ${CURL_SOURCE_DIR}/include
        ${CURL_BINARY_DIR}/include/curl)
        
    # Group under the "external" project folder in IDEs such as Visual Studio.
    set_property(TARGET curl PROPERTY FOLDER "external")
    set_property(TARGET libcurl PROPERTY FOLDER "external")
	
	set(CURL_INCLUDE_DIR ${CURL_SOURCE_DIR}/include)
	set(CURL_LIBRARY libcurl)
endif()

set_cache_variable(CURL_FOUND "Set if libcurl is found or built")
set_cache_variable(CURL_LIBRARIES "Location of libcurl")
set_cache_variable(CURL_INCLUDE_DIRS "Location of curl include files")


# Curl CPP

include_directories(${CURL_INCLUDE_DIRS})

if (WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(curlcpp)

# wxJSON

add_subdirectory(wxJSON)
