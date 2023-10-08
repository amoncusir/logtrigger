cmake_minimum_required(VERSION 3.22)

PROJECT(logtrigger
        VERSION 0.0.1
        LANGUAGES CXX C
        )

ADD_DEFINITIONS(-Os -Wall -Werror -Wmissing-declarations)

IF (DEBUG)
    ADD_DEFINITIONS(-DDEBUG -g3)
ENDIF ()

if (NOT DEFINED CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
    SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
endif ()

if (NOT DEFINED CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS ${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS})
endif ()

ADD_EXECUTABLE(${PROJECT_NAME}
        util.cpp
        event.cpp
        trigger.cpp
        args.cpp
        context.cpp

        ubus.c

        logtrigger.cpp
        )

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ubox
        ubus
        ${CMAKE_DL_LIBS}
        )

INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION sbin)
