include(FetchContent)

FetchContent_Declare(
  spdlog
  SYSTEM
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG 6fa36017cfd5731d617e1a934f0e5ea9c4445b13 # 1.15.3
)
set(SPDLOG_BUILD_PIC ON)
FetchContent_MakeAvailable(spdlog)

# Add the demo executables
file(GLOB DEMO_SOURCES ./*.cpp)
foreach(DEMO_SOURCE ${DEMO_SOURCES})
  get_filename_component(DEMO_NAME ${DEMO_SOURCE} NAME_WE)

  message(STATUS "Building demo: ${DEMO_NAME}")

  # The error_recovery_and_logging demo requires spdlog
  if(${DEMO_NAME} STREQUAL "error_recovery_and_logging")
    add_executable(${DEMO_NAME} ${DEMO_SOURCE})
    target_link_libraries(${DEMO_NAME} PRIVATE libtrossen_arm spdlog::spdlog)
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
