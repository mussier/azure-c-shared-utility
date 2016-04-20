
find_path(LIBRT_INCLUDE_DIRS time.h
/usr/local/include
/usr/include
)

find_library(LIBRT_LIBRARIES
  NAMES rt
  PATHS /lib /usr/lib /usr/local/lib
  )
  
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBUUID DEFAULT_MSG LIBUUID_LIBRARIES LIBUUID_INCLUDE_DIRS)
mark_as_advanced(LIBRT_INCLUDE_DIRS LIBRT_LIBRARIES)