cmake_minimum_required(VERSION 3.20)
project(FaceID_System)

# Define a versão do C++ (C++17 é um bom padrão moderno)
set(CMAKE_CXX_STANDARD 17)

# --- Encontrar os pacotes instalados pelo Vcpkg ---
find_package(dlib REQUIRED)
find_package(OpenCV REQUIRED)

# --- Criar o Executável ---
# Aqui dizemos que o programa "face_id_app" será criado a partir do arquivo "main.cpp"
add_executable(face_id_app main.cpp)

# --- Linkar as bibliotecas ao seu programa ---
target_link_libraries(face_id_app PRIVATE
    dlib::dlib
    ${OpenCV_LIBS}
)