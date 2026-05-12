# 🧀 Tom & Jerry Maze Chase

<p align="center">
  <img src="images/gameplay.gif" width="700">
</p>

<p align="center">
  Jogo inspirado em Pac-Man desenvolvido em C++ com SFML
</p>

---

## 🎮 Sobre o Projeto

Tom & Jerry Maze Chase é um jogo 2D inspirado no clássico Pac-Man, onde o jogador controla Jerry enquanto foge dos gatos em um labirinto.

O projeto foi desenvolvido para praticar:
- Programação Orientada a Objetos
- Lógica de jogos
- Colisão
- Inteligência artificial básica
- SFML com C++

---

## 🚀 Tecnologias

- C++
- SFML

---

## ✨ Funcionalidades

- Movimentação em grid
- Sistema de direção desejada
- Colisão precisa
- IA básica dos inimigos
- Sistema de pontuação
- Pixel Art

---

# ⚙️ Instalação da SFML e Compilação

# 🪟 Windows

## 1. Instale o compilador MinGW

Baixe e instale o MSYS2:

- https://www.msys2.org/

---

## 2. Abra o terminal MSYS2 UCRT64

Atualize os pacotes:

```bash
pacman -Syu
```

Feche o terminal e abra novamente o **MSYS2 UCRT64**.

---

## 3. Instale o compilador e a SFML

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-sfml
```

---

## 4. Compile o projeto

```bash
g++ *.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system
```

---

## 5. Execute

```bash
./game
```

---

# 🐧 Linux (Ubuntu)

### 1. Instale a SFML

```bash
sudo apt update
sudo apt install libsfml-dev
```

---

### 2. Compile o projeto

```bash
g++ *.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system
```

---

### 3. Execute

```bash
./game
```

---

# 🛠️ Caso os arquivos estejam em uma pasta `src`

Compile usando:

```bash
g++ src/*.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system
```
