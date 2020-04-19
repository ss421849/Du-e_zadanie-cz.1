/** @file
 * Interfejs klasy przechowuj¹cej stan gry gamma
 *
 * @author Marcin Peczarski <marpe@mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 18.03.2020
 */

#ifndef GAMMA_H
#define GAMMA_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Struktura przechowuj¹ca stan gry.
 */
typedef struct gamma gamma_t;

/** @brief Tworzy strukturê przechowuj¹c¹ stan gry.
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
gamma_t* gamma_new(uint32_t width, uint32_t height,
                   uint32_t players, uint32_t areas);

/** @brief Usuwa strukturê przechowuj¹c¹ stan gry.
 * Usuwa z pamiêci strukturê wskazywan¹ przez @p g.
 * Nic nie robi, jeœli wskaŸnik ten ma wartoœæ NULL.
 * @param[in] g       – wskaŸnik na usuwan¹ strukturê.
 */
void gamma_delete(gamma_t *g);

/** @brief Wykonuje ruch.
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
bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Wykonuje z³oty ruch.
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
bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Podaje liczbê pól zajêtych przez gracza.
 * Podaje liczbê pól zajêtych przez gracza @p player.
 * @param[in] g       – wskaŸnik na strukturê przechowuj¹c¹ stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiêksza od wartoœci
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba pól zajêtych przez gracza lub zero,
 * jeœli któryœ z parametrów jest niepoprawny.
 */
uint64_t gamma_busy_fields(gamma_t *g, uint32_t player);

/** @brief Podaje liczbê pól, jakie jeszcze gracz mo¿e zaj¹æ.
 * Podaje liczbê wolnych pól, na których w danym stanie gry gracz @p player mo¿e
 * postawiæ swój pionek w nastêpnym ruchu.
 * @param[in] g       – wskaŸnik na strukturê przechowuj¹c¹ stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiêksza od wartoœci
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba pól, jakie jeszcze mo¿e zaj¹æ gracz lub zero,
 * jeœli któryœ z parametrów jest niepoprawny.
 */
uint64_t gamma_free_fields(gamma_t *g, uint32_t player);

/** @brief Sprawdza, czy gracz mo¿e wykonaæ z³oty ruch.
 * Sprawdza, czy gracz @p player jeszcze nie wykona³ w tej rozgrywce z³otego
 * ruchu i jest przynajmniej jedno pole zajête przez innego gracza.
 * @param[in] g       – wskaŸnik na strukturê przechowuj¹c¹ stan gry,
 * @param[in] player  – numer gracza, liczba dodatnia niewiêksza od wartoœci
 *                      @p players z funkcji @ref gamma_new.
 * @return Wartoœæ @p true, jeœli gracz jeszcze nie wykona³ w tej rozgrywce
 * z³otego ruchu i jest przynajmniej jedno pole zajête przez innego gracza,
 * a @p false w przeciwnym przypadku.
 */
bool gamma_golden_possible(gamma_t *g, uint32_t player);

/** @brief Daje napis opisuj¹cy stan planszy.
 * Alokuje w pamiêci bufor, w którym umieszcza napis zawieraj¹cy tekstowy
 * opis aktualnego stanu planszy. Przyk³ad znajduje siê w pliku gamma_test.c.
 * Funkcja wywo³uj¹ca musi zwolniæ ten bufor.
 * @param[in] g       – wskaŸnik na strukturê przechowuj¹c¹ stan gry.
 * @return WskaŸnik na zaalokowany bufor zawieraj¹cy napis opisuj¹cy stan
 * planszy lub NULL, jeœli nie uda³o siê zaalokowaæ pamiêci.
 */
char* gamma_board(gamma_t *g);

#endif /* GAMMA_H */

