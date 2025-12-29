cmake_minimum_required(VERSION 4.1)
project(Astar)

set(CMAKE_CXX_STANDARD 23)

# --- 1. CONFIGURATION QT (La Magie) ---
set(CMAKE_AUTOMOC ON) # Gère les Signaux/Slots automatiquement
set(CMAKE_AUTORCC ON) # Gère les ressources (icônes...)
set(CMAKE_AUTOUIC ON) # Gère les fichiers .ui (interface designer)

# On cherche Qt 6 sur ton PC
# Si CMake ne le trouve pas, il faudra définir CMAKE_PREFIX_PATH (voir plus bas)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui OpenGLWidgets)

# --- 2. TES MODULES (Inchangés) ---
# (Je garde tes modules existants tels quels)
add_library(file_reader_surgeon)
add_library(calculator)
add_library(GEOJson_exporter)
add_library(pathfinder_engine)
add_library(pathfinder_engine_bidirectional)

target_sources(file_reader_surgeon PUBLIC FILE_SET CXX_MODULES FILES modules/file_reader_surgeon.ixx)
target_sources(calculator PUBLIC FILE_SET CXX_MODULES FILES modules/calculator.ixx)
target_sources(pathfinder_engine PUBLIC FILE_SET CXX_MODULES FILES modules/pathfinder_engine.ixx)
target_sources(pathfinder_engine_bidirectional PUBLIC FILE_SET CXX_MODULES FILES modules/pathfinder_engine_bidirectional.ixx)
target_sources(GEOJson_exporter PUBLIC FILE_SET CXX_MODULES FILES modules/GEOJson_exporter.ixx)

target_link_libraries(calculator PRIVATE file_reader_surgeon)
target_link_libraries(pathfinder_engine PRIVATE calculator file_reader_surgeon)
target_link_libraries(pathfinder_engine_bidirectional PRIVATE file_reader_surgeon calculator pathfinder_engine)
target_link_libraries(GEOJson_exporter PRIVATE file_reader_surgeon pathfinder_engine)

# --- 3. L'EXECUTABLE ---
add_executable(Astar main.cpp
    ui/mainwindow.cpp
    ui/mainwindow.h
    modules/engine/engine.cpp
    modules/engine/engine.h
    ui/MapView.h
    ui/MapView.cpp)

# On lie tout ensemble
target_link_libraries(Astar PRIVATE
        Qt6::Widgets
        Qt6::Core
        Qt6::Gui
        Qt6::OpenGLWidgets

        # Tes modules
        file_reader_surgeon
        pathfinder_engine
        pathfinder_engine_bidirectional
        GEOJson_exporter
)