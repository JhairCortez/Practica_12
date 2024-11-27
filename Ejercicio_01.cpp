// Materia: Programación I, Paralelo 1
// Autor: Jhair Anderson Cortez Peña
// Fecha creación: 27/11/2024
// Número de ejercicio: 1
// Problema planteado: Crear un vector que contenga los siguientes datos para los clientes de un banco
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct Cliente 
{
    int numeroCuenta;
    char nombre[50];
    double saldo;
    char fechaApertura[15];
};

struct VectorClientes 
{
    Cliente* datos;
    int tamano;
    int capacidad;
    void inicializar(int cap) 
    {
        tamano = 0;
        capacidad = cap;
        datos = new Cliente[cap];
    }
    void agregar(Cliente cliente) 
    {
        if (tamano == capacidad) 
        {
            capacidad *= 2;
            Cliente* nuevoDatos = new Cliente[capacidad];
            for (int i = 0; i < tamano; ++i) 
            {
                nuevoDatos[i] = datos[i];
            }
            delete[] datos;
            datos = nuevoDatos;
        }
        datos[tamano++] = cliente;
    }
    void liberar() 
    {
        delete[] datos;
        tamano = 0;
        capacidad = 0;
    }
};

void procesarTransacciones(VectorClientes& clientes, const char* archivo) 
{
    ifstream archivoTransacciones(archivo);
    if (!archivoTransacciones.is_open()) 
    {
        cout << "Error al abrir el archivo de transacciones." << endl;
        return;
    }
    char linea[100];
    while (archivoTransacciones.getline(linea, sizeof(linea))) 
    {
        int numeroCuenta;
        char tipoTransaccion[10];
        double monto;
        char* token = strtok(linea, ";");
        numeroCuenta = atoi(token);
        token = strtok(nullptr, ";");
        strcpy(tipoTransaccion, token);
        token = strtok(nullptr, ";");
        monto = atof(token);
        for (int i = 0; i < clientes.tamano; ++i) {
            if (clientes.datos[i].numeroCuenta == numeroCuenta) 
            {
                if (strcmp(tipoTransaccion, "Deposito") == 0) 
                {
                    clientes.datos[i].saldo += monto;
                } else if (strcmp(tipoTransaccion, "Retiro") == 0) 
                {
                    clientes.datos[i].saldo -= monto;
                }
                break;
            }
        }
    }

    archivoTransacciones.close();
}

void mostrarClientes(VectorClientes& clientes) 
{
    for (int i = 0; i < clientes.tamano; ++i) 
    {
        cout << "Cuenta: " << clientes.datos[i].numeroCuenta
             << ", Nombre: " << clientes.datos[i].nombre
             << ", Saldo: " << clientes.datos[i].saldo
             << ", Fecha: " << clientes.datos[i].fechaApertura << endl;
    }
}

int main() 
{
    VectorClientes clientes;
    clientes.inicializar(5);
    Cliente cliente1 = {12345, "Juan Perez", 5000.00, "2022-01-01"};
    Cliente cliente2 = {11532, "Maria Gomez", 3000.00, "2023-02-15"};
    Cliente cliente3 = {83767, "Carlos Lopez", 1500.00, "2021-10-20"};
    clientes.agregar(cliente1);
    clientes.agregar(cliente2);
    clientes.agregar(cliente3);
    cout << "Datos iniciales de los clientes:" << endl;
    mostrarClientes(clientes);
    procesarTransacciones(clientes, "Transacciones.txt");
    cout << "\nDatos de los clientes después de procesar transacciones:" << endl;
    mostrarClientes(clientes);
    clientes.liberar();
    return 0;
}