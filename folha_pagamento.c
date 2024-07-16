#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*ESTRUTURAS*/

/*CONTRATOS*/
typedef enum
{
    CLT,
    TERCEIRIZADO
} TipoContrato;

/*FUNCIONÁRIO*/
typedef struct
{
    char nome[50];
    TipoContrato tipo_contrato;
    union
    {
        struct
        {
            float salario;
        } clt;
        struct
        {
            float valor_por_hora;
            int horas_trabalhadas;
        } terceirizado;
    } detalhes;
    int contratos_assinados;
} Funcionario;

/*FUNÇÕES*/

/*ENTRADA DE NUMERO*/
int ehNumero(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
        {
            return 0;
        }
    }
    return 1;
}

/*LEITURA DE ENTRADA*/
void lerEntrada(char *prompt, void *valor, char tipo)
{
    char entrada[50];
    while (1)
    {
        printf("%s", prompt);
        scanf("%s", entrada);
        if (tipo == 'f' && sscanf(entrada, "%f", (float *)valor) == 1)
            break;
        if (tipo == 'd' && ehNumero(entrada))
        {
            *(int *)valor = atoi(entrada);
            break;
        }
        printf("Digite um número válido!\n");
    }
}

/*CADASTRO DE FUNCIONÁRIO*/
void cadastrarFuncionario(Funcionario *f)
{
    printf("Nome: ");
    scanf("%s", f->nome);

    lerEntrada("Tipo de contrato (0 - CLT, 1 - Terceirizado): ", &f->tipo_contrato, 'd');

    if (f->tipo_contrato == CLT)
    {
        lerEntrada("Salario: ", &f->detalhes.clt.salario, 'f');
    }
    else
    {
        lerEntrada("Valor por hora: ", &f->detalhes.terceirizado.valor_por_hora, 'f');
        lerEntrada("Horas trabalhadas: ", &f->detalhes.terceirizado.horas_trabalhadas, 'd');
    }

    lerEntrada("Contratos assinados: ", &f->contratos_assinados, 'd');
}

/*CALCULO PARA CADA FUNCIONÁRIO*/
float calcularTotalPagar(Funcionario f, float comissao_por_contrato)
{
    float total = 0.0;
    if (f.tipo_contrato == CLT)
    {
        total = f.detalhes.clt.salario;
    }
    else
    {
        total = f.detalhes.terceirizado.valor_por_hora * f.detalhes.terceirizado.horas_trabalhadas;
    }
    total += f.contratos_assinados * comissao_por_contrato;
    return total;
}

/*EXIBIR FUNCIONÁRIO*/
void exibirFuncionario(Funcionario f, float comissao_por_contrato)
{
    printf("\n---------------\n");
    printf("%s:\n", f.nome);
    if (f.tipo_contrato == CLT)
    {
        printf("Contrato: CLT\n");
        printf("Salario: R$ %.2f\n", f.detalhes.clt.salario);
    }
    else
    {
        printf("Contrato: Terceirizado\n");
        printf("Valor por hora: R$ %.2f\n", f.detalhes.terceirizado.valor_por_hora);
        printf("Horas trabalhadas: %d\n", f.detalhes.terceirizado.horas_trabalhadas);
    }
    printf("Contratos assinados: %d\n", f.contratos_assinados);
    if (f.contratos_assinados > 0)
    {
        printf("Comissão por contrato: R$ %.2f\n", comissao_por_contrato);
    }
    printf("Total à receber: R$ %.2f\n", calcularTotalPagar(f, comissao_por_contrato));
    printf("---------------\n");
}

int main()
{
    int quantidade_funcionarios;
    float comissao_por_contrato;

    lerEntrada("Valor do bonus fixo por contrato assinado: ", &comissao_por_contrato, 'f');
    lerEntrada("Quantidade de funcionarios: ", &quantidade_funcionarios, 'd');

    if (quantidade_funcionarios <= 0)
    {
        printf("A quantidade de funcionarios deve ser maior que 0.\n");
        return 1;
    }

    /*ALOCAÇÃO EM MEMÓRIA*/
    Funcionario *funcionarios = (Funcionario *)malloc(quantidade_funcionarios * sizeof(Funcionario));
    if (funcionarios == NULL)
    {
        printf("Erro ao gravar na memória.\n");
        return 1;
    }

    for (int i = 0; i < quantidade_funcionarios; i++)
    {
        printf("---------------\n");
        printf("Funcionario %d:\n", i + 1);
        printf("---------------\n");
        cadastrarFuncionario(&funcionarios[i]);
    }

    printf("\nResultados:\n");
    float totalFolha = 0.0;

    for (int i = 0; i < quantidade_funcionarios; i++)
    {
        exibirFuncionario(funcionarios[i], comissao_por_contrato);

        /*CALCULO TOTAL DA FOLHA DE PAGAMENTO*/
        totalFolha += calcularTotalPagar(funcionarios[i], comissao_por_contrato);
    }

    printf("\n===============\n");
    printf("Custo total da folha de pagamento da empresa: R$ %.2f\n", totalFolha);
    printf("===============\n");

    free(funcionarios);

    printf("Pressione qualquer tecla para continuar...\n");
    getchar();
    getchar();

    return 0;
}
