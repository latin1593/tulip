CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(sip)

FIND_PACKAGE(PythonLibs REQUIRED)
FIND_PACKAGE(PythonInterp REQUIRED)

IF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

SET(sip_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/export.c
    ${CMAKE_CURRENT_SOURCE_DIR}/extracts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gencode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/heap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lexer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.c
    ${CMAKE_CURRENT_SOURCE_DIR}/transform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/type_hints.c
    ${CMAKE_CURRENT_BINARY_DIR}/../siplib/sip.h
)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/../siplib/sip.h
                   COMMAND ${PYTHON_EXECUTABLE} -B ${CMAKE_CURRENT_SOURCE_DIR}/../configure.py --sip-module ${SIP_LIB}
                                                -b ${CMAKE_CURRENT_BINARY_DIR}/../sipgen
                                                -e ${CMAKE_CURRENT_BINARY_DIR}/../siplib
                                                -d ${CMAKE_CURRENT_BINARY_DIR}/../siplib

                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../siplib/sip.h.in
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../ VERBATIM)

ADD_EXECUTABLE(sip ${sip_SRCS})

ENDIF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
