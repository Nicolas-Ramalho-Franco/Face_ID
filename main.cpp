#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>
#include <vector>
#include <map>

using namespace std;
using namespace cv;
using namespace cv::face;

// Variáveis globais
Ptr<FaceDetectorYN> detector;
Ptr<FaceRecognizerSF> recognizer;

// Mapa para guardar ID -> Nome automaticamente
map<int, string> nomes_conhecidos;

// Função para carregar as fotos e extrair ID e NOME do arquivo
map<int, Mat> carregarBancoDeDados() {
    map<int, vector<Mat>> amostras;
    map<int, Mat> banco_final;
    vector<String> filenames;
    
    cout << "Carregando e lendo nomes da pasta 'dados'..." << endl;
    glob("dados/*.jpg", filenames, false);

    if (filenames.empty()) {
        cout << "AVISO: Nenhuma foto encontrada em 'dados'." << endl;
        return banco_final;
    }

    for (auto fn : filenames) {
        // O arquivo agora é: dados\user.1.Joao.5.jpg
        
        // 1. Achar onde começa "user."
        size_t pos_user = fn.find("user.");
        if (pos_user == string::npos) continue;

        // Pula "user." (5 letras)
        string resto = fn.substr(pos_user + 5); 
        // Agora temos: 1.Joao.5.jpg

        // 2. Pegar o ID (pega tudo até o primeiro ponto)
        size_t primeiro_ponto = resto.find('.');
        string str_id = resto.substr(0, primeiro_ponto);
        int id = stoi(str_id);

        // 3. Pegar o NOME (está entre o primeiro e o segundo ponto)
        string resto_sem_id = resto.substr(primeiro_ponto + 1); // Joao.5.jpg
        size_t segundo_ponto = resto_sem_id.find('.');
        string nome_extraido = resto_sem_id.substr(0, segundo_ponto);

        // Salva o nome no mapa global se ainda não salvou
        if (nomes_conhecidos.find(id) == nomes_conhecidos.end()) {
            nomes_conhecidos[id] = nome_extraido;
            cout << "Detectado: ID " << id << " = " << nome_extraido << endl;
        }

        Mat img = imread(fn);
        if (img.empty()) continue;

        // Detecta rosto na foto de treino
        detector->setInputSize(img.size());
        Mat faces;
        detector->detect(img, faces);

        if (faces.rows < 1) continue;

        Mat aligned_face, feature;
        recognizer->alignCrop(img, faces.row(0), aligned_face);
        recognizer->feature(aligned_face, feature);

        amostras[id].push_back(feature.clone());
    }

    // Tira média das características
    for (auto const& [id, features] : amostras) {
        Mat media = Mat::zeros(features[0].size(), features[0].type());
        for (auto feature : features) {
            media += feature;
        }
        media /= (double)features.size();
        banco_final[id] = media;
    }

    return banco_final;
}

int main() {
    // 1. Carregar Modelos ONNX
    String pathDetector = "face_detection_yunet_2023mar.onnx";
    String pathRecognizer = "face_recognition_sface_2021dec.onnx";

    detector = FaceDetectorYN::create(pathDetector, "", Size(320, 320), 0.9f, 0.3f, 5000);
    recognizer = FaceRecognizerSF::create(pathRecognizer, "");

    if (detector.empty() || recognizer.empty()) {
        cerr << "ERRO: Arquivos ONNX nao encontrados!" << endl;
        return -1;
    }

    // 2. Aprender rostos
    map<int, Mat> banco_de_dados = carregarBancoDeDados();

    // 3. Iniciar Camera
    VideoCapture cap(0);
    if (!cap.isOpened()) return -1;

    Mat frame;
    cout << "--- SISTEMA INICIADO ---" << endl;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        detector->setInputSize(frame.size());
        Mat faces;
        detector->detect(frame, faces);

        for (int i = 0; i < faces.rows; i++) {
            int x = (int)faces.at<float>(i, 0);
            int y = (int)faces.at<float>(i, 1);
            int w = (int)faces.at<float>(i, 2);
            int h = (int)faces.at<float>(i, 3);

            Mat aligned_face, feature;
            recognizer->alignCrop(frame, faces.row(i), aligned_face);
            recognizer->feature(aligned_face, feature);

            int id_encontrado = -1;
            double maior_score = 0.0;

            for (auto const& [id, feat_db] : banco_de_dados) {
                double score = recognizer->match(feature, feat_db, FaceRecognizerSF::FR_COSINE);
                if (score > maior_score) {
                    maior_score = score;
                    id_encontrado = id;
                }
            }

            Scalar cor = Scalar(0, 0, 255); 
            string nome_display = "DESCONHECIDO";

            // Se a confiança for boa
            if (maior_score > 0.4) {
                cor = Scalar(0, 255, 0); 
                
                // Busca o nome que lemos automaticamente dos arquivos
                if (nomes_conhecidos.count(id_encontrado)) {
                    nome_display = nomes_conhecidos[id_encontrado];
                } else {
                    nome_display = "ID " + to_string(id_encontrado);
                }
            }

            rectangle(frame, Rect(x, y, w, h), cor, 2);
            
            string texto = nome_display + " (" + to_string((int)(maior_score * 100)) + "%)";
            putText(frame, texto, Point(x, y - 10), FONT_HERSHEY_SIMPLEX, 0.6, cor, 2);
        }

        imshow("OpenCV DNN Face ID", frame);
        if (waitKey(1) == 27) break;
    }
    return 0;
}