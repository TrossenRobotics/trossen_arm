# Add the demo executables
file(GLOB DEMO_SOURCES ./*.cpp)
foreach(DEMO_SOURCE ${DEMO_SOURCES})
  get_filename_component(DEMO_NAME ${DEMO_SOURCE} NAME_WE)

  message(STATUS "Building demo: ${DEMO_NAME}")

  # The error_recovery_and_logging demo requires spdlog
  if(${DEMO_NAME} STREQUAL "error_recovery_and_logging")
    find_package(spdlog)
    # If spdlog is not found, warn the user and continue without building this demo
    if(NOT spdlog_FOUND)
      message(WARNING "spdlog not found, the error_recovery_and_logging demo will not be built.")
      continue()
    else()
      add_executable(${DEMO_NAME} ${DEMO_SOURCE})
      target_link_libraries(${DEMO_NAME} PRIVATE libtrossen_arm spdlog::spdlog)
    endif()
  else()
    # Other demos do not require spdlog
    add_executable(${DEMO_NAME} ${DEMO_SOURCE})
    target_link_libraries(${DEMO_NAME} PRIVATE libtrossen_arm)
  endif()

  install(
    TARGETS ${DEMO_NAME}
    RUNTIME DESTINATION bin
  )
endforeach()
