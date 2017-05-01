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

#============================================================================
# List the subdirectories to build...
#============================================================================
#ADD_SUBDIRECTORY(lib_behaviors-test)
ADD_SUBDIRECTORY(pMoosKiller)
IF(BUILD_WOLFSSL)
  ADD_SUBDIRECTORY(pWolfEncrypt)
  ADD_SUBDIRECTORY(pWolfDecrypt)
ENDIF()

##############################################################################
#                           END of CMakeLists.txt
##############################################################################

