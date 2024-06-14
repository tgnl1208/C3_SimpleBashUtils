#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Options {  // задаем переменные для флагов
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} Opt;

void set_options(char current_opt, Opt *options);
void print_file(char *name, int *line_counter, Opt options);
void reading();
void show_nonprinting(char cur_sym);
static const char SHORTOTS[] = "beEnstvT";  // короткая версия для getopt_long
static const struct option LONGOPTS[] = {
    // расписываем длинную структуру под getopt_long
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {NULL, 0, NULL, 0}  // не пон зачем
};
static int PREV_SYM = '\n';
static int PREV_SYM_2 = '\n';

int main(int argc, char **argv) {
  Opt options = {0};
  char current_opt = getopt_long(argc, argv, SHORTOTS, LONGOPTS, 0);
  while (current_opt != -1) {
    set_options(current_opt, &options);
    current_opt = getopt_long(argc, argv, SHORTOTS, LONGOPTS, 0);
  }
  int line_counter = 1;
  for (int i = optind; i < argc; i++) {
    print_file(argv[i], &line_counter, options);
  }
  // printf("b = %d, e = %d, n = %d, s = %d, t = %d, v = %d\n", options.b,
  //        options.e, options.n, options.s, options.t, options.v);
  return 0;
}

void set_options(char current_opt, Opt *options) {
  // функцией задаем опции по флагам при нажатии
  switch (current_opt) {
    case 'b':
      options->b = 1;
      break;
    case 'E':
      options->e = 1;
      break;
    case 'e':
      options->e = 1;
      options->v = 1;
      break;
    case 'n':
      options->n = 1;
      break;
    case 's':
      options->s = 1;
      break;
    case 't':
      options->t = 1;
      options->v = 1;
      break;
    case 'T':
      options->t = 1;
      break;
    case 'v':
      options->v = 1;
      break;
    case '?':
    default:
      fprintf(stderr, "invalid option -- %c\n", current_opt);
      exit(EXIT_FAILURE);
  }
  if (options->b) {
    options->n = 0;
  }
}

void print_file(char *name, int *line_counter, Opt options) {
  FILE *f = fopen(name, "rt");
  if (f != NULL) {
    char current_sym = fgetc(f);

    while (!feof(f)) {
      if (options.n && PREV_SYM == '\n') {
        printf("%6d\t", *line_counter);
        *line_counter += 1;
      }
      if (options.b && PREV_SYM == '\n' && current_sym != '\n') {
        printf("%6d\t", *line_counter);
        *line_counter += 1;
      }
      if (options.e && current_sym == '\n') {
        printf("%c", '$');
      }
      if (options.t && current_sym == '\t') {
        current_sym = 'I';
        printf("^");
      }
      if (options.s && PREV_SYM == '\n' && PREV_SYM_2 == '\n' &&
          current_sym == '\n') {
        current_sym = fgetc(f);
        continue;
      }
      if (options.v && current_sym != '\n' && current_sym != '\t')
        show_nonprinting(current_sym);
      else
        putc(current_sym, stdout);
      PREV_SYM_2 = PREV_SYM;
      PREV_SYM = current_sym;
      current_sym = fgetc(f);
    }
    fclose(f);
  } else {
    printf("No such file a directory\n");
  }
}

void show_nonprinting(char cur_sym) {
  if ((signed char)cur_sym < 0) {
    printf("M-");
    cur_sym = (signed char)cur_sym + 128;
    if ((cur_sym >= 0 && cur_sym < 32) || cur_sym == 127) {
      printf("^");
      cur_sym += 64;
    }
    putc(cur_sym, stdout);
  } else if (cur_sym < 32 || cur_sym == 127) {
    printf("^");
    cur_sym = cur_sym > 126 ? cur_sym - 128 + 64 : cur_sym + 64;
    putc(cur_sym, stdout);
  } else {
    putc(cur_sym, stdout);
  }
}