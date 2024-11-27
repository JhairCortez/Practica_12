// Materia: Programación I, Paralelo 1
// Autor: Jhair Anderson Cortez Peña
// Fecha creación: 27/11/2024
// Número de ejercicio: 2
// Problema planteado: crear un programa que permita manipular las siguientes funciones
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct Cliente 
{
    int numeroCuenta;
    char nombre[50];
    double saldo;
};

void crearCuenta(const char* archivoBinario) 
{
    Cliente cliente;
    cout << "Ingrese el número de cuenta: ";
    cin >> cliente.numeroCuenta;
    cin.ignore();
    cout << "Ingrese el nombre: ";
    cin.getline(cliente.nombre, 50);
    cout << "Ingrese el saldo inicial: ";
    cin >> cliente.saldo;
    ofstream archivo(archivoBinario, ios::binary | ios::app);
    if (!archivo) 
    {
        cout << "Error al abrir el archivo binario." << endl;
        return;
    }
    archivo.write((char*)&cliente, sizeof(Cliente));
    archivo.close();
    cout << "Cuenta creada exitosamente." << endl;
}

void listarCuentas(const char* archivoBinario) 
{
    ifstream archivo(archivoBinario, ios::binary);
    if (!archivo) 
    {
        cout << "Error al abrir el archivo binario." << endl;
        return;
    }
    Cliente cliente;
    cout << "Nº Cuenta\tNombre\t\tSaldo" << endl;
    while (archivo.read((char*)&cliente, sizeof(Cliente))) 
    {
        cout << cliente.numeroCuenta << "\t\t" << cliente.nombre << "\t\t" << cliente.saldo << endl;
    }
    archivo.close();
}

void modificarCuenta(const char* archivoBinario) 
{
    int numeroCuenta;
    cout << "Ingrese el número de cuenta a modificar: ";
    cin >> numeroCuenta;
    fstream archivo(archivoBinario, ios::binary | ios::in | ios::out);
    if (!archivo) 
    {
        cout << "Error al abrir el archivo binario." << endl;
        return;
    }
    Cliente cliente;
    while (archivo.read((char*)&cliente, sizeof(Cliente))) 
    {
        if (cliente.numeroCuenta == numeroCuenta) 
        {
            cout << "Ingrese el nuevo número de cuenta: ";
            cin >> cliente.numeroCuenta;
            cin.ignore();
            cout << "Ingrese el nuevo nombre: ";
            cin.getline(cliente.nombre, 50);
            archivo.seekp((int)archivo.tellg() - sizeof(Cliente));
            archivo.write((char*)&cliente, sizeof(Cliente));
            cout << "Cuenta modificada exitosamente." << endl;
            archivo.close();
            return;
        }
    }
    cout << "Cuenta no encontrada." << endl;
    archivo.close();
}

void eliminarCuenta(const char* archivoBinario) 
{
    int numeroCuenta;
    cout << "Ingrese el número de cuenta a eliminar: ";
    cin >> numeroCuenta;
    ifstream archivo(archivoBinario, ios::binary);
    ofstream archivoTemp("Temp.bin", ios::binary);
    if (!archivo || !archivoTemp) 
    {
        cout << "Error al abrir los archivos." << endl;
        return;
    }
    Cliente cliente;
    bool encontrado = false;
    while (archivo.read((char*)&cliente, sizeof(Cliente))) 
    {
        if (cliente.numeroCuenta == numeroCuenta) 
        {
            encontrado = true;
        } else 
        {
            archivoTemp.write((char*)&cliente, sizeof(Cliente));
        }
    }
    archivo.close();
    archivoTemp.close();
    remove(archivoBinario);
    rename("Temp.bin", archivoBinario);
    if (encontrado) 
    {
        cout << "Cuenta eliminada exitosamente." << endl;
    } else {
        cout << "Cuenta no encontrada." << endl;
    }
}

void adicionarTransaccion(const char* archivoTexto) 
{
    int numeroCuenta;
    double monto;
    cout << "Ingrese el número de cuenta: ";
    cin >> numeroCuenta;
    cout << "Ingrese el monto de la transacción (positivo para depósito, negativo para retiro): ";
    cin >> monto;
    ofstream archivo(archivoTexto, ios::app);
    if (!archivo) {
        cout << "Error al abrir el archivo de transacciones." << endl;
        return;
    }
    archivo << numeroCuenta << ";" << monto << endl;
    archivo.close();
    cout << "Transacción adicionada exitosamente." << endl;
}

void actualizarSaldos(const char* archivoBinario, const char* archivoTexto) 
{
    ifstream archivoTransacciones(archivoTexto);
    if (!archivoTransacciones) 
    {
        cout << "Error al abrir el archivo de transacciones." << endl;
        return;
    }
    struct Transaccion 
    {
        int numeroCuenta;
        double monto;
    };
    Transaccion transacciones[100];
    int totalTransacciones = 0;
    char linea[100];
    while (archivoTransacciones.getline(linea, sizeof(linea))) 
    {
        int numeroCuenta;
        double monto;
        char* token = strtok(linea, ";");
        numeroCuenta = atoi(token);
        token = strtok(nullptr, ";");
        monto = atof(token);

        transacciones[totalTransacciones++] = {numeroCuenta, monto};
    }
    archivoTransacciones.close();
    fstream archivoCuentas(archivoBinario, ios::binary | ios::in | ios::out);
    if (!archivoCuentas) 
    {
        cout << "Error al abrir el archivo binario." << endl;
        return;
    }
    Cliente cliente;
    for (int i = 0; i < totalTransacciones; ++i) 
    {
        archivoCuentas.clear();
        archivoCuentas.seekg(0);
        while (archivoCuentas.read((char*)&cliente, sizeof(Cliente))) 
        {
            if (cliente.numeroCuenta == transacciones[i].numeroCuenta) 
            {
                cliente.saldo += transacciones[i].monto;
                archivoCuentas.seekp((int)archivoCuentas.tellg() - sizeof(Cliente));
                archivoCuentas.write((char*)&cliente, sizeof(Cliente));
                break;
            }
        }
    }
    archivoCuentas.close();
    cout << "Saldos actualizados exitosamente." << endl;
}

int main() 
{
    const char* archivoBinario = "Cuentas.bin";
    const char* archivoTexto = "Transacciones.txt";
    int opcion;
    do 
    {
        cout << "\n--- Menú ---\n";
        cout << "1. Crear cuenta bancaria\n";
        cout << "2. Listar cuentas bancarias\n";
        cout << "3. Modificar cuenta\n";
        cout << "4. Eliminar cuenta\n";
        cout << "5. Adicionar transacción\n";
        cout << "6. Actualizar saldos desde transacciones\n";
        cout << "7. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        switch (opcion) 
        {
            case 1: crearCuenta(archivoBinario); break;
            case 2: listarCuentas(archivoBinario); break;
            case 3: modificarCuenta(archivoBinario); break;
            case 4: eliminarCuenta(archivoBinario); break;
            case 5: adicionarTransaccion(archivoTexto); break;
            case 6: actualizarSaldos(archivoBinario, archivoTexto); break;
            case 7: cout << "Saliendo del programa.\n"; break;
            default: cout << "Opción inválida. Intente de nuevo.\n";
        }
    } while (opcion != 7);
    return 0;
}