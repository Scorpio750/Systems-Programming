#ifndef BOOKORDER_H
#define BOOKORDER_H

/*
 * Alison Wong
 * Patrick Wu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct Order{
	char *title;
	double cost;
	int id;
	char *category;
	struct Order *prev;
	struct Order *next;
};
typedef struct Order Order;

struct Queue{
	char *category;
	Order *head;
	Order *tail;
};
typedef struct Queue Queue;

struct Customer{
	char *name;
	int id;
	double credit;
	char *address;
	char *state;
	char *zipcode;
	struct Customer *next;
};
typedef struct Customer Customer;

struct Database{
	double revenue;
	Customer *head;	
};
typedef struct Database Database;

Database *create_database();
Customer *create_customer();
Queue *create_queue(char *category);
Order *create_order();
void enqueue(Queue *q, Order *order);
Order *dequeue(Queue *q);
void read_db_file(FILE *db_file, Database *database);
int count_categories(FILE *categories);
void add_to_category_q(Queue **category_q, Order *order, int  num_category);
void fnc(FILE *orders, FILE *categories, Queue **category_q, int num_category);
void print_linked_list(Database *db);
void print_category_q(Queue **category_q, int num_category);
#endif
