#
# Doxygen configuration (documentation)
#

#set(DOXY_DEPS_DIRECTORY ${PROJECT_SOURCE_DIR}/deps)
set(DOXY_SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
set(DOXY_DOCS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(DOXY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

configure_file(doxy.in doxy.cfg @ONLY)

message("Doxygen build started")

add_custom_target(
    docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXY_OUTPUT_DIRECTORY}/doxy.cfg
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
    SOURCES
        dox/extra.dox
        md/main.md       
        doxy.in
)


