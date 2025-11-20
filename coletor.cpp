#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <direct.h> 

using namespace cv;
using namespace std;

int main() {
    // --- CONFIGURAÇÃO ---
    // Se quiser usar um arquivo de video gravado no celular, coloque o nome dele aqui:
    // Exemplo: VideoCapture cap("meu_video.mp4");
    // Se for webcam, deixe 0.
    VideoCapture cap(0); 

    if (!cap.isOpened()) {
        cout << "Erro ao abrir camera ou arquivo de video!" << endl;
        return -1;
    }

    CascadeClassifier face_cascade;
    if (!face_cascade.load("haarcascade_frontalface_default.xml")) {
        cout << "ERRO: XML nao encontrado." << endl;
        return -1;
    }
    _mkdir("dados");

    Mat frame, gray;
    int count = 0;
    int id;
    int max_fotos = 80; // Define quantas fotos quer pegar (50 é um bom número)

    cout << "Digite o numero do ID para cadastro: ";
    cin >> id;
    
    cout << "--- ATENCAO ---" << endl;
    cout << "A captura sera RAPIDA (tipo video)." << endl;
    cout << "Assim que a camera abrir, MEXA A CABECA devagar (lados, cima, baixo)." << endl;
    cout << "Pressione qualquer tecla para comecar..." << endl;
    
    // Pausa para você se preparar antes de começar
    system("pause"); 

    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        vector<Rect> faces;
        
        // Detecção ajustada
        face_cascade.detectMultiScale(gray, faces, 1.2, 5, 0, Size(100, 100));

        for (auto& face : faces) {
            // Só salva se o rosto não estiver muito na borda (evita cortes)
            if (face.x < 10 || face.y < 10 || (face.x + face.width) > frame.cols - 10) continue;

            count++;
            
            // Salva a foto
            string nome_arquivo = "dados/user." + to_string(id) + "." + to_string(count) + ".jpg";
            imwrite(nome_arquivo, gray(face));

            // Desenha na tela visualmente
            rectangle(frame, face, Scalar(0, 255, 0), 2);
            putText(frame, to_string(count) + "/" + to_string(max_fotos), 
                    Point(face.x, face.y-10), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0,255,0), 2);
        }

        imshow("Captura Rapida", frame);

        // AQUI ESTÁ O SEGREDO: waitKey(1) é apenas 1 milissegundo.
        // Ele não espera nada, processa o mais rápido que o PC aguentar.
        if (waitKey(1) == 27 || count >= max_fotos) break;
    }
    
    cout << "Pronto! " << count << " frames capturados." << endl;
    return 0;
}