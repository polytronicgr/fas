FIND_PATH(LIBLFDS711_INCLUDE_DIR NAMES liblfds711.h PATHS ${ROOT}/lib/liblfds7.1.1/liblfds711/inc NO_DEFAULT_PATH)
FIND_LIBRARY(LIBLFDS711_LIBRARIES NAMES lfds711 PATHS ${ROOT}/lib/liblfds7.1.1/liblfds711/bin NO_DEFAULT_PATH)

MESSAGE(STATUS "${LIBLFDS711_LIBRARIES}")

IF(LIBLFDS711_INCLUDE_DIR AND LIBLFDS711_LIBRARIES)
	SET(LIBLFDS711_FOUND TRUE)
ENDIF(LIBLFDS711_INCLUDE_DIR AND LIBLFDS711_LIBRARIES)

IF(LIBLFDS711_FOUND)
	IF (NOT Liblfds711_FIND_QUIETLY)
		MESSAGE(STATUS "Found liblfds711 includes: ${LIBLFDS711_INCLUDE_DIR}/liblfds711.h")
		MESSAGE(STATUS "Found liblfds711 library: ${LIBLFDS711_LIBRARIES}")
	ENDIF (NOT Liblfds711_FIND_QUIETLY)
ELSE(LIBLFDS711_FOUND)
	IF (Liblfds711_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could NOT find liblfds711 development files")
	ENDIF (Liblfds711_FIND_REQUIRED)
ENDIF(LIBLFDS711_FOUND)