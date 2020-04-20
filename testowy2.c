#include <stdio.h>
#include <stdlib.h>>

int main() {
    gamma_t *g;

  g = gamma_new(0, 0, 0, 0);
  if(g == NULL)
    printf("1");

  g = gamma_new(10, 10, 2, 3);
  if(g != NULL)
    printf("2");
  else
    printf("NUUL");

  if(gamma_move(g, 1, 0, 0))
    printf("3");
  if(gamma_busy_fields(g, 1) == 1)
    printf("4");
  if(gamma_busy_fields(g, 2) == 0)
    printf("5");
  if(gamma_free_fields(g, 1) == 99)
    printf("6");
  if(gamma_free_fields(g, 2) == 99)
    printf("7");
  if(!gamma_golden_possible(g, 1))
    printf("8");
  if(gamma_move(g, 2, 3, 1))
    printf("9");
  if(gamma_busy_fields(g, 1) == 1)
    printf("10");
  if(gamma_busy_fields(g, 2) == 1)
    printf("11");
  if(gamma_free_fields(g, 1) == 98)
    printf("12");
  if(gamma_free_fields(g, 2) == 98)
    printf("13");
  if(gamma_move(g, 1, 0, 2))
    printf("14");
  if(gamma_move(g, 1, 0, 9))
    printf("15");
  if(!gamma_move(g, 1, 5, 5))
    printf("16");
  if(gamma_free_fields(g, 1) == 6)
    printf("17");
  if(gamma_move(g, 1, 0, 1))
    printf("18");
  if(gamma_free_fields(g, 1) == 95)
    printf("19");
  if(gamma_move(g, 1, 5, 5))
    printf("20");
  if(!gamma_move(g, 1, 6, 6))
    printf("21");
  if(gamma_busy_fields(g, 1) == 5)
    printf("22");
  if(gamma_free_fields(g, 1) == 10)
    printf("23 ");
  if(gamma_move(g, 2, 2, 1))
    printf("24 ");
  if(gamma_move(g, 2, 1, 1))
    printf("25 ");
  if(gamma_free_fields(g, 1) == 9)
    printf("26");
  if(gamma_free_fields(g, 2) == 92)
    printf("27");
  if(!gamma_move(g, 2, 0, 1))
    printf("28");
  if(gamma_golden_possible(g, 2))
    printf("29");
  if(!gamma_golden_move(g, 2, 0, 1))
    printf("30");
  if(gamma_golden_move(g, 2, 5, 5))
    printf("31");
  if(!gamma_golden_possible(g, 2))
    printf("32");
  if(gamma_move(g, 2, 6, 6))
    printf("33");
  if(gamma_busy_fields(g, 1) == 4)
    printf("34");
  if(gamma_free_fields(g, 1) == 91)
    printf("35");
  if(gamma_busy_fields(g, 2) == 5)
    printf("36");
  if(gamma_free_fields(g, 2) == 13)
    printf("37");
  if(gamma_golden_move(g, 1, 3, 1))
    printf("38");
  if(gamma_busy_fields(g, 1) == 5)
    printf("39");
  if(gamma_free_fields(g, 1) == 8)
    printf("40");
  if(gamma_busy_fields(g, 2) == 4)
    printf("41");
  if(gamma_free_fields(g, 2) == 10)
    printf("42");

  char *p = gamma_board(g);
  if(p)
    printf("43");
  if(strcmp(p, board) == 0)
    printf("44");
  printf(p);
  free(p);

  gamma_delete(g);
  return 0;
}
