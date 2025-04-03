# Add the demo executables
file(GLOB DEMO_SOURCES ./*.cpp)
find_package(yaml-cpp REQUIRED)
foreach(DEMO_SOURCE ${DEMO_SOURCES})
  get_filename_component(DEMO_NAME ${DEMO_SOURCE} NAME_WE)
  add_executable(${DEMO_NAME} ${DEMO_SOURCE})
  target_link_libraries(${DEMO_NAME} PRIVATE libtrossen_arm ${YAML_CPP_LIBRARIES})
  install(
    TARGETS ${DEMO_NAME}
    RUNTIME DESTINATION bin
  )
endforeach()
