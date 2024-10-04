#--------------------------------------------------------
# The CMakeLists.txt for:               pWolfDecrypt
# Author(s):                            Scott R. Sideleau
#--------------------------------------------------------

FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE(pWolfDecrypt ${SRC})

TARGET_LINK_LIBRARIES(pWolfDecrypt
    ${MOOS_LIBRARIES}
    ${MOOSGeodesy_LIBRARIES}
    wolfssl
    m
    pthread)

