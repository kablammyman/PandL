project(PandL)
include_directories($ENV{MY_STRING_UTILS} )


IF(WIN32)
include_directories(${CMAKE_SOURCE_DIR}/include) 
set(MY_STRING_LIB_32_D $ENV{MY_X86_DEBUG_LIBS}/StringUtils.lib)
set(MY_STRING_LIB_64_D $ENV{MY_X64_DEBUG_LIBS}/StringUtils.lib)
set(MY_STRING_LIB_32_R $ENV{MY_X86_RELEASE_LIBS}/StringUtils.lib)
set(MY_STRING_LIB_64_R $ENV{MY_X64_RELEASE_LIBS}/StringUtils.lib)


ELSE()
#notice the "lib" in front of my stuff
set(MY_STRING_LIB_32_D $ENV{MY_X86_DEBUG_LIBS}/libStringUtils.a)
set(MY_STRING_LIB_64_D $ENV{MY_X64_DEBUG_LIBS}/libStringUtils.a)
set(MY_STRING_LIB_32_R $ENV{MY_X86_RELEASE_LIBS}/libStringUtils.a)
set(MY_STRING_LIB_64_R $ENV{MY_X64_RELEASE_LIBS}/libStringUtils.a)


ENDIF()

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.cpp)
add_executable(PandL ${SOURCES})


target_link_libraries (PandL debug   ${MY_STRING_LIB_64_D} )
target_link_libraries (PandL optimized   ${MY_STRING_LIB_64_D} )
#optimized ${MY_STRING_LIB_64_R} )
