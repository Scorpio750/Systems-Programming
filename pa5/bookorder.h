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
	double balance;
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
	Order *successful;
	Order *successful_tail;
	Order *rejected;
	Order *rejected_tail;
};
typedef struct Customer Customer;

struct Database{
	double revenue;
	Customer *head;	
};
typedef struct Database Database;

struct Structures{
	Database *database;
	Queue **category_q;
	FILE *orders;
	FILE *categories;
	int num_category;
};
typedef struct Structures Structures;

Database *create_database();
Customer *create_customer();
Queue *create_queue(char *category);
Order *create_order();
Structures *create_structures(Database *db, Queue **category_q, FILE *orders, FILE *categories, int num_category);
void enqueue(Queue *q, Order *order);
Order *dequeue(Queue *q);
void read_db_file(FILE *db_file, Database *database);
int count_categories(FILE *categories);
void add_to_category_q(Queue **category_q, Order *order, int  num_category);
void fnc(FILE *orders, FILE *categories, Queue **category_q, int num_category);
//void *produce(void *arg);
Customer *find_customer(Customer *head, int id);
int check_credit(Customer *customer, double cost);
Customer *add_to_success(Customer *customer, Order *node);
Customer *add_to_reject(Customer *customer, Order *node);
void consume(Queue *q, Database *database);
void print_orders(Order *head);
void final_report(Database *db);
void print_category_q(Queue **category_q, int num_category);
#endif
