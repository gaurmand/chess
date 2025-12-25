set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(chessapp
    src/main.cpp
    src/mainwindow.cpp
)

target_link_libraries(chessapp PRIVATE Qt6::Core Qt6::Widgets)
