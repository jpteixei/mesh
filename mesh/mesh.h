/*
* @file mesh.h
* @brief Header file for Mesh library.
*
* Declares data structures and function prototypes for mesh manipulation and operations.
* This library is based on Claudia Patricia Ordónez Rodríguez work.
*
* @author Joao Pedro Teixeira de Quadros and Priscila Cardoso Calegari
*/

#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include "mmio.h"
#include "linalg.h"

#define PI 3.14159265358979323846

typedef struct{
	size_t i; // row index of the cell in the mesh
	size_t j; // column index of the cell in the mesh
	size_t l; // level of the cell in the mesh
	size_t id; // unique identifier for the cell
	size_t is_null; // flag to indicate if the cell is null (not used)
}Cell;

typedef struct Node{
	struct Node *previous; // pointer to the previous node in the list
	struct Node *next; // pointer to the next node in the list
	Cell data; // data of the cell stored in the node
	unsigned long int key; // key for hashing the cell
}Node;

typedef struct{
	Node *first; // pointer to the first node in the list
	Node *last; // pointer to the last node in the list
	size_t size; // number of nodes in the list
}Linked_list;

typedef struct{
	Linked_list *table; // hash table implemented as an array of linked lists
	size_t size; // current size of the hash table
	size_t max_l; // maximum level of the cells in the mesh
	size_t n_cells; // number of cells currently in the mesh
	size_t N; // number of rows in the original mesh
	size_t M; // number of columns in the original mesh
	unsigned long int *id_map; // array to map cell ids to their corresponding keys in the hash table
}HT;

HT innitialize_mesh(size_t l_max, size_t N, size_t M);
void insert_cell(HT *mesh, Cell cell);
Cell search_cell(HT *mesh, unsigned long int key);
void remove_cell(HT *mesh, unsigned long int key);
void export_mesh(HT *mesh, double Lx, double Ly, const char *filename);
void refine_cell(HT *mesh, unsigned long int key);

//Linked_list functions
void insert_node(Linked_list *list, Cell cell, unsigned long int key); // insert a new node with the given cell data and key at the end of the list
Node* search_node(Linked_list *list, unsigned long int key); // return a pointer to the node if found, otherwise return NULL
void remove_node(Linked_list *list, unsigned long int key); // remove the node with the given key from the list and free its memory
#endif // MESH_H_INCLUDED