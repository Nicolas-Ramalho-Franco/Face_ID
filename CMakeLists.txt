cmake_minimum_required(VERSION 3.20)
project(FaceID_System)

# Define a versão do C++ e garante que ela seja usada
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------- ENCONTRAR DEPENDÊNCIAS -------------------

find_package(dlib REQUIRED)

# --- CORREÇÃO AQUI ---
# Adicionei 'objdetect' na lista. Ele contém o CascadeClassifier.
find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc objdetect)

# ------------------- CRIAR EXECUTÁVEL -------------------

add_executable(face_id_app main.cpp)

# ------------------- DEFINIR CAMINHOS DE INCLUSÃO -------------------

target_include_directories(face_id_app PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${DLIB_INCLUDE_DIRS}
)

# ------------------- VINCULAR BIBLIOTECAS -------------------

target_link_libraries(face_id_app PRIVATE dlib::dlib ${OpenCV_LIBS})