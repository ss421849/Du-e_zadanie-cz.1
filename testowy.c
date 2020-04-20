#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/*
 * Struktura przechowująca parę liczb
 */
typedef struct Pair {
    uint32_t x;
    uint32_t y;
} pair;

/* @brief Struktura przechowująca opis gracza
 *
 */
typedef struct Gamer {
    uint64_t owned_fields;
    uint64_t free_neighbours;
    uint64_t busy_neighbours;
    uint32_t areas;
    bool golden_move_done;
} gamer;

/* @brief Struktura przechowująca opis pola
 */
typedef struct Field {
    uint32_t colour;
    pair *parent;
    uint64_t visited;
} field;

/* @brief Struktura przechowuj¹ca stan gry.
 */
typedef struct gamma {
    uint32_t height;
    uint32_t width;

    uint32_t number_of_gamers;
    gamer **gamers;

    uint32_t max_areas;
    uint64_t free_fields;
    field **board;

    uint64_t last_visit;
} gamma_t;

/* @brief Tworzy strukturę kolejki przechowującą współrzędne pól
 */
typedef struct queue_of_pairs {
    struct queue_of_pairs *next;
    pair *coordinates;
} queue;

/* @brief Tworzy strukturę przechowującą parę liczb
 */
pair *make_pair(uint32_t x, uint32_t y) {
    pair *p = malloc(sizeof(pair));

    p->x = x;
    p->y = y;

    return p;
}

/* @brief Tworzy kopię struktury przechowującej parę liczb
 */
pair *copy_pair(pair *p) {
    if(p) {
        return make_pair(p->x, p->y);
    }
    else {
        return NULL;
    }
}

/* @brief Sprawdza, czy podane pary mają obie wartości takie same
 */
bool pair_cmp(pair *p, pair *q) {
    if (p->x == q->x && p->y == q->y)
        return true;
    else
        return false;
}

/* @brief Zwraca współrzędne wektora do sąsiada
 */
pair *neighbour(uint8_t n) {
    switch(n) {
    case 1:
        return make_pair(0, 1);
    case 2:
        return make_pair(1, 0);
    case 3:
        return make_pair(0, -1);
    case 4:
        return make_pair(-1, 0);
    }
    return NULL;
}

/* @brief Tworzy strukturę przechowującą opis planszy
 */
field **create_board(uint32_t height, uint32_t width) {
    field **board = (field**)malloc(width * sizeof(field*));

    for (uint32_t x = 0; x < width; x++) {
        board[x] = (field*)malloc(height * sizeof(field));

        for (uint32_t y = 0; y < height; y++) {
            board[x][y].colour = 0;
            board[x][y].parent = make_pair(x, y);
            board[x][y].visited = 0;
        }
    }

    return board;
}

/* @brief Usuwa strukturę przechowującą opis planszy
 */
void delete_board(gamma_t *g) {
    for (uint32_t x = 0; x < g->width; x++) {
        for (uint32_t y = 0; y < g->height; y++) {
            free(g->board[x][y].parent);
        }
        free(g->board[x]);
    }
    free(g->board);
}

bool on_the_board(gamma_t *g, uint32_t x, uint32_t y) {
    if (x < g->width && y < g->height)
        return true;
    else
        return false;
}

/* @brief Tworzy strukturę przechowującą opis gracza
 */
gamer *create_gamer() {
    gamer *new_player = malloc(sizeof(gamer));

    new_player->areas = 0;
    new_player->free_neighbours = 0;
    new_player->busy_neighbours = 0;
    new_player->golden_move_done = false;
    new_player->owned_fields = 0;

    return new_player;
}
/* @brief Tworzy tablicę struktur przechowującą opis wszystkich graczy
 *
 */
gamer **create_gamers(uint32_t n) {
    gamer **new_gamers = (gamer**)malloc((n + 1) * sizeof(gamer*));

    for (uint32_t i = 1; i <= n; i++) {
        new_gamers[i] = create_gamer();
    }

    return new_gamers;
}

/* @brief Usuwa tablicę struktur przechowującą opis wszystkich graczy
 */
void delete_gamers(gamer **gamers, uint32_t n) {
    for (uint32_t i = 1; i <= n; i++) {
        free(gamers[i]);
    }
    free(gamers);
}

/* @brief Tworzy strukturê przechowuj¹c¹ stan gry.
 * Alokuje pamiêæ na now¹ strukturê przechowuj¹c¹ stan gry.
 * Inicjuje tê strukturê tak, aby reprezentowa³a pocz¹tkowy stan gry.
 * @param[in] width   – szerokoœæ planszy, liczba dodatnia,
 * @param[in] height  – wysokoœæ planszy, liczba dodatnia,
 * @param[in] players – liczba graczy, liczba dodatnia,
 * @param[in] areas   – maksymalna liczba obszarów,
 *                      jakie mo¿e zaj¹æ jeden gracz.
 * @return WskaŸnik na utworzon¹ strukturê lub NULL, gdy nie uda³o siê
 * zaalokowaæ pamiêci lub któryœ z parametrów jest niepoprawny.
 */
gamma_t *gamma_new(uint32_t height, uint32_t width, uint32_t players, uint32_t areas) {
    if (height == 0 || width == 0 || players == 0 || areas == 0) {
        return NULL;
    }
    else {
        gamma_t *g = malloc(sizeof(gamma_t));

        g->height = height;
        g->width = width;

        g->number_of_gamers = players;
        g->gamers = create_gamers(players);

        g->max_areas = areas;
        g->board = create_board(height, width);
        g->free_fields = width * height;

        g->last_visit = 0;
        return g;
    }
}

/* @brief Usuwa strukturê przechowuj¹c¹ stan gry.
 * Usuwa z pamiêci strukturê wskazywan¹ przez @p g.
 * Nic nie robi, jeœli wskaŸnik ten ma wartoœæ NULL.
 * @param[in] g       – wskaŸnik na usuwan¹ strukturê.
 */
void gamma_delete(gamma_t *g) {
    if (g) {
        delete_board(g);
        delete_gamers(g->gamers, g->number_of_gamers);
        free(g);
    }
}

/* @brief kompresuje reprezentantów każdego pola
 * przechodzi całą ścieżkę reprezentantów od wskazanego pola,
 * aż do ostatniego reprezentanta, tj. będącego swoim własnym reprezentantem
 * i ustawia  reprezentanta każdego pola bezpośrednio jako tego ostatniego
 * po czym zwraca wskaźnik na ostatniego reprezentanta
 */
pair *compress(field **board, pair *p) {
    if (board[p->x][p->y].parent->x != p->x || board[p->x][p->y].parent->y != p->y) {
        pair *p2 = copy_pair(compress(board, board[p->x][p->y].parent));

        free(board[p->x][p->y].parent);
        board[p->x][p->y].parent = p2;
    }

    return board[p->x][p->y].parent;
}

/* @brief Łączy obszary
 * Łączy obecnie zajmowane pole razem z sąsiadującymi obszarami tego samego gracza
 * tak, aby miały wspólnego reprezentanta
 * jednocześnie aktualizuje liczbę obszarów zajmowanych przez gracza,
 */
void unify_areas(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    g->gamers[player]->areas++;
    for (uint32_t i = 1; i <= 4; i++) {
        pair *v = neighbour(i);

        if (on_the_board(g, x + v->x, y + v->y)) {
            if (g->board[x + v->x][y + v->y].colour == player) {
                pair *q = make_pair(v->x + x, v->y + y);
                pair *p = copy_pair(compress(g->board, q));
                free(q);

                //jeśli nie zostały jeszcze połączone
                if (!pair_cmp(g->board[x][y].parent, p)) {
                    g->gamers[player]->areas--;

                    q = copy_pair(g->board[x][y].parent);
                    free(g->board[q->x][q->y].parent);
                    g->board[q->x][q->y].parent = p;
                    free(q);

                    q = make_pair(x, y);
                    compress(g->board, q);
                    free(q);
                }
                else {
                    free(p);
                }
            }
        }

        free(v);
    }
}

/* @brief Sprawdza, czy dane pole nie przylega do obszaru gracza
 */
bool no_owned_neighbour(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
     for (int i = 1; i <= 4; i++) {
        pair *v = neighbour(i);

        if (on_the_board(g, x + v->x, y + v->y)) {
            if (g->board[x + v->x][y + v->y].colour == player) {
                free(v);
                return false;
            }
        }
        free(v);
    }

    return true;
}

/* @brief Przegląda sasiadów pola i na tej podstawie aktualizuje parametry graczy
 * Za każde sąsiednie pole, które jest puste i nie przylegało dotychczas
 * do żadnego obszaru gracza, zwiększa liczbę sąsiadów o c.
 * Jednocześnie, jęsli obecnie rozpatrywane pole przylega do obszaru któregoś gracza,
 * to zmniejsza jego liczbę sąsiadów o c.
 */
void check_neighbours(gamma_t *g, uint32_t player, uint32_t x, uint32_t y, uint64_t c) {
    bool checked[g->number_of_gamers + 1];
    for (uint32_t i = 1; i <= g->number_of_gamers; i++)
        checked[i] = 0;

    for (int i = 1; i <= 4; i++) {
        pair *v = neighbour(i);

        if (on_the_board(g, x + v->x, y + v->y)) {
            if (g->board[x + v->x][y + v->y].colour == 0) {
                if (no_owned_neighbour(g, player, x + v->x, y + v->y)) {
                    g->gamers[player]->free_neighbours += c;
                }
            }
            else {
                if (!checked[g->board[x + v->x][y + v->y].colour]) {
                    checked[g->board[x + v->x][y + v->y].colour] = true;
                    g->gamers[g->board[x + v->x][y + v->y].colour]->free_neighbours -= c;

                    if (g->board[x + v->x][y + v->y].colour != player) {
                        g->gamers[g->board[x + v->x][y + v->y].colour]->busy_neighbours += c;
                    }
                }

                if (no_owned_neighbour(g, player, x, y)) {
                    g->gamers[player]->busy_neighbours += c;
                }
            }
        }

        free(v);
    }
}

/* @brief Wykonuje ruch.
 * Ustawia pionek gracza @p player na polu (@p x, @p y).
 * @param[in,out] g   – wskaŸnik na strukturê przechowuj¹c¹ stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiêksza od wartoœci
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartoœci
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartoœci
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartoœæ @p true, jeœli ruch zosta³ wykonany, a @p false,
 * gdy ruch jest nielegalny lub któryœ z parametrów jest niepoprawny.
 */
bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g->height <= y || g->width <= x || g->number_of_gamers < player) {
        return false;
    }
    else if (g->board[x][y].colour != 0) {
        return false;
    }
    else {
        unify_areas(g, player, x, y);
        if (g->gamers[player]->areas <= g->max_areas) {
            check_neighbours(g, player, x, y, 1);
            g->board[x][y].colour = player;
            g->free_fields--;
            g->gamers[player]->owned_fields++;
            return true;
        }
        else {
            g->gamers[player]->areas--;
            return false;
        }
    }
}

/* @brief Daje napis opisuj¹cy stan planszy.
 * Alokuje w pamiêci bufor, w którym umieszcza napis zawieraj¹cy tekstowy
 * opis aktualnego stanu planszy. Przyk³ad znajduje siê w pliku gamma_test.c.
 * Funkcja wywo³uj¹ca musi zwolniæ ten bufor.
 * @param[in] g       – wskaŸnik na strukturê przechowuj¹c¹ stan gry.
 * @return WskaŸnik na zaalokowany bufor zawieraj¹cy napis opisuj¹cy stan
 * planszy lub NULL, jeœli nie uda³o siê zaalokowaæ pamiêci.
 */
char *gamma_board(gamma_t *g) {
    int i = 0;
    char *answer = malloc(((g->height + 1) * g->width + 1) * sizeof(char));

    if (g->number_of_gamers < 10) {

        for (int y = g->height - 1; y >= 0; y--) {
            for (uint32_t x = 0; x < g->width; x++) {
                if (g->board[x][y].colour == 0)
                    answer[i++] = '.';
                else
                    answer[i++] = '0' + g->board[x][y].colour;
            }
            answer[i++] = '\n';
        }
    }
    else {
        uint64_t size = 2 * ((g->height + 1) * g->width + 1);
        answer = realloc(answer, size * sizeof(char));

        for (int y = g->height - 1; y >= 0; y--) {
            for (uint32_t x = 0; x < g->width; x++) {
                if (g->board[x][y].colour == 0) {
                    answer[i++] = '.';
                    answer[i++] = '\t';
                }
                else {
                    uint32_t k = 1;

                    while (g->board[x][y].colour / k / 10 > 0) {
                        k *= 10;
                        size++;
                    }

                    if (k > 1) {
                        uint32_t d = g->board[x][y].colour;
                        while (k > 0) {
                            answer = realloc(answer, size * sizeof(char));
                            answer[i++] = '0' + d / k;
                            d %= k;
                            k /= 10;
                        }
                        answer[i++] = '\t';
                    }
                    else {
                        answer[i++] = '0' + g->board[x][y].colour;
                        answer[i++] = '\t';
                    }
                }
            }
            answer[i++] = '\n';
        }
    }

    answer[i] = 0;
    return answer;
}

 /* @brief Dodaje na koniec kolejki nowy element i zwraca wskaźnik na niego wskaźnik
  */
 queue *add_to_queue(queue *last, pair *p) {
    queue *new_element = malloc(sizeof(queue));

    new_element->coordinates = p;
    new_element->next = NULL;

    if (last)
        last->next = new_element;

    return new_element;
 }

 /* @brief Usuwa z kolejki pierwszy element i zwraca wskaźnik na następny
  */
queue *rm_from_queue(queue *first) {
    queue *q = first;
    first = first->next;

    free(q->coordinates);
    free(q);

    return first;
}

 /* @brief Wykonuje bfs, po obszarze gracza ustawiając nowego reprezentanta
  */
 void bfs(gamma_t *g, uint32_t player, pair *new_parent) {
    queue *first = add_to_queue(NULL, copy_pair(new_parent));
    queue *last = first;

    for (int i = 1; i <= 4; i++) {
        pair *v =neighbour(i);

        pair *p = first->coordinates;
        if (on_the_board(g, p->x + v->x, p->y + v->y)) {
            if (g->board[p->x + v->x][p->y + v->y].colour == player) {
                last = add_to_queue(last, make_pair(p->x + v->x, p->y + v->y));
            }
        }

        free(v);
    }
    first = rm_from_queue(first);

    while (first) {
        if (g->board[first->coordinates->x][first->coordinates->y].visited < g->last_visit) {
            free(g->board[first->coordinates->x][first->coordinates->y].parent);
            g->board[first->coordinates->x][first->coordinates->y].parent = copy_pair(new_parent);

            for (int i = 1; i <= 4; i++) {
                pair *v = neighbour(i);

                pair *p = first->coordinates;
                if (on_the_board(g, p->x + v->x, p->y + v->y)) {
                    if (g->board[p->x + v->x][p->y + v->y].colour == player) {
                        last = add_to_queue(last, make_pair(p->x + v->x, p->y + v->y));
                    }
                }

                free(v);
            }
            g->board[first->coordinates->x][first->coordinates->y].visited = g->last_visit;
        }
        first = rm_from_queue(first);
    }
 }


 /* @brief zwraca liczbę nowo powstałych obszarów
  * rozbija obszar, z którego usunięto pole (x,y)
  * i zwraca liczbę obszarów, na jakie został rozbity
  */
 uint32_t number_of_parts_of_area(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    uint32_t counted_areas = 0;
    g->board[x][y].colour = 0;

    g->last_visit++;
    for (int i = 1; i <= 4; i++) {
        pair *v = neighbour(i);

        if (on_the_board(g, x + v->x, y + v->y)) {
            if (g->board[x + v->x][y + v->y].colour == player) {
                pair *p = make_pair(x + v->x, y + v->y);

                if (g->board[x + v->x][y + v->y].visited < g->last_visit) {
                    counted_areas++;
                    bfs(g, player, p);
                }

                free(p);
            }
        }

        free(v);
    }

    return counted_areas;
 }

/* @brief Wykonuje z³oty ruch.
 * Ustawia pionek gracza @p player na polu (@p x, @p y) zajêtym przez innego
 * gracza, usuwaj¹c pionek innego gracza.
 * @param[in,out] g   – wskaŸnik na strukturê przechowuj¹c¹ stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiêksza od wartoœci
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartoœci
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartoœci
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartoœæ @p true, jeœli ruch zosta³ wykonany, a @p false,
 * gdy gracz wykorzysta³ ju¿ swój z³oty ruch, ruch jest nielegalny
 * lub któryœ z parametrów jest niepoprawny.
 */
bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g->height <= y || g->width <= x || g->number_of_gamers < player) {
        return false;
    }
    else if (g->board[x][y].colour == 0 || g->board[x][y].colour == player) {
        return false;
    }
    else if (g->gamers[player]->golden_move_done) {
        return false;
    }
    else {
        if (no_owned_neighbour(g, player, x, y) && g->gamers[player]->areas == g->max_areas) {
            return false;
        }
        else {
            uint32_t looser = g->board[x][y].colour;

            uint32_t new_areas = g->gamers[looser]->areas - 1;
            new_areas += number_of_parts_of_area(g, looser, x, y);

            if (new_areas > g->max_areas) {
                uint32_t old_areas = g->gamers[looser]->areas;
                g->board[x][y].colour = looser;
                unify_areas(g, looser, x, y);
                g->gamers[looser]->areas = old_areas;

                return false;
            }
            else {
                if (gamma_move(g, player, x, y)) {
                    check_neighbours(g, looser, x, y, -1);

                    g->gamers[looser]->areas = new_areas;

                    g->free_fields++;
                    g->gamers[looser]->owned_fields--;
                    g->gamers[player]->golden_move_done = true;

                    return true;
                }
                else {
                    g->board[x][y].colour = looser;
                    unify_areas(g, looser, x, y);

                    return false;
                }

            }
        }
    }
}

/* @brief Podaje liczbê pól, jakie jeszcze gracz mo¿e zaj¹æ.
 * Podaje liczbê wolnych pól, na których w danym stanie gry gracz @p player mo¿e
 * postawiæ swój pionek w nastêpnym ruchu.
 * @param[in] g       – wskaŸnik na strukturê przechowuj¹c¹ stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiêksza od wartoœci
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba pól, jakie jeszcze mo¿e zaj¹æ gracz lub zero,
 * jeœli któryœ z parametrów jest niepoprawny.
 */
uint64_t gamma_free_fields(gamma_t *g, uint32_t player) {
    if (g->number_of_gamers < player) {
        return 0;
    }
    else {
        if (g->gamers[player]->areas < g->max_areas) {
            return g->free_fields;
        }
        else {
            return g->gamers[player]->free_neighbours;
        }
    }
}

/* @brief Podaje liczbê pól zajêtych przez gracza.
 * Podaje liczbê pól zajêtych przez gracza @p player.
 * @param[in] g       – wskaŸnik na strukturê przechowuj¹c¹ stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiêksza od wartoœci
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba pól zajêtych przez gracza lub zero,
 * jeœli któryœ z parametrów jest niepoprawny.
 */
uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {
    return g->gamers[player]->owned_fields;
}

/* @brief Sprawdza, czy gracz mo¿e wykonaæ z³oty ruch.
 * Sprawdza, czy gracz @p player jeszcze nie wykona³ w tej rozgrywce z³otego
 * ruchu i jest przynajmniej jedno pole zajête przez innego gracza.
 * @param[in] g       – wskaŸnik na strukturê przechowuj¹c¹ stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiêksza od wartoœci
 *                      @p players z funkcji @ref gamma_new.
 * @return Wartoœæ @p true, jeœli gracz jeszcze nie wykona³ w tej rozgrywce
 * z³otego ruchu i jest przynajmniej jedno pole zajête przez innego gracza,
 * a @p false w przeciwnym przypadku.
 */
bool gamma_golden_possible(gamma_t *g, uint32_t player) {
    if (g->gamers[player]->golden_move_done) {
        return false;
    }
    else if (g->height * g->width == g->free_fields + g->gamers[player]->owned_fields) {
        return false;
    }
    else {
        return true;
    }
}


int main() {
    gamma_t *g;

  g = gamma_new(0, 0, 0, 0);
  assert(g == NULL);

  g = gamma_new(10, 10, 2, 3);
  assert(g != NULL);

  assert(gamma_move(g, 1, 0, 0));
  assert(gamma_busy_fields(g, 1) == 1);
  assert(gamma_busy_fields(g, 2) == 0);
  assert(gamma_free_fields(g, 1) == 99);
  assert(gamma_free_fields(g, 2) == 99);
  assert(!gamma_golden_possible(g, 1));
  assert(gamma_move(g, 2, 3, 1));
  assert(gamma_busy_fields(g, 1) == 1);
  assert(gamma_busy_fields(g, 2) == 1);
  assert(gamma_free_fields(g, 1) == 98);
  assert(gamma_free_fields(g, 2) == 98);
  assert(gamma_move(g, 1, 0, 2));
  assert(gamma_move(g, 1, 0, 9));
  assert(!gamma_move(g, 1, 5, 5));
  assert(gamma_free_fields(g, 1) == 6);
  assert(gamma_move(g, 1, 0, 1));
  assert(gamma_move(g, 1, 5, 5));
  assert(!gamma_move(g, 1, 6, 6));
  assert(gamma_busy_fields(g, 1) == 5);
  assert(gamma_free_fields(g, 1) == 10);
  assert(gamma_move(g, 2, 2, 1));
  assert(gamma_move(g, 2, 1, 1));
  assert(gamma_free_fields(g, 1) == 9);
  assert(gamma_free_fields(g, 2) == 92);
  assert(!gamma_move(g, 2, 0, 1));
  assert(gamma_golden_possible(g, 2));
  assert(!gamma_golden_move(g, 2, 0, 1));
  assert(gamma_golden_move(g, 2, 5, 5));
  assert(!gamma_golden_possible(g, 2));
  assert(gamma_move(g, 2, 6, 6));
  assert(gamma_busy_fields(g, 1) == 4);
  assert(gamma_free_fields(g, 1) == 91);
  assert(gamma_busy_fields(g, 2) == 5);
  assert(gamma_free_fields(g, 2) == 13);
  assert(gamma_golden_move(g, 1, 3, 1));
  assert(gamma_busy_fields(g, 1) == 5);
  assert(gamma_free_fields(g, 1) == 8);
  assert(gamma_busy_fields(g, 2) == 4);
  assert(gamma_free_fields(g, 2) == 10);


  gamma_delete(g);
  return 0;
}
