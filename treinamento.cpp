#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace cv;
using namespace cv::face;
using namespace std;

int main() {
    // Pega todas as imagens da pasta 'dados'
    vector<String> filenames;
    glob("dados/*.jpg", filenames, false);

    if (filenames.empty()) {
        cout << "ERRO: Nenhuma foto encontrada na pasta 'dados'!" << endl;
        return -1;
    }

    vector<Mat> images;
    vector<int> labels;

    cout << "Lendo " << filenames.size() << " fotos para treinamento..." << endl;

    for (size_t i = 0; i < filenames.size(); i++) {
        Mat img = imread(filenames[i], IMREAD_GRAYSCALE);
        if (img.empty()) continue;

        // Extrai o ID do nome do arquivo (ex: dados\user.1.5.jpg -> ID = 1)
        // Procura onde começa "user."
        size_t p1 = filenames[i].find("user.");
        if (p1 == string::npos) continue;

        // Pega o número depois de "user."
        string resto = filenames[i].substr(p1 + 5); 
        int id = stoi(resto); // Converte texto para numero

        images.push_back(img);
        labels.push_back(id);
    }

    // Cria o modelo LBPH (o melhor para reconhecimento simples)
    Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();

    cout << "Treinando a Inteligencia Artificial..." << endl;
    // AQUI A MÁGICA ACONTECE
    model->train(images, labels);

    // Salva o resultado
    model->save("treinamento.yml");

    cout << "Sucesso! Arquivo 'treinamento.yml' criado." << endl;
    return 0;
}