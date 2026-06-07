#include "err.h"
#include "lexer.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

void parse_panic(char const *msg) {
	fflush(stdout);
	fputs(msg, stderr);
	exit(EXIT_FAILURE);
}

void codegen_panic(char const *msg) {
	fflush(stdout);
	fputs(msg, stderr);
	exit(EXIT_FAILURE);
}

static void print_node(struct ast *node);

static void print_scope(struct scope *scope) {
	size_t i;
	printf("Scope: %zu syms, %zu nodes\n", scope->num_syms, scope->num_nodes);
	for (i = 0; i < scope->num_syms; ++i) {
		printf("Symbol %s\n", scope->syms[i].name);
	}
	for (i = 0; i < scope->num_nodes; ++i) {
		print_node(scope->nodes + i);
	}
}

static void print_node(struct ast *node) {
	switch (node->typ) {
	case AstNone:
		puts("Error: No node\n");
		break;
	case AstScope:
		print_scope(&node->val.scope);
		break;
	case AstBinOp:
		printf("Operation: %s\nLeft:\n", OPERATORS[node->val.binop.operator]);
		print_node(node->val.binop.left);
		printf("Right:\n");
		print_node(node->val.binop.right);
		break;
	case AstUnOp:
		printf("Operation: %s\n", OPERATORS[node->val.unop.operator]);
		print_node(node->val.unop.node);
		break;
	case AstOrder:
		printf("Order\n");
		switch (node->val.order.ordr) {
		case OrderNone:
			printf("No node!\n");
			break;
		case OrderReturn:
			printf("Return!\n");
			print_node(node->val.order.val.node);
			break;
		case OrderBreak:
			printf("Break!\n");
			break;
		case OrderContinue:
			printf("Continue!\n");
			break;
		case OrderGoto:
			printf("Goto...\n");
			break;
		}
		break;
	case AstFunc:
		printf("Function\n");
		printf("Named %s\n", node->val.func.sym->name);
		print_scope(&node->val.func.body);
		break;
	case AstCond:
		printf("Conditional %i\n", node->val.cond.typ);
		printf("Condition:\n");
		print_node(node->val.cond.cond);
		printf("Body:\n");
		print_node(node->val.cond.body);
		if (node->val.cond.els) {
			printf("Else:\n");
			print_node(node->val.cond.els);
		}
		break;
	case AstConst:
		printf("Constant of type %i\n", node->val.cnst->typ);
		switch (node->val.cnst->typ) {
		case ConstNone:
			printf("No constant\n");
			break;
		case ConstInt:
			printf("Integer of %ld\n", node->val.cnst->val.i);
			break;
		case ConstFloat:
			printf("Float of %Lf\n", node->val.cnst->val.f);
			break;
		case ConstString:
			printf("String of %s\n", node->val.cnst->val.s);
			break;
		}
		break;
	case AstRef:
		printf("Reference to symbol %s\n", node->val.ref->name);
		break;
	case AstCall:
		printf("Call of:\n");
		print_node(node->val.call.of);
		printf("With\n");
		{
			size_t i;
			for (i = 0; i < node->val.call.args.num_nodes; ++i) {
				print_node(&node->val.call.args.nodes[i]);
			}
		}
		break;
	}
	fflush(stdout);
}

void tree_print(void) {
	fflush(stdout);
	print_node(&tree);
}
