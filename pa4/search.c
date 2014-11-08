/*
 * Alison Wong
 * Patrick Wu
 */

Root *createRoot(){
	Root *tree = malloc(sizeof(Root));
	head->root = NULL;
	return tree;
}

Tnode *createNode(char c){
	TNode *node = (TNode *)malloc(sizeof(TNode));
	node->c = c;
	node->children = (TNode**)calloc(36, sizeof(TNode));
	node->isWord = false;
	return node;
}

int hash(char c) {
	char * index = strchr(acceptable, c);
	if (index == NULL) {
		return -1;
	}
	return (int)(index - acceptable);
}

void addToTree(FILE *file, TNode *root){
	TNode *ptr = root;
	c = fgetc(file);
	int index;

	while (c != '\n'){
		index = hash(c);
		if (ptr->next[index] == NULL){
			ptr->next[index] = createNode(c);
		}
		ptr = ptr->next[index];
		c = fgetc(file);
	}
	ptr->isWord = true;
	//put the files in now
}

void readIndex(FILE *file, Root tree){
	int c = fgetc(file);
	if (c == EOF){
		fprintf(stderr, "Empty Inverted Index\n");
		fclose(file);
		return;
	}

	if (tree->root == NULL){
		tree->root = createNode(' ');
	}

	while (c != ' '){
		c = fgetc(file);
	}
	addToTree(file, tree->root);
	fclose(file);
}

 int main (int arc, char **argv){
 	if (argc != 2){
 		fprintf(stderr, "Invalid number of arguments.\n");
 		return 1;
 	}

 	FILE *index = fopen(argv[1], "r");
 	if (index == NULL){
 		fprintf(stderr, "File does not exist.\n");
 		return 1;
 	}

 	Root *tree = createRoot();

 	readIndex(index,tree);
 	free(tree);
 }