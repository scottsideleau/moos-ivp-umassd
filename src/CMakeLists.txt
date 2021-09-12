##############################################################################
# FILE:        moos-ivp-umassd/src/CMakeLists.txt
# DATE:        08-JUL-2017
# DESCRIPTION: CMakeLists.txt file for the moos-ivp-umassd source directory
##############################################################################


#============================================================================
# Add dependencies, as required
#============================================================================
IF(BUILD_DCCL)
  FIND_PACKAGE(Protobuf REQUIRED)
  INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIRS})
  SET(PROTOBUF_IMPORT_DIRS ${PROTOBUF_INCLUDE_DIRS})

  FIND_PACKAGE(DCCL REQUIRED)
  IF(NOT DEFINED DCCL_INCLUDE_DIR)
    # Requires CMake 3.0.2+
    GET_TARGET_PROPERTY(DCCL_INCLUDE_DIR dccl INTERFACE_INCLUDE_DIRECTORIES)
  ENDIF()
  IF(NOT DEFINED DCCL_INCLUDE_DIR)
    # Legacy CMake: Assume DCCL adjacent to this directory's root
    SET(DCCL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}../dccl/include")
  ENDIF()
  IF(DCCL_INCLUDE_DIR)
    MESSAGE("DCCL_INCLUDE_DIR: " ${DCCL_INCLUDE_DIR})
    INCLUDE_DIRECTORIES(${DCCL_INCLUDE_DIR})
    SET(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS} ${DCCL_INCLUDE_DIR})
    SET(PROTOBUF_IMPORT_DIRS ${PROTOBUF_IMPORT_DIRS} ${DCCL_INCLUDE_DIR})
  ENDIF() 
ENDIF(BUILD_DCCL)

#============================================================================
# Add the libraries in the current directory to the include path
#============================================================================
FILE(GLOB LOCAL_LIBRARY_DIRS ./lib_*)
INCLUDE_DIRECTORIES(${LOCAL_LIBRARY_DIRS})

#============================================================================
# List the subdirectories to build...
#============================================================================
IF(BUILD_DCCL)
  ADD_SUBDIRECTORY(lib_acommsMsgs)
ENDIF()
ADD_SUBDIRECTORY(pMoosKiller)
ADD_SUBDIRECTORY(uSimpleRobot)
IF(BUILD_WOLFSSL)
  ADD_SUBDIRECTORY(pWolfEncrypt)
  ADD_SUBDIRECTORY(pWolfDecrypt)
ENDIF()

##############################################################################
#                           END of CMakeLists.txt
##############################################################################

