#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "iniparser-static" for configuration ""
set_property(TARGET iniparser-static APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(iniparser-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libiniparser.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS iniparser-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_iniparser-static "${_IMPORT_PREFIX}/lib/libiniparser.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
