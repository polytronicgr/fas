FIND_PATH(LIBAUBIO_INCLUDE_DIR NAMES aubio/aubio.h)
FIND_LIBRARY(LIBAUBIO_LIBRARIES NAMES aubio)

IF(LIBAUBIO_INCLUDE_DIR AND LIBAUBIO_LIBRARIES)
	SET(LIBAUBIO_FOUND TRUE)
ENDIF(LIBAUBIO_INCLUDE_DIR AND LIBAUBIO_LIBRARIES)

IF(LIBAUBIO_FOUND)
	IF (NOT LibAubio_FIND_QUIETLY)
		MESSAGE(STATUS "Found libaubio includes: ${LIBAUBIO_INCLUDE_DIR}/libaubio.h")
		MESSAGE(STATUS "Found libaubio library: ${LIBAUBIO_LIBRARIES}")
	ENDIF (NOT LibAubio_FIND_QUIETLY)
ELSE(LIBAUBIO_FOUND)
	IF (LibAubio_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could NOT find libaubio development files")
	ENDIF (LibAubio_FIND_REQUIRED)
ENDIF(LIBAUBIO_FOUND)