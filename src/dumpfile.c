#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;

typedef struct buffer_ {
  char *ptr;
  size_t length;
} Buffer;


Buffer read_file(const char *filename)
{
    char * buffer = 0;
    long length;
    FILE * fp = fopen (filename, "rb");

    if (fp)
    {
        fseek (fp, 0, SEEK_END);
        length = ftell(fp);
        fseek (fp, 0, SEEK_SET);
        buffer = (char *) malloc(length);
        if (buffer)
        {
            fread (buffer, 1, length, fp);
        }
        fclose (fp);
    }

    Buffer b = {buffer, length};
    return b;
}

Buffer read_stdin()
{
    size_t cap = 4096;
    size_t len = 0; 

    char *buffer = (char *)malloc(cap * sizeof (char));
    int c;

    while ((c = fgetc(stdin)) != EOF)
        {
            buffer[len] = c;

            if (++len == cap)
                // Make the output buffer twice its current size
                buffer = (char *)realloc(buffer, (cap *= 2) * sizeof (char));
        }

    // Trim off any unused bytes from the buffer
    buffer = (char *)realloc(buffer, (len + 1) * sizeof (char));

    buffer[len] = '\0';

    Buffer b = {buffer, len};
    return b;
}

int
main (int argc, char **argv)
{
  int sleep_sec = 0;
  int dumpout_flag = 0;
  int c;

  opterr = 0;

  while ((c = getopt (argc, argv, "os:")) != -1)
    switch (c)
      {
      case 's':
        sleep_sec = atoi(optarg);
        break;
      case 'o':
        dumpout_flag = 1;
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
    fprintf(stderr, "Reading file %s\n", argv[optind]);

    Buffer buf = read_file(argv[optind]);

    if (sleep_sec > 0)
      sleep(sleep_sec);

    if (dumpout_flag == 1)
      fwrite(buf.ptr, sizeof(char), buf.length, stdout);

    return 0;
  }
  
}