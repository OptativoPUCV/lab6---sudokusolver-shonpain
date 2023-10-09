#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n) {
    int used[10] = {0};  // Arreglo para marcar los números usados (inicializado en 0)

    // Validar filas
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int num = n->sudo[i][j];
            if (num != 0) {
                if (used[num] == 1)
                    return 0;  // Número repetido en la fila
                used[num] = 1;
            }
        }
        // Reinicializar el arreglo para la siguiente fila
        for (int k = 0; k < 10; k++)
            used[k] = 0;
    }

    // Validar columnas
    for (int j = 0; j < 9; j++) {
        for (int i = 0; i < 9; i++) {
            int num = n->sudo[i][j];
            if (num != 0) {
                if (used[num] == 1)
                    return 0;  // Número repetido en la columna
                used[num] = 1;
            }
        }
        // Reinicializar el arreglo para la siguiente columna
        for (int k = 0; k < 10; k++)
            used[k] = 0;
    }

    // Validar submatrices 3x3
    for (int k = 0; k < 9; k++) {
        int start_row = 3 * (k / 3);
        int start_col = 3 * (k % 3);
        
        for (int p = 0; p < 9; p++) {
            int i = start_row + (p / 3);
            int j = start_col + (p % 3);
            int num = n->sudo[i][j];
            if (num != 0) {
                if (used[num] == 1)
                    return 0;  // Número repetido en la submatriz
                used[num] = 1;
            }
        }
        // Reinicializar el arreglo para la siguiente submatriz
        for (int m = 0; m < 10; m++)
            used[m] = 0;
    }

    return 1;  // El estado es válido
}




List* get_adj_nodes(Node* n){
        List* list = createList();

    // Verificar si la matriz ya está llena
    int estaLlena = 1;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (n->sudo[i][j] == 0) {
                estaLlena = 0;
                break;
            }
        }
        if (!estaLlena) break;
    }
    if (estaLlena) {
        return list;
    }

    for (int num = 1; num <= 9; num++) {
        Node* nuevoNodo = copy(n);
        nuevoNodo->sudo[0][2] = num;
        pushBack(list, nuevoNodo);
    }

    return list;
}


int is_final(Node* n){
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (n->sudo[i][j] == 0) {
                return 0;  // Si hay un valor 0, no es un estado final
            }
        }
    }
    return 1; 
}

Node* DFS(Node* initial, int* cont){
    if (is_final(initial)) {
        // Se alcanzó un estado final, hacer algo con el nodo final
        printf("Se alcanzó un estado final:\n");
        print_sudo(initial->sudo);  // Suponiendo que tienes una función para imprimir la matriz
        printf("\n");
        (*cont)++;  // Incrementar el contador de estados finales encontrados
        return initial;
    } else {
        // Generar nodos adyacentes y explorar recursivamente
        List* adj_nodes = get_adj_nodes(initial);
        Node* final_node = NULL;  // Para almacenar el nodo final encontrado

        for (int i = 0; i < adj_nodes->size; i++) {
            node* adj_node = adj_nodes->nodes[i]->data;
            Node* result = DFS(adj_node, cont);
            if (result != NULL) {
                final_node = result;  // Guardar el nodo final encontrado
                // Puedes detener la búsqueda aquí si solo quieres encontrar el primer nodo final
                // return final_node;
            }
        }
        // Liberar memoria de la lista de nodos adyacentes
        clean(adj_nodes);
        free(adj_nodes);

        return final_node;  // Devolver el nodo final encontrado (o NULL si no se encontró ninguno)
    }
}



/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/