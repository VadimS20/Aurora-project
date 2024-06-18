# CMakeList.txt: проект CMake для htttplib_client; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("htttplib_client")

include_directories("D:/git/vckpg/vcpkg/installed/x64-windows/include")
link_directories("D:/git/vckpg/vcpkg/installed/x64-windows/lib")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (htttplib_client "htttplib_client.cpp" "htttplib_client.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET htttplib_client PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
