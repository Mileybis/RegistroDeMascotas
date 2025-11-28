# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/RegistroDeMascotas_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/RegistroDeMascotas_autogen.dir/ParseCache.txt"
  "RegistroDeMascotas_autogen"
  )
endif()
