#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "arena.h"
#include "bbcode.h"
#include "unicode.h"

void print_usage(const char *argv0);
void process(FILE *ifile, FILE *ofile);

static struct alloc_arena *arena;

#ifndef TEST
int main(int argc, char *argv[])
{
	int	    opt;
	int	    ret;
	const char *ofname;
	FILE	   *ofile;
	const char *ifname;
	FILE	   *ifile;

	ofname = NULL;
	ofile  = NULL;
	ret    = EXIT_SUCCESS;

	arena = arena_new();

	while ((opt = getopt(argc, argv, ":o:")) != -1)
	{
		switch (opt)
		{
		case 'o':
			ofname = optarg;
			break;
		case ':':
			fprintf(stderr, "option '-%c' requires an operand\n",
				optopt);
			print_usage(argv[0]);
			ret = EXIT_FAILURE;
			goto exit;
		case '?':
		default:
			fprintf(stderr, "unknown option '-%c'\n", optopt);
			print_usage(argv[0]);
			ret = EXIT_FAILURE;
			goto exit;
		}
	}

	if (optind >= argc)
	{
		ifile  = stdin;
		ifname = "(stdin)";
	}
	else
	{
		ifname = argv[optind];
		ifile  = fopen(ifname, "r");
		if (ifile == NULL)
		{
			perror(ifname);
			goto close_exit;
		}
	}

	if (ofname == NULL)
	{
		ofile  = stdout;
		ofname = "(stdout)";
	}
	else
	{
		ofile = fopen(ofname, "w");
		if (ofile == NULL)
		{
			perror(ofname);
			goto close_exit;
		}
	}

	process(ifile, ofile);

close_exit:
	if (ifile && ifile != stdin && fclose(ifile) == EOF)
		perror("fclose(ifile)");

	if (ofile && ofile != stdout && fclose(ifile) == EOF)
		perror("fclose(ofile)");

exit:
	arena_destroy(arena);
	exit(ret);
}
#endif

void print_usage(const char *argv0)
{
	fprintf(stderr, "usage: %s [-o outfile] infile\n", argv0);
}

void process(FILE *ifile, FILE *ofile)
{
	struct bbcode_doc *doc;

	(void) ofile;

	if ((doc = bbcode_parse(ifile, arena)) == NULL)
		return;
}
