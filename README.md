# Renderizador de Raytracing em C/C++

[![C++](https://img.shields.io/badge/C++-17%2B-blue.svg)](https://en.cppreference.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Um renderizador de raytracing simples e eficiente escrito em C++ moderno, criado como um projeto educacional para entender os fundamentos da computação gráfica.

## Demonstração

O projeto gera uma cena 3D contendo várias esferas com diferentes materiais (metal, vidro e material difuso) e salva o resultado em um arquivo PPM.

## Características

- Renderização 3D baseada em raytracing
- Suporte a múltiplos materiais (difuso, metálico e dielétrico)
- Reflexão e refração de luz
- Profundidade de campo
- Anti-aliasing
- Código otimizado para melhor desempenho em hardwares mais modestos

## Pré-requisitos

- Compilador C++17 ou superior (GCC, Clang ou MSVC)
- CMake (opcional, para builds mais fáceis)

## Como compilar e executar

### Compilação no Windows (usando MinGW):
***Primeiramente escolha que arquivo você quer rodar, digamos que o main.cpp***
```bash
g++ main.cpp -o main && main > imagem.ppm
```

### Executando:
```bash
./main > imagem.ppm (Se você não tiver um visualizador ppm, rode no google ppm viewer)
```

## Cena de exemplo

O código renderiza uma cena contendo:
- Um chão cinza
- Uma esfera de vidro no centro
- Uma esfera marrom fosca à esquerda
- Uma esfera metálica à direita
- Várias esferas adicionais menores com diferentes materiais

## Configurações

No arquivo `main.cpp`, você pode ajustar:
- `image_width`: Largura da imagem de saída
- `samples_per_pixel`: Número de amostras por pixel (aumenta a qualidade)
- `max_depth`: Profundidade máxima de recursão para raios
- `vfov`: Campo de visão vertical
- `lookfrom` e `lookat`: Posição e direção da câmera

## Licença

Este projeto está licenciado sob a Licença MIT - veja o arquivo [LICENSE](LICENSE) para detalhes.


## Recursos

- Baseado no livro "Ray Tracing in One Weekend" de Peter Shirley: https://raytracing.github.io/books/RayTracingInOneWeekend.html#rays,asimplecamera,andbackground/therayclass
- [Série de artigos sobre Ray Tracing](https://raytracing.github.io/)



Se você tiver alguma dúvida ou sugestão, sinta-se à vontade para entrar em contato!
