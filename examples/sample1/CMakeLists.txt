################################################################################
#
# Example 1 -
#
##############################################################################ää

FILE	( GLOB example1_src
		${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	)

ADD_EXECUTABLE	( example1
			${example1_src}
		)

TARGET_LINK_LIBRARIES	( example1
				CANUtils_shared
			)

TARGET_INCLUDE_DIRECTORIES	( example1
					PRIVATE ${PROJECT_SOURCE_DIR}/src/
				)

INSTALL	( TARGETS example1
		EXPORT CANUtils_example1
		DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
