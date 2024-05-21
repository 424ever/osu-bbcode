#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bbcode.h"

struct bbcode_doc *bbcode_parse(FILE *ifile)
{
	struct bbcode_doc *doc;
	char		   buf[256];

	doc = malloc(sizeof(*doc));
	if (doc == NULL)
		goto err_exit;

	doc->source_size = 0;
	doc->source	 = malloc(doc->source_size);

	if (doc->source == NULL)
		goto free_doc_exit;

	while (fgets(buf, sizeof(buf), ifile) != NULL)
	{
		doc->source_size += strlen(buf);
		doc->source = realloc(doc->source, doc->source_size);
	}

	if (ferror(ifile))
	{
		perror("read file");
		goto free_source_exit;
	}

	return doc;

free_source_exit:
	free(doc->source);
free_doc_exit:
	free(doc);
err_exit:
	return NULL;
}
