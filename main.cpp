#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


//VARIAVEIS
SDL_Window* SWindow = NULL;
SDL_Surface* MSurface = NULL;

struct no
{
    int cont;              // contador de eventos
    int chute_x, chute_y; //coordenadas de chute
    int gol_x, gol_y;     // coordenadas de posicionamento do goleiro
    char resultado[30];   //variavel que recebe o resultado da cobranca - gol, defesa ou chute para fora.
    struct no *prox;
};

typedef struct
{
    struct no *inicio;
    struct no *fim;
} fila;

void create(fila *s); //funcao para criar a fila
int pushCobranca(fila *s, int cx, int cy, int gx, int gy, int cont); //cx = coord chute x, cy = coord chute y, gx = coord gol x, gy = coord gol y
void mostraCobranca(int cx, int cy, int gx, int gy, int cont, int resultado); //exibe os pontos de chute e goleiro. Por fim, exibe qual o resultado.
int isEmpty (fila s); //ver se fila esta vazia
void geraCoordenadas(int *cx, int *cy, int *gx, int *gy); //funcao que gera as coordenadas para o chute e para posicao do goleiro
void mostrafila (fila s); //exibir as filas
int verifica_cobranca (int cx, int cy, int gx, int gy); //determina resultado da cobranca de acordo com as coordenadas e retorna valor
void imprimeGOL(); //exibicao grafica dos pontos de interesse no simulador - VERIFIQUEM AQUI
void exibeEstatisticas(int cont, int contFora, int contDefesa, int contGol); //exibir estatisticas gerais de simulacao
int teclaContinuar(); //pressione qualquer tecla para continuar + getch
void telaBranco ();
void telaMenu();
void telaexplicamenu();
void telalistacompleta();
void telalistadeacertos();
void telalistadefesa();
void telaerro();
void estatistica();

int main (int argv, char** args)
{

    // inicio da parte de funcionamento
        int cx, cy, gx, gy, opcao;
        int cont=0, contFora = 0, contDefesa = 0, contGol = 0;

        fila cobranca;
        fila fora;
        fila defesa;
        fila gol;

        srand(time(0)); //semente para gerar numeros aleatorios

        create(&cobranca);
        create(&fora);
        create(&defesa);
        create(&gol);


    //INICIA A SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);


    // CRIA JANELA
    SDL_Window *window = SDL_CreateWindow("Penalty Simulator", 600, 300, 700, 414, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //SDL_Log("TELA DE INICIO");                          //INFORMACAO NO PROMPT


    //RENDERIZA IMAGEM DE INICIO
    SDL_Surface *inicio = IMG_Load("images/inicio.jpg");
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, inicio);

    SDL_Rect texture_destination;
    texture_destination.x = 0;
    texture_destination.y = 0;
    texture_destination.w = 700;
    texture_destination.h = 414;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(inicio);

    // ESSE LOOP MANTEM A JANELA ABERTA
    while (true)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))                           // PEGA O PROXIMO EVENTO
        {


                    printf("\n Simulador de cobrancas de penalti\n\n");
                    teclaContinuar();

                    //system("cls"); //funçao do windows para limpar a tela

                    //RENDERIZA IMAGEM DE EXPLICACAO
                    SDL_Surface *explicacao = IMG_Load("images/explicacao.jpg");
                    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, explicacao);

                    SDL_Rect texture_destination;
                    texture_destination.x = 0;
                    texture_destination.y = 0;
                    texture_destination.w = 700;
                    texture_destination.h = 414;

                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
                    SDL_RenderPresent(renderer);

                    SDL_FreeSurface(explicacao);
                    system("cls");
                    teclaContinuar();
                    telaBranco();
                    system("cls");


                    do //loop de execuçao do codigo de cobrancas e armazenamento dos dados nas filas
                    {
                        cont++; //contador de cobrancas
                        geraCoordenadas(&cx, &cy, &gx, &gy); //chamada da funcao que gera randomicamente as coordenadas de chute e goleiro

                        pushCobranca(&cobranca, cx, cy, gx, gy, cont); //insere os pontos na fila cobranca

                        if(verifica_cobranca (cx, cy, gx, gy)==-1) //verifica se foi para fora
                        {
                            pushCobranca(&fora, cx, cy, gx, gy, cont);
                            mostraCobranca(cx, cy, gx, gy, cont, -1);
                            contFora++; //contador de chutes fora
                        }

                        if(verifica_cobranca(cx, cy, gx, gy)==0) //verifica se foi defesa
                        {
                            pushCobranca(&defesa, cx, cy, gx, gy, cont);
                            mostraCobranca(cx, cy, gx, gy, cont, 0);
                            contDefesa++; //contador de defesas
                        }

                        if(verifica_cobranca(cx, cy, gx, gy)==1) //verifica se foi gol
                        {
                            pushCobranca(&gol, cx, cy, gx, gy, cont);
                            mostraCobranca(cx, cy, gx, gy, cont, 1);
                            contGol++;  //contador de gols
                        }

                        printf(" Tecle para parar...\n\n");

                        //SDL_Delay(1500); //funcao para pausar execucao - no caso, pausa por 1 segundo


                        if(verifica_cobranca (cx, cy, gx, gy)==-1)
                        {
                            //RENDERIZA IMAGEM DE FORA

                            SDL_Surface *telafora = IMG_Load("images/fora.jpg");
                            SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, telafora);

                            SDL_Rect texture_destination;
                            texture_destination.x = 0;
                            texture_destination.y = 0;
                            texture_destination.w = 700;
                            texture_destination.h = 414;

                            SDL_RenderClear(renderer);
                            SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
                            SDL_RenderPresent(renderer);

                            SDL_FreeSurface(telafora);

                            telaBranco();
                            //SDL_Delay(10);
                            ;
                        }


                        else if(verifica_cobranca (cx, cy, gx, gy)==0)
                        {
                            //RENDERIZA IMAGEM DE DEFESA
                            SDL_Surface *teladefesa = IMG_Load("images/defesa.jpg");
                            SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, teladefesa);

                            SDL_Rect texture_destination;
                            texture_destination.x = 0;
                            texture_destination.y = 0;
                            texture_destination.w = 700;
                            texture_destination.h = 414;

                            SDL_RenderClear(renderer);
                            SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
                            SDL_RenderPresent(renderer);

                            SDL_FreeSurface(teladefesa);


                           telaBranco();
                            //SDL_Delay(10);
                        }
                        else
                        {
                            //RENDERIZA IMAGEM DE GOL
                            SDL_Surface *telagol = IMG_Load("images/gol.jpg");
                            SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, telagol);

                            SDL_Rect texture_destination;
                            texture_destination.x = 0;
                            texture_destination.y = 0;
                            texture_destination.w = 700;
                            texture_destination.h = 414;

                            SDL_RenderClear(renderer);
                            SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
                            SDL_RenderPresent(renderer);

                            SDL_FreeSurface(telagol);


                           telaBranco();
                            //SDL_Delay(10);
                        }

                        Sleep(2000);

                    }
                    while(!kbhit());  // kbhit = ler tecla qualquer do teclado. Ou seja, executa o loop enquanto nenhuma tecla for apertada.



                    do //loop de execuçao do menu
                    {
                        telaMenu ();

                        system("cls");
                        printf("\n Menu de Dados\n");
                        printf(" 1 - Explicacao do modo de funcionamento da simulacao\n");
                        printf(" 2 - Ver a lista completa de cobrancas\n");
                        printf(" 3 - Ver a lista de Acertos\n");
                        printf(" 4 - Ver a lista de Defesas\n");
                        printf(" 5 - Ver a lista de Erros\n");
                        printf(" 6 - Ver as estatisticas gerais\n");
                        printf(" 9 - Sair do programa\n");
                        printf(" Digite a opcao desejada:");
                        scanf("%d", &opcao);
                        switch(opcao)
                        {
                        case 1:
                            telaexplicamenu();
                            system("cls");
                            imprimeGOL();
                            teclaContinuar();
                            break;
                        case 2:
                            telalistacompleta();
                            system("cls");
                            mostrafila(cobranca);
                            teclaContinuar();
                            break;
                        case 3:
                            telalistadeacertos();
                            system("cls");
                            mostrafila(gol);
                            teclaContinuar();
                            break;
                        case 4:
                            telalistadefesa();
                            system("cls");
                            mostrafila(defesa);
                            teclaContinuar();
                            break;
                        case 5:
                            telaerro();
                            system("cls");
                            mostrafila(fora);
                            teclaContinuar();
                            break;
                        case 6:
                            estatistica();
                            system("cls");
                            exibeEstatisticas(cont, contFora, contDefesa, contGol);
                            teclaContinuar();
                            break;
                        case 9:
                            printf("\n Saindo...\n");
                            // FINALIZA JANELA
                            SDL_FreeSurface(inicio);
                            SDL_DestroyWindow(window);
                            SDL_Quit();
                            //return 3;
                            break;
                        default:
                            printf(" Opcao inexistente. retornando ao menu...\n");
                            Sleep(1500);
                            break;
                        }
                    }
                    while(opcao!=9);
                }
                break;

        }
        return 0;
    }

//funções

void create(fila *s)
{
    s->inicio = NULL;
    s->fim = NULL;
}

int pushCobranca(fila *s, int cx, int cy, int gx, int gy, int cont)
{
    struct no *aux;
    aux =(struct no *)malloc(sizeof(struct no));
    if (aux==NULL)
        return 0;

    aux->cont = cont;
    aux->chute_x=cx;
    aux->chute_y=cy;
    aux->gol_x=gx;
    aux->gol_y=gy;
    //aux->prox= s->topo;
    //s->topo=aux;

    if(verifica_cobranca (cx, cy, gx, gy)==-1) //usa funcao verifica_cobranca para determinar o resultado e copiar a string referente a variavel de resultado na fila.
    {
        strcpy(aux->resultado, "Chute pra fora");
    }

    else if(verifica_cobranca (cx, cy, gx, gy)==0)
    {
        strcpy(aux->resultado, "Defesa do goleiro");
    }
    else
    {
        strcpy(aux->resultado, "Gol!");
    }

    aux->prox=NULL;
    if (s->inicio==NULL)
        s->inicio=aux;
    if (s->fim!=NULL)
        s->fim->prox=aux;
    s->fim = aux;

    return 1;
}

int isEmpty (fila s)
{

    if (s.inicio == NULL)
        return 1;
    return 0;
}

void geraCoordenadas(int *cx, int *cy, int *gx, int *gy) //gera valores aleatorios dentro dos limites estabelecidos para execuçao do simulador
{


    *cx = rand()%5;
    *cy = rand()%3;

    *gx = rand()%4;
    if(*gx<1) //if usado caso o valor gerado para goleiro x seja melhor que 1, oq e impossivel visto que goleiro deve estar entre 1 e 3 do eixo x.
        *gx++;

    *gy = rand()%2;


}

void mostrafila (fila s)
{
    struct no *aux;
    aux = s.inicio;

    if (aux == NULL)
        printf(" Sem dados a exibir.\n");
    else
    {
        while(aux != NULL)
        {
            printf(" Cobranca No. %d:\n Chute em:  (%d;%d)\n", aux->cont, aux->chute_x, aux->chute_y);
            printf(" Goleiro em (%d;%d)\n", aux->gol_x, aux->gol_y);
            printf(" Resultado: %s!\n\n", aux->resultado);
            aux = aux->prox;

        }
    }
}

int verifica_cobranca (int cx, int cy, int gx, int gy) //verifica o resultado da cobranca de acordo com posicao do chute e do goleiro
{

    if (cy == 2)
    {
        //Chute para fora;
        return -1;
    }
    else if (cx<0 || cx>3)
    {
        //Chute para fora
        return -1;
    }
    else if(cx == gx && cy == gy)
    {
        //Defesa do goleiro
        return 0;
    }

    else //Gol

        return 1;
}

void mostraCobranca(int cx, int cy, int gx, int gy, int cont, int resultado) //exibe o resultado da cobranca
{
    printf(" Cobranca No. %d\n", cont);
    printf(" Chute em   (%d ; %d)\n", cx, cy);
    printf(" Goleiro em (%d ; %d)\n", gx,gy);
    if(resultado == -1)
        printf(" Resultado: Chute pra Fora!\n");
    else if (resultado == 0)
        printf(" Resultado: Defesa do Goleiro!\n");
    else
        printf(" Resultado: Gol!\n");

}

void imprimeGOL() //exibe o funcionamento das coordenadas do simulador. **OLHEM ISSO**
{
    printf(R"EOF(
 Eis a determinacao do posicionamento no gol:

 (0;2) (1;2)(2;2)(3;2) (4;2)
       ---------------
 (0;1)|(1;1)(2;1)(3;1)|(4;1)
 (0;0)|(1;0)(2;0)(3;0)|(4;0)

)EOF");
}

void exibeEstatisticas(int cont, int contFora, int contDefesa, int contGol) //gera os dados das estatisticas do simulador. calculos feitos direto nos printf para economizar variaveis
{
    float contFloat; //usar conversao para variavel float pra nao ter erro na divisao entre inteiros
    contFloat = cont;

    printf("\n Estatisticas gerais de simulacao\n\n");
    printf(" Total de Cobrancas: %d\n Gols Feitos: %d\n Defesas: %d\n Chutes Fora: %d\n",cont, contGol, contDefesa, contFora);
    printf(" Porcentagem de acertos: %.2f%%\n", (contGol/contFloat)*100);
    printf(" Porcentagem de Defesas: %.2f%%\n", (contDefesa/contFloat)*100);
    printf(" Porcentagem de Chutes para Fora: %.2f%%\n\n", (contFora/contFloat)*100);
}

int teclaContinuar() //mensagem de execuçao em todo fim de tarefa.
{
    printf(" Digite ENTER para continuar: ");
    _getch();
}

void telaBranco()
{
                            SDL_Window *window = SDL_CreateWindow("Penalty Simulator", 1, 1, 1, 1, SDL_WINDOW_OPENGL);
                            SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_Surface *branco = IMG_Load("images/branco.jpg");
                            SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, branco);
                            SDL_DestroyWindow(window);

                            SDL_Rect texture_destination;
                            texture_destination.x = 0;
                            texture_destination.y = 0;
                            texture_destination.w = 100;
                            texture_destination.h = 100;

                            SDL_RenderClear(renderer);
                            SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
                            SDL_RenderPresent(renderer);

                            SDL_FreeSurface(branco);
                            SDL_DestroyWindow(window);

}

void telaMenu()
{
    SDL_Quit();

    //INICIA A SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);


    // CRIA JANELA
    SDL_Window *window = SDL_CreateWindow("Penalty Simulator", 600, 300, 700, 414, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    //RENDERIZA IMAGEM DE MENU
    SDL_Surface *inicio = IMG_Load("images/menu.jpg");
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, inicio);

    SDL_Rect texture_destination;
    texture_destination.x = 0;
    texture_destination.y = 0;
    texture_destination.w = 700;
    texture_destination.h = 414;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(inicio);
}

void telaexplicamenu()
{
    SDL_Quit();

    //INICIA A SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);


    // CRIA JANELA
    SDL_Window *window = SDL_CreateWindow("Penalty Simulator", 1100, 150, 700, 414, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    //RENDERIZA IMAGEM DE MENU
    SDL_Surface *inicio = IMG_Load("images/explicamenu.jpg");
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, inicio);

    SDL_Rect texture_destination;
    texture_destination.x = 0;
    texture_destination.y = 0;
    texture_destination.w = 700;
    texture_destination.h = 414;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(inicio);
}


void telalistadeacertos()
{
    SDL_Quit();

    //INICIA A SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);


    // CRIA JANELA
    SDL_Window *window = SDL_CreateWindow("Penalty Simulator", 600, 300, 700, 414, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    //RENDERIZA IMAGEM DA LISTA
    SDL_Surface *inicio = IMG_Load("images/listadeacertos.png");
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, inicio);

    SDL_Rect texture_destination;
    texture_destination.x = 0;
    texture_destination.y = 0;
    texture_destination.w = 700;
    texture_destination.h = 414;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(inicio);
}

void telalistadefesa()
{
    SDL_Quit();

    //INICIA A SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);


    // CRIA JANELA
    SDL_Window *window = SDL_CreateWindow("Penalty Simulator", 600, 300, 700, 414, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    //RENDERIZA IMAGEM DA LISTA
    SDL_Surface *inicio = IMG_Load("images/listadefesa.png");
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, inicio);

    SDL_Rect texture_destination;
    texture_destination.x = 0;
    texture_destination.y = 0;
    texture_destination.w = 700;
    texture_destination.h = 414;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(inicio);
}


void telaerro()
{
    SDL_Quit();

    //INICIA A SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);


    // CRIA JANELA
    SDL_Window *window = SDL_CreateWindow("Penalty Simulator", 600, 300, 700, 414, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    //RENDERIZA IMAGEM DA LISTA
    SDL_Surface *inicio = IMG_Load("images/listaerro.png");
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, inicio);

    SDL_Rect texture_destination;
    texture_destination.x = 0;
    texture_destination.y = 0;
    texture_destination.w = 700;
    texture_destination.h = 414;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(inicio);
}

void telalistacompleta()
{
    SDL_Quit();

    //INICIA A SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);


    // CRIA JANELA
    SDL_Window *window = SDL_CreateWindow("Penalty Simulator", 600, 300, 700, 414, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    //RENDERIZA IMAGEM DA LISTA
    SDL_Surface *inicio = IMG_Load("images/listacompleta.png");
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, inicio);

    SDL_Rect texture_destination;
    texture_destination.x = 0;
    texture_destination.y = 0;
    texture_destination.w = 700;
    texture_destination.h = 414;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(inicio);
}

void estatistica()
{
    SDL_Quit();

    //INICIA A SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);


    // CRIA JANELA
    SDL_Window *window = SDL_CreateWindow("Penalty Simulator", 600, 300, 700, 414, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    //RENDERIZA IMAGEM DA LISTA
    SDL_Surface *inicio = IMG_Load("images/estatisticas.png");
    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, inicio);

    SDL_Rect texture_destination;
    texture_destination.x = 0;
    texture_destination.y = 0;
    texture_destination.w = 700;
    texture_destination.h = 414;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, image_texture, NULL, &texture_destination);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(inicio);
}
