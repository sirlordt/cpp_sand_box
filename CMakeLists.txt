cmake_minimum_required(VERSION 3.14)

project(main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#Clang++ require the next line to debug std::string
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")

# add_executable(main
#  test01.hpp
#  Vector.hpp
#  SystemUtilities.hpp
#  CallStackFrame.hpp
#  main.cpp
# )

# add_executable(main
#   Vector.hpp
#   SystemUtilities.hpp
#   main5.cpp
# )

# add_executable(main
#   main6.cpp
#   Vector.hpp
# )

# add_executable(main
#   main7.cpp
#   Vector.hpp
#   String.hpp
# )

# add_executable(main
#   main12_CallStackFrameAllocator.cpp
#   CallStackFrame.hpp
#   CommonUtilities.hpp
# )

# add_executable(main
#   main13_ExceptionAllocator.cpp
#   CallStackFrame.hpp
#   Exception.hpp
#   CommonUtilities.hpp
# )

add_executable(main
  main14_type_name_static_string.cpp
)

#target_link_options(main PRIVATE --static -static-libgcc -static-libstdc++)

#dw bfd dwarf elf
target_link_libraries(main PRIVATE rt dl m pthread)
#target_link_libraries(main PRIVATE dw unwind supc++)
#target_link_libraries(main PRIVATE dw unwind supc++)

