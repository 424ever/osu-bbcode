#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void process(const char *ifname, const char *ofname);

int main(int argc, char *argv[]) {
  int opt;
  int closein;
  int ret;
  int closeout;
  const char *ofname;
  FILE *ofile;
  const char *ifname;
  FILE *ifile;

  ofname = NULL;
  ret = EXIT_SUCCESS;

  while ((opt = getopt(argc, argv, "o:")) != -1) {
    switch (opt) {
    case 'o':
      ofname = optarg;
      break;
    case ':':
      fprintf(stderr, "option '-%c' requires an operand\n", optopt);
      ret = EXIT_FAILURE;
      goto exit;
    case '?':
      fprintf(stderr, "unknown option '-%c'\n", optopt);
      ret = EXIT_FAILURE;
      goto exit;
    }
  }

  if (optind >= argc) {
    closein = 0;
    ifile = stdin;
    ifname = "(stdin)";
  } else {
    closein = 1;
    ifname = argv[optind];
    ifile = fopen(ifname, "r");
    if (ifile == NULL) {
      perror(ifname);
      goto close_exit;
    }
  }

  if (ofname == NULL) {
    closeout = 0;
    ofile = stdout;
    ofname = "(stdout)";
  } else {
    closeout = 1;
    ofile = fopen(ofname, "w");
    if (ofile == NULL) {
      perror(ofname);
      goto close_exit;
    }
  }

  process(ifname, ofname);

close_exit:
  if (closein && ifile)
    fclose(ifile);

  if (closeout && ofile)
    fclose(ofile);

exit:
  exit(ret);
}

void process(const char *ifname, const char *ofname) {
  fprintf(stdout, "will process '%s' -> '%s'\n", ifname, ofname);
}
