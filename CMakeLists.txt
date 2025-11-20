cmake_minimum_required(VERSION 3.20)
project(FaceID_System)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------- DEPENDÊNCIAS -------------------

find_package(dlib REQUIRED)

# --- MUDANÇA 1: Adicionei 'face' ---
# O componente 'face' é necessário para o reconhecimento facial (LBPH) que faremos depois
find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc objdetect face)

# ------------------- PROGRAMA 1: O Principal (main.cpp) -------------------

add_executable(face_id_app main.cpp)

target_include_directories(face_id_app PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${DLIB_INCLUDE_DIRS}
)

target_link_libraries(face_id_app PRIVATE dlib::dlib ${OpenCV_LIBS})

# ------------------- PROGRAMA 2: O Coletor (coletor.cpp) -------------------
# --- MUDANÇA 2: Adicionei o executável para tirar as fotos ---

add_executable(coletor coletor.cpp)

target_include_directories(coletor PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${DLIB_INCLUDE_DIRS}
)

target_link_libraries(coletor PRIVATE dlib::dlib ${OpenCV_LIBS})
# --- PROGRAMA 3: O Treinador ---
add_executable(treinamento treinamento.cpp)
target_include_directories(treinamento PRIVATE ${OpenCV_INCLUDE_DIRS} ${DLIB_INCLUDE_DIRS})
target_link_libraries(treinamento PRIVATE dlib::dlib ${OpenCV_LIBS})