cmake_minimum_required(VERSION 3.5)
set(CMAKE_OSX_ARCHITECTURES x86_64) #Apple Silicon
list(APPEND CMAKE_FRAMEWORK_PATH /Library/Frameworks) #Mac, why not auto?
project(Jeu LANGUAGES CXX)

find_package(Imagine REQUIRED COMPONENTS Graphics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Jeu main.cpp
    physics.h
    physics.cpp
    vector.h
    vector.cpp
    affichage.h
    affichage.cpp
    player.h
    player.cpp
    niveaux.h
    niveaux.cpp
    globals.h
    element.h)

target_link_libraries(Jeu Imagine::Graphics)

include(GNUInstallDirs)
install(TARGETS Jeu
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
