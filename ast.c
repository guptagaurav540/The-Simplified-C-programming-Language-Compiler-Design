#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------AST NODE MANAGEMENT-------------------- */
/* The basic node */
AST_Node *new_ast_node(Node_Type type, AST_Node *left, AST_Node *right){
	// allocate memory
	AST_Node *v = malloc (sizeof (AST_Node));
	
	// set entries
	v->type = type;
	v->left = left;
	v->right = right;
	
	// return the result
	return v;
}

/* Declarations */

AST_Node *new_ast_decl_node(int data_type, list_t **names, int names_count){
	// allocate memory
	AST_Node_Decl *v = malloc (sizeof (AST_Node_Decl));
	
	// set entries
	v->type = DECL_NODE;
	v->data_type = data_type;
	v->names = names;
	v->names_count = names_count;
	
	// return type-casted result
	return (struct AST_Node *) v;
}

AST_Node *new_ast_const_node(int const_type, Value val){
	// allocate memory
	AST_Node_Const *v = malloc (sizeof (AST_Node_Const));
	
	// set entries
	v->type = CONST_NODE;
	v->const_type = const_type;
	v->val = val;
	
	// return type-casted result
	return (struct AST_Node *) v;
}

/* Statements */

AST_Node *new_statements_node(AST_Node **statements, int statement_count, AST_Node *statement){
	// allocate memory
	AST_Node_Statements *v = malloc (sizeof (AST_Node_Statements));
	
	// set node type
	v->type = STATEMENTS;
	
	// first statement
	if(statements == NULL){
		statements = (AST_Node**) malloc (sizeof (AST_Node*));
		statements[0] = statement;
		statement_count = 1;
	}
	// add new statement
	else{
		statements = (AST_Node**) realloc (statements, (statement_count + 1) * sizeof (AST_Node*));
		statements[statement_count] = statement;
		statement_count++;
	}
	
	// set entries
	v->statements = statements;
	v->statement_count = statement_count;
	
	// return type-casted result
	return (struct AST_Node *) v;
}


AST_Node *new_ast_while_node(AST_Node *condition, AST_Node *while_branch,int *loop_no){
	// allocate memory
	AST_Node_While *v = malloc (sizeof (AST_Node_While));
	
	// set entries
	v->type = WHILE_NODE;
	v->loop_no=*loop_no;
	(*loop_no)++;
	v->condition = condition;
	v->while_branch = while_branch;
	
	// return type-casted result
	return (struct AST_Node *) v;
}

AST_Node *new_ast_assign_node(list_t *entry, int ref, AST_Node *assign_val){
	// allocate memory
	AST_Node_Assign *v = malloc (sizeof (AST_Node_Assign));
	
	// set entries
	v->type = ASSIGN_NODE;

	v->entry = entry;
	v->ref = ref;
	v->assign_val = assign_val;
	
	// return type-casted result
	return (struct AST_Node *) v;
}

AST_Node *new_ast_simple_node(int statement_type){
	// allocate memory
	AST_Node_Simple *v = malloc (sizeof (AST_Node_Simple));
	
	// set entries
	v->type = SIMPLE_NODE;
	v->statement_type = statement_type;
	
	// return type-casted result
	return (struct AST_Node *) v;
}
						 
AST_Node *new_ast_incr_node(list_t *entry, int incr_type, int pf_type){
	// allocate memory
	AST_Node_Incr *v = malloc (sizeof (AST_Node_Incr));
	
	// set entries
	v->type = INCR_NODE;
	v->entry = entry;
	v->incr_type = incr_type;
	v->pf_type = pf_type;
	
	// return type-casted result
	return (struct AST_Node *) v;
}
/* Expressions */

AST_Node *new_ast_arithm_node(enum Arithm_op op, AST_Node *left, AST_Node *right){
	// allocate memory
	AST_Node_Arithm *v = malloc (sizeof (AST_Node_Arithm));
	
	// set entries
	v->type = ARITHM_NODE;
	v->op = op;
	v->left = left;
	v->right = right;
	
	// return type-casted result
	return (struct AST_Node *) v;
}

AST_Node *new_ast_bool_node(enum Bool_op op, AST_Node *left, AST_Node *right){
	// allocate memory
	AST_Node_Bool *v = malloc (sizeof (AST_Node_Bool));
	
	// set entries
	v->type = BOOL_NODE;
	v->op = op;
	v->left = left;
	v->right = right;
	
	// return type-casted result
	return (struct AST_Node *) v;
}

AST_Node *new_ast_rel_node(enum Rel_op op, AST_Node *left, AST_Node *right){
	// allocate memory
	AST_Node_Rel *v = malloc (sizeof (AST_Node_Rel));
	
	// set entries
	v->type = REL_NODE;
	v->op = op;
	v->left = left;
	v->right = right;
	
	// return type-casted result
	return (struct AST_Node *) v;
}

AST_Node *new_ast_equ_node(enum Equ_op op, AST_Node *left, AST_Node *right){
	// allocate memory
	AST_Node_Equ *v = malloc (sizeof (AST_Node_Equ));
	
	// set entries
	v->type = EQU_NODE;
	v->op = op;
	v->left = left;
	v->right = right;
	
	// return type-casted result
	return (struct AST_Node *) v;	
}

AST_Node *new_ast_ref_node(list_t *entry, int ref){
	// allocate memory
	AST_Node_Ref *v = malloc (sizeof (AST_Node_Ref));
	
	// set entries
	v->type = REF_NODE;
	v->entry = entry;
	v->ref = ref;
	
	// return type-casted result
	return (struct AST_Node *) v;	
}

/* Tree Traversal */

void ast_print_node(AST_Node *node){
	/* temp nodes */
	AST_Node_Decl *temp_decl;
	AST_Node_Const *temp_const;
	AST_Node_Statements *temp_statements;
	AST_Node_Assign *temp_assign;
	AST_Node_Simple *temp_simple;
	AST_Node_Incr *temp_incr;
	AST_Node_Arithm *temp_arithm;
	AST_Node_Bool *temp_bool;
	AST_Node_Rel *temp_rel;
	AST_Node_Equ *temp_equ;
	AST_Node_Ref *temp_ref;
	
	switch(node->type){
		case BASIC_NODE:
		//	printf("Basic Node\n");
			break;
		case DECL_NODE:
			temp_decl = (struct AST_Node_Decl *) node;
		//	printf("Declaration Node of data-type %d for %d names\n",
//				temp_decl->data_type, temp_decl->names_count);
			break;
		case CONST_NODE:
			temp_const = (struct AST_Node_Const *) node;

			IC3_const_exp_code(node->tem_no,temp_const->val);
		//	printf("Constant Node of const-type %d with value ", temp_const->const_type);
			switch(temp_const->const_type){
				case INT_TYPE:
		//			printf("%d\n", temp_const->val.ival);
					break;
				case REAL_TYPE:
		//			printf("%.2f\n", temp_const->val.fval);
					break;
				case CHAR_TYPE:
		//			printf("%c\n",  temp_const->val.cval);
					break;
			}
			break;
		case STATEMENTS:
			temp_statements = (struct AST_Node_Statements *) node;
		//	printf("Statements Node with %d statements\n", temp_statements->statement_count);
			break;
		case WHILE_NODE:
		//	printf("LOOP :");
		//	printf("While Node\n");
			break;
		case ASSIGN_NODE:
			temp_assign = (struct AST_Node_Assign *) node;

			printf("t%d = t%d\n",temp_assign->entry->tem_no,temp_assign->assign_val->tem_no);
			printf("%s = t%d\n",temp_assign->entry->st_name,temp_assign->entry->tem_no);
		//	printf("Assign Node of entry %s\n", temp_assign->entry->st_name);
			break;
		case SIMPLE_NODE:
			temp_simple = (struct AST_Node_Simple *) node;
		//	printf("Simple Node of statement %d\n", temp_simple->statement_type);
			break;
		case INCR_NODE:
			temp_incr = (struct AST_Node_Incr *) node;
			IC3_incr_code(temp_incr->entry->st_name,temp_incr->incr_type,node->tem_no);
//			printf("Increment Node of entry %s being %d %d\n", 
				//temp_incr->entry->st_name, temp_incr->incr_type, temp_incr->pf_type);
			break;
		case ARITHM_NODE:
			temp_arithm = (struct AST_Node_Arithm *) node;
			AST_Node_Arithm *temp = (struct AST_Node_Arithm *)node;
			IC3_arith_code(temp->op,node->left->tem_no,node->right->tem_no,node->tem_no);
		//	printf("Arithmetic Node of operator %d\n", temp_arithm->op);
			break;
		case BOOL_NODE:

			temp_bool = (struct AST_Node_Bool *) node;
			
		//	printf("Boolean Node of operator %d\n", temp_bool->op);
			break;
		case REL_NODE:
			temp_rel = (struct AST_Node_Rel *) node;
		//	printf("Relational Node of operator %d\n", temp_rel->op);
			break;
		case EQU_NODE:
			temp_equ = (struct AST_Node_Equ *) node;
		//	printf("Equality Node of operator %d\n", temp_equ->op);
			break;
		case REF_NODE:
			temp_ref = (struct AST_Node_Ref *) node;
			IC3_var_ref_code(temp_ref->entry->st_name,node->tem_no);

		//	printf("Reference Node of entry %s\n", temp_ref->entry->st_name);
			break;
		default: /* wrong choice case */
			fprintf(stderr, "Error in node selection!\n");
			exit(1);
	}
}

void ast_traversal(AST_Node *node){
	int i;
	
	/* check if empty */
	if(node == NULL){
		return;
	}

	/* left and right child cases */
	if(node->type == BASIC_NODE || node->type == ARITHM_NODE || node->type == BOOL_NODE
	|| node->type == REL_NODE || node->type == EQU_NODE){
		
		ast_traversal(node->left);
		ast_traversal(node->right);
		ast_print_node(node); // postfix
	}
	/* statements case */
	else if(node->type == STATEMENTS){
		AST_Node_Statements *temp_statements = (struct AST_Node_Statements *) node;	
		//ast_print_node(node);	
		for(i = 0; i < temp_statements->statement_count; i++){
		ast_traversal(temp_statements->statements[i]);
		}
	}
	/* while case */
	else if(node->type == WHILE_NODE){
		AST_Node_While *temp_while = (struct AST_Node_While *) node;
		//	ast_print_node(node);
		int i=1;
		ast_traversal(temp_while->condition);

		printf("L%d%d:\nif ",temp_while->loop_no,i);
		if(temp_while->condition->type==BOOL_NODE)
		{

			AST_Node_Bool *m=(AST_Node_Bool *)temp_while->condition;
			IC3_condition_code(m->op,m->left->tem_no,m->right->tem_no);
		}
		if(temp_while->condition->type==REL_NODE)
		{

			AST_Node_Rel *m=(AST_Node_Rel *)temp_while->condition;
			IC3_Rel_code(m->op,m->left->tem_no,m->right->tem_no);
		}
		printf("goto L%d%d:\n",temp_while->loop_no,i+1);
		printf("goto L%d1\nL%d%d :",temp_while->loop_no+1,temp_while->loop_no,i+1);

		ast_traversal(temp_while->while_branch);
		printf("\ngoto L%d1\nL%d1 :\n:",temp_while->loop_no,temp_while->loop_no+1);
	}
	/* assign case */
	else if(node->type == ASSIGN_NODE){
		AST_Node_Assign *temp_assign = (struct AST_Node_Assign *) node;

		//ast_print_node(node);
	//	printf("Assigning:\n");
		ast_traversal(temp_assign->assign_val);
			IC3_var_ref_code(temp_assign->entry->st_name,temp_assign->assign_val->tem_no);

	}
	/* others */
	else{
		ast_print_node(node);
	}
}


void IC3_var_init_code(list_t *entry,Value val)
{
	printf("%s = %d\n",entry->st_name,val.ival);
	printf("t%d = %s\n",entry->tem_no,entry->st_name);
}
void IC3_const_exp_code(int tem_no,Value val)
{
	printf("t%d = %d\n",tem_no,val.ival);
}

void IC3_arith_code(enum Arithm_op op,int arg1,int arg2,int result)
{
	op=ADD;
	if(op==ADD)
	{
		printf("t%d = t%d + t%d\n",result,arg1,arg2);
	}else if(op==MUL)
	{
		printf("t%d = t%d * t%d\n",result,arg1,arg2);
	}
	else if(op==DIV)
	{
		printf("t%d = t%d / t%d\n",result,arg1,arg2);
	}
}
void IC3_var_ref_code(char *name,int right_tem)
{
	printf("%s = t%d\n",name,right_tem);
}
void IC3_id_code(char *name,int tem_no)
{
//	printf("t%d = %s\n",tem_no,name);
}
void IC3_while_code()
{
	printf("LOOP :");
}

void IC3_condition_code(int op,int arg1,int arg2)
{

    if(op==OR)
	{
		printf(" t%d OR t%d ",arg1,arg2);
	}
	else if(op==AND)
	{
		printf(" t%d AND t%d ",arg1,arg2);
	}
	else if(op==NOT)
	{
		printf(" NOT t%d ",arg1);
	}
	if(op==GREATER)
	{
		printf(" t%d > t%d ",arg1,arg2);
	}
	else if(op==LESS)
	{
		printf(" t%d < t%d ",arg1,arg2);
	}
	
	else if(op==GREATER_EQUAL)
	{
		printf(" t%d >= t%d ",arg1,arg2);
	}
	
	else if(op==LESS_EQUAL)
	{
		printf(" t%d <= t%d ",arg1,arg2);
	}
}
void IC3_incr_code(char *name,int type,int tem_no)
{
	if(type==0)
	{
		printf("t%d = %s\n",tem_no,name);
		printf("t%d = t%d + 1\n",tem_no,tem_no);
		printf("%s = t%d\n",name,tem_no);
	}
	else
	{
		printf("t%d = %s\n",tem_no,name);
		printf("t%d = t%d - 1\n",tem_no,tem_no);
		printf("%s = t%d\n",name,tem_no);
	}
}
void IC3_Rel_code(int op,int arg1,int arg2)
{
   if(op==GREATER)
	{
		printf(" t%d > t%d ",arg1,arg2);
	}
	else if(op==LESS)
	{
		printf(" t%d < t%d ",arg1,arg2);
	}
	
	else if(op==GREATER_EQUAL)
	{
		printf(" t%d >= t%d ",arg1,arg2);
	}
	
	else if(op==LESS_EQUAL)
	{
		printf(" t%d <= t%d ",arg1,arg2);
	}
	
}
