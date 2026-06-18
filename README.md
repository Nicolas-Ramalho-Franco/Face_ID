# 📷 Face_ID

> Projeto de visão computacional para detecção facial em tempo real e coleta de amostras, desenvolvido durante o estágio na Eniac Academy.

---

## 📋 Descrição

O **Face_ID** é um aplicativo desenvolvido em C++ que captura o feed de vídeo da webcam, detecta faces frontais em tempo real utilizando o algoritmo Haar Cascade da biblioteca OpenCV e permite a coleta estruturada de amostras de imagens. Essas amostras são tratadas e salvas para serem utilizadas no treinamento futuro de modelos de reconhecimento facial (como Eigenfaces, Fisherfaces ou LBPH).

### ✨ Principais Recursos
* **Detecção em Tempo Real:** Identificação fluida de faces via webcam.
* **Coleta Automatizada:** Captura e salvamento de amostras com comandos do teclado.
* **Pré-processamento:** Imagens salvas automaticamente em escala de cinza e redimensionadas para 200x200 pixels (padrão ideal para algoritmos de reconhecimento).

---

## 🛠️ Tecnologias e Ferramentas

* **Linguagem:** C++ (padrão C++17 ou superior)
* **Visão Computacional:** [OpenCV 4](https://opencv.org/)
* **Sistema de Build:** CMake (versão 3.10+)
* **Gerenciador de Dependências:** VCPKG (Modo Manifesto)
* **Algoritmo:** Haar Cascades (`haarcascade_frontalface_default.xml`)

---

## 📁 Estrutura do Projeto

```text
Face_ID/
├─ build/                  # Diretório de compilação (gerado pelo CMake)
├─ .vs/                    # Configurações locais do Visual Studio
├─ .vscode/                # Configurações de workspace do VS Code
├─ coletor.cpp             # Implementação do módulo de captura e salvamento
├─ main.cpp                # Ponto de entrada e loop principal da aplicação
├─ CMakeLists.txt          # Script de configuração do CMake
├─ vcpkg.json              # Manifesto de dependências (OpenCV)
└─ haarcascade_frontalface_default.xml # Classificador pré-treinado
