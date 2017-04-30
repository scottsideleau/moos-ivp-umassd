#--------------------------------------------------------
# The CMakeLists.txt for:               pWolfEncrypt
# Author(s):                            Scott R. Sideleau
#--------------------------------------------------------

FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE(pWolfEncrypt ${SRC})

TARGET_LINK_LIBRARIES(pWolfEncrypt
    MOOS
    MOOSGeodesy
    wolfssl
    m
    pthread)

