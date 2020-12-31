#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;

int
main (int argc, char **argv)
{
  int sleep_sec = 0;
  int c;

  opterr = 0;

  while ((c = getopt (argc, argv, "s:")) != -1)
    switch (c)
      {
      case 's':
        sleep_sec = atoi(optarg);
        break;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }

  if (optind >= argc) {
    fprintf(stderr, "Error: file must be provided\n");
    return 1;
  } else {
    printf ("Reading file %s\n", argv[optind]);

    FILE *f = fopen(argv[optind], "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *buf = (char *) malloc(fsize + 1);
    fread(buf, 1, fsize, f);
    fclose(f);

    buf[fsize] = 0;

    if (sleep_sec > 0)
      sleep(sleep_sec);

    return 0;
  }
  
}