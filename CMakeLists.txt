cmake_minimum_required(VERSION 3.10)
project(JeuPlateforme VERSION 1.0 LANGUAGES CXX)

# Standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver SFML
find_package(SFML 2.5 COMPONENTS graphics audio system window REQUIRED)

# Fichiers sources
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/Player.cpp
    src/Obstacle.cpp
    src/AirObstacle.cpp
    src/GroundObstacle.cpp
    src/GameWorld.cpp
    src/Reward.cpp
    src/Bunker.cpp
    src/Menu.cpp
    src/LevelSelect.cpp
)

# Exécutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Inclure les en-têtes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Lier SFML
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio sfml-system sfml-window)

# Copier les ressources après compilation
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
