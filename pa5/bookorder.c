/*
 * Alison Wong
 * Patrick Wu
 */

#include "bookorder.h"

Database *create_database(){
	Database *database = malloc(sizeof(Database));
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
	return customer;
}

Queue *create_queue(char *category){
	Queue *q = malloc(sizeof(Queue));
	q->category = category;
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
	}else{
		order->next = q->head;
		q->head->prev = order;
		q->head = order;
	}
}

Order *dequeue(Queue *q){
	Order *order = q->tail;
	if (order == NULL){
		return NULL;
	}else if(order->prev == NULL && order->next == NULL){
		q->tail = NULL;
		return order;
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
/*
  void *produce(void *arg){
	Structures *structure = (Structures *)arg;
	FILE *orders = structure->orders;
	FILE *categories = structure->categories;
	Queue **category_q = structure->category_q;
	int num_category = structure->num_category;
*/
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
//	return NULL;
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


void consume (Queue *q, Database *database){
	if (q == NULL){
		return;
	}

	Order *ptr;
	Customer *customer;
	int credit_check;
	while(q->head != NULL){
		ptr = dequeue(q);
		if(ptr == NULL){
			return;
		}
		customer = find_customer(database->head, ptr->id);
		credit_check = check_credit(customer, ptr->cost);
		if (credit_check == 1){
			//credit limit checks out
			printf("Order Confirmation: %s|%.2f\n", ptr->title, ptr->cost);
			printf("Shipping Information: %s|%s|%s|%d\n", customer->name, customer->address, customer->state, customer->zipcode);
			customer->credit = (customer->credit - ptr->cost);
			database->revenue = (database->revenue + ptr->cost);
			ptr->balance = customer->credit;
			customer = add_to_success(customer, ptr);
		}else{
			//credit limit is too low
			printf("REJECTED:\n");
			printf("Name: %s\n", customer->name);
			printf("Book Order Details: %s | %.2f \n", ptr->title, ptr->cost);
			printf("Remaining Credit Limit: %.2f \n", customer->credit);
			ptr->balance = customer->credit;
			customer = add_to_reject(customer, ptr);
		}
	}
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
		printf("=== BEGIN CUSTOMER INFO ===\n");
		printf("Customer Name: %s\n", ptr->name);
		printf("Customer ID number: %d\n", ptr->id);	
		printf("Remaining credit: [%.2f]\n", ptr->credit);
		printf("### SUCCESSFUL ORDERS ###\n");
		print_orders(ptr->successful);
		printf("### REJECTED ORDERS\n");
		print_orders(ptr->rejected);
		printf("=== END CUSTOMER INFO ===\n");
		printf("\n");
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

	int i;
	Database *database = create_database();
	read_db_file(db_file, database);

	//print_linked_list(database);
	int num_categories = count_categories(categories);
	Queue **category_q = malloc(num_categories * sizeof(Queue *));

	
	fnc(orders, categories, category_q, num_categories);
	print_category_q(category_q, num_categories);	
 	
	for(i = 0; i < num_categories; i++){	
		consume(category_q[i], database);
	}

	final_report(database);
	/*
	Structures *structure = create_structures(database, category_q, orders, categories, num_categories);

	pthread_t producer;
	pthread_create(&producer, NULL, produce, structure);
	print_category_q(category_q, num_categories);

	for (i = 0; i < num_categories; i++){
		pthread_t t;
		pthread_create(&t, NULL, consume, category_q[i]);
	}
*/
	fclose(db_file);
	fclose(orders);
	fclose(categories);
	return 0;
}
