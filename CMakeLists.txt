#####################################################################
# i2p_applications cmake
#####################################################################

cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME "i2p_Nadine_Control")
project(${PROJECT_NAME})

#SET(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W0 /GR /EHa /clr")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DWIN32 /D_WINDOWS /W0 /GR /EHa /clr")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DWIN32 /D_WINDOWS /W0 /GR /EHa /clr")


set(CMAKE_MODULE_PATH $ENV{I2P_MODULE}/i2pCmake)

if (WIN32)
set(BOOST_ROOT "$ENV{IMI_LIBRARIES}/boost_1_49_0")
endif()

#dependencies for this project

include($ENV{I2P_MODULE}/i2pCmake/i2p.cmake)

FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(Thrift REQUIRED)
FIND_PACKAGE(Cvoice_Checker_Sxmlc REQUIRED)

IF( NOT THRIFT_FOUND )
	message("Failed to find thrift!")
ELSE()
	include_directories(${THRIFT_INCLUDE_DIR})
ENDIF()

#####################################################################
# Generate Thrift stubs
#####################################################################
include_directories($ENV{I2P_MODULE}/i2pThrift/gen-cpp)
GENERATE_STUBS( I2P.thrift cpp )
GENERATE_STUBS( EventPublisher.thrift cpp )
GENERATE_STUBS( Inputs.thrift cpp )
GENERATE_STUBS( Control.thrift cpp )
GENERATE_STUBS( Feedback.thrift cpp )

INCLUDE_DIRECTORIES(${SXMLC_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CEREVOICE_ENG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CEREVOICE_AUD_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CHECKER_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${THRIFT_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES($ENV{I2P_MODULE}/i2pThrift/tools/cpp)
INCLUDE_DIRECTORIES($ENV{I2P_MODULE}/i2pThrift/gen-cpp)
INCLUDE_DIRECTORIES(${PROJECT_NAME})

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

file(TO_CMAKE_PATH "$ENV{I2P_MODULE}/i2pThrift" I2P_MODULE_PATH)

set( I2P_SRCS
    ${I2P_MODULE_PATH}/gen-cpp/I2P_constants.cpp
    ${I2P_MODULE_PATH}/gen-cpp/I2P_constants.h
    ${I2P_MODULE_PATH}/gen-cpp/I2P_types.cpp
    ${I2P_MODULE_PATH}/gen-cpp/I2P_types.h
    ${I2P_MODULE_PATH}/gen-cpp/Inputs_constants.cpp
    ${I2P_MODULE_PATH}/gen-cpp/Inputs_constants.h
    ${I2P_MODULE_PATH}/gen-cpp/Inputs_types.cpp
    ${I2P_MODULE_PATH}/gen-cpp/Inputs_types.h
    ${I2P_MODULE_PATH}/gen-cpp/AgentControl.h
    ${I2P_MODULE_PATH}/gen-cpp/AgentControl.cpp
    ${I2P_MODULE_PATH}/gen-cpp/AgentFeedbackService.h
    ${I2P_MODULE_PATH}/gen-cpp/AgentFeedbackService.cpp
    ${I2P_MODULE_PATH}/gen-cpp/WorldQuery.h
    ${I2P_MODULE_PATH}/gen-cpp/WorldQuery.cpp
    ${I2P_MODULE_PATH}/gen-cpp/EventPublisher_types.h
    ${I2P_MODULE_PATH}/gen-cpp/EventPublisher_types.cpp
    ${I2P_MODULE_PATH}/gen-cpp/EventPublisher_constants.h
    ${I2P_MODULE_PATH}/gen-cpp/EventPublisher_constants.cpp
    ${I2P_MODULE_PATH}/tools/cpp/ProtectedClient.h #this is a tool
	
)

set(Nadine_SRC 
	src/checkerWrapper.cpp
	src/Cervevoice.cpp
	src/LipAnimationGenerator.cpp
	src/MayaWtenConverter.cpp
	src/NadineControl.cpp
	src/ThriftServer.cpp
	src/i2p_Nadine_Server.cpp
	src/NadineServer.cpp
	src/NadineAnimations.cpp
	src/TrackingService_server.cpp
	src/checkerCpp.cpp
	src/WorldClient.cpp
	src/XMLWrapper.cpp
	src/Joint.cpp)
set(Nadine_HDR 
	src/checkerWrapper.h
	src/Cervevoice.h
	src/LipAnimationGenerator.h
	src/MayaWtenConverter.h
	src/NadineControl.h
	src/ThriftServer.h
	src/NadineServer.h
	src/AgentControl_server.h
	src/NadineAnimations.h
	src/TrackingService
	src/checkerCpp.h
	src/WorldClient.h
	src/XMLWrapper.h
	src/Joint.h)
	

SOURCE_GROUP( i2p FILES ${I2P_SRCS})

#####################################################################
# Common Tools from I2P
set( COMMON_HDRS
    ${I2P_MODULE_PATH}/tools/cpp/ThriftTools.hpp
)
set( COMMON_SRCS
    ${I2P_MODULE_PATH}/tools/cpp/ThriftTools.cpp
)

if(WIN32)
    set( COMMON_SRCS ${COMMON_SRCS} 
        ${I2P_MODULE_PATH}/tools/cpp/ThriftTools_Win32.cpp ) 
else(WIN32)
    set( COMMON_SRCS ${COMMON_SRCS} 
        ${I2P_MODULE_PATH}/tools/cpp/ThriftTools_Unix.cpp ) 
endif(WIN32)

SOURCE_GROUP( I2PCommonSources FILES ${COMMON_SRCS})
set(HDRS ${HDRS} ${COMMON_HDRS})
set(SRCS ${SRCS} ${COMMON_SRCS})

ADD_EXECUTABLE(${PROJECT_NAME}
    ${I2P_SRCS}
	${HDRS} ${SRCS} 
	${Nadine_HDR} 
	${Nadine_SRC}
)

set( OPT_LIBS ${THRIFT_LIBRARY_RELEASE} ${CHECKER_LIBRARY_RELEASE} ${CEREVOICE_LIB} ${CEREVOICE_ENG_LIB} ${CEREVOICE_AUD_LIB} ${CEREVOICE_CERE_LIB}  ${SXMLC_RELEASE_LIB} )
foreach( OPT_LIB ${OPT_LIBS})
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME}
		optimized ${OPT_LIB} 
	)
endforeach()

set( DBG_LIBS ${THRIFT_LIBRARY_DEBUG} ${CHECKER_LIBRARY_DEBUG}  ${CEREVOICE_LIB} ${CEREVOICE_ENG_LIB} ${CEREVOICE_AUD_LIB} ${CEREVOICE_CERE_LIB} ${SXMLC_DEBUG_LIB})
foreach( DBG_LIB ${DBG_LIBS})
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME}
		debug ${DBG_LIB} 
	)
endforeach()

#message("Current CXX flags: ${CMAKE_CXX_FLAGS}" )


