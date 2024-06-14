#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {  // задаем переменные для флагов
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int regex_flag;
} Options;

void set_options(char current_opt, Options *options, char **pattern);
void get_pattern_from_file(char *filename, char **pattern);
void grep_file(char *name, Options options, regex_t preg, int file_count);
void check_enter(char *line);
regex_t preg;
static const char SHORTOTS[] =
    "e:ivclnhsf:o";  // короткая версия для getopt_long
// static int PREV_SYM = '\n';
// static int PREV_SYM_2 = '\n';

int main(int argc, char *argv[]) {
  char *pattern = 0;
  Options options = {0};
  int file_count = argc - optind;
  char current_opt = getopt(argc, argv, SHORTOTS);
  int file_pointer = 0;
  while (current_opt != -1) {
    set_options(current_opt, &options, &pattern);
    current_opt = getopt(argc, argv, SHORTOTS);
  }
  // int line_counter = 1;
  if (options.e || options.f) {
    file_pointer = optind;
    file_count = argc - optind;
  } else {
    file_count = argc - optind - 1;
    file_pointer = optind + 1;
    pattern = (char *)malloc((strlen(argv[optind]) + 1) * sizeof(char));
    strcpy(pattern, argv[optind]);
  }
  regcomp(&preg, pattern, options.regex_flag);
  for (int i = file_pointer; i < argc; i++) {
    grep_file(argv[i], options, preg, file_count);
  }
  free(pattern);
  regfree(&preg);
  return 0;
}

void set_options(char current_opt, Options *options, char **pattern) {
  // функцией задаем опции по флагам при нажатии
  switch (current_opt) {
    case 'e':
      options->e = 1;
      options->regex_flag |= REG_EXTENDED;
      *pattern = (char *)calloc(strlen(optarg) + 1, sizeof(char));
      strcpy(*pattern, optarg);
      break;
    case 'i':
      options->i = 1;
      options->regex_flag |= REG_ICASE;
      break;
    case 'v':
      options->v = 1;
      break;
    case 'c':
      options->c = 1;
      break;
    case 'l':
      options->l = 1;
      break;
    case 'n':
      options->n = 1;
      break;
    case 'h':
      options->h = 1;
      break;
    case 's':
      options->s = 1;
      break;
    case 'f':
      options->f = 1;
      get_pattern_from_file(optarg, &(*pattern));
      break;
    case 'o':
      options->o = 1;
      break;
    case '?':
      fprintf(stderr,
              "Usage: grep [OPTION]... PATTERNS [FILE]...\nTry 'grep --help' "
              "for more information.\n");
      exit(EXIT_FAILURE);
    default:
      break;
  }
  if (options->c) {
    options->o = 0;
  }
  if (options->l) {
    options->c = 0;
    options->n = 0;
  }
}

void get_pattern_from_file(char *filename, char **pattern) {
  FILE *f = fopen(filename, "r");
  if (f != NULL) {
    char *line = 0;
    size_t len = 0;
    int pattern_len0 = 0;
    getline(&line, &len, f);
    *pattern = (char *)malloc((strlen(line) + 2) * sizeof(char));
    strcpy(*pattern, line);
    int pattern_len = strlen(*pattern) + 1;
    while (getline(&line, &len, f) != EOF) {
      strcpy(*pattern + strlen(*pattern) - 1, "\\|\0");
      pattern_len0 = pattern_len;
      pattern_len += strlen(line) + 1;
      *pattern = (char *)realloc(*pattern, (pattern_len + 2) * sizeof(char));
      strcpy(*pattern + pattern_len0, line);
    }
    free(line);
    fclose(f);
  } else {
    printf("grep: %s: No such file or directory\n", filename);
    exit(EXIT_FAILURE);
  }
}

void grep_file(char *name, Options options, regex_t preg, int file_count) {
  FILE *f = fopen(name, "rt");
  if (f != NULL) {
    char *line = NULL;
    size_t len = 0;
    regmatch_t match;
    int regex_res = 0;
    int count_match = 0;
    int line_number = 1;
    char *line_ptr = 0;
    /* char current_sym = fgetc(f);
    int counter = 0; */
    if (options.h) file_count = 0;
    while (getline(&line, &len, f) != EOF) {
      regex_res = regexec(&preg, line, 1, &match, options.regex_flag);
      if (options.v) regex_res = !regex_res;
      if (options.l && !regex_res) {
        printf("%s\n", name);
        break;
      }
      if (!regex_res) count_match++;
      if (!options.c && !regex_res && !options.o) {
        if (!regex_res && file_count < 2) {
          if (options.n && !regex_res) printf("%d:", line_number);
          printf("%s", line);
        } else if (!regex_res) {
          printf("%s:", name);
          if (options.n && !regex_res) printf("%d:", line_number);
          printf("%s", line);
        }
      } else if (options.o && !regex_res) {
        line_ptr = line;
        while (!regexec(&preg, line_ptr, 1, &match, 0)) {
          if (!regex_res && file_count < 2) {
            if (options.n && !regex_res) printf("%d:", line_number);
            printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                   line_ptr + (int)(match.rm_so));
          } else if (!regex_res) {
            printf("%s:", name);
            if (options.n && !regex_res) printf("%d:", line_number);
            printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
                   line_ptr + (int)(match.rm_so));
          }
          line_ptr += match.rm_eo;
        }
      }
      line_number++;
      if (!regex_res && !options.c && !options.o) check_enter(line);
    }

    if (options.c && file_count < 2)
      printf("%d\n", count_match);
    else if (options.c)
      printf("%s:%d\n", name, count_match);
    fclose(f);
    free(line);
  } else if (!options.s) {
    fprintf(stderr, "grep: %s: No such file or directory\n", name);
  }
}

void check_enter(char *line) {
  char last_sym = line[strlen(line) - 1];
  if (last_sym != '\n') printf("\n");
}