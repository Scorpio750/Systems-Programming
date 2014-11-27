/*
 * Alison Wong
 * Patrick Wu
 */

#include "bookorder.h"

Database *database;
sem_t mutex;

Database *create_database(){
	Database *database = malloc(sizeof(Database));
	pthread_mutex_init(&(database->lock), NULL);
	database->revenue = 0.0;
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
	customer->successful = NULL;
	customer->rejected = NULL;
	pthread_mutex_init(&(customer->lock), NULL);
	return customer;
}

Queue *create_queue(){
	Queue *q = malloc(sizeof(Queue));
	q->category = NULL;
	q->flag = 0;
	q->count = 0;
	pthread_mutex_init(&(q->lock), NULL);
	sem_init(&(q->mutex), 0, 1);
	q->head = NULL;
	q->tail = NULL;
	return q;
}

Order *create_order(){
	Order *order = malloc(sizeof(Order));
	order->title = NULL;
	order->id = 0;
	order->category = NULL;
	order->balance = 0.0;
	order->prev = NULL;
	order->next = NULL;
	return order;
}

Structures *create_structures(Database *db, Queue **category_q, FILE *orders, FILE *categories, int num_category){
	Structures *structure = malloc(sizeof(Structures));
	structure->database = db;
	structure->category_q = category_q;
	structure->orders = orders;
	structure->categories = categories;
	structure->num_category = num_category;
	return structure;
}

void enqueue(Queue *q, Order *order){
	
	if (q->head == NULL){
		q->head = order;
		q->tail = order;
		q->count++;
		sem_post(&(q->mutex));
	}else{
		order->next = q->head;
		q->head->prev = order;
		q->head = order;
		q->count++;
		sem_post(&(q->mutex));
	}
}

Order *dequeue(Queue *q){
	Order *order = q->tail;
	if (order == NULL){
		return NULL;
	}else if(order->prev == NULL && order->next == NULL){
		q->tail = NULL;
		q->head = NULL;
		q->count--;
		return order;
	}else{
		order->prev->next = NULL;
		q->tail = order->prev;
		q->count--;
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
	while (num != -1){
		customer = create_customer();

		token = strtok(lineptr, "|");
		customer->name = (char *)calloc((strlen(token)+1),sizeof(char));
		strcpy(customer->name, token);

		token = strtok(NULL, "|");
		customer->id = atoi(token);

		token = strtok(NULL, "|");
		customer->credit = atof(token);

		token = strtok(NULL, "|");
		customer->address = (char *)calloc((strlen(token)+1),sizeof(char));
		strcpy(customer->address, token);

		token = strtok(NULL, "|");
		customer->state = (char *)calloc((strlen(token)+1),sizeof(char));
		strcpy(customer->state, token);

		token = strtok(NULL, "|");
		token = strtok(token, "\n");
		customer->zipcode = (char *)calloc((strlen(token)+1),sizeof(char));
		strcpy(customer->zipcode, token);

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
	}
}

int count_categories(FILE *categories){
	char *lineptr = NULL;
	size_t len = 0;
	ssize_t num = getline(&lineptr, &len, categories);
	int count = 0;
	while (num != -1){
		count++;
		if (lineptr){
			free(lineptr);
			lineptr = NULL;
		}
		num = getline(&lineptr, &len, categories);
	}
	rewind(categories);
	return count;
}

void add_to_category_q(Queue **category_q, Order *order, int num_category){
	Queue *q;
	int i;
	for(i = 0; i < num_category; i++){
		pthread_mutex_lock(&(q->lock)); //lock q
		if (category_q[i]->category == NULL){
			category_q[i]->category = order->category;
			q = category_q[i];
			enqueue(q, order);
			category_q[i] = q;
			pthread_mutex_unlock(&(q->lock)); //unlock q
			return;
		}
		if (strcmp(category_q[i]->category, order->category) == 0){
			q = category_q[i];
			enqueue(q, order);
			category_q[i] = q;
			pthread_mutex_unlock(&(q->lock)); //unlock q
			return;
		}
	}
}

void *produce(void *arg){
	Structures *structure = (Structures *)arg;
	FILE *orders = structure->orders;
	FILE *categories = structure->categories;
	Queue **category_q = structure->category_q;
	int num_category = structure->num_category;

	int i;
	char *token = calloc(500, sizeof(char));
	char *lineptr = NULL;
	size_t len = 0;
	ssize_t num = getline(&lineptr, &len, orders);
	while (num != -1){
		Order *order = create_order();

		token = strtok(lineptr, "|");
		order->title = (char *)calloc((strlen(token)+1),sizeof(char));	
		strcpy(order->title, token);
		
		token = strtok(NULL, "|");
		order->cost = atof(token);

		token = strtok(NULL, "|");
		order->id = atoi(token);

		token = strtok(NULL, "|");
		token = strtok(token, "\n");
		order->category = (char *)calloc((strlen(token)+1),sizeof(char));
		strcpy(order->category, token);
		
		add_to_category_q(category_q, order, num_category);

		if (lineptr){
			free(lineptr);
			lineptr = NULL;
		}
		num = getline(&lineptr, &len, orders);
	}

	for (i = 0; i < num_category; i++){
		//sets all the flags to 1 = done
		pthread_mutex_lock(&(category_q[i]->lock)); //lock the q in category_q[i]
		category_q[i]->flag = 1;	
		pthread_mutex_unlock(&(category_q[i]->lock)); //unlock the q in category_q[i]
	}
	return NULL;
}

Customer *find_customer(Customer *head, int id){
	Customer *ptr;
	if (head == NULL){
		return NULL;
	}else{
		for(ptr = head; ptr != NULL; ptr = ptr->next){
			if (ptr->id == id){
				return ptr;
			}
		}
	}
	return NULL; 
}

int check_credit(Customer *customer, double cost){
	if (customer->credit < cost){
		//credit limit is too low
		return 0;
	}else{
		//credit limit checks out
		return 1;
	}
}

Customer *add_to_success(Customer *customer, Order *node){
	Order *ptr = create_order();
	ptr->title = node->title;
	ptr->cost = node->cost;
	ptr->id = node->id;
	ptr->category = node->category;
	ptr->balance = node->balance;
	if (customer->successful == NULL){
		customer->successful = ptr;
		customer->successful_tail = ptr;
		return customer;
	}else{
		customer->successful_tail->next = ptr;
		customer->successful_tail = ptr;
		return customer;
	}
}

Customer *add_to_reject(Customer *customer, Order *node){
	Order *ptr = create_order();
	ptr->title = node->title;
	ptr->cost = node->cost;
	ptr->id = node->id;
	ptr->category = node->category;
	ptr->balance = node->balance;
	if (customer->rejected == NULL){
		customer->rejected = ptr;
		customer->rejected_tail = ptr;
		return customer;
	}else{
		customer->rejected_tail->next = ptr;
		customer->rejected_tail = ptr;
		return customer;
	}
}

void *consume(void *arg){
	Queue *q = (Queue *)arg;	
	Order *ptr;
	Customer *customer;
	int credit_check;
	int value;

	sem_getvalue(&(q->mutex), &value);

	if (value == 1){
		printf("Consumer waits because queue is empty.\n");
	}
	while(value == 1){
		if (q->flag == 1) {
			sem_getvalue(&(q->mutex), &value);
			if (value == 1){
				sem_post(&mutex);
				return NULL;
			}else if (value != 1){
				printf("Consumer resumes because there are orders ready for processing.\n");
				break;
			}
		}	
		sem_getvalue(&(q->mutex), &value);
		if (value != 1){
			printf("Consumer resumes because there are orders ready for processing.\n");
		}
	}

while (value != 1){	
		pthread_mutex_lock(&(q->lock)); //lock the q
		ptr = dequeue(q);
		sem_wait(&(q->mutex));
		pthread_mutex_unlock(&(q->lock)); //unlock the q
		
		customer = find_customer(database->head, ptr->id);
		pthread_mutex_lock(&(customer->lock)); //lock the customer
		credit_check = check_credit(customer, ptr->cost);
		if (credit_check == 1){
			//credit limit checks out
			printf("Order Confirmation: %s|%.2f\n", ptr->title, ptr->cost);
			printf("Shipping Information: %s|%s|%s|%d\n", customer->name, customer->address, customer->state, customer->zipcode);
			customer->credit = (customer->credit - ptr->cost);
			pthread_mutex_lock(&(database->lock)); //lock database
			database->revenue = (database->revenue + ptr->cost);
			pthread_mutex_unlock(&(database->lock)); //unlock database
			ptr->balance = customer->credit;
			customer = add_to_success(customer, ptr);
			pthread_mutex_unlock(&(customer->lock)); //unlock the customer
		}else{
			//credit limit is too low
			printf("REJECTED:\n");
			printf("Name: %s\n", customer->name);
			printf("Book Order Details: %s | %.2f \n", ptr->title, ptr->cost);
			printf("Remaining Credit Limit: %.2f \n", customer->credit);
			ptr->balance = customer->credit;
			customer = add_to_reject(customer, ptr);
			pthread_mutex_unlock(&(customer->lock)); //unlock the customer
		}
		sem_getvalue(&(q->mutex), &value);

		if (value == 1){
			printf("Consumer waits because queue is empty.\n");
		}

		while(value == 1){
			if (q->flag == 1) {
				sem_getvalue(&(q->mutex), &value);
			if (value == 1){
				sem_post(&mutex);
				return NULL;
			}else if (value != 1){
				printf("Consumer resumes because there are orders ready for processing.\n");
				break;
			}
		}	
		sem_getvalue(&(q->mutex), &value);
		if (value != 1){
			printf("Consumer resumes because there are orders ready for processing.\n");
		}
	}
}
	return NULL;
}

void print_orders(Order *head){
	Order *ptr;
	if (head == NULL){
		return;
	}else{
		for(ptr = head; ptr != NULL; ptr = ptr->next){
			printf("%s|%.2f|%.2f\n", ptr->title, ptr->cost, ptr->balance);
		}
	}
}

void final_report(Database *db){
	Customer *ptr;
	if (db == NULL){
		printf("The database is NULL.\n");
	}
	if (db->head == NULL){
		printf("No linked list exists.\n");
	}

	for(ptr = db->head; ptr != NULL; ptr= ptr->next){
		printf("\n");
		printf("=== BEGIN CUSTOMER INFO ===\n");
		printf("Customer Name: %s\n", ptr->name);
		printf("Customer ID number: %d\n", ptr->id);	
		printf("Remaining credit: [%.2f]\n", ptr->credit);
		printf("### SUCCESSFUL ORDERS ###\n");
		print_orders(ptr->successful);
		printf("### REJECTED ORDERS\n");
		print_orders(ptr->rejected);
		printf("=== END CUSTOMER INFO ===\n");
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

/*
// destroy functions
void destroyOrder(Order *order) {
	if (order == NULL) {
		return;
	}
	destroyOrder(order->next);
	free(order->title);
	free(order->category);
	free(order);
}

// not sure how to walk through and free this
void destroyQueue(Queue *q) {
	if (q == NULL) {
		return;
	}
	free(q->category);
	destroyOrder(q->head);
	destroyOrder(q->tail);
}

void destroyCustomer(Customer *cust) {
	if (cust == NULL) {
		return;
	}
	destroyCustomer(cust->next);
	free(cust->name);
	free(cust->address);
	free(cust->state);
	free(cust->zipcode);
	destroyOrder(cust->successful);
	destroyOrder(cust->successful_tail);
	destroyOrder(cust->rejected);
	destroyOrder(cust->rejected_tail);
}

void destroyDatabase(Database *db) {
	if (db->head == NULL) return;
	destroyCustomer(db->head);
	free(db);
}

void destroyStructure(Structures *structure) {
	int i;
	destroyDatabase(structure->database);
	for (i = 0; i < structure->num_category; i++) {
		destroyQueue(structure->category_q[i]);
	}
	free(structure->orders);
	free(structure->categories);
	free(structure);
}
*/




void destroy_order(Order *head){
	if (head == NULL){
		return;
	}
	destroy_order(head->next);
	if (head->prev != NULL){
		head->prev = NULL;
	}
	if (head->title != NULL){
		free(head->title);
		head->title = NULL;
	}
	if (head->category != NULL){
		free(head->category);
		head->category = NULL;
	}
	head->cost = 0;
	head->id = 0;
	head->balance = 0;
	free(head);
	head = NULL;
	return;
}

void destroy_queue(Queue *q){
	if (q->category != NULL){
		free(q->category);
		q->category = NULL;
	}	
	q->flag = 0;
	q->count = 0;
	pthread_mutex_destroy(&(q->lock));
	sem_destroy(&(q->mutex));
	destroy_order(q->head);
	if (q->tail != NULL){
		q->tail = NULL;
	}
}

void destroy_category_q(Queue **category_q, int num_categories){
	int i;
	for (i = 0; i < num_categories; i++){
		destroy_queue(category_q[i]);
	}
	free(category_q);
	category_q = NULL;
}

void destroy_customer(Customer *head){
	if (head == NULL){
		return;
	}
	destroy_customer(head->next);
	if (head->name != NULL){
		free(head->name);
		head->name = NULL;
	}
	if (head->address != NULL){
		free(head->address);
		head->address = NULL;
	}
	if (head->state != NULL){
		free(head->state);
		head->state = NULL;
	}
	if (head->zipcode != NULL){
		free(head->zipcode);
		head->zipcode = NULL;
	}
	head->id = 0;
	head->credit = 0;
	if (head->successful != NULL){
		destroy_order(head->successful);
	}
	head->successful_tail = NULL;
	if (head->rejected != NULL){
		destroy_order(head->rejected);
	}
	head->successful_tail = NULL;
	pthread_mutex_destroy(&(database->lock));
	free(head);
	head = NULL;
	return;
}

void destroy_database(Database *database){
	destroy_customer(database->head);
	pthread_mutex_destroy(&(database->lock));
	free(database);
	database = NULL;
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

	int i;
	database = create_database();
	read_db_file(db_file, database);

	int num_categories = count_categories(categories);
	Queue **category_q = malloc(num_categories * sizeof(Queue *));
	Queue *q;
	Structures *structure = create_structures(database, category_q, orders, categories, num_categories);
	sem_init(&mutex, 0, 1);
	
	
	for (i = 0; i < num_categories; i++){
		category_q[i] = create_queue();
	}

	pthread_t producer;
	pthread_create(&producer, NULL, produce, structure);

	for (i = 0; i < num_categories; i++){
		pthread_t consumer;
		q = category_q[i];
		pthread_create(&consumer, NULL, consume, q);
	}

	pthread_join(producer, NULL);

	for (i = 0; i < num_categories+1; i++){
		sem_wait(&mutex);	//semwait
	}

	sem_destroy(&mutex);
	final_report(database);	

	destroy_category_q(category_q, num_categories);
	destroy_database(database);

	fclose(db_file);
	fclose(orders);
	fclose(categories);

	structure->num_category = 0;
	free(structure);
	structure = NULL;
	return 0;
}
