/* ---------------------NODE TYPES-------------------------- */

typedef enum Node_Type {
	BASIC_NODE,  // no special usage (for roots only)
	// declarations
	DECL_NODE,   // declaration
	CONST_NODE,  // constant
	// statements
	STATEMENTS,  // statements
	WHILE_NODE,  // while statement
	ASSIGN_NODE, // assigment
	SIMPLE_NODE, // continue or break statement
	INCR_NODE,   // increment statement (non-expression one)
	// expressions
	ARITHM_NODE, // arithmetic expression
	BOOL_NODE,   // boolean expression
	REL_NODE,    // relational expression
	EQU_NODE,    // equality expression
	REF_NODE,	 // identifier in expression
}Node_Type;

/* --------------------OPERATOR TYPES----------------------- */

typedef enum Arithm_op{
	ADD,  // + operator
	SUB,  // - operator
	MUL,  // * operator
	DIV , // / operator
	INC, // ++ operator
	DEC, // -- operator
}Arithm_op;

typedef enum Bool_op{
	OR,  // || operator
	AND, // && operator
	NOT  // ! operator
}Bool_op;

typedef enum Rel_op{
	GREATER,        // > operator
	LESS,           // < operator
	GREATER_EQUAL,  // >= operator
	LESS_EQUAL     // <= operator
}Rel_op;

typedef enum Equ_op{
	EQUAL,    // == operator
	NOT_EQUAL // != operator
}Equ_op;

/* -----------------------AST NODES------------------------- */

/* The basic node */
typedef struct AST_Node{
	enum Node_Type type; // node type
	int tem_no;
	struct AST_Node *left;  // left child
	struct AST_Node *right; // right child
}AST_Node;

/* Declarations */
typedef struct AST_Node_Decl{
	enum Node_Type type; // node type
	
	// data type
	int data_type;
	
	// symbol table entries of the variables
	list_t **names;
	int names_count;
}AST_Node_Decl;

typedef struct AST_Node_Const{
	enum Node_Type type; // node type
	
	// data type
	int const_type;
	
	// constant value
	Value val;
}AST_Node_Const;

/* Statements */
typedef struct AST_Node_Statements{
	enum Node_Type type; // node type
	
	// statements
	struct AST_Node **statements;
	int statement_count;
}AST_Node_Statements;



typedef struct AST_Node_While{
	enum Node_Type type; // node type
	
	// condition
	struct AST_Node *condition;
	int loop_no;
	
	// branch
	struct AST_Node *while_branch;
}AST_Node_While;

typedef struct AST_Node_Assign{
	enum Node_Type type; // node type
	
	// symbol table entry
	list_t *entry;
	
	// reference or not
	int ref; // 0: not reference, 1: reference
	
	// assignment value
	struct AST_Node *assign_val;
}AST_Node_Assign;

typedef struct AST_Node_Simple{
	enum Node_Type type; // node type
	
	// continue: '0', break: '1'
	int statement_type;
}AST_Node_Simple;

typedef struct AST_Node_Incr{
	enum Node_Type type; // node type
	
	// identifier
	list_t *entry;
	
	// increment or decrement
	int incr_type; // 0: increment, 1: decrement
	
	// post- or prefix
	int pf_type; // 0: postfix, 1: prefix
}AST_Node_Incr;


/* Expressions */
typedef struct AST_Node_Arithm{
	enum Node_Type type; // node type
	
	// operator
	enum Arithm_op op;
	
	struct AST_Node *left;  // left child
	struct AST_Node *right; // right child
}AST_Node_Arithm;

typedef struct AST_Node_Bool{
	enum Node_Type type; // node type
	
	// operator
	enum Bool_op op;
	
	struct AST_Node *left;  // left child
	struct AST_Node *right; // right child
}AST_Node_Bool;

typedef struct AST_Node_Rel{
	enum Node_Type type; // node type
	
	// operator
	enum Rel_op op;
	
	struct AST_Node *left;  // left child
	struct AST_Node *right; // right child
}AST_Node_Rel;

typedef struct AST_Node_Equ{
	enum Node_Type type; // node type
	
	// operator
	enum Equ_op op;
	
	struct AST_Node *left;  // left child
	struct AST_Node *right; // right child
}AST_Node_Equ;

typedef struct AST_Node_Ref{
	enum Node_Type type; // node type
	
	// symbol table entry
	list_t *entry;
	
	// reference or not
	int ref; // 0: not reference, 1: reference
}AST_Node_Ref;

/* ------------------AST NODE MANAGEMENT-------------------- */

/* The basic node */
AST_Node *new_ast_node(Node_Type type, AST_Node *left, AST_Node *right); 	 // simple nodes

/* Declarations */
AST_Node *new_ast_decl_node(int data_type, list_t **names, int names_count); // declaration
AST_Node *new_ast_const_node(int const_type, Value val);					 // constant

/* Statements */
AST_Node *new_statements_node(AST_Node **statements, int statement_count, AST_Node *statement);
AST_Node *new_ast_while_node(AST_Node *condition, AST_Node *while_branch,int *loop_no);
AST_Node *new_ast_assign_node(list_t *entry, int ref, AST_Node *assign_val);
AST_Node *new_ast_simple_node(int statement_type);							 // continue or break
AST_Node *new_ast_incr_node(list_t *entry, int incr_type, int pf_type);      // increment decrement

/* Expressions */
AST_Node *new_ast_arithm_node(enum Arithm_op op, AST_Node *left, AST_Node *right);
AST_Node *new_ast_bool_node(enum Bool_op op, AST_Node *left, AST_Node *right);
AST_Node *new_ast_rel_node(enum Rel_op op, AST_Node *left, AST_Node *right);
AST_Node *new_ast_equ_node(enum Equ_op op, AST_Node *left, AST_Node *right);
AST_Node *new_ast_ref_node(list_t *entry, int ref);

/* Tree Traversal */
void ast_print_node(AST_Node *node);	// print information of node
void ast_traversal(AST_Node *node);		// tree traversal (for testing right now)

/***3AC code****/
void IC3_var_init_code(list_t *entry,Value val);
void IC3_const_exp_code(int tem_no,Value val);
void IC3_arith_code(enum Arithm_op op,int arg1,int arg2,int result);
void IC3_var_ref_code(char *name,int right_tem);
void IC3_id_code(char *name,int tem_no);
void IC3_incr_code(char *name,int type,int tem_no);
void IC3_condition_code(int op,int arg1,int arg2);
void IC3_Rel_code(int op,int arg1,int arg2);