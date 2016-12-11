// testa se o sistema � Windows, pois o "cls" funciona somente no Windows,
// sen�o utiliza o "clear" que � o comando para limpar a tela no Linux
#ifdef __WIN32
#define limpar_tela "cls"
#else
#define limpar_tela "clear"
#endif

// inclus�o de bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100 // quantidade m�xima de letras no nome do cliente e nome do carro

// defini��o da struct carro
typedef struct carro
{
	// campos da struct carro
	int id;
	int id_cliente;
	char modelo[MAX];
	char marca[MAX];
	char placa[MAX];
	char arcondicionado[MAX];
	char quantidadeportas[MAX];
	char anofabricacao[MAX];
	float preco;
} t_carro;

// struct data de nascimento

typedef struct data
{
	
	int dia[2];
	int mes[2];
	int ano[4];
	
} DATA;

// struct endere�o

typedef struct enderecodetal
{
	char rua[MAX];
	char numero[MAX];
	char cep[MAX];
	char bairro[MAX];
	char cidade[MAX];
	char estado[MAX];
	
	
} ENDERECODETAL;

// defini��o da struct cliente
typedef struct cliente
{
	// campos da struct cliente
	int id;
	char nome[MAX];
	int cpf[MAX];
	ENDERECODETAL endereco;
	char email[MAX];
	DATA nascimento;
	int telefone[MAX];
	
	
} t_cliente;


// prot�tipos de fun��es

/**
 * Exibe o menu com todas as op��es do programa
 * retorno: um caractere do n�mero correspondente a op��o
 */
char menu();

/**
 * Cadastra um cliente
 */
void cadastrar_cliente();

/**
 * Cadastra um carro
 */
void cadastrar_carro();

/**
 * Exibe todos os clientes cadastrados
 */
void listar_cliente();

/**
 * Mostra todos os carros cadastrados e as informa��es de aluguel
 */
void listar_carro();

int existe_cliente(FILE *arq_cliente, int id_cliente);

/**
 * Procura um carro pelo id do carro
 *
 * Par�metros:
 *   arq_carro: ponteiro para um arquivo aberto
 *   id_carro: ID do carro que ser� buscado
 * retorno: um ponteiro para o carro encontrado ou NULL caso o ID n�o exista
 */
t_carro *obter_carro(FILE *arq_carro, int id_carro);

/**
 * Procura um cliente pelo seu id
 *
 * Par�metros:
 *   arq_clientes: ponteiro para um arquivo aberto
 *   id_cliente: ID do cliente que ser� buscado
 * retorno: um ponteiro para o cliente encontrado ou NULL caso o ID n�o exista
 */
t_cliente *obter_cliente(FILE *arq_cliente, int id_cliente);

void entregar_carro();

void excluir_carro(); // exclui um carro do arquivo

void atualizar_carro(FILE *arq_carro, t_carro *carro_alugado);

void alugar_carro(); // aluga o carro




int str_somente_numeros(char str[]);

int main(int argc, char *argv[])
{
	char resp;

	// loop infinito do programa
	while(1)
	{
		// obt�m a escolha do usu�rio
		resp = menu();

		// testa o valor de "resp"
		if(resp == '1')
			cadastrar_carro();
		else if(resp == '2')
			cadastrar_cliente();
		else if(resp == '3')
			listar_carro();
		else if(resp == '4')
			listar_cliente();
		else if(resp == '5')
			alugar_carro();
		else if(resp == '6')
			entregar_carro();
		else if(resp == '7')
			excluir_carro();
		else if(resp == '0') // se for igual a 0, ent�o sai do loop while
			break;
		else
		{
			printf("\nOpcao invalida! Pressione <Enter> para continuar...");
			scanf("%*c");
			// uma forma de "limpar" o buffer de entrada
			fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso
		}
		system(limpar_tela);
	}

	printf("\nBye!\n");
	return 0;
}

char menu()
{
	char resp[2];

	printf("------------------------------ Locadora de Carros ------------------------------");
	printf("\n\n1 - Cadastrar um carro\n");
	printf("2 - Cadastrar um cliente\n");
	printf("3 - Listar carros\n");
	printf("4 - Listar Clientes\n");
	printf("5 - Alugar Carro\n");
	printf("6 - Entregar um carro\n");
	printf("7 - Excluir um carro\n");
	printf("0 - Sair\n");
	printf("Digite o numero da opcao: ");
	scanf("%1s%*c", resp); // o *c pega o Enter e descarta

	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso

	// se chegou aqui, � porque a op��o � v�lida
	return resp[0];
}

// fun��o que verifica se uma string cont�m somente n�meros
int str_somente_numeros(char str[])
{
	int i = 0;
	int len_str = strlen(str);

	for(i = 0; i < len_str; i++)
	{
		if(str[i] < '0' || str[i] > '9')
			return 0;
	}
	return 1;
}

void cadastrar_cliente()
{
	// abre o arquivo para escrita
	// a+b => acrescenta dados ao final do arquivo ou cria um arquivo bin�ria para leitura e escrita
	FILE *arq_cliente = fopen("cliente.bin", "a+b");

	// testa a abertura do arquivo
	if(arq_cliente == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); // aborta o programa
	}
	/*
		N�O � interessante que o usu�rio digite o ID do cliente, esse
		ID tem que ser gerado autom�tico, ent�o temos que pegar o
		ID do �ltimo usu�rio cadastrado
	*/
	int cont_bytes = 0;

	// cont ir� guardar o n�mero total de bytes
	// seta o ponteiro do arquivo para o final do arquivo
	fseek(arq_cliente, 0, SEEK_END);
	// pegamos a quantidade de bytes com a fun��o ftell
	cont_bytes = ftell(arq_cliente);

	t_cliente cliente;

	if(cont_bytes == 0)
	{
		// se for 0, ent�o n�o existe cliente cadastrado
		// coloco o ID come�ando de 1
		cliente.id = 1;
	}
	else
	{
		t_cliente ultimo_cliente;

		// utilizo a fun��o fseek para posicionar o arquivo
		// cont_bytes - sizeof(t_cliente) serve para posicionar
		// para que possamos pegar o �ltimo cliente cadastrado
		fseek(arq_cliente, cont_bytes - sizeof(t_cliente), SEEK_SET);

		// ler o cliente
		fread(&ultimo_cliente, sizeof(t_cliente), 1, arq_cliente);

		// o ID do cliente � o ID do �ltimo cliente acrescido em 1
		cliente.id = ultimo_cliente.id + 1;
	}
	// obt�m o nome do carro
	// ^\n indica para pegar at� a quebra de linha (enter)
	// %*c descarta o enter
	printf("\nnome do cliente: ");
	scanf("%99[^\n]%*c", cliente.nome);
	printf("\nCPF: ");
	scanf("%99[^\n]%*c", cliente.cpf);
	printf("\nEndereco: ");
	printf("\n\tRua: ");
	scanf("%99[^\n]%*c", cliente.endereco.rua);
	printf("\n\tNumero: ");
	scanf("%99[^\n]%*c", cliente.endereco.numero);
	printf("\n\tCEP: ");
	scanf("%99[^\n]%*c", cliente.endereco.cep);
	printf("\n\tBairro: ");
	scanf("%99[^\n]%*c", cliente.endereco.bairro);
	printf("\n\tCidade: ");
	scanf("%99[^\n]%*c", cliente.endereco.cidade);
	printf("\n\tEstado: ");
	scanf("%99[^\n]%*c", cliente.endereco.estado);	
	printf("\nEmail: ");
	scanf("%99[^\n]%*c", cliente.email);
	printf("\nData de Nascimento: ");
	printf("\n\t\tDia: ");
	scanf("%99[^\n]%*c", cliente.nascimento.dia);
	printf("\n\t\tMes: ");
	scanf("%99[^\n]%*c", cliente.nascimento.mes);
	printf("\n\t\tAno: ");
	scanf("%99[^\n]%*c", cliente.nascimento.ano);
	printf("\nTelefone: ");
	scanf("%99[^\n]%*c", cliente.telefone);

	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso

	// se o ponteiro n�o estiver no final do arquivo nada � escrito
	fseek(arq_cliente, 0, SEEK_END);
	// escreve no arquivo
	fwrite(&cliente, sizeof(t_cliente), 1, arq_cliente);

	// fecha o arquivo
	fclose(arq_cliente);

	printf("\nCliente \"%s\" cadastrado com sucesso!\n", cliente.nome);
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c"); // pega o Enter e descarta

	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso
}

// fun��o para cadastrar carro
void cadastrar_carro()
{
	// abre o arquivo para escrita
	// a+b => acrescenta dados ao final do arquivo ou cria
	// um arquivo bin�ria para leitura e escrita
	FILE *arq_carro = fopen("carro.bin", "a+b");

	// testa a abertura do arquivo
	if(arq_carro == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); // aborta o programa
	}
	t_carro carro;

	/*
		N�O � interessante que o usu�rio digite o ID do carro, esse
		ID tem que ser gerado autom�tico, ent�o temos que pegar o
		ID do �ltimo carro cadastrado
	*/
	int cont_bytes = 0;

	// seta o ponteiro do arquivo para o final do arquivo
	fseek(arq_carro, 0, SEEK_END);
	// cont ir� guardar o n�mero total de bytes
	cont_bytes = ftell(arq_carro);

	if(cont_bytes == 0)
	{
		// se for 0, ent�o n�o existe carro cadastrado
		// coloco o ID come�ando de 1
		carro.id = 1;
	}
	else
	{
		t_carro ultimo_carro;

		// utilizo a fun��o fseek para posicionar o arquivo
		// cont_bytes - sizeof(t_cliente) serve para posicionar
		// para que possamos pegar o �ltimo carro cadastrado
		fseek(arq_carro, cont_bytes - sizeof(t_carro), SEEK_SET);

		// ler o carro
		fread(&ultimo_carro, sizeof(t_carro), 1, arq_carro);

		// o ID do carro � o ID do �ltimo carro acrescido em 1
		carro.id = ultimo_carro.id + 1;
	}
	// obt�m o nome do carro
	// ^\n indica para pegar at� a quebra de linha (enter)
	// %*c descarta o enter
	printf("\nDigite o modelo do carro: ");
	scanf("%99[^\n]%*c", carro.modelo);
	printf("\nDigite a marca do carro: ");
	scanf("%99[^\n]%*c", carro.marca);
	printf("\nDigite a placa do carro: ");
	scanf("%99[^\n]%*c", carro.placa);
	printf("\nAr Condicionado: ");
	scanf("%99[^\n]%*c", carro.arcondicionado);
	printf("\nQuantidade de portas: ");
	scanf("%99[^\n]%*c", carro.quantidadeportas);
	printf("\nAno de fabricacao: ");
	scanf("%99[^\n]%*c", carro.anofabricacao);
	
	do
	{
		char str_preco[6];
		float float_preco;
		int somente_numeros = 1;

		// obt�m o pre�o do carro
		printf("\nDigite o preco do carro: ");
		scanf("%4s%*c", str_preco);

		fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso

		// verifica se o pre�o possui somente n�meros
		somente_numeros = str_somente_numeros(str_preco);
		// verifica se o pre�o cont�m somente n�meros
		if(somente_numeros == 1)
		{
			// se chegou aqui, � porque tudo est� validado
			// preencho a vari�vel float_preco com o valor de str_preco
			int int_preco;
			// exemplo: 49,50 deve digitar 4950
			sscanf(str_preco, "%d", &int_preco);
			float_preco = int_preco;
			carro.preco = float_preco;
			break;
		}
	}
	while(1);
		
	// inicializa o id_cliente com -1 indicando que o carro N�O est� alugado
	carro.id_cliente = -1;

	// se o ponteiro n�o estiver no final do arquivo nada � escrito
	fseek(arq_carro, 0, SEEK_END);
	// escreve no arquivo
	fwrite(&carro, sizeof(t_carro), 1, arq_carro);

	// fecha o arquivo
	fclose(arq_carro);

	printf("\nCarro \"%s\" cadastrado com sucesso!\n", carro.modelo);
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c"); // pega o Enter e descarta

	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso
}




// fun��o para listar todos os carros
void listar_carro()
{
	// lista de todos os carros

	// rb => abre para leitura somente, ponteiro para o in�cio do arquivo
	FILE *arq_carro = fopen("carro.bin", "rb");
	FILE *arq_cliente = fopen("cliente.bin", "rb");

	// se o arquivo de carros n�o existe
	if(arq_carro == NULL)
	{
		printf("\nFalha ao abrir arquivo ou ");
		printf("Nenhum carro cadastrado.\n");
		printf("\nPressione <Enter> para continuar...");
		scanf("%*c"); // pega o Enter e descarta

		// uma forma de "limpar" o buffer de entrada
		fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso
		return;
	}
	// vari�vel que indica se encontrou pelo menos 1 carro
	int encontrou_carro = 0;
	printf("\nListando todos os carros...\n");
	// loop para percorrer o arquivo
	t_carro carro;
	while(1)
	{
		// fread retorna o n�mero de elementos lidos com sucesso
		size_t result = fread(&carro, sizeof(t_carro), 1, arq_carro);

		// se for 0, � porque n�o h� mais elemento, ent�o sai do loop
		if(result == 0)
			break;

		// atualiza a vari�vel indicando que encontrou
		// pelo menos um carro
		encontrou_carro = 1;

		// mostra os dados do carro
		printf("\nID do carro: %d\n", carro.id);
		printf("Nome do carro: %s\n", carro.modelo);
		printf("Marca do carro: %s\n", carro.marca);
		printf("Placa do carro: %s\n", carro.placa);
		printf("Ar Condicionado: %s\n", carro.arcondicionado);
		printf("Quantidade de portas: %s\n", carro.quantidadeportas);
		printf("Ano de fabricacao: %s\n", carro.anofabricacao);
		printf("Preco: %6.2f\n", carro.preco);

		// se id_cliente for diferente de -1, ent�o o carro est� alugado, pois
		// possui algum id v�lido do cliente que alugou o carro.
		// se id_cliente for igual a -1, indica que o carro n�o est� alugado,
		// pois o id -1 N�O � um id v�lido
		if(carro.id_cliente != -1)
		{
			// se o arquivo de clientes n�o existir, aten��o esse teste
			// n�o pode ser realizado no in�cio da fun��o, pois pode acontecer de
			// todos os carros n�o estarem alugados
			if(arq_cliente == NULL)
			{
				printf("\nFalha ao abrir arquivo!\n");
				fclose(arq_carro); // libera recursos
				exit(1); // aborta o programa
			}
			t_cliente *cliente = obter_cliente(arq_cliente, carro.id_cliente);
			printf("Alugado? Sim. Cliente: %s\n", cliente->nome);
			free(cliente); // evita vazamento de mem�ria
		}
		else
			printf("Alugado? Nao\n");
	}

	// verifica se encontrou pelo menos um carro
	if(encontrou_carro == 0)
		printf("\nNenhum carro cadastrado.\n");

	// fecha os arquivos em ordem inversa que foram abertos
	if(arq_cliente != NULL)
		fclose(arq_cliente); // apenas se foi aberto
	fclose(arq_carro);

	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");

	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso
}

int existe_carro(FILE *arq_carro, int id_carro)
{
	// vai para o in�cio do arquivo, pois n�o sabemos a posi��o do ponteiro no arquivo
	rewind(arq_carro);

	t_carro carro;
	// loop para percorrer o arquivo
	while(1)
	{

		// fread retorna o n�mero de elementos lidos com sucesso
		size_t result = fread(&carro, sizeof(t_carro), 1, arq_carro);

		// se for 0, � porque n�o h� mais elemento, ent�o sai do loop
		if(result == 0)
			break;

		// verifica se o ID � igual
		if(carro.id == id_carro)
			return 1;
	}

	// se chegou aqui � porque N�O existe o carro, ent�o retorna 0
	return 0;
}

// fun��o para listar todos os clientes
void listar_cliente()
{
	// rb => abre arquivo bin�rio para leitura apenas
	FILE *arq_cliente = fopen("cliente.bin", "rb");

	// testa a abertura do arquivo
	if(arq_cliente == NULL)
	{
		printf("\nFalha ao abrir arquivo(s) ou ");
		printf("Nenhum cliente cadastrado.\n");
		printf("\nPressione <Enter> para continuar...");
		scanf("%*c"); // pega o Enter e descarta

		// uma forma de "limpar" o buffer de entrada
		fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso
		return;
	}

	// vari�vel que indica se encontrou pelo menos 1 cliente
	int encontrou_cliente = 0;
	t_cliente cliente;

	printf("\nListando todos os clientes...\n");
	// loop para percorrer o arquivo
	while(1)
	{
		// fread retorna o n�mero de elementos lidos com sucesso
		size_t result = fread(&cliente, sizeof(t_cliente), 1, arq_cliente);

		// se for 0, � porque n�o h� mais elemento, ent�o sai do loop
		if(result == 0)
			break;

		// atualiza a vari�vel indicando que encontrou
		// pelo menos um cliente
		encontrou_cliente = 1;

		// mostra os dados do cliente
		printf("\nID do cliente: %d\n", cliente.id);
		printf("Nome do cliente: %s\n", cliente.nome);
		printf("CPF: %s\n", cliente.cpf);
		printf("Endereco: ");
		printf("\n\tRua: %s \n\tNumero %s \n\tCEP: %s \n\tBairro %s \n\tCidade: %s \n\tEstado: %s", cliente.endereco.rua, cliente.endereco.numero, cliente.endereco.cep, cliente.endereco.bairro, cliente.endereco.cidade, cliente.endereco.estado);
		printf("\nEmail: %s\n", cliente.email);
		printf("Data de Nascimento: %s/%s/%s\n", cliente.nascimento.dia, cliente.nascimento.mes, cliente.nascimento.ano);
		printf("Telefone: %s\n", cliente.telefone);
		
	}

	if(encontrou_cliente == 0)
		printf("\nNenhum cliente cadastrado.\n");

	fclose(arq_cliente);

	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");

	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso
}


// fun��o que obt�m um cliente pelo ID
t_cliente *obter_cliente(FILE *arq_cliente, int id_cliente)
{
	// vai para o in�cio do arquivo
	rewind(arq_cliente);

	t_cliente *cliente;

	// loop para percorrer o arquivo
	// busca linear O(n), como o ID � crescente � poss�vel fazer uma busca bin�ria O(log(n))
	// aloca espa�o mesmo sem saber se o cliente existe
	cliente = (t_cliente *)malloc(sizeof(t_cliente));
	while(1)
	{

		// fread retorna o n�mero de elementos lidos com sucesso
		size_t result = fread(cliente, sizeof(t_cliente), 1, arq_cliente);

		// se for 0, � porque n�o h� mais elemento, ent�o sai do loop
		if(result == 0)
		{
			free(cliente); // libera a m�moria, pois o cliente n�o foi encontrado
			return NULL;
		}
		// verifica se os ID's s�o iguais
		if(cliente->id == id_cliente)
			break;
	}
	return cliente;
}

// fun��o respons�vel pelo aluguel dos carros
void alugar_carro()
{
	char str_id_cliente[10];
	int id_cliente;

	// abre os arquivos para leitura/atualiza��o
	FILE *arq_carro = fopen("carro.bin", "rb+");
	FILE *arq_cliente = fopen("cliente.bin", "rb+");

	// se n�o conseguiu abrir, ent�o cria o arquivo
	// wb+ abre para escrita/atualiza��o (cria o arquivo se ele N�O existir)
	if(arq_carro == NULL)
	{
		arq_carro = fopen("carro.bin", "wb+");
		if(arq_carro == NULL)
		{
			printf("\nFalha ao criar arquivo(s)!\n");
			exit(1); // aborta o programa
		}
	}

	if(arq_cliente == NULL)
	{
		arq_cliente = fopen("cliente.bin", "wb+");
		if(arq_cliente == NULL)
		{
			printf("\nFalha ao criar arquivo(s)!\n");
			exit(1); // aborta o programa
		}
	}

	// obt�m o ID do cliente
	printf("\nDigite o ID do cliente: ");
	scanf("%10s%*c", str_id_cliente); // recebe o que foi digitado pelo usu�rio

	fseek(stdin, 0, SEEK_END); // coloca o ponteiro para o inicio do arquivo

	if(str_somente_numeros(str_id_cliente) == 1) // verifica se o que o usu�rio digitou � s� n�meros
	{
		sscanf(str_id_cliente, "%d", &id_cliente); // preenche a vari�vel "id_cliente" com o valor de "str_id_cliente"

		if(existe_cliente(arq_cliente, id_cliente)) // verifica se o ID do do cliente existe
		{
			char str_id_carro[10];
			int id_carro;

			printf("\nDigite o ID do carro: ");
			scanf("%10s%*c", str_id_carro);

			fseek(stdin, 0, SEEK_END);

			if(str_somente_numeros(str_id_carro) == 1)
			{
				sscanf(str_id_carro, "%d", &id_carro);

				t_carro *carro = obter_carro(arq_carro, id_carro); // obt�m o carro pelo ID
				
				if(carro != NULL) // verifica se o carro est� cadastrado
				{
					if(carro->id_cliente != -1) // verifica se o carro j� est� alugado
						printf("\nO carro \"%s\" ja esta alugado!\n", carro->modelo);
					else
					{
						carro->id_cliente = id_cliente;
						atualizar_carro(arq_carro, carro); // atualiza o carro no arquivo
						printf("\ncarro \"%s\" alugado com sucesso!\n", carro->modelo);
					}
					free(carro); // libera o carro alocado
				}
				else
					printf("\nNao existe carro com o ID \"%d\".\n", id_carro);
			}
			else
				printf("\nO ID so pode conter numeros!\n");
		}
		else
			printf("\nNao existe cliente com o ID \"%d\".\n", id_cliente);
	}
	else
		printf("\nO ID so pode conter numeros!\n");

	// fecha os arquivos
	fclose(arq_cliente);
	fclose(arq_carro);

	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");

	fseek(stdin, 0, SEEK_END); // volta o ponteiro para o inicio do arquivo
}

// fun��o para atualizar um carro
// recebe o ponteiro para o arquivo e o carro
void atualizar_carro(FILE *arq_carro, t_carro *carro_alugado)
{
	// vai para o in�cio do arquivo
	rewind(arq_carro);

	t_carro carro;
	while(1) // loop para percorrer o arquivo
	{

		size_t result = fread(&carro, sizeof(t_carro), 1, arq_carro); // fread retorna o n�mero de elementos lidos com sucesso

		// se for 0, � porque n�o h� mais elemento, ent�o sai do loop
		if(result == 0)
			break;

		if(carro.id == carro_alugado->id) // verifica se os ID's s�o iguais
		{
			fseek(arq_carro, - sizeof(t_carro), SEEK_CUR); // fseek posiciona o arquivo
	
			fwrite(carro_alugado, sizeof(t_carro), 1, arq_carro); // atualiza o carro
			break; // sai do loop
		}
	}
}

// fun��o que verifica se um cliente existe
int existe_cliente(FILE *arq_cliente, int id_cliente)
{
	rewind(arq_cliente); // vai para o in�cio do arquivo

	t_cliente cliente;

	while(1) // loop para percorrer o arquivo
	{
		size_t result = fread(&cliente, sizeof(t_cliente), 1, arq_cliente); // fread retorna o n�mero de elementos lidos com sucesso

		if(result == 0) // se for 0, � porque n�o h� mais elemento, ent�o sai do loop
			break;

		if(cliente.id == id_cliente) // verifica se o ID � igual
			return 1;
	}
	return 0; 	// se chegou aqui � porque N�O existe o cliente, ent�o retorna 0
}

// fun��o que obt�m um carro pelo ID
t_carro *obter_carro(FILE *arq_carro, int id_carro)
{
	rewind(arq_carro); // vai para o in�cio do arquivo

	t_carro *carro; // loop para percorrer o arquivo

	carro = (t_carro *)malloc(sizeof(t_carro)); 	// aloca espa�o
	while(1)
	{

		size_t result = fread(carro, sizeof(t_carro), 1, arq_carro); // fread retorna o n�mero de elementos lidos com sucesso

		if(result == 0) // se for 0, � porque n�o h� mais elemento, ent�o sai do loop
			break;

		if(carro->id == id_carro) // verifica se os ID's s�o iguais
			return carro;
	}
	free(carro); // libera recursos
	return NULL;
}

// fun��o respons�vel pela entrega dos carros
void entregar_carro()
{
	char str_id_carro[10];
	int id_carro;

	// rb+ abre para leitura/atualiza��o
	FILE *arq_carro = fopen("carro.bin", "rb+");

	// se n�o conseguiu abrir, ent�o cria o arquivo
	// wb+ abre para escrita/atualiza��o (cria o arquivo se ele N�O existir)
	if(arq_carro == NULL)
	{
		arq_carro = fopen("carro.bin", "wb+");
		if(arq_carro == NULL)
		{
			printf("\nFalha ao criar arquivo(s)!\n");
			exit(1); // aborta o programa
		}
	}

	printf("\nDigite o ID do carro: ");
	scanf("%10s%*c", str_id_carro);

	fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso

	if(str_somente_numeros(str_id_carro) == 1)
	{
		// se chegou aqui � porque o ID do carro � v�lido
		sscanf(str_id_carro, "%d", &id_carro);

		// obt�m o carro pelo ID
		t_carro *carro = obter_carro(arq_carro, id_carro);

		// testa se o carro existe...
		if(carro != NULL)
		{
			// se chegou aqui � porque o carro existe
			// testa se o carro j� foi entregue
			if(carro->id_cliente == -1)
				printf("\nO carro \"%s\" ja esta disponivel!\n", carro->modelo);
			else
			{
				// se o carro N�O foi entregue, ent�o seta o
				// id_cliente para -1 para indicar que ele foi entregue
				carro->id_cliente = -1;
				atualizar_carro(arq_carro, carro); // atualiza o carro no arquivo
				printf("\ncarro \"%s\" entregue com sucesso!\n", carro->modelo);
			}
			free(carro); // libera mem�ria
		}
		else
			printf("\nNao existe carro com o ID \"%d\".\n", id_carro);
	}
	else
		printf("\nO ID so pode conter numeros!\n");

	// fecha o arquivo
	fclose(arq_carro);

	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");

	fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso
}

// fun��o respons�vel por excluir carros
void excluir_carro()
{
	char str_id_carro[10];
	int id_carro;

	printf("\nDigite o ID do carro: ");
	scanf("%10s%*c", str_id_carro);

	fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso

	// verifica se str_id_carro s� cont�m n�meros
	if(str_somente_numeros(str_id_carro) == 1)
	{
		// se chegou aqui � porque o ID do carro v�lido
		sscanf(str_id_carro, "%d", &id_carro);

		// rb abre para leitura (o arquivo deve existir)
		FILE *arq_carro = fopen("carro.bin", "rb");

		if(arq_carro == NULL)
		{
			printf("\nFalha ao abrir arquivo(s)!\n");
			exit(1); // aborta o programa
		}

		// verifica se o carro existe
		if(existe_carro(arq_carro, id_carro) == 1)
		{
			char nome_carro[MAX];
			// abre um novo arquivo tempor�rio
			FILE *arq_temp = fopen("temp_carro.bin", "a+b");
			if(arq_temp == NULL)
			{
				printf("\nFalha ao criar arquivo temporario!\n");
				fclose(arq_carro);
				exit(1); // aborta o programa
			}
			rewind(arq_carro); // vai para o in�cio do arquivo

			t_carro carro;
			while(1) // loop para percorrer o arquivo
			{

				// fread retorna o n�mero de elementos lidos com sucesso
				size_t result = fread(&carro, sizeof(t_carro), 1, arq_carro);

				// se for 0, � porque n�o h� mais elemento, ent�o sai do loop
				if(result == 0)
					break;

				// s� copia pro novo arquivo se for diferente
				if(carro.id != id_carro)
				{
					// escreve no arquivo tempor�rio
					fwrite(&carro, sizeof(t_carro), 1, arq_temp);
				}
				else
					strcpy(nome_carro, carro.modelo);
			}

			// antes de fazer opera��es de remover arquivo e renomear,
			// � preciso fechar os dois arquivos
			fclose(arq_carro);
			fclose(arq_temp);

			// depois de fechar o arquivo, ent�o tentamos remover
			if(remove("carro.bin") != 0)
				printf("\nErro ao deletar o arquivo \"carro.bin\"\n");
			else
			{
				// renomeia o arquivo
				int r = rename("temp_carro.bin", "carro.bin");
				if(r != 0)
				{
					printf("\nPermissao negada para renomear o arquivo!\n");
					printf("Feche esse programa bem como o arquivo \"temp_carro.bin\" e renomeie manualmente para \"carro.bin\"\n");
				}
				else
					printf("\ncarro \"%s\" removido com sucesso!\n", nome_carro);
			}
		}
		else
		{
			fclose(arq_carro);
			printf("\nNao existe carro com o ID \"%d\".\n", id_carro);
		}
	}
	else
		printf("\nO ID so pode conter numeros!\n");

	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
	fseek(stdin, 0, SEEK_END); // n�o recomend�vel o uso
}


