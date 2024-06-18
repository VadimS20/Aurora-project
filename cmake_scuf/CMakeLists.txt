# CMakeList.txt: проект CMake для cmake_skuf_test; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("cmake_skuf_test")

# Добавьте источник в исполняемый файл этого проекта.

include_directories("D:/git/vckpg/vcpkg/installed/x64-windows/include")
link_directories("D:/git/vckpg/vcpkg/installed/x64-windows/lib")

add_executable (cmake_skuf_test "cmake_skuf_test.cpp" "cmake_skuf_test.h" "ScannerClass.cpp" "ScannerClass.h" "TextClass.h" "TextClass.cpp" "ServerClass.h" "ServerClass.cpp")


target_link_libraries(cmake_skuf_test PRIVATE  tesseract53 ${OpenCV_LIBS} ${Tesseract_LIBRARIES} ${Boost_LIBRARIES})


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cmake_skuf_test PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
