cmake_minimum_required(VERSION 3.5)
project(DIRenderer VERSION 0.1 DESCRIPTION "Renderer module" LANGUAGES CXX)


file( GLOB SOURCES **.cpp )

SetFlags()
SetLibraries( DIWindow )
CreateLibrary( ${PROJECT_NAME} )