/* maximum size of hash table */
#define SIZE 211

/* maximum size of tokens-identifiers */
#define MAXTOKENLEN 40

/* how parameter is passed */
#define BY_VALUE 1
#define BY_REFER 2

/* Types of values that we can have */
typedef union Value{
	int ival;
	double fval;
	char cval;
	char *sval;
}Value;

/* a linked list of references (lineno's) for each variable */
typedef struct RefList{ 
    int lineno;
    struct RefList *next;
}RefList;

// struct that represents a list node
typedef struct list_t{
	// name, size of name, scope and occurrences (lines)
	char st_name[MAXTOKENLEN];
    int st_size;
    int scope;
    RefList *lines;
    int tem_no;
	// to store value
	Value val;
	
	// type
    int st_type;
    
    // for arrays (info type), for pointers (pointing type)
	// and for functions (return type)
	int inf_type;
	
	// pointer to next item in the list
	struct list_t *next;
}list_t;

/* Queue of identifiers to revisit */
typedef struct revisit_queue{
	// name of identifier
	char *st_name;
	
	// type of revisit
	int revisit_type;
	
	// maybe additional information to simplify the process ...
		
	struct revisit_queue *next;
}revisit_queue;

/* revisit types */
#define PARAM_CHECK 1 /* Check parameters of function call when functions gets declared */

/* static structures */
static list_t **hash_table;
static revisit_queue *queue;


// Symbol Table Functions
void init_hash_table(); // initialize hash table
unsigned int hash(char *key); // hash function 
void insert(char *name, int len, int type, int lineno,int *tem_no); // insert entry
list_t *lookup(char *name); // search for entry
void symtab_dump(FILE *of); // dump file

// Type Functions
void set_type(char *name, int st_type, int inf_type); // set the type of an entry (declaration)
int get_type(char *name); // get the type of an entry

// Scope Management Functions
void hide_scope(); // hide the current scope
void incr_scope(); // go to next scope

// Revisit Queue Functions
void add_to_queue(char *name, int type); // add to queue
int revisit(char *name); // revisit entry by also removing it from queue
void revisit_dump(FILE *of); // dump file
