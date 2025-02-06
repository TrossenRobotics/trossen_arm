@PACKAGE_INIT@

set(PACKAGE_INCLUDE_DIR "@PACKAGE_INCLUDE_DIR@")
set(PACKAGE_LIB_DIR "@PACKAGE_LIB_DIR@")

if(NOT TARGET libtrossen_arm)
  add_library(libtrossen_arm STATIC IMPORTED)
  set_target_properties(libtrossen_arm PROPERTIES
    IMPORTED_LOCATION "@PACKAGE_LIB_DIR@/libtrossen_arm.a"
    INTERFACE_INCLUDE_DIRECTORIES "@PACKAGE_INCLUDE_DIR@"
  )
endif()
