#include <iostream>
#include <dlib/image_processing.h>
#include <opencv2/opencv.hpp>

int main() {
    std::cout << "Ambiente Configurado com Sucesso!" << std::endl;
    
    // Teste rápido do OpenCV
    cv::Mat imagem_teste = cv::Mat::zeros(100, 100, CV_8UC3);
    std::cout << "OpenCV carregou. Matriz criada." << std::endl;

    // Teste rápido do Dlib
    dlib::rectangle retangulo(0, 0, 100, 100);
    std::cout << "Dlib carregou. Retangulo criado." << std::endl;

    return 0;
}