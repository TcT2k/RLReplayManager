cmake_minimum_required(VERSION 2.8)

project(wxJSON)

include_directories(include)

add_library(wxJSON
	include/wx/jsonreader.h
	include/wx/jsonval.h
	include/wx/jsonwriter.h

	src/jsonreader.cpp
	src/jsonval.cpp
	src/jsonwriter.cpp
	)
