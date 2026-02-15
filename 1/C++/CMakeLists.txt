cmake_minimum_required(VERSION 3.15)
project(CrowServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Скачиваем Crow
include(FetchContent)
FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.0.5
)
FetchContent_MakeAvailable(crow)

# Скачиваем Asio (нужен для Crow)
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-28-0
)
FetchContent_MakeAvailable(asio)

add_executable(crow_server main.cpp)

target_include_directories(crow_server PRIVATE ${asio_SOURCE_DIR}/include)
target_link_libraries(crow_server Crow::Crow)

# Включаем поддержку сжатия (опционально)
target_compile_definitions(crow_server PRIVATE CROW_ENABLE_COMPRESSION)