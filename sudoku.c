#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define CAMINHO_ARQUIVO_TABULEIROS "tabuleiro.sudoku"

// tamanho do tabuleiro
#define SIZE_TABULEIRO 9

// tamanho do quadrado
#define SIZE_QUADRADO 3

#define INPUT_TERMINAR_JOGO -1
#define INPUT_INVALIDO -2

void clear(void)
{
    system("cls||clear");
}

void limpar_buffer_de_input(void)
{
    char c;
    do 
    {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

void aguardar_comando(void)
{
    puts("Digite qualquer tecla para continuar...");
    getchar();
}

//retorna um ponteiro de char (string) - usar free após uso da função
char* input_string(size_t max_len_str)
{
    char* str = malloc(sizeof(char) * max_len_str);

    fgets(str, max_len_str, stdin);

    int new_line_index = strlen(str) - 1;
    if (new_line_index >= 0 && str[new_line_index] == '\n')
        str[new_line_index] = '\0';

    limpar_buffer_de_input();

    return str;
}

void print_board(int board[SIZE_TABULEIRO][SIZE_TABULEIRO])
{
    puts("  A B C  D E F  G H I");
    for(int i=0; i<SIZE_TABULEIRO; i++)
    {
        printf("%d|", i+1);
        for(int j=0; j<SIZE_TABULEIRO; j++)
        {
            if((j+1)%3)
                printf("%d ", board[i][j]);
            else 
                printf("%d  ", board[i][j]);
        }

        if((i+1)%3)
            printf("\n");
        else
            printf("\n\n");
    }

    return;
}

int checkIfIsValidInput(int board[SIZE_TABULEIRO][SIZE_TABULEIRO], int linha, int coluna, int valor)
{    
    if(valor == 0)
        return 1;

    for(int k = 0; k < SIZE_TABULEIRO; k++)
    {
        if(board[linha][k] == valor || board[k][coluna] == valor)
        {
            return 0;
        }
    }

    // usando divisão de int, reduzimos a linha e coluna para 0(0,1,2), 1(3,4,5) ou 2(6,7,8) 
    // e multiplicamos por 3 para obter o primeiro index do quadrado sendo acessado
    int linha_quadrado = (linha / SIZE_QUADRADO) * SIZE_QUADRADO;
    int coluna_quadrado = (coluna / SIZE_QUADRADO) * SIZE_QUADRADO;;

    for(int i = 0; i < SIZE_QUADRADO; i++)
    {
        int linha_index_atual = linha_quadrado + i;
        for(int j = 0; j < SIZE_QUADRADO; j++)
        {
            int coluna_index_atual = coluna_quadrado + j;

            if(linha_index_atual == linha && coluna_index_atual == coluna)
            {
                continue;
            }

            if(board[linha_index_atual][coluna_index_atual] == valor)
            {
                return 0;
            }
        }
    }
    
    return 1;
}

int checkIfIsVictory(int board[SIZE_TABULEIRO][SIZE_TABULEIRO])
{
    for(int i=0; i<SIZE_TABULEIRO; i++)
        for(int j=0; j<SIZE_TABULEIRO; j++)
            if(board[i][j] == 0)
                return 0;
    
    return 1;
}

//se jogada INPUT_TERMINAR_JOGO, terminar jogo, 
//se jogada INPUT_INVALIDO, input invalido, 
//se outra coisa positiva ou nula, jogada valida
int obter_jogada(int *linha, char* coluna)
{
    const int MAX_SIZE_TABULEIRO_INPUT = 5;

    int jogada;

    puts("Para sair, digite \"EXIT\".");
    puts("Insira sua jogada, digitando X(1-9)X(A-I) N(0-9).");
    puts("Exemplo: 1A 9.");
    puts("Obs: O valor \"0\" representa um setor vazio.");


    char* input = input_string(MAX_SIZE_TABULEIRO_INPUT);
    int len_input = strlen(input);
    for(int i = 0; i < len_input; i++)
        input[i] = toupper(input[i]);

    if(strncmp(input, "EXIT", MAX_SIZE_TABULEIRO_INPUT) == 0)
    {
        jogada = INPUT_TERMINAR_JOGO;
    }
    else if(sscanf(input, "%1d%c %1d", linha, coluna, &jogada) != 3)
    {
        jogada = INPUT_INVALIDO;
    }
    else {
        //se dados foram obtidos com sucesso, verificar se estão no range correto:

        if((*linha) < 1 || (*linha) > SIZE_TABULEIRO)
        {
            jogada = INPUT_INVALIDO;
        }

        if((*coluna) < 'A' || (*coluna) > 'I')
        {
            jogada = INPUT_INVALIDO;
        }

        if(jogada < 0 || jogada > SIZE_TABULEIRO)
        {
            jogada = INPUT_INVALIDO;
        }
    }

    free(input);
    return jogada;
}

void play(int board[SIZE_TABULEIRO][SIZE_TABULEIRO]) // função principal do jogo
{
    int linha, coluna_int;
    char coluna_char;
    int jogada;

    clear();

    while(1)
    {
        print_board(board);
        
        jogada = obter_jogada(&linha, &coluna_char);

        if(jogada == INPUT_TERMINAR_JOGO)
        {
            clear();
            puts("Finalizando jogo...");
            break;
        }

        if(jogada == INPUT_INVALIDO)
        {
            puts("Entrada inválida. Digite novamente:");
            aguardar_comando();
            clear();
            continue;
        }

        coluna_int = coluna_char - 'A' + 1;

        if(checkIfIsValidInput(board, linha - 1, coluna_int - 1, jogada))
        {
            board[linha - 1][coluna_int - 1] = jogada; 
            clear();
        }
        else 
        {
            printf("Jogada anterior inválida.\n");
            aguardar_comando();
            clear(); 
            continue;
        }

        if(checkIfIsVictory(board))
        {
            printf("Você ganhou! Parabéns!");
            aguardar_comando();
            clear();
            break;
        }
    }
}

int obter_quantidade_tabuleiros_no_arquivo(FILE* arquivo_tabuleiros)
{
    int qtd_tabuleiros_encontrados_no_arquivo = 0;

    if(arquivo_tabuleiros == NULL)
        return qtd_tabuleiros_encontrados_no_arquivo;

    //começar contagem do zero
    fseek(arquivo_tabuleiros, 0, SEEK_SET);

    char c;
    do {
        c = fgetc(arquivo_tabuleiros);

        if(c == '-')
        {
            qtd_tabuleiros_encontrados_no_arquivo++;
        }
    } while(c != EOF);

    //resetar cursor do arquivo para o inicio após a leitura
    fseek(arquivo_tabuleiros, 0, SEEK_SET);

    return (qtd_tabuleiros_encontrados_no_arquivo / 2) + 1;
}

void pular_n_linhas_arquivo(FILE* arquivo, int qtd_linhas)
{
    int linhas_puladas = 0;
    while(linhas_puladas < qtd_linhas)
    {
        char c = fgetc(arquivo);
        if(c == '\n') linhas_puladas++;
    }
}

void gerar_tabuleiro_aleatorio(int board[SIZE_TABULEIRO][SIZE_TABULEIRO])
{
    //tabuleiro padrão para caso não consiga obter dados do arquivo
    int board_padrao_em_caso_de_erro[SIZE_TABULEIRO][SIZE_TABULEIRO] = { 
        {5,3,0,0,7,0,0,0,0},
        {6,0,0,1,9,5,0,0,0},
        {0,9,8,0,0,0,0,6,0},
        {8,0,0,0,6,0,0,0,3},
        {4,0,0,8,0,3,0,0,1},
        {7,0,0,0,2,0,0,0,6},
        {0,6,0,0,0,0,2,8,0},
        {0,0,0,4,1,9,0,0,5},
        {0,0,0,0,8,0,0,7,9}
    };

    //inicializar tabuleiro vazio
    int board_file[SIZE_TABULEIRO][SIZE_TABULEIRO] = { 
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
    };

    FILE* arquivo_tabuleiros = fopen(CAMINHO_ARQUIVO_TABULEIROS, "r");

    //obter quantidade de tabuleiros presentes no arquivo
    int qtd_tabuleiros = obter_quantidade_tabuleiros_no_arquivo(arquivo_tabuleiros);

    if(arquivo_tabuleiros != NULL && qtd_tabuleiros > 0)
    {
        //numero do tabuleiro buscando, index based 1
        int numero_aleatorio_tabuleiro = rand() % qtd_tabuleiros;

        long qtd_linhas_para_tabuleiro_correto = numero_aleatorio_tabuleiro + 1 + 
            (numero_aleatorio_tabuleiro * SIZE_TABULEIRO);

        //mover arquivo para o local de leitura exato
        pular_n_linhas_arquivo(arquivo_tabuleiros, qtd_linhas_para_tabuleiro_correto);

        for(int i = 0; i < SIZE_TABULEIRO; i++)
        {
            for(int j = 0; j < SIZE_TABULEIRO; j++)
            {
                //ler número do arquivo
                board_file[i][j] = fgetc(arquivo_tabuleiros) - '0';

                //o ultimo número não tem espaço
                if(j < SIZE_TABULEIRO - 1)
                {
                    //ler espaço
                    fgetc(arquivo_tabuleiros);
                }
            }
            //ler \n(new line)
            fgetc(arquivo_tabuleiros);
        }

        fclose(arquivo_tabuleiros);
    } 
    else 
    {
        for(int i = 0; i < SIZE_TABULEIRO; i++)
        {
            for(int j = 0; j < SIZE_TABULEIRO; j++)
            {
                board_file[i][j] = board_padrao_em_caso_de_erro[i][j];
            }
        }
    }

    for(int i = 0; i < SIZE_TABULEIRO; i++)
    {
        for(int j = 0; j < SIZE_TABULEIRO; j++)
        {
            board[i][j] = board_file[i][j];
        }
    }
}

bool verificar_se_deseja_outra_partida(void)
{
    // variavel que guarda se o usuario quer terminar o programa ou jogar novamente
    char user_option_continua;

    //variavel que diz se o input é valido
    bool input_valido = false;

    // verifica se o usuario quer encerrar o programa ou jogar uma nova partida
    while(!input_valido)
    {
        puts("Deseja jogar outra partida? Digite \"1\" para Sim - \"0\" para Não.");
        printf("Sua escolha: ");

        char user_option_continua = getchar();
        limpar_buffer_de_input();

        if(user_option_continua != '1' && user_option_continua != '0')
            puts("Entrada anterior inválida.");
        else
            input_valido = true;
    }

    // Caso opção seja '1' retorna true (continuar jogo), o contrario retorna falso (parar jogo)
    return user_option_continua == '1';
}

void run_game(void)
{
    puts("Bem-vindo ao jogo de Sudoku!");
    aguardar_comando();

    //variavel que representa se o jogo vai continuar rodando
    bool jogo_rodando = true;

    while(jogo_rodando)
    {
        int board[SIZE_TABULEIRO][SIZE_TABULEIRO];

        //gerar novo tabuleiro com preenchimento aleatorio
        gerar_tabuleiro_aleatorio(board);
        
        //executa a função principal, quando ela termina: o jogador ganhou ou escolheu sair da partida
        play(board);

        jogo_rodando = verificar_se_deseja_outra_partida();
    }

    printf("Até a proxima!");
}

int main(void)
{
    //obter seed para aleatoriedade
    srand(time(NULL));
    
    run_game();

    return 0;
}