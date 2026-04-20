#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>

using namespace std;

// Estructura para representar cada celda del parqueadero
struct Celda {
    char tipo; // 'P' Pared, 'V' Via, 'S' Slot (Parqueadero), 'E' Entrada, 'L' Salida
    bool ocupado;
    string placa;
    time_t horaEntrada;
};

const int TAM = 16;
const double TARIFA_MINUTO = 100.0; // Valor ejemplo en Pesos Colombianos

// --- PROTOTIPOS (Obligatorio usar funciones) ---
void inicializarMapa(Celda mapa[TAM][TAM]);
void mostrarMapa(Celda mapa[TAM][TAM]);
void registrarIngreso(Celda mapa[TAM][TAM]);
void registrarSalida(Celda mapa[TAM][TAM]);
Celda* buscarEspacioLibre(Celda mapa[TAM][TAM]); // Uso obligatorio de PUNTEROS

void inicializarMapa(Celda mapa[TAM][TAM]) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            mapa[i][j].ocupado = false;
            mapa[i][j].placa = "";
            // Diseño del mapa: Paredes en los bordes
            if (i == 0 || i == TAM - 1 || j == 0 || j == TAM - 1) mapa[i][j].tipo = 'P';
            // Vías de acceso (Pasillos)
            else if (j == 4 || j == 11 || i == 7) mapa[i][j].tipo = 'V';
            // Entrada y Salida
            else if (i == 1 && j == 0) mapa[i][j].tipo = 'E';
            else if (i == TAM - 2 && j == TAM - 1) mapa[i][j].tipo = 'L';
            // Resto son espacios de parqueo
            else mapa[i][j].tipo = 'S';
        }
    }
}

void mostrarMapa(Celda mapa[TAM][TAM]) {
    cout << "\n=== ESTADO DEL PARQUEADERO (16x16) ===\n";
    cout << "  ";
    for(int j=0; j<TAM; j++) cout << hex << j << " "; 
    cout << endl;

    for (int i = 0; i < TAM; i++) {
        cout << hex << i << " ";
        for (int j = 0; j < TAM; j++) {
            if (mapa[i][j].tipo == 'P') cout << "██";
            else if (mapa[i][j].tipo == 'V') cout << "  ";
            else if (mapa[i][j].tipo == 'S') {
                if (mapa[i][j].ocupado) cout << "\033[31mRR\033[0m"; // Rojo si está ocupado
                else cout << "\033[32mLL\033[0m"; // Verde si está libre
            }
            else if (mapa[i][j].tipo == 'E') cout << "IN";
            else if (mapa[i][j].tipo == 'L') cout << "OUT";
            cout << " ";
        }
        cout << endl;
    }
    cout << "\033[32mLL: Libre\033[0m | \033[31mRR: Ocupado\033[0m | ██: Pared\n";
}

// Implementación usando PUNTEROS (Requisito 3)
Celda* buscarEspacioLibre(Celda mapa[TAM][TAM]) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (mapa[i][j].tipo == 'S' && !mapa[i][j].ocupado) {
                return &mapa[i][j]; // Retornamos la dirección de memoria
            }
        }
    }
    return nullptr;
}

void registrarIngreso(Celda mapa[TAM][TAM]) {
    Celda* espacio = buscarEspacioLibre(mapa);
    if (espacio == nullptr) {
        cout << "¡Lo sentimos! Parqueadero lleno.\n";
        return;
    }
    cout << "Ingrese la placa del vehiculo: ";
    cin >> espacio->placa;
    espacio->ocupado = true;
    espacio->horaEntrada = time(0);
    cout << "Vehiculo registrado con exito en el espacio seleccionado.\n";
}

void registrarSalida(Celda mapa[TAM][TAM]) {
    string placa;
    cout << "Ingrese la placa a retirar: ";
    cin >> placa;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (mapa[i][j].ocupado && mapa[i][j].placa == placa) {
                time_t ahora = time(0);
                double segundos = difftime(ahora, mapa[i][j].horaEntrada);
                double minutos = segundos / 60.0;
                if (minutos < 1) minutos = 1; // Cobro mínimo 1 min para el ejemplo

                cout << "Tiempo de permanencia: " << fixed << setprecision(2) << minutos << " minutos.\n";
                cout << "Total a pagar: $" << minutos * TARIFA_MINUTO << " COP.\n";
                
                mapa[i][j].ocupado = false;
                mapa[i][j].placa = "";
                cout << "Salida registrada con exito.\n";
                return;
            }
        }
    }
    cout << "Vehiculo no encontrado.\n";
}

int main() {
    Celda parqueadero[TAM][TAM];
    inicializarMapa(parqueadero);
    int opcion;

    do {
        cout << "\n--- SISTEMA DE GESTION: JUAN MEJIA & KEVIN BARRERA ---\n";
        cout << "1. Ver Mapa\n2. Registrar Ingreso\n3. Registrar Salida\n4. Salir\nOpcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: mostrarMapa(parqueadero); break;
            case 2: registrarIngreso(parqueadero); break;
            case 3: registrarSalida(parqueadero); break;
        }
    } while (opcion != 4);

    return 0;
}
