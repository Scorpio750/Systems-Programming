/*
 * Alison Wong
 * Patrick Wu
 */

#include "bookorder.h"

Database *create_database(){
	Database *database = malloc(sizeof(Database));
	database->revenue = 0;
	database->head = NULL;
	return database;
}

Customer *create_customer(){
	Customer *customer = malloc(sizeof(Customer));
	customer->name = NULL;
	customer->id = 0;
	customer->credit = 0.0;
	customer->address = NULL;
	customer->state = NULL;
	customer->zipcode = NULL;
	customer->next = NULL;
	return customer;
}

Queue *create_queue(char *category){
	Queue *q = malloc(sizeof(Queue));
	q->category = category;
	q->head = NULL;
	q->tail = NULL;
}

Order *create_order(){
	Order *order = malloc(sizeof(Order));
	order->title = NULL;
	order->id = 0;
	order->category = NULL;
	order->prev = NULL;
	order->next = NULL;
}

void enqueue(Queue *q, Order *order){
	if (q->head == NULL){
		q->head = order;
		q->tail = order;
	}else{
		order->next = q->head;
		q->head = order;
	}
}

Order *dequeue(Queue *q){
	Order *order = q->tail;
	if (order == NULL){
		return NULL;
	}else{
		order->prev->next = NULL;
		q->tail = order->prev;
		return order;
	}
}

void read_db_file(FILE *db_file, Database *database){
	Customer *ptr = database->head;
	Customer *customer = NULL;
	char *token = calloc(500, sizeof(char));
	char *lineptr = NULL;
	size_t len = 0;
	ssize_t num = getline(&lineptr, &len, db_file);
	printf("num : [%d]\n", num);
	while (num != -1){
		customer = create_customer();

		token = strtok(lineptr, "|");
		customer->name = (char *)calloc((strlen(token)+1),sizeof(char));
		strcpy(customer->name, token);
		printf("Name: [%s]\n", customer->name);

		token = strtok(NULL, "|");
		customer->id = atoi(token);
		printf("ID: [%d]\n", customer->id);

		token = strtok(NULL, "|");
		customer->credit = atof(token);
		printf("Credit: [%.2f]\n", customer->credit);

		token = strtok(NULL, "|");
		customer->address = (char *)calloc((strlen(token)+1),sizeof(char));
		strcpy(customer->address, token);
		printf("Address: [%s]\n", customer->address);

		token = strtok(NULL, "|");
		customer->state = (char *)calloc((strlen(token)+1),sizeof(char));
		strcpy(customer->state, token);
		printf("State: [%s]\n", customer->state);

		token = strtok(NULL, "|");
		token = strtok(token, "\n");
		customer->zipcode = (char *)calloc((strlen(token)+1),sizeof(char));
		strcpy(customer->zipcode, token);
		printf("Zipcode: [%s]\n", customer->zipcode);

		if (ptr == NULL){
			database->head = customer;
			ptr = database->head;
		}else{
			ptr->next = customer;
			ptr = ptr->next;
		}
		if (lineptr){
			free(lineptr);
			lineptr = NULL;
		}
		num = getline(&lineptr, &len, db_file);
		printf("num : [%d]\n", num);
	}
}

int count_categories(FILE *categories){
	char *lineptr = NULL;
	size_t len = 0;
	ssize_t num = getline(&lineptr, &len, categories);
	printf("num :[%d]\n", num);
	int count = 0;
	while (num != -1){
		count++;
		if (lineptr){
			free(lineptr);
			lineptr = NULL;
		}
		num = getline(&lineptr, &len, categories);
		printf("num :[%d]\n", num);
	}
	rewind(categories);
	printf("Count: [%d]\n", count);
	return count;
}

void add_to_category_q(Queue **category_q, Order *order, int num_category){
	Queue *q;
	int i;
	for(i = 0; i < num_category; i++){
		if (category_q[i] == NULL){
			fprintf(stderr, "Category_q[%d] is NULL\n", i);
			q = create_queue(order->category);
			fprintf(stderr, "Category: [%s]\n");
			enqueue(q, order);
			category_q[i] = q;
			fprintf(stderr, "Category: [%s]\n", q->category);
			return;
		}
		fprintf(stderr, "queue: %s\n", category_q[i]->category);
		fprintf(stderr, "order: %s\n", order->category);
		if (strcmp(category_q[i]->category, order->category) == 0){
			fprintf(stderr, "Category_q[%d] has found a match!\n", i);
			q = category_q[i];
			enqueue(q, order);
			return;
		}
		fprintf(stderr, "Category_q[%d] did not find a match\n", i);
	}
}

void fnc(FILE *orders, FILE *categories, Queue **category_q, int num_category){
	char *token = calloc(500, sizeof(char));
	char *lineptr = NULL;
	size_t len = 0;
	ssize_t num = getline(&lineptr, &len, orders);
	printf("num :[%d]\n", num);
	while (num != -1){
		Order *order = create_order();

		token = strtok(lineptr, "|");
		order->title = (char *)calloc((strlen(token)+1),sizeof(char));	
		strcpy(order->title, token);
		printf("Title :[%s]\n", order->title);
		
		token = strtok(NULL, "|");
		order->cost = atof(token);
		printf("Cost :[%.2f]\n", order->cost);

		token = strtok(NULL, "|");
		order->id = atoi(token);
		printf("ID :[%d]\n", order->id);

		token = strtok(NULL, "|");
		token = strtok(token, "\n");
		order->category = (char *)calloc((strlen(token)+1),sizeof(char));
		strcpy(order->category, token);
		printf("Category :[%s]\n", order->category);

		add_to_category_q(category_q, order, num_category);

		if (lineptr){
			free(lineptr);
			lineptr = NULL;
		}
		num = getline(&lineptr, &len, orders);
		printf("num :[%d]\n", num);
	}
}

void print_linked_list(Database *db){
	Customer *ptr;
	if (db == NULL){
		printf("The database is NULL.\n");
	}
	if (db->head == NULL){
		printf("No linked list exists.\n");
	}

	for(ptr = db->head; ptr != NULL; ptr= ptr->next){
		printf(">> Name [%s]\n", ptr->name);
		printf("   ID [%d]\n", ptr->id);	
		printf("   Credit: [%.2f]\n", ptr->credit);
		printf("   Address: [%s]\n", ptr->address);
		printf("   State: [%s]\n", ptr->state);
		printf("   Zipcode: [%s]\n", ptr->zipcode);
	}
}

void print_category_q(Queue **category_q, int num_category){
	int i;
	Order *ptr;
	if(category_q == NULL){
		printf("Category_q is NULL\n");
	}
	for(i = 0; i < num_category; i++){
		if (category_q[i] == NULL){
			printf("category_q[%d] is NULL\n", i);
		}
		for(ptr = category_q[i]->head; ptr != NULL; ptr = ptr->next){
			printf(">> Title [%s]\n", ptr->title);
			printf("   Cost [%.2f]\n", ptr->cost);
			printf("   ID [%d]\n", ptr->id);
			printf("   Category [%s]\n", ptr->category);
		}
	}
}

int main (int argc, char **argv) {

	if (argc != 4) {
		fprintf(stderr, "Invalid number of arguments.\n");
		return 1;
	}

	FILE *db_file = fopen(argv[1], "r");
	if (db_file == NULL){
		fprintf(stderr, "File does not exist.\n");
		return 1;
	}

	FILE *orders = fopen(argv[2], "r");
	if (orders == NULL){
		fprintf(stderr, "File does not exist.\n");
		fclose(db_file);
		return 1;
	}

	FILE *categories = fopen(argv[3], "r");
	if (categories == NULL){
		fprintf(stderr, "File does not exist.\n");
		fclose(db_file);
		fclose(orders);
		return 1;
	}

	Database *database = create_database();
	read_db_file(db_file, database);

	//print_linked_list(database);
	int num_categories = count_categories(categories);
	Queue **category_q = malloc(num_categories * sizeof(Queue *));
	
	fnc(orders, categories, category_q, num_categories);
	print_category_q(category_q, num_categories);

	pthread_t producer;
//	pthread_create(&producer, NULL, producerfnc, "Producer Thread");

	fclose(db_file);
	fclose(orders);
	fclose(categories);
	return 0;
}
