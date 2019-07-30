#include <iostream>
#include <locale.h>
#include <ctime>
#include <windows.h>
#include <conio.h>

using namespace std;

//DECLARANDO FUNCOES
string dataAtual();
void alterarSaldo(FILE *arquivo);
void visualizarSaldo(FILE* arquivo);
void deposito(FILE* arquivo);
void historicoDepositos(FILE* arquivo);
void saque(FILE* arquivo);
void historicoSaques(FILE* arquivo);

typedef struct
{
    float saldo;
    char dataSaldo[50];
}dadosSaldo;

typedef struct
{
    char info[256];
    char data[50];
    float valor;
}infoTransacoes;

void stringToChar(string origem, char *destino)
{
    for(unsigned int i=0; i<sizeof(origem); i++)
    {
        destino[i]=origem[i];
    }
}

string dataAtual()
{
    time_t timer;
    struct tm *horarioLocal;

    time(&timer); // Obtem informações de data e hora
    horarioLocal = localtime(&timer); // Converte a hora atual para a hora local

    int dia = horarioLocal->tm_mday;
    int mes = horarioLocal->tm_mon + 1;
    int ano = horarioLocal->tm_year + 1900;
    /*
    int hora = horarioLocal->tm_hour;
    int min  = horarioLocal->tm_min;
    int sec  = horarioLocal->tm_sec;

    cout << "Horário: " << hora << ":" << min << ":" << sec << endl;
    cout << "Data: "    << dia  << "/" << mes << "/" << ano << endl;
    */
    string data = to_string(dia) + "/" + to_string(mes) + "/" + to_string(ano);
    return data;
}

void alterarSaldo(FILE *arquivo)
{
    arquivo=fopen("Saldo.dat", "wb");
    dadosSaldo dados;
    string data;
    if(arquivo==NULL)
    {
        cout << "ERRO DE ARQUIVO" << endl;
    }
    else
    {
        cout << "+---------------------------------+" << endl;
        cout << "|-------AUXILIAR FINANCEIRO-------|" << endl;
        cout << "|---------Cadastrar Saldo---------|" << endl;
        cout << "|Informe o saldo: R$";
        cin >> dados.saldo;
        data=dataAtual();
        stringToChar(data, dados.dataSaldo);
        cout<<"|Data: "<<dados.dataSaldo<<endl;
        fwrite(&dados, sizeof(dadosSaldo), 1, arquivo);
    }
    fclose(arquivo);
}

void visualizarSaldo(FILE* arquivo)
{
    arquivo=fopen("Saldo.dat", "rb");
    dadosSaldo* dados=(dadosSaldo*)malloc(sizeof(dadosSaldo));
    if(arquivo==NULL)
    {
        cout << "ERRO DE ARQUIVO" << endl;
    }
    else
    {
        fread(dados, sizeof(dadosSaldo), 1, arquivo);
        cout << "+---------------------------------+" << endl;
        cout << "|-------AUXILIAR FINANCEIRO-------|" << endl;
        cout << "|--------Visualizar Saldo---------|" << endl;
        cout << fixed;
        cout.precision(2);
        cout << "|Saldo: ";
        if(dados->saldo<=0)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
        else
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
        cout << "R$"<<dados->saldo<<endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
        cout << "|Última atualização: "<<dados->dataSaldo<<endl;
        cout << "+---------------------------------+" << endl;
        free(dados);
    }
    fclose(arquivo);
}

void deposito(FILE* arquivo)
{
    arquivo=fopen("Depositos.dat","ab");
    FILE* arquivoSaldoIn = fopen("Saldo.dat", "rb");

    infoTransacoes deposito;
    dadosSaldo infoSaldo;

    if(arquivo==NULL)
    {
        cout << "ERRO DE ARQUIVO" << endl;
    }
    else
    {
        cout << "+---------------------------------+" << endl;
        cout << "|-------AUXILIAR FINANCEIRO-------|" << endl;
        cout << "|------------Depósito-------------|" << endl;
        cout << "|Valor: R$";
        cin >> deposito.valor;
        if(deposito.valor<0)
        {
            cout<<"Parece que você quer sacar... Redirecionando para função de saque :P"<<endl;
            Sleep(2000);
            system("cls");
            saque(arquivo);
            fclose(arquivo);
            return;
        }
        stringToChar(dataAtual(), deposito.data);
        cout << "|Data: "<<deposito.data<<endl;
        cout << "|Adicionar mais informações[S/N]? ";
        char opc;
        cin >> opc;

        if(opc=='S'|| opc=='s'){
            cout << "|Infomações adicionais: ";
            cin.ignore();
            cin.getline(deposito.info, 256);
        }
        else
        {
            stringToChar("N/A", deposito.info);
        }

        fread(&infoSaldo, sizeof(dadosSaldo), 1, arquivoSaldoIn);

        infoSaldo.saldo=infoSaldo.saldo+deposito.valor;

        stringToChar(dataAtual(), infoSaldo.dataSaldo);

        fclose(arquivoSaldoIn);

        fwrite(&deposito, sizeof(infoTransacoes), 1, arquivo);
        fclose(arquivo);

        FILE* arquivoSaldoOut = fopen("Saldo.dat","wb");
        fwrite(&infoSaldo, sizeof(dadosSaldo), 1, arquivoSaldoOut);

        fclose(arquivoSaldoOut);
    }
}

void historicoDepositos(FILE* arquivo)
{
    arquivo=fopen("Depositos.dat", "rb");
    infoTransacoes deposito;
    if(arquivo==NULL)
    {
        cout << "ERRO DE ARQUIVO"<<endl;
    }
    else
    {
        cout << "+---------------------------------+" << endl;
        cout << "|-------AUXILIAR FINANCEIRO-------|" << endl;
        cout << "|------Histórico de Depósitos-----|" << endl;
        cout << "+---------------------------------+" << endl;
        while(!feof(arquivo) && fread(&deposito, sizeof(infoTransacoes), 1, arquivo)==1)
        {
            cout << fixed;
            cout.precision(2);
            cout << "Valor do Depósito..........";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9); //AZUL
            cout<<"+R$"<<deposito.valor<<endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            cout << "Data do Depósito..........." << deposito.data<<endl;
            cout << "Informações................" << deposito.info<<endl;
            cout << "-----------------------------------" <<endl;
        };
    }
    fclose(arquivo);
}

void saque(FILE* arquivo)
{
    arquivo = fopen("Saques.dat","ab");
    FILE* arquivoSaldoIn = fopen("Saldo.dat", "rb");

    infoTransacoes saque;
    dadosSaldo infoSaldo;

    if(arquivo==NULL || arquivoSaldoIn==NULL)
    {
        cout << "ERRO DE ARQUIVO" << endl;
    }
    else
    {
        cout << "+---------------------------------+" << endl;
        cout << "|-------AUXILIAR FINANCEIRO-------|" << endl;
        cout << "|--------------Saque--------------|" << endl;
        cout << "|Valor: R$";
        cin >> saque.valor;
        if(saque.valor<0)
        {
            cout<<"Parece que você quer depositar... \nRedirecionando para função de depósito :P"<<endl;
            Sleep(2000);
            system("cls");
            deposito(arquivo);
            fclose(arquivo);
            return;
        }

        stringToChar(dataAtual(), saque.data);

        cout << "|Data: "<<saque.data<<endl;
        cout << "|Adicionar mais informações[S/N]? ";
        char opc;
        cin >> opc;

        if(opc=='S'|| opc=='s'){
            cout << "|Infomações adicionais: ";
            cin.ignore();
            cin.getline(saque.info, 256);
        }
        else
        {
            stringToChar("N/A", saque.info);
        }

        fread(&infoSaldo, sizeof(dadosSaldo), 1, arquivoSaldoIn);

        infoSaldo.saldo=infoSaldo.saldo-saque.valor;

        stringToChar(dataAtual(), infoSaldo.dataSaldo);

        fclose(arquivoSaldoIn);

        fwrite(&saque, sizeof(infoTransacoes), 1, arquivo);
        fclose(arquivo);

        FILE* arquivoSaldoOut = fopen("Saldo.dat","wb");
        fwrite(&infoSaldo, sizeof(dadosSaldo), 1, arquivoSaldoOut);

        fclose(arquivoSaldoOut);
    }
}

void historicoSaques(FILE* arquivo)
{
    arquivo=fopen("Saques.dat", "rb");
    infoTransacoes saque;
    if(arquivo==NULL)
    {
        cout << "ERRO DE ARQUIVO"<<endl;
    }
    else
    {
        cout << "+---------------------------------+" << endl;
        cout << "|-------AUXILIAR FINANCEIRO-------|" << endl;
        cout << "|-------Histórico de Saques-------|" << endl;
        cout << "+---------------------------------+" << endl;
        while(!feof(arquivo) && fread(&saque, sizeof(infoTransacoes), 1, arquivo)==1)
        {
            cout << fixed;
            cout.precision(2);
            cout << "Valor do Saque..........";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12); //VERMELHO
            cout<<"-R$"<<saque.valor<<endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            cout << "Data do Saque..........." << saque.data<<endl;
            cout << "Informações............." << saque.info<<endl;
            cout << "-----------------------------------" <<endl;
        };
    }
    fclose(arquivo);
}

