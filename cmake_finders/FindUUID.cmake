
find_path(LIBUUID_INCLUDE_DIRS uuid/uuid.h
/usr/local/include
/usr/include
)

find_library(LIBUUID_LIBRARIES
  NAMES uuid
  PATHS /lib /usr/lib /usr/local/lib
  )
  
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBUUID DEFAULT_MSG LIBUUID_LIBRARIES LIBUUID_INCLUDE_DIRS)
mark_as_advanced(LIBUUID_INCLUDE_DIRS LIBUUID_LIBRARIES)