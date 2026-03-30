/*
* @file mesh.c
* @brief Library of mesh manipulation and operations.
*
* Implements functions for mesh manipulation and operations declared in mesh.h.
*
* @author Joao Pedro Teixeira de Quadros and Priscila Cardoso Calegari
*/
#include "mesh.h"

HT innitialize_mesh(size_t l_max, size_t N, size_t M) {
	HT mesh;
	mesh.N = N;
	mesh.M = M;
	mesh.max_l = l_max;
	mesh.size = 0.1 * N * M * pow(2, 2 * mesh.max_l); // 10% of the total number of cells at the maximum level
	mesh.n_cells = 0;
	mesh.table = (Linked_list *)malloc(mesh.size * sizeof(Linked_list));
	for(int k = 0; k < mesh.size; k++){
		mesh.table[k].first = NULL;
		mesh.table[k].last = NULL;
		mesh.table[k].size = 0;
	}
	mesh.id_map = (unsigned long int *)malloc(mesh.size * sizeof(unsigned long int)); // or NxM
	return mesh;
}

void insert_cell(HT *mesh, Cell cell) {
	if (mesh->n_cells >= mesh->size) {
		printf("FATAL ERROR! Hash table is full. Cannot insert new cell. Look id_map size and hash table size.\n");
		return;
	}	
	unsigned long int key = (cell.i + cell.j * mesh->N * pow(2, mesh->max_l)) * pow(2, mesh->max_l - cell.l);
	unsigned long int p = (unsigned long int) (mesh->size * (0.61803398875 * key - floor(0.61803398875 * key)));
	insert_node(&mesh->table[p], cell, key);
	mesh->n_cells++;
	mesh->id_map[cell.id] = key;
}

Cell search_cell(HT *mesh, unsigned long int key) {
	unsigned long int p = (unsigned long int) (mesh->size * (0.61803398875 * key - floor(0.61803398875 * key)));
	Node *current = mesh->table[p].first;
	for(int k = 0; k < mesh->table[p].size; k++){
		if(current->key == key){
			return current->data;
		}
		current = current->next;
	}
	return (Cell){0, 0, 0, 0, 1}; // Return a null cell if not found
}

void remove_cell(HT *mesh, unsigned long int key) {
    unsigned long int p = (unsigned long int) (mesh->size * (0.61803398875 * key - floor(0.61803398875 * key)));
    remove_node(&mesh->table[p], key);
}

void export_mesh(HT *mesh, double Lx, double Ly, const char *filename) {
	FILE *file = fopen(filename, "w");
	if(file == NULL){
		printf("Error opening file for writing.\n");
		return;
	}
	for(int k = 0; k < mesh->n_cells; k ++) {
		unsigned long int key = mesh->id_map[k];

		Cell c = search_cell(mesh, key);
		if(c.is_null == 0){
			double x_step = Lx / (mesh->N * pow(2, c.l));
			double y_step = Ly / (mesh->M * pow(2, c.l));
			double x_min = c.j * x_step;
			double x_max = (c.j + 1) * x_step;
			double y_min = c.i * y_step;
			double y_max = (c.i + 1) * y_step;
			if (c.is_null == 0) {
				fprintf(file, "%f %f %f %f\n", x_min, x_max, y_min, y_max); 
			}
		}
	}
	fclose(file);
    printf("Exported!");
}

void refine_cell(HT *mesh, unsigned long int key) {
    Cell cell = search_cell(mesh, key);
    if(cell.is_null == 0 && cell.l < mesh->max_l){
        
        size_t parent_id = cell.id; 
        
        // 1. Calcula a posição do Pai na Hash Table
        unsigned long int p = (unsigned long int) (mesh->size * (0.61803398875 * key - floor(0.61803398875 * key)));
        
        // 2. Apaga SÓ O NÓ, ignorando completamente o id_map (Zero Swap!)
        remove_node(&mesh->table[p], key); 
        
        // 3. Decrementa temporariamente o n_cells (a 1ª filha vai repor isso)
        mesh->n_cells--;
        
        size_t new_level = cell.l + 1;
        size_t i = cell.i * 2;
        size_t j = cell.j * 2;
        
        int child_count = 0;
        
        for(int m = 0; m < 2; m++){
            for(int n = 0; n < 2; n++){
                // A primeira filha (0) pega o parent_id e tampa o buraco no id_map!
                size_t new_id = (child_count == 0) ? parent_id : mesh->n_cells;
                
                Cell new_cell = {i + m, j + n, new_level, new_id, 0};
                insert_cell(mesh, new_cell); 
                
                child_count++;
            }
        }
    }
}

void insert_node(Linked_list *list, Cell cell, unsigned long int key){
	Node *new_node = (Node *)malloc(sizeof(Node));
	new_node->data = cell;
	new_node->key = key;
	new_node->previous = list->last;
	new_node->next = NULL;

	if(list->last == NULL){
		list->first = new_node;
	} else {
		list->last->next = new_node;
	}
	list->last = new_node;
	list->size++;
}

Node* search_node(Linked_list *list, unsigned long int key){
	if(list->size == 0){
		return NULL;
	}
	else{
		Node *current = list->first;
		while(current != NULL){
			if(current->key == key){
				return current;
			}
			current = current->next;
		}
		return NULL;
	}
}

void remove_node(Linked_list *list, unsigned long int key){
	if(list->size == 0){
		return;
	}
	else{
		Node *current = list->first;
		while(current != NULL){
			if(current->key == key){
				if(current->previous != NULL) current->previous->next = current->next;
				if(current->next != NULL) current->next->previous = current->previous;
				if(current == list->first) list->first = current->next;
				if(current == list->last) list->last = current->previous;
				free(current);
				list->size--;
				return;
			}
			current = current->next;
		}
	}
}