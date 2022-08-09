%{
	#include "semantics.c"
	#include "symtab.c"
	#include "ast.h"
	#include "ast.c"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	extern FILE *yyin;
	extern FILE *yyout;
	extern int lineno;
	extern int tem_no;
	extern int loop_no;
	extern int yylex();
	void yyerror();
	
	// for declarations
	void add_to_names(list_t *entry);
	list_t **names;
	int nc = 0;
	
	// for the initializations of arrays
	void add_to_vals(Value val);
	Value *vals;
	int vc = 0;
	
	// for else ifs
	void add_elseif(AST_Node *elsif);
	AST_Node **elsifs;
	int elseif_count = 0;
%}

/* YYSTYPE union */
%union{
	// different types of values
	Value val;   
	
	// structures
	list_t* symtab_item;
	AST_Node* node;
	
	// for declarations
	int data_type;
	int const_type;
	
	// for arrays
	int array_size;
}

/* token definition */
%token<val> CHAR INT FLOAT DOUBLE IF ELSE WHILE FOR CONTINUE BREAK VOID RETURN
%token<val> ADDOP MULOP DIVOP INCR OROP ANDOP NOTOP EQUOP RELOP
%token<val> LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE SEMI DOT COMMA ASSIGN REFER
%token <symtab_item> ID
%token <val> 	 ICONST
%token <val>  	 FCONST
%token <val> 	 CCONST
%token <val>     STRING

/* precedencies and associativities */
%left COMMA
%right ASSIGN
%left OROP
%left ANDOP
%left EQUOP
%left RELOP
%left ADDOP
%left MULOP DIVOP
%right NOTOP INCR REFER MINUS
%left LPAREN RPAREN LBRACK RBRACK
%left WHILE

/* rule (non-terminal) definitions */
%type <node> program
%type <node> declarations declaration
%type <data_type> type
%type <symtab_item> variable
%type <symtab_item> init var_init
%type <node> constant
%type <node> expression var_ref
%type <node> statement assigment
%type <node> statements tail
%type <node> while_statement

%start program

%%

program: declarations statements { ast_traversal($2); } ;

/* declarations */
declarations: declarations declaration | declaration ;

declaration: type { declare = 1; } names { declare = 0; } SEMI
	{
		int i;
		$$ = new_ast_decl_node($1, names, nc);
		nc = 0;
		
		AST_Node_Decl *temp = (AST_Node_Decl*) $$;
		
		// declare types of the names
		for(i=0; i < temp->names_count; i++){
			// variable
			if(temp->names[i]->st_type == UNDEF){
				set_type(temp->names[i]->st_name, temp->data_type, UNDEF);
			}
			// pointer
			else if(temp->names[i]->st_type == POINTER_TYPE){
				set_type(temp->names[i]->st_name, POINTER_TYPE, temp->data_type);
			}
			// array
			else if(temp->names[i]->st_type == ARRAY_TYPE){
				set_type(temp->names[i]->st_name, ARRAY_TYPE, temp->data_type);
			}
		}
		ast_traversal($$); /* just for testing */
	}
;

type: INT  		{ $$ = INT_TYPE;   }
	| CHAR 		{ $$ = CHAR_TYPE;  }
	| FLOAT 	{ $$ = REAL_TYPE;  }
	| DOUBLE 	{ $$ = REAL_TYPE;  }
	| VOID 		{ $$ = VOID_TYPE;  }
;

names: names COMMA variable
	{
		add_to_names($3);
	}
	| names COMMA init
	{
		add_to_names($3);
	}
	| variable
	{
		add_to_names($1);
	}
	| init
	{ 
		add_to_names($1);
	}
;

variable: ID { 
	$$ = $1;
	};

init:
	var_init { $$ = $1; }
; 

var_init : ID ASSIGN constant
{ 
	AST_Node_Const *temp = (AST_Node_Const*) $$;
	$1->val = temp->val;
	$1->st_type = temp->const_type;
	$$ = $1;
	AST_Node_Const *t=(AST_Node_Const *)$3;
	
	AST_Node *t2=(AST_Node *)$3;
	$1->tem_no=t2->tem_no;
}
;
statements:
	statements statement
	{
		AST_Node_Statements *temp = (AST_Node_Statements*) $1;
		$$ = new_statements_node(temp->statements, temp->statement_count, $2);
	}
	| statement
	{
		$$ = new_statements_node(NULL, 0, $1);
	}
;

statement:
	 while_statement
	{
		$$ = $1; /* just pass information */
	}
	| assigment SEMI
	{
		$$ = $1; /* just pass information */
	}
	| CONTINUE SEMI
	{ 
		$$ = new_ast_simple_node(0);
	}
	| BREAK SEMI
	{ 
		$$ = new_ast_simple_node(1);
	}
	| ID INCR SEMI
	{
		/* increment */
		if($2.ival == INC){
			$$ = new_ast_incr_node($1, 0, 0);
		}
		else{
			$$ = new_ast_incr_node($1, 1, 0);
		}
	}
	| INCR ID SEMI
	{
		/* increment */
		if($1.ival == INC){
			$$ = new_ast_incr_node($2, 0, 1);
		}
		else{
			$$ = new_ast_incr_node($2, 1, 1);
		}
	}
;


while_statement: WHILE LPAREN expression RPAREN tail
{

	$$ = new_ast_while_node($3, $5, &loop_no);
}
;

tail: LBRACE statements RBRACE
{ 
	$$ = $2; /* just pass information */
}
;

expression:
    expression ADDOP expression
	{ 
	    $$ = new_ast_arithm_node($2.ival, $1, $3);
		AST_Node *temp=(AST_Node *)$$;
		AST_Node *temp1=(AST_Node *)$1;
		AST_Node *temp2=(AST_Node *)$3;
		temp->tem_no=tem_no;
		tem_no++;

		
	}
	| expression MULOP expression
	{
	    $$ = new_ast_arithm_node(MUL, $1, $3);
		AST_Node *temp=(AST_Node *)$$;
		AST_Node *temp1=(AST_Node *)$1;
		AST_Node *temp2=(AST_Node *)$3;
		temp->tem_no=tem_no;
		tem_no++;

	}
	| expression DIVOP expression
	{
		$$ = new_ast_arithm_node(DIV, $1, $3);
		
		AST_Node *temp=(AST_Node *)$$;
		AST_Node *temp1=(AST_Node *)$1;
		AST_Node *temp2=(AST_Node *)$3;
		temp->tem_no=tem_no;
		tem_no++;

	}
	| ID INCR
	{
		/* increment */
		if($2.ival == INC){
			$$ = new_ast_incr_node($1, 0, 0);
		}
		else{
			$$ = new_ast_incr_node($1, 1, 0);
		}	
		AST_Node *temp=(AST_Node *)$$;
		temp->tem_no=$1->tem_no;
		

	}
	| INCR ID
	{
		/* increment */
		if($1.ival == INC){
			$$ = new_ast_incr_node($2, 0, 1);
		}
		else{
			$$ = new_ast_incr_node($2, 1, 1);
		}	
		AST_Node *temp=(AST_Node *)$$;
		temp->tem_no=$1->tem_no;
	}
	| expression OROP expression
	{
		$$ = new_ast_bool_node(OR, $1, $3);
		
		
	}
	| expression ANDOP expression
	{
		$$ = new_ast_bool_node(AND, $1, $3);
		
		
	}
	| NOTOP expression
	{
	    $$ = new_ast_bool_node(NOT, $2, NULL);
		
		
	}
	| expression EQUOP expression
	{
		$$ = new_ast_equ_node($2.ival, $1, $3);

	}
	| expression RELOP expression
	{
		$$ = new_ast_rel_node($2.ival, $1, $3);

	}
	| LPAREN expression RPAREN
	{
		$$ = $2; /* just pass information */
	}
	| var_ref
	{ 
		$$ = $1; /* just pass information */
	
		AST_Node_Ref *t=(AST_Node_Ref *)$1;
		AST_Node *t2=(AST_Node *)$$;
		t2->tem_no=(t->entry->tem_no);
	
	}
	| constant
	{
		$$ = $1; /* no sign */
		
		AST_Node *t=(AST_Node *)$1;
		t->tem_no=tem_no;
		AST_Node *t2=(AST_Node *)$$;
		t2->tem_no=tem_no;
		AST_Node_Const *t3=(AST_Node_Const *)$1;
		tem_no++;

	}
	| ADDOP constant %prec MINUS
	{
		/* plus sign error */
		if($1.ival == ADD){
			fprintf(stderr, "Error having plus as a sign!\n");
			exit(1);
		}
		else{
			AST_Node_Const *temp = (AST_Node_Const*) $2;
		
			/* inverse value depending on the constant type */
			switch(temp->const_type){
				case INT_TYPE:
					temp->val.ival *= -1;
					break;
				case REAL_TYPE:
					temp->val.fval *= -1;
					break;
				case CHAR_TYPE:
					/* sign before char error */
					fprintf(stderr, "Error having sign before character constant!\n");
					exit(1);
					break;
			}
			
			$$ = (AST_Node*) temp;
		}
	}
constant:
	ICONST   { $$ = new_ast_const_node(INT_TYPE, $1);  }
	| FCONST { $$ = new_ast_const_node(REAL_TYPE, $1); }
	| CCONST { $$ = new_ast_const_node(CHAR_TYPE, $1); }
;

assigment: var_ref ASSIGN expression
{
	AST_Node_Ref *temp = (AST_Node_Ref*) $1;
	$$ = new_ast_assign_node(temp->entry, temp->ref, $3);

}
;

var_ref: variable
	{
		$$ = new_ast_ref_node($1, 0); /* no reference */
	}
	| REFER variable
	{
		$$ = new_ast_ref_node($2, 1); /* reference */
	}
; 



%%

void yyerror ()
{
  fprintf(stderr, "Syntax error at line %d\n", lineno);
  exit(1);
}

void add_to_names(list_t *entry){
	// first entry
	if(nc == 0){
		nc = 1;
		names = (list_t **) malloc( 1 * sizeof(list_t *));
		names[0] = entry;
	}
	// general case
	else{
		nc++;
		names = (list_t **) realloc(names, nc * sizeof(list_t *));
		names[nc - 1] = entry;		
	}
}

void add_to_vals(Value val){
	// first entry
	if(vc == 0){
		vc = 1;
		vals = (Value *) malloc(1 * sizeof(Value));
		vals[0] = val;
	}
	// general case
	else{
		vc++;
		vals = (Value *) realloc(vals, vc * sizeof(Value));
		vals[vc - 1] = val;
	}
}

void add_elseif(AST_Node *elsif){
	// first entry
	if(elseif_count == 0){
		elseif_count = 1;
		elsifs = (AST_Node **) malloc(1 * sizeof(AST_Node));
		elsifs[0] = elsif;
	}
	// general case
	else{
		elseif_count++;
		elsifs = (AST_Node **) realloc(elsifs, elseif_count * sizeof(AST_Node));
		elsifs[elseif_count - 1] = elsif;
	}
}

int main (int argc, char *argv[]){
	
	// initialize symbol table
	init_hash_table();
	
	// initialize revisit queue
	queue = NULL;
	
	// parsing
	int flag;
	yyin = fopen(argv[1], "r");
	flag = yyparse();
	fclose(yyin);
	
	printf("Parsing finished!\n");
	
	if(queue != NULL){
		printf("Warning: Something has not been checked in the revisit queue!\n");
	}
	
	// symbol table dump
	yyout = fopen("symtab_dump.out", "w");
	symtab_dump(yyout);
	fclose(yyout);
	
	// revisit queue dump
	yyout = fopen("revisit_dump.out", "w");
	revisit_dump(yyout);
	fclose(yyout);
	
	return flag;
}
