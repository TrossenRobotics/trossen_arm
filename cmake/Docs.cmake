# Build documentation if requested
if(BUILD_DOCS)
  find_package(Doxygen)
  if(NOT Doxygen_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()
  add_custom_target(
    doc ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/docs/Doxyfile
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
endif()
