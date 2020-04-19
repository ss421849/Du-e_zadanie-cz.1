/** @file
 * Interfejs klasy przechowuj�cej stan gry gamma
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
 * Struktura przechowuj�ca stan gry.
 */
typedef struct gamma gamma_t;

/** @brief Tworzy struktur� przechowuj�c� stan gry.
 * Alokuje pami�� na now� struktur� przechowuj�c� stan gry.
 * Inicjuje t� struktur� tak, aby reprezentowa�a pocz�tkowy stan gry.
 * @param[in] width   � szeroko�� planszy, liczba dodatnia,
 * @param[in] height  � wysoko�� planszy, liczba dodatnia,
 * @param[in] players � liczba graczy, liczba dodatnia,
 * @param[in] areas   � maksymalna liczba obszar�w,
 *                      jakie mo�e zaj�� jeden gracz.
 * @return Wska�nik na utworzon� struktur� lub NULL, gdy nie uda�o si�
 * zaalokowa� pami�ci lub kt�ry� z parametr�w jest niepoprawny.
 */
gamma_t* gamma_new(uint32_t width, uint32_t height,
                   uint32_t players, uint32_t areas);

/** @brief Usuwa struktur� przechowuj�c� stan gry.
 * Usuwa z pami�ci struktur� wskazywan� przez @p g.
 * Nic nie robi, je�li wska�nik ten ma warto�� NULL.
 * @param[in] g       � wska�nik na usuwan� struktur�.
 */
void gamma_delete(gamma_t *g);

/** @brief Wykonuje ruch.
 * Ustawia pionek gracza @p player na polu (@p x, @p y).
 * @param[in,out] g   � wska�nik na struktur� przechowuj�c� stan gry,
 * @param[in] player  � numer gracza, liczba dodatnia niewi�ksza od warto�ci
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       � numer kolumny, liczba nieujemna mniejsza od warto�ci
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       � numer wiersza, liczba nieujemna mniejsza od warto�ci
 *                      @p height z funkcji @ref gamma_new.
 * @return Warto�� @p true, je�li ruch zosta� wykonany, a @p false,
 * gdy ruch jest nielegalny lub kt�ry� z parametr�w jest niepoprawny.
 */
bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Wykonuje z�oty ruch.
 * Ustawia pionek gracza @p player na polu (@p x, @p y) zaj�tym przez innego
 * gracza, usuwaj�c pionek innego gracza.
 * @param[in,out] g   � wska�nik na struktur� przechowuj�c� stan gry,
 * @param[in] player  � numer gracza, liczba dodatnia niewi�ksza od warto�ci
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       � numer kolumny, liczba nieujemna mniejsza od warto�ci
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       � numer wiersza, liczba nieujemna mniejsza od warto�ci
 *                      @p height z funkcji @ref gamma_new.
 * @return Warto�� @p true, je�li ruch zosta� wykonany, a @p false,
 * gdy gracz wykorzysta� ju� sw�j z�oty ruch, ruch jest nielegalny
 * lub kt�ry� z parametr�w jest niepoprawny.
 */
bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Podaje liczb� p�l zaj�tych przez gracza.
 * Podaje liczb� p�l zaj�tych przez gracza @p player.
 * @param[in] g       � wska�nik na struktur� przechowuj�c� stan gry,
 * @param[in] player  � numer gracza, liczba dodatnia niewi�ksza od warto�ci
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba p�l zaj�tych przez gracza lub zero,
 * je�li kt�ry� z parametr�w jest niepoprawny.
 */
uint64_t gamma_busy_fields(gamma_t *g, uint32_t player);

/** @brief Podaje liczb� p�l, jakie jeszcze gracz mo�e zaj��.
 * Podaje liczb� wolnych p�l, na kt�rych w danym stanie gry gracz @p player mo�e
 * postawi� sw�j pionek w nast�pnym ruchu.
 * @param[in] g       � wska�nik na struktur� przechowuj�c� stan gry,
 * @param[in] player  � numer gracza, liczba dodatnia niewi�ksza od warto�ci
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba p�l, jakie jeszcze mo�e zaj�� gracz lub zero,
 * je�li kt�ry� z parametr�w jest niepoprawny.
 */
uint64_t gamma_free_fields(gamma_t *g, uint32_t player);

/** @brief Sprawdza, czy gracz mo�e wykona� z�oty ruch.
 * Sprawdza, czy gracz @p player jeszcze nie wykona� w tej rozgrywce z�otego
 * ruchu i jest przynajmniej jedno pole zaj�te przez innego gracza.
 * @param[in] g       � wska�nik na struktur� przechowuj�c� stan gry,
 * @param[in] player  � numer gracza, liczba dodatnia niewi�ksza od warto�ci
 *                      @p players z funkcji @ref gamma_new.
 * @return Warto�� @p true, je�li gracz jeszcze nie wykona� w tej rozgrywce
 * z�otego ruchu i jest przynajmniej jedno pole zaj�te przez innego gracza,
 * a @p false w przeciwnym przypadku.
 */
bool gamma_golden_possible(gamma_t *g, uint32_t player);

/** @brief Daje napis opisuj�cy stan planszy.
 * Alokuje w pami�ci bufor, w kt�rym umieszcza napis zawieraj�cy tekstowy
 * opis aktualnego stanu planszy. Przyk�ad znajduje si� w pliku gamma_test.c.
 * Funkcja wywo�uj�ca musi zwolni� ten bufor.
 * @param[in] g       � wska�nik na struktur� przechowuj�c� stan gry.
 * @return Wska�nik na zaalokowany bufor zawieraj�cy napis opisuj�cy stan
 * planszy lub NULL, je�li nie uda�o si� zaalokowa� pami�ci.
 */
char* gamma_board(gamma_t *g);

#endif /* GAMMA_H */

