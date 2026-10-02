cmake_minimum_required(VERSION 3.15)
project(SimuladorBiotSavart VERSION 1.0 LANGUAGES CXX)

# Exigir C++17 como estándar mínimo para características modernas
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Activar advertencias del compilador para código estricto y profesional
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Definir dónde buscar los headers (.hpp / .h)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Recopilar todos los archivos fuente (.cpp)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Generar el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})