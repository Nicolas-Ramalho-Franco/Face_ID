#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <direct.h> 

using namespace cv;
using namespace std;

int main() {
    VideoCapture cap(0); 

    if (!cap.isOpened()) {
        cout << "Erro ao abrir camera!" << endl;
        return -1;
    }

    CascadeClassifier face_cascade;
    // Certifique-se que este arquivo xml esta na pasta do executavel
    if (!face_cascade.load("haarcascade_frontalface_default.xml")) {
        cout << "ERRO: XML nao encontrado." << endl;
        return -1;
    }
    _mkdir("dados");

    Mat frame, gray;
    int count = 0;
    int id;
    string nome;
    int max_fotos = 50; 

    cout << "Digite o numero do ID (ex: 1): ";
    cin >> id;

    cout << "Digite o NOME da pessoa (Sem espacos, ex: Joao): ";
    cin >> nome;
    
    cout << "--- ATENCAO ---" << endl;
    cout << "Olhe para a camera e mexa a cabeca levemente." << endl;
    cout << "Pressione qualquer tecla para comecar..." << endl;
    system("pause"); 

    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        vector<Rect> faces;
        
        face_cascade.detectMultiScale(gray, faces, 1.2, 5, 0, Size(100, 100));

        for (auto& face : faces) {
            // Evita fotos muito na borda
            if (face.x < 10 || face.y < 10 || (face.x + face.width) > frame.cols - 10) continue;

            count++;
            
            // --- AQUI ESTA A MUDANCA ---
            // Formato: user.ID.NOME.FOTO.jpg
            string nome_arquivo = "dados/user." + to_string(id) + "." + nome + "." + to_string(count) + ".jpg";
            
            imwrite(nome_arquivo, gray(face));

            rectangle(frame, face, Scalar(0, 255, 0), 2);
            putText(frame, to_string(count) + "/" + to_string(max_fotos), 
                    Point(face.x, face.y-10), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0,255,0), 2);
        }

        imshow("Captura Rapida", frame);

        if (waitKey(1) == 27 || count >= max_fotos) break;
    }
    
    cout << "Pronto! Fotos salvas para o usuario: " << nome << endl;
    return 0;
}