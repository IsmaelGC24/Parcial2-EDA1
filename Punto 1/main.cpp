#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// Generar matriz aleatoria con semilla fija
vector<vector<double>> generarMatriz(int filas, int cols, unsigned int seed = 42) {
    vector<vector<double>> M(filas, vector<double>(cols));
    mt19937 gen(seed); // generador con semilla fija
    uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < cols; j++) {
            M[i][j] = dist(gen);
        }
    }
    return M;
}

// Versión clásica: triple ciclo i-j-k
vector<vector<double>> matmul_base(const vector<vector<double>>& A,
                                   const vector<vector<double>>& B) {
    int m = A.size();
    int p = A[0].size();
    int n = B[0].size();

    vector<vector<double>> C(m, vector<double>(n, 0.0));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < p; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

// Versión optimizada: orden i-k-j (mejor en row-major)
vector<vector<double>> matmul_ikj(const vector<vector<double>>& A,
                                  const vector<vector<double>>& B) {
    int m = A.size();
    int p = A[0].size();
    int n = B[0].size();

    vector<vector<double>> C(m, vector<double>(n, 0.0));

    for (int i = 0; i < m; i++) {
        for (int k = 0; k < p; k++) {
            double a_ik = A[i][k]; // guardar en registro
            for (int j = 0; j < n; j++) {
                C[i][j] += a_ik * B[k][j];
            }
        }
    }
    return C;
}

// Calcular norma infinito para comparar matrices
double errorMatriz(const vector<vector<double>>& X,
                const vector<vector<double>>& Y) {
    double maxError = 0.0;
    for (size_t i = 0; i < X.size(); i++) {
        for (size_t j = 0; j < X[0].size(); j++) {
            maxError = max(maxError, fabs(X[i][j] - Y[i][j]));
        }
    }
    return maxError;
}

// Medir tiempo de ejecución de una función de multiplicación
template <typename Func>
double medirTiempo(Func f, int n) {
    auto A = generarMatriz(n, n, 42);
    auto B = generarMatriz(n, n, 1337);

    // Calcular C para exactitud con versión base
    auto C_ref = matmul_base(A, B);

    vector<double> tiempos;
    for (int r = 0; r < 3; r++) {
        auto start = high_resolution_clock::now();
        auto C = f(A, B);
        auto end = high_resolution_clock::now();
        duration<double> elapsed = end - start;
        tiempos.push_back(elapsed.count());

        // Validar exactitud
        double err = errorMatriz(C_ref, C);
        if (err > 1e-9) {
            cerr << "Error alto en validación: " << err << endl;
        }
    }
    sort(tiempos.begin(), tiempos.end());
    return tiempos[1]; // mediana
}

int main() {
    vector<int> tamaños = {256, 512, 768, 1024};

    cout << fixed << setprecision(6);
    cout << "Comparacion de multiplicacion de matrices" << endl;
    cout << "Tamaño\tBase(s)\tOptimizada i-k-j(s)\tSpeedup" << endl;

    for (int n : tamaños) {
        double t_base = medirTiempo(matmul_base, n);
        double t_opt = medirTiempo(matmul_ikj, n);

        cout << n << "x" << n << "\t" 
            << t_base << "\t" 
            << t_opt << "\t\t" 
            << (t_base / t_opt) << "x" << endl;
    }

    return 0;
}