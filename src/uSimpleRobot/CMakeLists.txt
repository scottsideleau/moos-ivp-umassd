#--------------------------------------------------------
# The CMakeLists.txt for:               uSimpleRobot
# Author(s):                            Scott R. Sideleau
#--------------------------------------------------------

FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE(uSimpleRobot ${SRC})

TARGET_LINK_LIBRARIES(uSimpleRobot
    ${MOOS_LIBRARIES}
    ${MOOSGeodesy_LIBRARIES}
    m
    pthread)

