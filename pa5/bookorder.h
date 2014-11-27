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
#include <semaphore.h>

struct Order_{
	char *title;
	double cost;
	int id;
	char *category;
	double balance;
	struct Order_ *prev;
	struct Order_ *next;
};
typedef struct Order_ Order;

struct Queue_{
	char *category;
	int flag;
	int count;
	pthread_mutex_t lock;
	sem_t mutex;
	Order *head;
	Order *tail;
};
typedef struct Queue_ Queue;

struct Customer_{
	char *name;
	int id;
	double credit;
	char *address;
	char *state;
	char *zipcode;
	struct Customer_ *next;
	Order *successful;
	Order *successful_tail;
	Order *rejected;
	Order *rejected_tail;
	pthread_mutex_t lock;
};
typedef struct Customer_ Customer;

struct Database_{
	pthread_mutex_t lock;
	double revenue;
	Customer *head;	
};
typedef struct Database_ Database;

struct Structures_{
	Database *database;
	Queue **category_q;
	FILE *orders;
	FILE *categories;
	int num_category;
};
typedef struct Structures_ Structures;

// create functions
Database *create_database();
Customer *create_customer();
Queue *create_queue();
Order *create_order();
Structures *create_structures(Database *db, Queue **category_q, FILE *orders, FILE *categories, int num_category);

// thread operations
void enqueue(Queue *q, Order *order);
Order *dequeue(Queue *q);

// I/O functions
void read_db_file(FILE *db_file, Database *database);
int count_categories(FILE *categories);
void add_to_category_q(Queue **category_q, Order *order, int  num_category);
void *produce(void *arg);
void fnc(FILE *orders, FILE *categories, Queue **category_q, int num_category);

// customer functions
Customer *find_customer(Customer *head, int id);
int check_credit(Customer *customer, double cost);
Customer *add_to_success(Customer *customer, Order *node);
Customer *add_to_reject(Customer *customer, Order *node);
void *consume(void *arg);
void print_orders(Order *head);
void final_report(Database *db);
void print_category_q(Queue **category_q, int num_category);

//destroy functions
void destroy_order(Order *head);
void destroy_queue(Queue *q);
void destroy_category_q(Queue **category_q, int num_categories);
void destroy_customer(Customer *head);
void destroy_database(Database *database);
#endif
