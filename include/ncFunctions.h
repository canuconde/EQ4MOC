//                     eq4moc - EQ for MOC
//     Copyright (C) 2024 Sebastián Bergara <canu.conde@gmail.com>
//
// This file is part of eq4moc (EQ 4 MOC).
//
// eq4moc (EQ 4 MOC) is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// eq4moc (EQ 4 MOC) is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with eq4moc (EQ 4 MOC).
// If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <ncurses.h>
#include <string>

using namespace std;
using namespace std::filesystem;

/**
 * @file ncFunctions.h
 * @brief Funciones auxiliares para la interfaz ncurses de EQ4MOC
 * 
 * Contiene declaraciones de funciones para crear y actualizar ventanas
 * de la interfaz gráfica basada en ncurses (terminal UI).
 * 
 * La interfaz se compone de:
 * - Una ventana principal que muestra información del preset actual
 * - 10 barras de control visuales (una por banda de frecuencia)
 * - Una ventana de diálogo para guardar presets con nuevo nombre
 * 
 * Colores disponibles (definidos en main):
 * - COLOR_PAIR(1): Título de ventana
 * - COLOR_PAIR(2): Estado habilitado
 * - COLOR_PAIR(3): Estado deshabilitado
 * - COLOR_PAIR(4): Información/valores
 * - COLOR_PAIR(5): Título/destaque
 * - COLOR_PAIR(6): Marco/fondo
 */

// Forward declaration para acceso desde headers
class eqSet;

/**
 * @brief Crea una barra visual de control para una banda de ecualizador
 * 
 * Dibuja una barra vertical que muestra gráficamente el nivel de ganancia (-20 a +20 dB).
 * El gráfico es un histograma vertical usando caracteres de bloque.
 * 
 * Layout de la barra:
 * ```
 * +-----+
 * |60   |    <- Etiqueta de frecuencia
 * |-----|    <- Línea separadora
 * |█ █ █|    <- Gráfico de ganancia (bloques rellenados según valor)
 * |█ █ █|
 * |█ █ █|
 * |+50% |    <- Valor actual en porcentaje
 * +-----+
 * ```
 * 
 * @param height Altura de la barra en caracteres de terminal
 * @param width Ancho de la barra en caracteres de terminal
 * @param starty Posición Y inicial en la pantalla (fila)
 * @param startx Posición X inicial en la pantalla (columna)
 * @param label Etiqueta visible de la frecuencia (ej: "60 ", "1 K", "16K")
 * @param value Valor inicial de ganancia en dB a mostrar
 * 
 * @return Puntero a la ventana ncurses creada (WINDOW*)
 *         El llamador es responsable de liberar esta memoria con destroy_win()
 * 
 * @note Las coordenadas starty/startx pueden ser calculadas para distribuir
 *       las 10 barras uniformemente en la pantalla
 */
WINDOW *create_eqbar(int height, int width, int starty, int startx, const string &label, float value);

/**
 * @brief Crea la ventana principal de información del preset
 * 
 * Muestra:
 * - Título: "EQ for MOC"
 * - Nombre del preset actual (ej: "<EQ_4_MOC>")
 * - Frecuencia y ganancia de la banda seleccionada
 * - Atajos de teclado disponibles
 * - Estado del preset (ON/OFF, ACTIVE/UNACTIVE, SAVED/UNSAVED)
 * 
 * @param height Altura total de la pantalla (típicamente LINES)
 * @param width Ancho total de la pantalla (típicamente COLS)
 * @param starty Posición Y inicial (típicamente 0 para pantalla completa)
 * @param startx Posición X inicial (típicamente 0 para pantalla completa)
 * @param local_eqSet Referencia al preset actual a mostrar
 * 
 * @return Puntero a la ventana principal creada
 * 
 * @note Esta ventana ocupa toda la pantalla y contiene el marco y la información
 * @note Las barras de control se superponen sobre esta ventana
 */
WINDOW *create_mainwin(int height, int width, int starty, int startx, eqSet &local_eqSet);

/**
 * @brief Crea la ventana de diálogo "Guardar como"
 * 
 * Permite al usuario ingresar interactivamente un nuevo nombre para guardar
 * el preset actual como un nuevo archivo.
 * 
 * Muestra:
 * - Título: "> Enter new file name <"
 * - Instrucciones: "ESC: Cancel  ENTER: Confirm"
 * - Campo de entrada de texto
 * 
 * @param height Altura del diálogo (típicamente 4 caracteres)
 * @param width Ancho del diálogo (típicamente 2/3 del ancho de pantalla)
 * @param starty Posición Y (típicamente LINES/2-2 para centrar verticalmente)
 * @param startx Posición X (típicamente COLS/6 para centrar horizontalmente)
 * 
 * @return Puntero a la ventana de diálogo creada
 * 
 * @note Habilita automáticamente el cursor y el echo para entrada de texto
 * @note El cursor es visible (curs_set(1)) para que el usuario vea dónde escribe
 */
WINDOW *create_saveaswin(int height, int width, int starty, int startx);

/**
 * @brief Actualiza el contenido visual de una barra de control
 * 
 * Dibuja un gráfico de barras que representa visualmente el nivel de ganancia.
 * El gráfico usa caracteres de tablero de ajedrez (ACS_CKBOARD) para llenar
 * la barra desde abajo.
 * 
 * Conversión de valor a porcentaje:
 * - Rango de entrada: -20 dB a +20 dB
 * - Rango visual: 0% a 100%
 * - Fórmula: percent = ((value + 20) * 100) / 40
 * 
 * @param local_win Ventana (barra) a actualizar
 * @param value Valor en dB (-20 a +20) a mostrar
 * 
 * @note La barra se redibuja completamente en cada llamada
 * @note El valor también se muestra como porcentaje en la parte inferior
 */
void update_control(WINDOW *local_win, float value);

/**
 * @brief Actualiza la ventana principal con información actual
 * 
 * Refresca todos los elementos de información mostrados:
 * - Nombre del preset actual
 * - Frecuencia y ganancia de la banda seleccionada
 * - Atajos de teclado disponibles
 * - Indicadores de estado (ON/OFF, ACTIVE/UNACTIVE, SAVED/UNSAVED)
 * 
 * @param local_win Ventana principal a actualizar
 * @param local_eqSet Preset actual con información a mostrar
 * @param exband Etiqueta expandida de la banda seleccionada (ej: "1 kHz") [opcional]
 * @param value Valor de amplitud en dB de la banda seleccionada [opcional]
 * 
 * @note Esta función es llamada después de cada cambio de entrada del usuario
 * @note Los parámetros opcionales por defecto son "" y 0
 */
void update_window(WINDOW *local_win, eqSet &local_eqSet, string exband="", float value=0);

/**
 * @brief Limpia el contenido visual de una barra de control
 * 
 * Remueve el gráfico de barras y el valor mostrado, pero mantiene intacto
 * el marco (box) y la etiqueta de frecuencia.
 * 
 * @param local_win Barra de control a limpiar
 * 
 * @note Se utiliza antes de rededibujar el gráfico con nuevos valores
 */
void clean_control(WINDOW *local_win);

/**
 * @brief Limpia el contenido informativo de la ventana principal
 * 
 * Borra las líneas que contienen información dinámica (nombre del preset,
 * frecuencia, estado) pero mantiene intacto el marco y título.
 * 
 * @param local_win Ventana principal a limpiar
 * 
 * @note Se utiliza antes de rededibujar con nueva información
 */
void clean_window(WINDOW *local_win);

/**
 * @brief Destruye una ventana ncurses y libera su memoria
 * 
 * Debe llamarse para cada ventana creada con newwin() cuando ya no se necesita,
 * típicamente al finalizar la aplicación o cuando se redimensiona la terminal.
 * 
 * @param local_win Ventana a destruir y liberar
 * 
 * @note ncurses utiliza memoria dinámica para cada ventana
 * @note No validar si el puntero es NULL antes de llamar esta función
 */
void destroy_win(WINDOW *local_win);