#ifndef __ERROR__H__
#define __ERROR__H__

typedef enum {
	ERR_INTERNAL,
	ERR_FILE,
	ERR_SYNTAX,
	ERR_TREE,
	ERR_CODEGEN,
} error_e;

void error(error_e type, char const *msg);

#endif //!__ERROR__H__
