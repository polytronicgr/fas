FIND_PATH(LIBFAUST_INCLUDE_DIR NAMES libfaust.h PATHS /usr/local/include/faust/*)
FIND_LIBRARY(LIBFAUST_LIBRARIES NAMES libfaust.a PATHS /usr/local/lib/)

IF(LIBFAUST_INCLUDE_DIR AND LIBFAUST_LIBRARIES)
	SET(LIBFAUST_FOUND TRUE)
ENDIF(LIBFAUST_INCLUDE_DIR AND LIBFAUST_LIBRARIES)

IF(LIBFAUST_FOUND)
	IF (NOT LibFaust_FIND_QUIETLY)
		MESSAGE(STATUS "Found libfaust includes: ${LIBFAUST_INCLUDE_DIR}/libfaust.h")
		MESSAGE(STATUS "Found libfaust library: ${LIBFAUST_LIBRARIES}")
	ENDIF (NOT LibFaust_FIND_QUIETLY)
ELSE(LIBFAUST_FOUND)
	IF (LibFaust_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could NOT find libfaust development files")
	ENDIF (LibFaust_FIND_REQUIRED)
ENDIF(LIBFAUST_FOUND)