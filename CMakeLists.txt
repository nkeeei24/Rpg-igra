cmake_minimum_required(VERSION 3.16)
project(TextRPG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(TextRPG
    src/main.cpp
    src/core/Game.cpp
    src/core/Character.cpp
    src/core/World.cpp
    src/core/CombatSystem.cpp
    src/core/DialogueSystem.cpp
    src/core/QuestSystem.cpp
    src/core/Inventory.cpp
    src/core/SaveSystem.cpp
    src/systems/UI.cpp
    src/systems/Dice.cpp
    src/content/ContentFactory.cpp
)
