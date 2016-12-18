#--------------------------------------------------------
# The CMakeLists.txt for:               pMoosKiller
# Author(s):                            Scott R. Sideleau
#--------------------------------------------------------

FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE(pMoosKiller ${SRC})

TARGET_LINK_LIBRARIES(pMoosKiller
    MOOS
    m
    pthread)

