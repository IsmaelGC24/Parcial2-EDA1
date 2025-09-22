#include <iostream>   
#include <vector>     
using namespace std;


// Definición del nodo del árbol
struct Node {
    int key;       // Clave o valor almacenado en el nodo
    Node* left;    // Puntero al hijo izquierdo
    Node* right;   // Puntero al hijo derecho
    // Constructor: inicializa un nodo con valor k, y sin hijos
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};


// Clase que implementa el BST
class BST {
private:
    Node* root;  // Puntero a la raíz del árbol

    /*
    * Inserta una clave en el árbol
    * Recibe: puntero a un nodo y la clave
    * Devuelve: el nodo actualizado después de la inserción
    */
    Node* insert(Node* node, int key) {
        if (!node) return new Node(key); // Si el nodo está vacío, se crea uno nuevo
        if (key < node->key) node->left = insert(node->left, key);   // Inserta en subárbol izquierdo
        else if (key > node->key) node->right = insert(node->right, key); // Inserta en subárbol derecho
        else { 
            // Política de duplicados: insertar siempre a la derecha
            node->right = insert(node->right, key);
        }
        return node;
    }

    /*
     * Funciones para los recorridos del árbol (inorder, preorder y postorder)
     * Cada uno recibe un nodo y no devuelve nada, imprime las claves
     */
    void RecorridoInorder(Node* node) {
        if (!node) return;
        RecorridoInorder(node->left);
        cout << node->key << " --> ";
        RecorridoInorder(node->right);
    }

    void RecorridoPreorder(Node* node) {
        if (!node) return;
        cout << node->key << " --> ";
        RecorridoPreorder(node->left);
        RecorridoPreorder(node->right);
    }

    void RecorridoPostorder(Node* node) {
        if (!node) return;
        RecorridoPostorder(node->left);
        RecorridoPostorder(node->right);
        cout << node->key << " --> ";
    }

    /*
     * Funcion altura
     * Calcula la altura del árbol
     * Recibe: un nodo
     * Devuelve: número de niveles desde ese nodo hasta la hoja más lejana
     */
    int altura(Node* node) {
        if (!node) return -1; // Altura de árbol vacío es -1
        return 1 + max(altura(node->left), altura(node->right));
    }

    /*
     * Funcion gradoMax
     * Calcula el grado máximo del árbol
     * Recibe: un nodo
     * Devuelve: el máximo número de hijos que tiene algún nodo en el árbol
     */
    int gradoMax(Node* node) {
        if (!node) return 0;
        int grado = 0;
        if (node->left) grado++;
        if (node->right) grado++;
        return std::max(grado, std::max(gradoMax(node->left), gradoMax(node->right)));
    }

    /*
    * funcion find
    * Busca una clave en el árbol
    * Recibe: nodo, clave a buscar y vector para guardar el camino recorrido
    * Devuelve: true si existe la clave, false si no
    */
    bool find(Node* node, int key, vector<int>& camino) {
        if (!node) return false;
        camino.push_back(node->key); // Guarda cada nodo visitado
        if (key == node->key) return true;              // Si la clave está en el nodo actual
        if (key < node->key) return find(node->left, key, camino);   // Buscar en el subárbol izquierdo
        else return find(node->right, key, camino);                 // Buscar en el subárbol derecho
    }

public:
    // Constructor: inicializa el árbol vacío
    BST() : root(nullptr) {}

    // Inserta una clave en el árbol
    void insert(int key) { root = insert(root, key); }

    // Imprime recorridos
    void printInorder() { RecorridoInorder(root); cout << "NULL" << endl; }
    void printPreorder() { RecorridoPreorder(root); cout << "NULL" << endl; }
    void printPostorder() { RecorridoPostorder(root); cout << "NULL" << endl; }

    // Devuelve altura y grado máximo
    int getAltura() { return altura(root); }
    int getGradoMax() { return gradoMax(root); }

    // Busca una clave y muestra el camino recorrido
    void buscar(int key) {
        vector<int> camino;
        bool existe = find(root, key, camino);
        cout << "Buscando " << key << ": ";
        if (existe) cout << "Encontrado. Camino: ";
        else cout << "No encontrado. Camino recorrido: ";
        for (int c : camino) cout << c << " --> ";
        cout << "NULL" << endl;
    }
};

//main
int main() {
    BST tree;
    // Insertamos claves en el árbol
    vector<int> valores = {15, 10, 20, 8, 12, 17, 25, 23, 11, 28, 99, 12};
    for (int v : valores) tree.insert(v);

    // Mostramos los recorridos
    cout << "Recorrido Inorder: "; tree.printInorder();
    cout << "Recorrido Preorder: "; tree.printPreorder();
    cout << "Recorrido Postorder: "; tree.printPostorder();

    // Propiedades del árbol
    cout << "Altura: " << tree.getAltura() << endl;
    cout << "Grado maximo: " << tree.getGradoMax() << endl;

    // Búsquedas de prueba
    tree.buscar(12); // Existe
    tree.buscar(30); // No existe
    tree.buscar(99); // Existe

    return 0;
}
