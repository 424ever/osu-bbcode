#include <stdlib.h>

#include "bbcode.h"

void bbcode_free(struct bbcode_doc *doc)
{
	free(doc->source);
	free(doc);
}
