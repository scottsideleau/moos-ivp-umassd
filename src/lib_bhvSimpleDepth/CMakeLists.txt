#--------------------------------------------------------
# The CMakeLists.txt for:              lib_bhvSimpleDepth
# Author(s):                            Scott R. Sideleau
#--------------------------------------------------------

# Set System Specific Libraries
if (${WIN32})
  # Windows Libraries
  SET(SYSTEM_LIBS
      )
else (${WIN32})
  # Linux and Apple Libraries
  SET(SYSTEM_LIBS
      m )
endif (${WIN32})


MACRO(ADD_BHV BHV_NAME)
  ADD_LIBRARY(${BHV_NAME} SHARED ${BHV_NAME}.cpp)
  TARGET_LINK_LIBRARIES(${BHV_NAME} 
    helmivp
    behaviors 
    ivpbuild 
    logic 
    ivpcore 
    bhvutil 
    mbutil 
    geometry 
    ${SYSTEM_LIBS} )
ENDMACRO(ADD_BHV)


#--------------------------------------------------------
#                                         BHV_SimpleDepth
#--------------------------------------------------------
ADD_LIBRARY(BHV_SimpleDepth SHARED 
  BHV_SimpleDepth.cpp )
TARGET_LINK_LIBRARIES(BHV_SimpleDepth
   helmivp
   behaviors 
   ivpbuild 
   logic 
   ivpcore 
   bhvutil 
   mbutil 
   geometry 
   ${SYSTEM_LIBS} )
