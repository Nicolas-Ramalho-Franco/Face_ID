#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

int main() {
    cv::CascadeClassifier face_cascade;
    
    if (!face_cascade.load("haarcascade_frontalface_default.xml")) {
        std::cout << "ERRO: XML nao encontrado." << std::endl;
        return -1;
    }

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "Erro na camera" << std::endl;
        return -1;
    }

    cv::Mat frame, frame_gray;
    
    std::cout << "Iniciando... diga XXXXXXXXXX!" << std::endl;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(frame_gray, frame_gray);

        std::vector<cv::Rect> faces;
        
        // --- MUDANÇA AQUI ---
        // Mudei minNeighbors de 5 para 3 (fica mais sensível)
        // ScaleFactor 1.1 (analisa a imagem reduzindo 10% a cada passada)
        face_cascade.detectMultiScale(frame_gray, faces, 1.2, 5, 0, cv::Size(100, 100));

        // --- DEBUG NO TERMINAL ---
        // Se achar rosto, escreve no terminal
        if (faces.size() > 0) {
            std::cout << "Rostos detectados: " << faces.size() << "\r"; // \r sobrescreve a linha
        }

        for (size_t i = 0; i < faces.size(); i++) {
            cv::rectangle(frame, faces[i], cv::Scalar(0, 255, 0), 2);
            
            // Adicionei um texto em cima do retângulo
            cv::putText(frame, "ROSTO", cv::Point(faces[i].x, faces[i].y - 10), 
                        cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("Reconhecimento Facial", frame);

        if (cv::waitKey(30) == 27) break;
    }

    return 0;
}