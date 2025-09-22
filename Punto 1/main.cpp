#include <iostream>   
#include <vector>     
#include <cstdlib>    // Para rand() y srand(), generación de números aleatorios
#include <ctime>      // Para time(), usado en srand() como semilla
#include <chrono>     // Para medir tiempos de ejecución con high_resolution_clock

using namespace std;
using namespace std::chrono;

// ===============================================================
// Función: generarMatriz
// Genera una matriz cuadrada n x n con valores aleatorios en [0,1).
// 
// Parámetros:
//   n -> tamaño de la matriz (filas = columnas).
// Devuelve:
//   Una matriz (vector de vectores de double) de dimensión n x n.
// ===============================================================
vector<vector<double>> generarMatriz(int n) {
    vector<vector<double>> M(n, vector<double>(n)); // Matriz n x n inicializada en 0
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // rand() genera un entero en [0, RAND_MAX].
            // Lo dividimos por RAND_MAX para obtener un número en [0,1).
            M[i][j] = (double)rand() / RAND_MAX;
        }
    }
    return M;
}

// ===============================================================
// Función: matmul_base
// Multiplicación clásica de matrices con orden de bucles i-j-k.
// 
// Parámetros:
//   A -> Matriz cuadrada n x n
//   B -> Matriz cuadrada n x n
// Devuelve:
//   C = A * B (matriz n x n).
//
// Características:
//   - Complejidad O(n^3).
//   - Orden más básico y fácil de entender.
// ===============================================================
vector<vector<double>> matmul_base(const vector<vector<double>>& A,
                            const vector<vector<double>>& B) {
    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n, 0.0)); // Inicializamos en 0

    for (int i = 0; i < n; i++) {          // Recorremos filas de A
        for (int j = 0; j < n; j++) {      // Recorremos columnas de B
            for (int k = 0; k < n; k++) {  // Producto escalar fila*columna
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

// ===============================================================
// Función: matmul_ikj
// Multiplicación optimizada con orden de bucles i-k-j.
// 
// Parámetros:
//   A -> Matriz cuadrada n x n
//   B -> Matriz cuadrada n x n
// Devuelve:
//   C = A * B (matriz n x n).
//
// Características:
//   - También O(n^3), pero cambia el orden de acceso.
//   - Mejora el rendimiento al aprovechar mejor la memoria caché
//     (se accede de forma más secuencial a B y C).
// ===============================================================
vector<vector<double>> matmul_ikj(const vector<vector<double>>& A,
                                const vector<vector<double>>& B) {
    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; i++) {          // Fila de A
        for (int k = 0; k < n; k++) {      // Columna intermedia
            double a = A[i][k];            // Guardamos A[i][k] en registro (más rápido)
            for (int j = 0; j < n; j++) {  // Recorremos fila de B y C
                C[i][j] += a * B[k][j];
            }
        }
    }
    return C;
}

// ===============================================================
// Función: matmul_blocked
// Multiplicación bloqueada (tiling).
// Divide la matriz en sub-bloques de tamaño BS para mejorar la
// localidad en caché y reutilizar datos.
//
// Parámetros:
//   A -> Matriz cuadrada n x n
//   B -> Matriz cuadrada n x n
//   BS -> Tamaño del bloque (por defecto 64)
//
// Devuelve:
//   C = A * B (matriz n x n).
//
// Características:
//   - Complejidad sigue siendo O(n^3).
//   - Más rápida para matrices grandes por mejor uso de caché.
// ===============================================================
vector<vector<double>> matmul_blocked(const vector<vector<double>>& A,
                                    const vector<vector<double>>& B,
                                    int BS = 64) {
    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n, 0.0));

    // Recorremos bloques
    for (int ii = 0; ii < n; ii += BS) {
        for (int kk = 0; kk < n; kk += BS) {
            for (int jj = 0; jj < n; jj += BS) {
                // Recorremos dentro del bloque
                for (int i = ii; i < min(ii + BS, n); i++) {
                    for (int k = kk; k < min(kk + BS, n); k++) {
                        double a = A[i][k];
                        for (int j = jj; j < min(jj + BS, n); j++) {
                            C[i][j] += a * B[k][j];
                        }
                    }
                }
            }
        }
    }
    return C;
}

// ===============================================================
// Función: matmul_blocked64
// Wrapper de la función bloqueada.
// Llama a matmul_blocked con BS = 64 sin necesidad de pasar el
// parámetro extra.
//
// Parámetros:
//   A -> Matriz cuadrada n x n
//   B -> Matriz cuadrada n x n
// Devuelve:
//   C = A * B (matriz n x n).
// ===============================================================
vector<vector<double>> matmul_blocked64(const vector<vector<double>>& A,
                                        const vector<vector<double>>& B) {
    return matmul_blocked(A, B, 64);
}

// ===============================================================
// Función: medir
// Mide el tiempo de ejecución de una función de multiplicación.
// Usa std::chrono::high_resolution_clock.
//
// Parámetros:
//   f -> función que multiplica matrices (ej: matmul_base)
//   A -> Matriz cuadrada n x n
//   B -> Matriz cuadrada n x n
//
// Devuelve:
//   Tiempo en segundos (double).
// ===============================================================
template<typename Func>
double medir(Func f, const vector<vector<double>>& A,
            const vector<vector<double>>& B) {
    auto start = high_resolution_clock::now(); // Tiempo inicial
    auto C = f(A, B);                          // Ejecutamos la función
    auto end = high_resolution_clock::now();   // Tiempo final
    return duration<double>(end - start).count(); // Diferencia en segundos
}

// ===============================================================
// Función principal: main
// - Inicializa semilla aleatoria (srand).
// - Define una lista de tamaños de matriz.
// - Genera matrices A y B aleatorias.
// - Mide tiempos de las tres versiones (base, ikj, bloqueada).
// - Imprime los resultados.
// ===============================================================
int main() {
    srand(time(0)); // Semilla aleatoria basada en el reloj del sistema

    vector<int> tamaños = {256, 512, 768, 1024, 2048};
    for (int n : tamaños) {
        auto A = generarMatriz(n); // Generar matriz A
        auto B = generarMatriz(n); // Generar matriz B

        // Medir tiempos
        double t1 = medir(matmul_base, A, B);
        double t2 = medir(matmul_ikj, A, B);
        double t3 = medir(matmul_blocked64, A, B);

        // Imprimir resultados
        cout << n << "x" << n << endl;
        cout << "Base:    " << t1 << " s" << endl;
        cout << "IKJ:     " << t2 << " s" << endl;
        cout << "Blocked: " << t3 << " s (BS=64)" << endl;
        cout << "-------------------------------" << endl;
    }
    return 0;
}
