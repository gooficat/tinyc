#include "toks.h"
#include "utils/hash.h"
#include "utils/vector.h"

static char const *KEYWORDS_UNMAPPED[] = {
	"signed", "unsigned", "short", "long", "char", "return", "goto", "if", "sizeof", NULL,
};

static char const *PUNCTUATORS_UNMAPPED[] = {
	"(", ")", "[", "]", "{", "}", ";", ":", ",", NULL,
};

const struct hash_map KEYWORDS;
const struct hash_map PUNCTUATORS;
vec(char *) IDENTIFIERS;

void init_tok_maps() {
	hashmap_from_list(KEYWORDS_UNMAPPED, (struct hash_map *)&KEYWORDS);
	hashmap_from_list(PUNCTUATORS_UNMAPPED, (struct hash_map *)&PUNCTUATORS);
}
