#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace cv::face;
using namespace std;

int main() {
    // --- 1. CARREGAR O DETECTOR DE ROSTOS (HAAR CASCADE) ---
    CascadeClassifier face_cascade;
    if (!face_cascade.load("haarcascade_frontalface_default.xml")) {
        cout << "ERRO CRITICO: Arquivo xml nao encontrado na pasta do executavel." << endl;
        return -1;
    }

    // --- 2. CARREGAR O CEREBRO TREINADO (LBPH) ---
    Ptr<LBPHFaceRecognizer> recognizer = LBPHFaceRecognizer::create();
    try {
        recognizer->read("treinamento.yml");
    } catch (cv::Exception& e) {
        cout << "ERRO CRITICO: Arquivo 'treinamento.yml' nao encontrado." << endl;
        cout << "Voce rodou o 'treinamento.exe' depois de tirar as fotos?" << endl;
        return -1;
    }

    // --- 3. INICIAR CAMERA ---
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Erro ao abrir a webcam." << endl;
        return -1;
    }

    Mat frame, gray;
    cout << "Sistema Iniciado! Pressione ESC para sair." << endl;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Converter para cinza
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        
        vector<Rect> faces;
        
        // --- AJUSTE ANTI-FALSOS POSITIVOS ---
        // 1.2 -> ScaleFactor
        // 8   -> MinNeighbors (Alto para evitar detectar objetos como rostos)
        // Size(120, 120) -> Tamanho minimo (Ignora coisas pequenas no fundo)
        face_cascade.detectMultiScale(gray, faces, 1.2, 8, 0, Size(120, 120));

        for (size_t i = 0; i < faces.size(); i++) {
            int id = -1;
            double confidence = 0.0;
            
            // O computador tenta adivinhar quem é
            recognizer->predict(gray(faces[i]), id, confidence);

            string nome;
            Scalar cor;

            // --- CALIBRAGEM DE CONFIANÇA ---
            // LBPH: Quanto MENOR o numero, MAIS PARECIDO é.
            // 0 = Identico | 40 = Muito parecido | 80 = Diferente
            // Tente ajustar esse 60 para mais ou menos conforme o ambiente.
            int limite_confianca = 60; 

            if (confidence < limite_confianca) {
                // --- PESSOA CONHECIDA ---
                cor = Scalar(0, 255, 0); // Verde
                
                if (id == 1) {
                    nome = "NICOLAS";
                } 
                else if (id == 2) {
                    nome = "USUARIO 2"; // Mude aqui para o nome do seu amigo
                } 
                else {
                    nome = "CADASTRO " + to_string(id);
                }
                
            } else {
                // --- PESSOA DESCONHECIDA ---
                cor = Scalar(0, 0, 255); // Vermelho
                nome = "DESCONHECIDO";
            }

            // Desenha o retângulo
            rectangle(frame, faces[i], cor, 2);

            // Escreve o nome e a confiança na tela para você monitorar
            string texto_tela = nome + " (" + to_string((int)confidence) + ")";
            putText(frame, texto_tela, Point(faces[i].x, faces[i].y - 10), 
                    FONT_HERSHEY_SIMPLEX, 0.7, cor, 2);
        }

        imshow("Sistema de Reconhecimento", frame);

        // Sai com ESC (código 27)
        if (waitKey(30) == 27) break;
    }

    return 0;
}