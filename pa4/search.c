/*
 * Alison Wong
 * Patrick Wu
 */

Tnode *createNode(char c){
	TNode *node = (TNode *)malloc(sizeof(TNode));
	node->c = c;
	node->children = (TNode**)calloc(36, sizeof(TNode));
	node->isWord = false;
	return node;
}

TNode *createTree(FILE *file){
	
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
 }