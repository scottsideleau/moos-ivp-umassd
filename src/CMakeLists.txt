##############################################################################
# FILE:        moos-ivp-umassd/src/CMakeLists.txt
# DATE:        2015/12/22
# DESCRIPTION: CMakeLists.txt file for the moos-ivp-umassd source directory
##############################################################################


#============================================================================
# Add the libraries in the current directory to the include path
#============================================================================
FILE(GLOB LOCAL_LIBRARY_DIRS ./lib_*)
INCLUDE_DIRECTORIES(${LOCAL_LIBRARY_DIRS})

IF(BUILD_DCCL)
	FIND_PACKAGE(Protobuf REQUIRED)
	INCLUDE_DIRECTORIES(${Protobuf_INCLUDE_DIRS})

	FIND_PACKAGE(DCCL REQUIRED)
	IF(NOT DEFINED DCCL_INCLUDE_DIR)
		# Requires DCCL 3.0.3+
		GET_TARGET_PROPERTY(DCCL_INCLUDE_DIR dccl INTERFACE_INCLUDE_DIRECTORIES)
	ENDIF()
	IF(DCCL_INCLUDE_DIR)
		INCLUDE_DIRECTORIES(${DCCL_INCLUDE_DIR})
		SET(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS} ${DCCL_INCLUDE_DIR})
		SET(PROTOBUF_IMPORT_DIRS ${PROTOBUF_IMPORT_DIRS} ${DCCL_INCLUDE_DIR})
	ENDIF() 

ENDIF(BUILD_DCCL)

#============================================================================
# List the subdirectories to build...
#============================================================================
IF(BUILD_DCCL)
  ADD_SUBDIRECTORY(lib_acommsMsgs)
ENDIF()
ADD_SUBDIRECTORY(pMoosKiller)
IF(BUILD_WOLFSSL)
  ADD_SUBDIRECTORY(pWolfEncrypt)
  ADD_SUBDIRECTORY(pWolfDecrypt)
ENDIF()

##############################################################################
#                           END of CMakeLists.txt
##############################################################################

