
#include "funcoes.h"

int main()
{
    FILE *arquivo;
    setlocale(LC_ALL, "portuguese");
    SetConsoleOutputCP(1254);
    SetConsoleCP(1254);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15); //BRANCO
    int opc;
    do
    {
        system("cls");
        cout << "+---------------------------------+" << endl;
        cout << "|-------AUXILIAR FINANCEIRO-------|" << endl;
        cout << "|1 - Alterar Saldo Manualmente    |" << endl;
        cout << "|2 - Cadastrar Depósito           |" << endl;
        cout << "|3 - Cadastrar Saque              |" << endl;
        cout << "|4 - Visualizar Saldo             |" << endl;
        cout << "|5 - Histórico de Saques          |" << endl;
        cout << "|6 - Histórico de Depósitos       |" << endl;
        cout << "|7 - Sair                         |" << endl;
        cout << "+---------------------------------+" << endl;
        cout << "Opção: ";
        cin >> opc;
        switch(opc)
        {
        case 1:
            system("cls");
            alterarSaldo(arquivo);
            system("pause");
            break;
        case 2:
            system("cls");
            deposito(arquivo);
            system("pause");
            break;
        case 3:
            system("cls");
            saque(arquivo);
            system("pause");
            break;
        case 4:
            system("cls");
            visualizarSaldo(arquivo);
            system("pause");
            break;
        case 5:
            system("cls");
            historicoSaques(arquivo);
            system("pause");
            break;
        case 6:
            system("cls");
            historicoDepositos(arquivo);
            system("pause");
            break;
        case 7:
            cout << "Saindo..."<<endl;
            break;
        default:
            cout << "Opção Incorreta" << endl;
            break;
        }
    }while(opc!=7);
    return 0;
}
