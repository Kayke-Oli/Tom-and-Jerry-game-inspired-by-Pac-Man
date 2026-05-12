#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <sstream>
#include <cstring>
#include <functional>
using namespace std;

const int SIZE = 38;      // Tamanho de cada celula do mapa
const int  MapaY = 13;
const int MapaX = 25;

char mapa[MapaY][MapaX +1] = {  // +1 para o terminador null
"111111111111g111111111111",
"d00000000001010000000000b",
"1011111101610161011111101",
"1000001101110111011000001",
"1111101000000000001011111",
"e00010101111511110101000f",
"1110060000344430000600111",
"1000111110111110111110001",
"1010000000000000000000101",
"1010111101110111011110101",
"1010111000110110001110101",
"a06000001000000010000060c",
"111111111111h111111111111",
};

char mapaOriginal[MapaY][MapaX + 1];
void resetarMapa() {
    for (int i = 0; i < MapaY; i++)
        strcpy(mapa[i], mapaOriginal[i]);
}

struct Gatos{
    int num; //Num é um modo de identificar o gato e definir sua dificuldade (1: so movimentos aleatorios, 2: persegue o player sempre quando ve-lo, 3: sempre anda na direção do player)
    double gatoX;
    double gatoY;
    const int inicialGatoX;
    const int inicialGatoY;
};

Gatos Gato[3] = {
    {1, 12, 6, 12, 6},
    {2, 11, 6, 11, 6},
    {3, 13, 6, 13, 6},
};

double posx = 12;
double posy = 8;
double inicialx = posx;
double inicialy = posy;
double velocidade = 0.04;
double velocidadegato = 0.04;
int MaxPontos = -1, pontos = -1;
int I = 0, I1 = 0, I2 = 0, I3 = 0;
int L = 0, L1 = 0, L2 = 0, L3 = 0;

bool cima1 = false, cima2 = false, cima3 = false;
bool baixo1 = false, baixo2 = false, baixo3 = false;
bool esq1 = false, esq2 = false, esq3 = false;
bool dir1 = false, dir2 = false, dir3 = false;
bool cima = false;
bool baixo = false;
bool esq = false;
bool dir = false;
bool continuar = true;
bool cimac, baixoc, esqc, dirc;
bool jogadorComecou = false;
bool trajetoInicialGato1 = false;
bool trajetoInicialGato2 = false;
bool trajetoInicialGato3 = false;
bool bateu = false;
bool cachorro = false;

bool olhadireita = false;
bool olhacima = false;
bool olhabaixo = false;

bool vitoria = false;
sf::Clock TempoCachorro;

//Funcoes para verificar alinhamento com a grade e eventualmente forca-lo de forma sutil;
bool AlinhadoX(double& MyposX){
    if (abs(round(MyposX) - MyposX) <= velocidade){
        MyposX = round(MyposX);
        return true;
    }
    return false;
}

bool AlinhadoY(double& MyposY){
    if (abs(round(MyposY) - MyposY) <= velocidade){
        MyposY = round(MyposY);
        return true;
    }
    return false;
}

bool EhProibido(char c) { //Onde player nao pode ir
    return (c == '1' || c == '3' || c == '4' || c == '5');
}

bool EhProibidoG(char c) { //Onde gato nao pode ir
    return (c == '1' || c > 59);
}

//Funções para verificar capicidade de se movimentar (Player)
bool PodeCima(double MyposX, double MyposY){
    return(MyposY > velocidade && !EhProibido(mapa[(int)floor(MyposY-velocidade)][(int)round(MyposX)]) && AlinhadoX(MyposX));
}
bool PodeBaixo(double MyposX, double MyposY){
    return (MyposY < (MapaY-velocidade) && !EhProibido(mapa[(int)floor(MyposY+1)][(int)round(MyposX)]) && AlinhadoX(MyposX) && MyposY+1 < MapaY);
}
bool PodeEsq(double MyposX, double MyposY){
    return (MyposX > velocidade && !EhProibido(mapa[(int)round(MyposY)][(int)floor(MyposX-velocidade)]) && AlinhadoY(MyposY));
}
bool PodeDir(double MyposX, double MyposY){
    return (MyposX < (MapaX-velocidade) && !EhProibido(mapa[(int)round(MyposY)][(int)floor(MyposX+1)]) && AlinhadoY(MyposY) && MyposX+1 < MapaX);
}

//Funções para verificar capicidade de se movimentar (Gato)
bool PodeCimaG(double MyposX, double MyposY){
    return(MyposY > velocidadegato && !EhProibidoG(mapa[(int)floor(MyposY-velocidadegato)][(int)round(MyposX)]) && AlinhadoX(MyposX));
}
bool PodeBaixoG(double MyposX, double MyposY){
    return (MyposY < (MapaY-velocidadegato) && !EhProibidoG(mapa[(int)floor(MyposY+1)][(int)round(MyposX)]) && AlinhadoX(MyposX) && MyposY+1 < MapaY);
}
bool PodeEsqG(double MyposX, double MyposY){
    return (MyposX > velocidadegato && !EhProibidoG(mapa[(int)round(MyposY)][(int)floor(MyposX-velocidadegato)]) && AlinhadoY(MyposY));
}
bool PodeDirG(double MyposX, double MyposY){
    return (MyposX < (MapaX-velocidadegato) && !EhProibidoG(mapa[(int)round(MyposY)][(int)floor(MyposX+1)]) && AlinhadoY(MyposY) && MyposX+1 < MapaX);
}

//Detecta se a casa em que pacman está é um portal

bool EmPortal(double MyposX, double MyposY){
    if(mapa[(int)round(MyposY)][(int)round(MyposX)] > 59){
        return true;
    }
    else
        return false;
}

//Mover gato aleatoriamente
string moverGatoAleatorio(double& gx, double& gy, const string& ultimaDirecao) {
    vector<pair<string, bool(*)(double, double)>> opcoes;

    if (PodeCimaG(gx, gy)) 
        opcoes.push_back({"cima", PodeCimaG});
    if (PodeBaixoG(gx, gy)) 
        opcoes.push_back({"baixo", PodeBaixoG});
    if (PodeEsqG(gx, gy)) 
        opcoes.push_back({"esq", PodeEsqG});
    if (PodeDirG(gx, gy)) 
        opcoes.push_back({"dir", PodeDirG});

    // Impede voltar pela direção contrária
    string direcaoContraria;
    if (ultimaDirecao == "cima") 
        direcaoContraria = "baixo";
    else if (ultimaDirecao == "baixo") 
        direcaoContraria = "cima";
    else if (ultimaDirecao == "esq") 
        direcaoContraria = "dir";
    else if (ultimaDirecao == "dir") 
        direcaoContraria = "esq";

    vector<pair<string, bool(*)(double, double)>> opcoesFiltradas;
    for (auto& op : opcoes)
        if (op.first != direcaoContraria)
            opcoesFiltradas.push_back(op);

    if (!opcoesFiltradas.empty()) {
        auto escolhido = opcoesFiltradas[rand() % opcoesFiltradas.size()];
        const string& dir = escolhido.first;

        if (dir == "dir") 
            gx += velocidadegato;
        else if (dir == "esq")
            gx -= velocidadegato;
        else if (dir == "cima")     
            gy -= velocidadegato;
        else if (dir == "baixo") 
            gy += velocidadegato;

        return dir;
    }

    // Se só pode voltar
    if (!opcoes.empty()) {
        auto escolhido = opcoes[rand() % opcoes.size()];
        const string& dir = escolhido.first;

        if (dir == "dir") 
            gx += velocidadegato;
        else if (dir == "esq") 
            gx -= velocidadegato;
        else if (dir == "cima") 
            gy -= velocidadegato;
        else if (dir == "baixo") 
            gy += velocidadegato;

        return dir;
    }

    return ultimaDirecao; // sem saída
}

bool temParedeX(double x1, double x2, double posY){
    int minX = std::min((int)std::round(x1), (int)std::round(x2));
    int maxX = std::max((int)std::round(x1), (int)std::round(x2));
    
    for(int i = minX + 1; i < maxX; i++){ // Verifica apenas entre as posições
        if(mapa[(int)std::round(posY)][i] == '1'){
            return true;
        }
    }
    return false;
}

bool temParedeY(double y1, double y2, double posX){
    int minY = std::min((int)std::round(y1), (int)std::round(y2));
    int maxY = std::max((int)std::round(y1), (int)std::round(y2));
    
    for(int i = minY + 1; i < maxY; i++){ // Verifica apenas entre as posições
        if(mapa[i][(int)std::round(posX)] == '1'){
            return true;
        }
    }
    return false;
}

bool gatoVeRato(){
    if (std::round(Gato[1].gatoX) == std::round(posx) && !temParedeY(Gato[1].gatoY, posy, Gato[1].gatoX)|| 
        std::round(Gato[1].gatoY) == std::round(posy) && !temParedeX(Gato[1].gatoX, posx, Gato[1].gatoY)){
        
        if((int)std::round(Gato[1].gatoX) == std::round(posx) && Gato[1].gatoY > posy ||
           (int)std::round(Gato[1].gatoX) == std::round(posx) && Gato[1].gatoY < posy || 
           (int)std::round(Gato[1].gatoY) == std::round(posy) && Gato[1].gatoX > posx ||
           (int)std::round(Gato[1].gatoY) == std::round(posy) && Gato[1].gatoX < posx)
        return true;
    }
    return false; // Retorna string vazia se não conseguir ver o rato
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SIZE*MapaX, SIZE*MapaY), "Pac-Man");

    
    sf::Font font;
    if (!font.loadFromFile("anja.ttf")) {
        return -1;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2);
    scoreText.setPosition(836, 10);

    sf::Text finishText;
    finishText.setFont(font);
    finishText.setCharacterSize(100);
    finishText.setFillColor(sf::Color::Red);
    finishText.setOutlineColor(sf::Color::Black);
    finishText.setOutlineThickness(4);
    finishText.setPosition(260, 153);

    sf::Text gameoverText;
    gameoverText.setFont(font);
    gameoverText.setCharacterSize(100);
    gameoverText.setFillColor(sf::Color::Red);
    gameoverText.setOutlineColor(sf::Color::Black);
    gameoverText.setOutlineThickness(4);
    gameoverText.setPosition(190, 153);

    sf::Clock reiniciar;
    float tempoTotal = 0.0f;
    bool visivel = true;
    sf::Text reiniciarText;
    reiniciarText.setFont(font);
    reiniciarText.setCharacterSize(25);
    reiniciarText.setFillColor(sf::Color::White);
    reiniciarText.setOutlineColor(sf::Color::Black);
    reiniciarText.setOutlineThickness(3);
    reiniciarText.setPosition(230, 270);

    sf::Texture parede;
    parede.loadFromFile("parede.png");
    sf::Sprite spriteParede(parede);

    sf::RectangleShape proibida(sf::Vector2f(SIZE, SIZE));
    proibida.setFillColor(sf::Color(40, 0, 0));

    sf::Clock animratoclock;

    sf::Texture texture[4];
    texture[0].loadFromFile("framedir0.png");
    texture[1].loadFromFile("framedir1.png");
    texture[2].loadFromFile("framedir2.png");
    texture[3].loadFromFile("framedir3.png");
    sf::Sprite spritedir(texture[I]);

    sf::Texture texture1[4];
    texture1[0].loadFromFile("frameesq0.png");
    texture1[1].loadFromFile("frameesq1.png");
    texture1[2].loadFromFile("frameesq2.png");
    texture1[3].loadFromFile("frameesq3.png");
    sf::Sprite spriteesq(texture1[I1]);

    sf::Texture texture2[4];
    texture2[0].loadFromFile("framecima0.png");
    texture2[1].loadFromFile("framecima1.png");
    texture2[2].loadFromFile("framecima2.png");
    texture2[3].loadFromFile("framecima3.png");
    sf::Sprite spritecima(texture2[I2]);

    sf::Texture texture3[4];
    texture3[0].loadFromFile("framebaixo0.png");
    texture3[1].loadFromFile("framebaixo1.png");
    texture3[2].loadFromFile("framebaixo2.png");
    texture3[3].loadFromFile("framebaixo3.png");
    sf::Sprite spritebaixo(texture3[I3]);

    sf::Clock animagatoclock;

    sf::Texture texturegato[4];
    texturegato[0].loadFromFile("framedirgato0.png");
    texturegato[1].loadFromFile("framedirgato1.png");
    texturegato[2].loadFromFile("framedirgato2.png");
    texturegato[3].loadFromFile("framedirgato3.png");
    sf::Sprite spritedirgato(texturegato[L]);

    sf::Texture texture1gato[4];
    texture1gato[0].loadFromFile("frameesqgato0.png");
    texture1gato[1].loadFromFile("frameesqgato1.png");
    texture1gato[2].loadFromFile("frameesqgato2.png");
    texture1gato[3].loadFromFile("frameesqgato3.png");
    sf::Sprite spriteesqgato(texture1gato[L1]);

    sf::Texture texture2gato[4];
    texture2gato[0].loadFromFile("framecimagato0.png");
    texture2gato[1].loadFromFile("framecimagato1.png");
    texture2gato[2].loadFromFile("framecimagato2.png");
    texture2gato[3].loadFromFile("framecimagato3.png");
    sf::Sprite spritecimagato(texture2gato[L2]);

    sf::Texture texture3gato[4];
    texture3gato[0].loadFromFile("framebaixogato0.png");
    texture3gato[1].loadFromFile("framebaixogato1.png");
    texture3gato[2].loadFromFile("framebaixogato2.png");
    texture3gato[3].loadFromFile("framebaixogato3.png");
    sf::Sprite spritebaixogato(texture3gato[L3]);

    sf::Texture texturecachorro[4];
    texturecachorro[0].loadFromFile("framedircachorro0.png");
    texturecachorro[1].loadFromFile("framedircachorro1.png");
    texturecachorro[2].loadFromFile("framedircachorro2.png");
    texturecachorro[3].loadFromFile("framedircachorro3.png");
    sf::Sprite spritedircachorro(texturecachorro[I]);

    sf::Texture texture1cachorro[4];
    texture1cachorro[0].loadFromFile("frameesqcachorro0.png");
    texture1cachorro[1].loadFromFile("frameesqcachorro1.png");
    texture1cachorro[2].loadFromFile("frameesqcachorro2.png");
    texture1cachorro[3].loadFromFile("frameesqcachorro3.png");
    sf::Sprite spriteesqcachorro(texture1cachorro[I1]);

    sf::Texture texture2cachorro[4];
    texture2cachorro[0].loadFromFile("framecimacachorro0.png");
    texture2cachorro[1].loadFromFile("framecimacachorro1.png");
    texture2cachorro[2].loadFromFile("framecimacachorro2.png");
    texture2cachorro[3].loadFromFile("framecimacachorro3.png");
    sf::Sprite spritecimacachorro(texture2cachorro[I2]);

    sf::Texture texture3cachorro[4];
    texture3cachorro[0].loadFromFile("framebaixocachorro0.png");
    texture3cachorro[1].loadFromFile("framebaixocachorro1.png");
    texture3cachorro[2].loadFromFile("framebaixocachorro2.png");
    texture3cachorro[3].loadFromFile("framebaixocachorro3.png");
    sf::Sprite spritebaixocachorro(texture3cachorro[I3]);

    sf::Texture TEXcomida;
    if(!TEXcomida.loadFromFile("queijo.png")) return 0;
    sf::Sprite spriteComida(TEXcomida);

    sf::Texture portaDir;
    portaDir.loadFromFile("portaDir.png");
    sf::Sprite spritePDir(portaDir);

    sf::Texture portaEsq;
    portaEsq.loadFromFile("portaEsq.png");
    sf::Sprite spritePEsq(portaEsq);

    sf::Texture portaCima;
    portaCima.loadFromFile("portaCima.png");
    sf::Sprite spritePCima(portaCima);

    sf::Texture portaBaixo;
    portaBaixo.loadFromFile("portaBaixo.png");
    sf::Sprite spritePBaixo(portaBaixo);
    
    sf::Texture barrabase;
    barrabase.loadFromFile("barra.png");
    sf::Sprite spriteBarra(barrabase);

    sf::Texture barraempebase;
    barraempebase.loadFromFile("barrape.png");
    sf::Sprite spriteBarraPe(barraempebase);

    sf::Texture TextureCasaCachorro;
    TextureCasaCachorro.loadFromFile("CasaCachorro.png");
    sf::Sprite spriteCasaCachorro(TextureCasaCachorro);

    sf::Texture TextureCachorro;
    TextureCachorro.loadFromFile("Cachorro.png");
    sf::Sprite spriteCachorro(TextureCachorro);

    sf::Clock clock;

    string direcaoGato1 = "dir";
    string direcaoGato2 = "dir";
    string direcaoGato3 = "dir";

    for (int i = 0; i < MapaY; i++)
    for (int j = 0; j < MapaX; j++){
        if(mapa[i][j] == '0'){
            MaxPontos++;
        }
    }

    for (int i = 0; i < MapaY; i++)
        strcpy(mapaOriginal[i], mapa[i]);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if(!vitoria){
                if ((event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) &&  !bateu){
                    esq = true; 
                    dir = cima = baixo = false;
                    I = I2 = I3 = 0;}
                else if ((event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) && bateu) {
                    esq = false; 
                    dir = cima = baixo = false;
                    I = I2 = I3 = 0;} 
                else if ((event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) && !bateu) {
                    dir = true; 
                    esq = cima = baixo = false;
                    I1 = I2 = I3 = 0;}
                else if ((event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) && bateu) {
                    dir = false; 
                    esq = cima = baixo = false;
                    I1 = I2 = I3 = 0;}
                else if ((event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) && !bateu) {
                    cima = true; 
                    baixo = esq = dir = false;
                    I = I1 = I3 = 0;}
                else if ((event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) && bateu) {
                    cima = false; 
                    baixo = esq = dir = false;
                    I = I1 = I3 = 0;}
                else if ((event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) && !bateu) {
                    baixo = true; 
                    cima = esq = dir = false;
                    I = I1 = I2 = 0;}
                else if ((event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) && bateu) {
                    baixo = false; 
                    cima = esq = dir = false;
                    I = I1 = I2 = 0;}}
                if(!vitoria || vitoria)
                    if (event.key.code == sf::Keyboard::R) {
                    posx = inicialx;
                    posy = inicialy;
                    dir = esq = cima = baixo = false;
                    olhadireita = olhabaixo = olhacima = false;
                    cimac = baixoc = esqc = dirc = false;
                    pontos = -1;
                    resetarMapa();
                    vitoria = false;
                    cima1 = cima2 = cima3 = false;
                    baixo1 = baixo2 = baixo3 = false;
                    esq1 = esq2 = esq3 = false;
                    dir1 = dir2 = dir3 = false;
                    for(int i=0; i < 3; i++){
                        Gato[i].gatoX = Gato[i].inicialGatoX;
                        Gato[i].gatoY = Gato[i].inicialGatoY;
                    }
                    jogadorComecou = false;
                    trajetoInicialGato1 = false;
                    trajetoInicialGato2 = false;
                    trajetoInicialGato3 = false;
                    bateu = false;
                    velocidade = 0.04;
                }
            }
        }  

        if (!jogadorComecou && (dir || esq || cima || baixo)) {
            jogadorComecou = true;}

        if(vitoria){
            velocidade = 0;
            baixo = cima = dir = esq = false;}

        if (clock.getElapsedTime() > sf::seconds(0.01)) {
            clock.restart();

            if (jogadorComecou && !vitoria) {
            // Gato 1 sobe até sair da "jaula"
            if (!trajetoInicialGato1) {
                if (PodeCimaG(Gato[0].gatoX, Gato[0].gatoY)) {
                    Gato[0].gatoY -= velocidadegato;}
                else {
                    trajetoInicialGato1 = true;}} 
            else {
                direcaoGato1 = moverGatoAleatorio(Gato[0].gatoX, Gato[0].gatoY, direcaoGato1);
            }

            // Gato 2 vai para a esquerda
            if (!trajetoInicialGato2) {
                if (PodeEsqG(Gato[1].gatoX, Gato[1].gatoY)) {
                    Gato[1].gatoX -= velocidadegato;} 
                else {
                    trajetoInicialGato2 = true;}} 
            else if(!cachorro){
                if(gatoVeRato()) { // Gato vê o rato
                    // Movimento horizontal (mesma linha Y)
                    if (std::round(Gato[1].gatoY) == std::round(posy) && AlinhadoY(Gato[1].gatoY)) {
                        if (Gato[1].gatoX > posx && PodeEsqG(Gato[1].gatoX, Gato[1].gatoY)) {
                            Gato[1].gatoX -= velocidadegato;
                            Gato[1].gatoY = (int)round(Gato[1].gatoY);
                        }
                        else if (Gato[1].gatoX < posx && PodeDirG(Gato[1].gatoX, Gato[1].gatoY)) {
                            Gato[1].gatoX += velocidadegato;
                            Gato[1].gatoY = (int)round(Gato[1].gatoY);
                        }
                    }
                    // Movimento vertical (mesma coluna X)
                    else if (std::round(Gato[1].gatoX) == std::round(posx) && AlinhadoX(Gato[1].gatoX)) {
                        if (Gato[1].gatoY > posy && PodeCimaG(Gato[1].gatoX, Gato[1].gatoY)) {
                            Gato[1].gatoY -= velocidadegato;
                                Gato[1].gatoX = (int)round(Gato[1].gatoX);
                        }
                        else if (Gato[1].gatoY < posy && PodeBaixoG(Gato[1].gatoX, Gato[1].gatoY) && AlinhadoX(Gato[1].gatoX)) {
                            Gato[1].gatoY += velocidadegato;
                            Gato[1].gatoX = (int)round(Gato[1].gatoX);
                        }
                    }
                    else
                    direcaoGato2 = moverGatoAleatorio(Gato[1].gatoX, Gato[1].gatoY, direcaoGato2);
                }
            else { // Se não vê o rato, move aleatoriamente
                direcaoGato2 = moverGatoAleatorio(Gato[1].gatoX, Gato[1].gatoY, direcaoGato2);
            }
            }
            else { // Se não vê o rato, move aleatoriamente
                direcaoGato2 = moverGatoAleatorio(Gato[1].gatoX, Gato[1].gatoY, direcaoGato2);
            }


            // Gato 3 vai para a direita
            if (!trajetoInicialGato3) {
                if (PodeDirG(Gato[2].gatoX, Gato[2].gatoY)) {
                    Gato[2].gatoX += velocidadegato;} 
                else {
                    trajetoInicialGato3 = true;
                }
            } 
            else if (!cachorro){
                // Gato 3 Persegue o rato
                const int CIMA = 0;
                const int BAIXO = 1;
                const int ESQUERDA = 2;
                const int DIREITA = 3;

                // Movimento nas direções: CIMA, BAIXO, ESQUERDA, DIREITA
                double dx[] = {0, 0, -0.041, 0.041}; // Esse valor precisa mudar conforme a velocidade pois valores <= 0.04 não tem efeito na posição
                double dy[] = {-velocidadegato, velocidadegato, 0, 0};

                if (AlinhadoX(Gato[2].gatoX) || AlinhadoY(Gato[2].gatoY)) {
                    int menorDistancia = 999999;
                    int melhorDirecao = -1;
                    int dirAnterior;
                    int dirPenultima = -1;
                    int contadorZigZag = 0;


                    for (int dir = 0; dir < 4; dir++) {
                        // Evita ir na direção oposta
                        if ((dirAnterior == CIMA && dir == BAIXO && PodeCimaG(Gato[2].gatoX, Gato[2].gatoY)) ||
                            (dirAnterior == BAIXO && dir == CIMA && PodeBaixoG(Gato[2].gatoX, Gato[2].gatoY)) ||
                            (dirAnterior == ESQUERDA && dir == DIREITA && PodeEsqG(Gato[2].gatoX, Gato[2].gatoY)) ||
                            (dirAnterior == DIREITA && dir == ESQUERDA && PodeDirG(Gato[2].gatoX, Gato[2].gatoY)))
                            continue;

                        double nx = round(Gato[2].gatoX) + (dx[dir] > 0 ? 1 : (dx[dir] < 0 ? -1 : 0));
                        double ny = round(Gato[2].gatoY) + (dy[dir] > 0 ? 1 : (dy[dir] < 0 ? -1 : 0));

                        bool podeMover = (dir == 0 && PodeCimaG(Gato[2].gatoX, Gato[2].gatoY)) ||
                                        (dir == 1 && PodeBaixoG(Gato[2].gatoX, Gato[2].gatoY)) ||
                                        (dir == 2 && PodeEsqG(Gato[2].gatoX, Gato[2].gatoY)) ||
                                        (dir == 3 && PodeDirG(Gato[2].gatoX, Gato[2].gatoY));
                        
                        if (podeMover) {
                            int distancia = abs(posx - nx) + abs(posy - ny);
                            if (distancia < menorDistancia){
                                menorDistancia = distancia;
                                melhorDirecao = dir;
                            }
                        }
                    }
                if (melhorDirecao != -1) {
                    dirAnterior = melhorDirecao;
                }
                    // Atualiza a posição do gato
                    if (dirAnterior == 0 && PodeCimaG(Gato[2].gatoX, Gato[2].gatoY)){
                        Gato[2].gatoY += dy[dirAnterior];
                        direcaoGato3 = "cima";
                    }
                    else if (dirAnterior == 1 && PodeBaixoG(Gato[2].gatoX, Gato[2].gatoY)){
                        Gato[2].gatoY += dy[dirAnterior];
                        direcaoGato3 = "baixo";
                    }
                    else if (dirAnterior == 2 && PodeEsqG(Gato[2].gatoX, Gato[2].gatoY)){
                        Gato[2].gatoX += dx[dirAnterior];
                        direcaoGato3 = "esq";
                    }
                    else if (dirAnterior == 3 && PodeDirG(Gato[2].gatoX, Gato[2].gatoY)){
                        Gato[2].gatoX += dx[dirAnterior];
                        direcaoGato3 = "dir";
                    }
            }
            }
            else { // Se não vê o rato, move aleatoriamente
                direcaoGato3 = moverGatoAleatorio(Gato[2].gatoX, Gato[2].gatoY, direcaoGato2);
            }
    
            }

            if(dir || esq || cima || baixo)
                bateu = false;

            // --- Verifica se o jogador colidiu com algum dos gatos ---
            if (round(posx) == round(Gato[0].gatoX) && round(posy) == round(Gato[0].gatoY)) {
                if(!cachorro){
                dir = esq = cima = baixo = false;
                dirc = esqc = baixoc = cimac = false;
                bateu = true;
                jogadorComecou = false;
                bateu = true;
                }
                else {
                    Gato[0].gatoX = Gato[0].inicialGatoX;
                    Gato[0].gatoY = Gato[0].inicialGatoY;
                }
            }
            else if(round(posx) == round(Gato[1].gatoX) && round(posy) == round(Gato[1].gatoY)){
                if(!cachorro){
                dir = esq = cima = baixo = false;
                dirc = esqc = baixoc = cimac = false;
                bateu = true;
                jogadorComecou = false;
                bateu = true;
                }
                else {
                    Gato[1].gatoX = Gato[0].inicialGatoX;
                    Gato[1].gatoY = Gato[0].inicialGatoY;
                }
            }
            else if(round(posx) == round(Gato[2].gatoX) && round(posy) == round(Gato[2].gatoY)){
                if(!cachorro){
                dir = esq = cima = baixo = false;
                dirc = esqc = baixoc = cimac = false;
                bateu = true;
                jogadorComecou = false;
                bateu = true;
                }
                else {
                    Gato[2].gatoX = Gato[2].inicialGatoX;
                    Gato[2].gatoY = Gato[2].inicialGatoY;
                }

            }

            if (cima && PodeCima(posx, posy)){
                baixoc = esqc = dirc = false;
                cimac = true; 
                olhacima = true;
                olhabaixo = olhadireita = false;
                posy -= velocidade;
            }
            else if (baixo && PodeBaixo(posx, posy)){
                cimac = esqc = dirc = false;
                baixoc = true; 
                olhabaixo = true;
                olhadireita = olhacima = false;
                posy += velocidade;
            }
            else if (esq && PodeEsq(posx, posy)){
                baixoc = cimac = dirc = false;
                esqc = true;
                olhabaixo = olhacima = olhadireita = false;
                posx -= velocidade;
            }
            else if (dir && PodeDir(posx, posy)){
                baixoc = esqc = cimac = false;
                dirc = true;
                olhadireita = true;
                olhabaixo = olhacima = false;
                posx += velocidade;
            }
            else{
                if(dirc && PodeDir(posx,posy)){
                    posx += velocidade;
                    olhadireita = true;
                    olhabaixo = olhacima = false;
                }
                else if(esqc && PodeEsq(posx,posy)){
                    posx -= velocidade;
                    olhabaixo = olhacima = olhadireita = false;
                }
                else if(baixoc && PodeBaixo(posx,posy)){
                    posy += velocidade;
                    olhabaixo = true;
                    olhadireita = olhacima = false;
                }
                else if(cimac && PodeCima(posx,posy)){
                    posy -= velocidade;
                    olhacima = true;
                    olhabaixo = olhadireita = false;
                }
                else{
                if (AlinhadoX(posx)) {
                    posx = round(posx); // Força PacMan a se alinhar no eixo X
                }
                if (AlinhadoY(posy)) {
                    posy = round(posy); // Força PacMan a se alinhar no eixo Y
                }
                }
            }}

            //Sistema de Comida / Pontos / Cachorro :: ************************************************************************************************************************

            if(mapa[(int)round(posy)][(int)round(posx)] == '0'  && AlinhadoX(posx) && AlinhadoY(posy)){
                pontos++;
                mapa[(int)posy][(int)posx] = '2';
            }
            else if(mapa[(int)round(posy)][(int)round(posx)] == '6'  && AlinhadoX(posx) && AlinhadoY(posy)){
                cachorro = true;
                mapa[(int)posy][(int)posx] = '2';
                TempoCachorro.restart();
            }

            if(TempoCachorro.getElapsedTime().asSeconds() > 7){
                cachorro  = false;
            }

            //Manuseamento do Portal:: ************************************************************************************************************************
            char portal;
            static sf::Clock portalClock; // Cria Relogio para adicionar delay
            static bool portalClockStarted = false; // Controla se o clock foi iniciado
            if(EmPortal(posx,posy)){
                if(mapa[(int)posy][(int)posx] > 59){
                    portal = mapa[(int)posy][(int)posx];
                

                if(!portalClockStarted){ // Inicia o clock apenas quando pisar no portal para adicionar pequeno delay de teleporte e aparentar liso
                portalClock.restart();
                portalClockStarted = true;
                }

                if(portalClock.getElapsedTime() > sf::seconds(0.2)){ // Delay de 0.2s
                    if(portal % 2 == 1){ //Check qual é o par do portal
                        for (int i = 0; i < MapaY; i++)
                        for (int j = 0; j < MapaX; j++)
                            if(mapa[i][j] == (portal+1)){
                                posx = j;
                                posy = i;
                                portalClockStarted = false; // Reset para o próximo portal
                            }
                    }
                    else{                     
                        for (int i = 0; i < MapaY; i++)
                        for (int j = 0; j < MapaX; j++)
                            if(mapa[i][j] == (portal-1)){
                                posx = j;
                                posy = i;
                            portalClockStarted = false; // Reset para o próximo portal
                            }
                    }
                }
                }
                
                else  portalClockStarted = false; // Reset para o próximo portal
        }

        stringstream ss;
        ss << "SCORE: " << pontos;
        scoreText.setString(ss.str());

        stringstream ss2;
        ss2 << "YOU WIN";
        finishText.setString(ss2.str());

        stringstream ss3;
        ss3 << "PRESSIONE R PARA JOGAR NOVAMENTE";
        reiniciarText.setString(ss3.str());

        stringstream ss4;
        ss4 << "GAME OVER";
        gameoverText.setString(ss4.str());

        if(!dir && !esq && !cima && !baixo)
            spriteesq.setTexture(texture[0]);
        
        else
        if (animratoclock.getElapsedTime() > sf::seconds(0.15)) {
            animratoclock.restart();
            // Avança o frame de animação do rato dependendo da direção
            if (olhadireita){
                I = (I + 1) % 4;
                spritedir.setTexture(texture[I]);
                if(cachorro)
                    spritedircachorro.setTexture(texturecachorro[I]);}
            else if (olhacima){
                I2 = (I2 + 1) % 4;
                spritecima.setTexture(texture2[I2]);
                if(cachorro)
                    spritecimacachorro.setTexture(texture2cachorro[I2]);}
            else if (olhabaixo){
                I3 = (I3 + 1) % 4;
                spritebaixo.setTexture(texture3[I3]);
                if(cachorro)
                    spritebaixocachorro.setTexture(texture3cachorro[I3]);} 
            else{
                I1 = (I1 + 1) % 4;
                spriteesq.setTexture(texture1[I1]);
                if(cachorro)
                    spriteesqcachorro.setTexture(texture1cachorro[I1]);}
        }

            if (animagatoclock.getElapsedTime() > sf::seconds(0.15)) {
            animagatoclock.restart();

            // Gato 1
            if (direcaoGato1 == "dir") {
                L1 = (L1 + 1) % 4;
                spritedirgato.setTexture(texturegato[L1]);
            } else if (direcaoGato1 == "esq") {
                L1 = (L1 + 1) % 4;
                spriteesqgato.setTexture(texture1gato[L1]);
            } else if (direcaoGato1 == "cima") {
                L1 = (L1 + 1) % 4;
                spritecimagato.setTexture(texture2gato[L1]);
            } else if (direcaoGato1 == "baixo") {
                L1 = (L1 + 1) % 4;
                spritebaixogato.setTexture(texture3gato[L1]);
            }

            // Gato 2
            if (direcaoGato2 == "dir") {
                L2 = (L2 + 1) % 4;
                spritedirgato.setTexture(texturegato[L2]);
            } else if (direcaoGato2 == "esq") {
                L2 = (L2 + 1) % 4;
                spriteesqgato.setTexture(texture1gato[L2]);
            } else if (direcaoGato2 == "cima") {
                L2 = (L2 + 1) % 4;
                spritecimagato.setTexture(texture2gato[L2]);
            } else if (direcaoGato2 == "baixo") {
                L2 = (L2 + 1) % 4;
                spritebaixogato.setTexture(texture3gato[L2]);
            }

            // Gato 3
            if (direcaoGato3 == "dir") {
                L3 = (L3 + 1) % 4;
                spritedirgato.setTexture(texturegato[L3]);
            } else if (direcaoGato3 == "esq") {
                L3 = (L3 + 1) % 4;
                spriteesqgato.setTexture(texture1gato[L3]);
            } else if (direcaoGato3 == "cima") {
                L3 = (L3 + 1) % 4;
                spritecimagato.setTexture(texture2gato[L3]);
            } else if (direcaoGato3 == "baixo") {
                L3 = (L3 + 1) % 4;
                spritebaixogato.setTexture(texture3gato[L3]);
            }
        }



        float delta = reiniciar.restart().asSeconds();
        tempoTotal += delta;
        if (tempoTotal >= 0.5f){
            visivel = !visivel;
            tempoTotal = 0.0f;}

        //Desenho de sprites:: ************************************************************************************************************************
        
        window.clear(sf::Color(100,100,100));

        
        for (int i = 0; i < MapaY; i++)
        for (int j = 0; j < MapaX; j++){
            if(mapa[i][j] == '0'){ // Desenha a comida:
                    // Centraliza a comida na célula
                spriteComida.setPosition(j * SIZE + SIZE/2 - spriteComida.getGlobalBounds().width/2, 
                                        i * SIZE + SIZE/2 - spriteComida.getGlobalBounds().height/2);
                window.draw(spriteComida);
            }
            if(mapa[i][j] == '6'){ // Desenha a Casa do cachrro:
                // Centraliza a casa na célula
                spriteCasaCachorro.setPosition(j * SIZE + SIZE/2 - spriteCasaCachorro.getGlobalBounds().width/2, 
                                        i * SIZE + SIZE/2 - spriteCasaCachorro.getGlobalBounds().height/2);
                window.draw(spriteCasaCachorro);
            }
        }

        if (olhadireita && !cachorro) {
            spritedir.setPosition((posx * SIZE) + (SIZE - spritedir.getGlobalBounds().width) / 2, (posy * SIZE) + (SIZE - spritedir.getGlobalBounds().height) /2); // Essa equação desenha o sprite centralizado
            window.draw(spritedir);} 
        else if (olhacima && !cachorro) {
            spritecima.setPosition((posx * SIZE) + (SIZE - spritedir.getGlobalBounds().width) / 2, (posy * SIZE) + (SIZE - spritedir.getGlobalBounds().height) /2);
            window.draw(spritecima);}
        else if (olhabaixo && !cachorro) {
            spritebaixo.setPosition((posx * SIZE) + (SIZE - spritedir.getGlobalBounds().width) / 2, (posy * SIZE) + (SIZE - spritedir.getGlobalBounds().height) /2);
            window.draw(spritebaixo);} 
        else if (!cachorro) {
            spriteesq.setPosition((posx * SIZE) + (SIZE - spritedir.getGlobalBounds().width) / 2, (posy * SIZE) + (SIZE - spritedir.getGlobalBounds().height) /2);
            window.draw(spriteesq);}
        else if (olhadireita && cachorro){
            spritedircachorro.setPosition((posx * SIZE) + (SIZE - spritedir.getGlobalBounds().width) / 2, (posy * SIZE) + (SIZE - spritedir.getGlobalBounds().height) /2);
            window.draw(spritedircachorro);}
        else if (olhacima && cachorro) {
            spritecimacachorro.setPosition((posx * SIZE) + (SIZE - spritedir.getGlobalBounds().width) / 2, (posy * SIZE) + (SIZE - spritedir.getGlobalBounds().height) /2);
            window.draw(spritecimacachorro);}
        else if (olhabaixo && cachorro) {
            spritebaixocachorro.setPosition((posx * SIZE) + (SIZE - spritedir.getGlobalBounds().width) / 2, (posy * SIZE) + (SIZE - spritedir.getGlobalBounds().height) /2);
            window.draw(spritebaixocachorro);}
        else if (cachorro) {
            spriteesqcachorro.setPosition((posx * SIZE) + (SIZE - spritedir.getGlobalBounds().width) / 2, (posy * SIZE) + (SIZE - spritedir.getGlobalBounds().height) /2);
            window.draw(spriteesqcachorro);}
        

        // Gato 1
        if (direcaoGato1 == "dir") {
            spritedirgato.setPosition(Gato[0].gatoX * SIZE, Gato[0].gatoY * SIZE);
            window.draw(spritedirgato);} 
        else if (direcaoGato1 == "esq") {
            spriteesqgato.setPosition(Gato[0].gatoX * SIZE, Gato[0].gatoY * SIZE);
            window.draw(spriteesqgato);} 
        else if (direcaoGato1 == "cima") {
            spritecimagato.setPosition(Gato[0].gatoX * SIZE, Gato[0].gatoY * SIZE);
            window.draw(spritecimagato);}  
        else if (direcaoGato1 == "baixo") {
            spritebaixogato.setPosition(Gato[0].gatoX * SIZE, Gato[0].gatoY * SIZE);
            window.draw(spritebaixogato);}

        // Gato 2
        if (direcaoGato2 == "dir") {
            spritedirgato.setPosition(Gato[1].gatoX * SIZE, Gato[1].gatoY * SIZE);
            window.draw(spritedirgato);} 
        else if (direcaoGato2 == "esq") {
            spriteesqgato.setPosition(Gato[1].gatoX * SIZE, Gato[1].gatoY * SIZE);
            window.draw(spriteesqgato);} 
        else if (direcaoGato2 == "cima") {
            spritecimagato.setPosition(Gato[1].gatoX * SIZE, Gato[1].gatoY * SIZE);
            window.draw(spritecimagato);} 
        else if (direcaoGato2 == "baixo") {
            spritebaixogato.setPosition(Gato[1].gatoX * SIZE, Gato[1].gatoY * SIZE);
            window.draw(spritebaixogato);}

        // Gato 3
        if (direcaoGato3 == "dir") {
            spritedirgato.setPosition(Gato[2].gatoX * SIZE, Gato[2].gatoY * SIZE);
            window.draw(spritedirgato);} 
        else if (direcaoGato3 == "esq") {
            spriteesqgato.setPosition(Gato[2].gatoX * SIZE, Gato[2].gatoY * SIZE);
            window.draw(spriteesqgato);} 
        else if (direcaoGato3 == "cima") {
            spritecimagato.setPosition(Gato[2].gatoX * SIZE, Gato[2].gatoY * SIZE);
            window.draw(spritecimagato);} 
        else if (direcaoGato3 == "baixo") {
            spritebaixogato.setPosition(Gato[2].gatoX * SIZE, Gato[2].gatoY * SIZE);
            window.draw(spritebaixogato);}


        for (int i = 0; i < MapaY; i++)
            for (int j = 0; j < MapaX; j++){
                if(mapa[i][j] == '3'){
                    spriteBarraPe.setPosition(j * SIZE, i * SIZE);
                    window.draw(spriteBarraPe);
                }
                else if(mapa[i][j] == '5'){
                    spriteBarra.setPosition(j * SIZE, i * SIZE);
                    window.draw(spriteBarra);
                }
                else if(mapa[i][j] == '1') {
                    spriteParede.setPosition(j * SIZE, i * SIZE);
                    window.draw(spriteParede);
                }
                else if(mapa[i][j] > 60){ //Desenha As Portas(Teleportes do mapa)
                    if(i == 0){
                        spritePCima.setPosition(j * SIZE, i * SIZE);
                        window.draw(spritePCima);
                    }
                    else if(i == MapaY-1){
                        spritePBaixo.setPosition(j * SIZE, i * SIZE);
                        window.draw(spritePBaixo);
                    }
                    else if(j == 0){
                        spritePEsq.setPosition(j * SIZE, i * SIZE);
                        window.draw(spritePEsq);
                    }
                    else if(j == MapaX-1){
                        spritePDir.setPosition(j * SIZE, i * SIZE);
                        window.draw(spritePDir);
                    }
                }
            }

            if (!vitoria && pontos >= MaxPontos) {
                vitoria = true;
                bateu = false;
                cima1 = cima2 = cima3 = false;
                baixo1 = baixo2 = baixo3 = false;
                esq1 = esq2 = esq3 = false;
                dir1 = dir2 = dir3 = false;
                baixo = cima = dir = esq = false;
                cimac = baixoc = esqc = dirc = false;}
            if (vitoria) {
                if (visivel)
                    window.draw(reiniciarText);
                window.draw(finishText);}
            if(bateu){
                window.draw(gameoverText);
                if (visivel)
                    window.draw(reiniciarText);}

            window.draw(scoreText);
        window.display();
        }
    return 0;}