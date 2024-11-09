set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall)

if(MONOLITH_DEVELOPER)
    add_compile_options(-Werror)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# Установка флагов компилятора для поддержки UTF-8
if(MSVC)
    add_compile_options(/utf-8)  # Для Microsoft Visual Studio
else()
    add_compile_options(-fexec-charset=UTF-8)  # Для GCC/Clang
endif()

