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

#include <ncurses.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "eqSet.hpp"
#include "ncFunctions.h"

using namespace std;

// ============================================================================
// create_saveaswin - Crea el diálogo de "Guardar como"
// ============================================================================
WINDOW *create_saveaswin(int height, int width, int starty, int startx){
    WINDOW *local_win;
    const string saveAsTitle = "> Enter new file name <";

    // Crear ventana con el tamaño y posición especificados
    local_win = newwin(height, width, starty, startx);
    
    // Configurar color de fondo
    wbkgd(local_win, COLOR_PAIR(6));

    // Dibujar marco alrededor
    box(local_win, 0, 0);
    
    // Mostrar título con color especial
    wattron(local_win, COLOR_PAIR(1));
    mvwprintw(local_win, 0, width/2 - saveAsTitle.size()/2, saveAsTitle.data());
    wattroff(local_win, COLOR_PAIR(1));

    // Mostrar instrucciones en la línea 2
    wmove(local_win, 2, width/2);
    
    wattron(local_win, A_BOLD);
    wprintw(local_win, "ESC: ");
    wattroff(local_win, A_BOLD);
    
    wattron(local_win, A_ITALIC);
    wprintw(local_win, "Cancel");
    wattroff(local_win, A_ITALIC);

    wattron(local_win, A_BOLD);
    wprintw(local_win, " ENTER: ");
    wattroff(local_win, A_BOLD);
    
    wattron(local_win, A_ITALIC);
    wprintw(local_win, "Confirm");
    wattroff(local_win, A_ITALIC);

    // Posicionar cursor en el campo de entrada (línea 1, columna 2)
    wmove(local_win, 1, 2);
    
    // Mostrar cursor y habilitar entrada
    curs_set(1);
    echo();
    keypad(local_win, TRUE);
    wrefresh(local_win);

    return local_win;
}

// ============================================================================
// create_eqbar - Crea una barra de control de ecualizador
// ============================================================================
WINDOW *create_eqbar(int height, int width, int starty, int startx, const string &label, float value){
    WINDOW *local_win;
    
    // Crear ventana con posición y tamaño especificados
    local_win = newwin(height, width, starty, startx);
    
    // Configurar color de fondo del marco
    wbkgd(local_win, COLOR_PAIR(6));
    
    // Mostrar etiqueta de frecuencia (ej: "60 ", "1 K", "16K")
    wattron(local_win, COLOR_PAIR(4) | A_BOLD);
    mvwprintw(local_win, 1, 1, label.data());
    wattroff(local_win, COLOR_PAIR(4) | A_BOLD);
    
    // Dibujar marco alrededor
    box(local_win, 0, 0);
    
    // Dibujar línea separadora horizontal en fila 2
    // mvwhline(y, x, car, len) dibuja línea horizontal con 0 (carácter default)
    mvwhline(local_win, 2, 1, 0, 3);
    
    // Mostrar el valor inicial
    update_control(local_win, value);
    
    return local_win;
}

// ============================================================================
// create_mainwin - Crea la ventana principal de información
// ============================================================================
WINDOW *create_mainwin(int height, int width, int starty, int startx, eqSet &local_eqSet){
    WINDOW *local_win;
    
    // Crear ventana ocupando toda la pantalla
    local_win = newwin(height, width, starty, startx);
    
    // Configurar color de fondo
    wbkgd(local_win, COLOR_PAIR(6));
    
    // Dibujar marco alrededor
    box(local_win, 0, 0);
    
    // Preparar título centrado
    string title = string("EQ for MOC").data();
    
    // Posicionar en la primera línea, centrado
    wmove(local_win, 0, width/2 - (title.size() + 2)/2);
    
    // Dibujar título con símbolos decorativos
    wprintw(local_win, "|");
    wattron(local_win, COLOR_PAIR(1));
    wprintw(local_win, title.data());
    wattroff(local_win, COLOR_PAIR(1));
    wprintw(local_win, "|");
    
    // Actualizar contenido de información
    update_window(local_win, local_eqSet);
    
    return local_win;
}

// ============================================================================
// update_window - Actualiza la ventana principal con información actual
// ============================================================================
void update_window(WINDOW *local_win, eqSet &local_eqSet, string exband, float value){
    int width, height;
    
    // Obtener dimensiones actuales de la ventana
    getmaxyx(local_win, height, width);
    
    // Preparar string con formato de ganancia en dB
    string db;
    stringstream strValue;
    strValue << fixed << setprecision(2) << value;

    db = " [ ";
    db += strValue.str();
    db += " dB ] ";

    // ========== SECCIÓN IZQUIERDA: Información del preset ==========
    
    // Mostrar nombre del preset actual (línea height-4)
    wmove(local_win, height-4, width*1/10);
    wprintw(local_win, "EQ preset: ");
    wattron(local_win, COLOR_PAIR(1) | A_BOLD);
    wprintw(local_win, string("<"+local_eqSet.name+">").data());
    wattroff(local_win, COLOR_PAIR(1) | A_BOLD);

    // Mostrar banda seleccionada y su ganancia (línea height-3)
    wmove(local_win, height-3, width*1/10);
    wprintw(local_win, exband.data());
    wattron(local_win, COLOR_PAIR(1) | A_BOLD);
    wprintw(local_win, db.data());
    wattroff(local_win, COLOR_PAIR(1) | A_BOLD);

    // ========== SECCIÓN DERECHA: Atajos de teclado ==========
    
    // Atajo K: cambiar preset
    wmove(local_win, height-4, width*7/10+2);
    wattron(local_win, A_BOLD);
    wprintw(local_win, "K: ");
    wattroff(local_win, A_BOLD);
    wattron(local_win, A_ITALIC);
    wprintw(local_win, "Change EQSET");
    wattroff(local_win, A_ITALIC);

    // Atajo Q: salir
    wmove(local_win, height-3, width*7/10+2);
    wattron(local_win, A_BOLD);
    wprintw(local_win, "Q: ");
    wattroff(local_win, A_BOLD);
    wattron(local_win, A_ITALIC);
    wprintw(local_win, "Exit");
    wattroff(local_win, A_ITALIC);

    // Atajo S: guardar
    wattron(local_win, A_BOLD);
    wprintw(local_win, " S: ");
    wattroff(local_win, A_BOLD);
    wattron(local_win, A_ITALIC);
    wprintw(local_win, "Save");
    wattroff(local_win, A_ITALIC);

    // Atajo N: guardar como
    wmove(local_win, height-2, width*7/10+2);
    wattron(local_win, A_BOLD);
    wprintw(local_win, "N: ");
    wattroff(local_win, A_BOLD);
    wattron(local_win, A_ITALIC);
    wprintw(local_win, "Save As");
    wattroff(local_win, A_ITALIC);

    // Atajo E: refrescar
    wattron(local_win, A_BOLD);
    wprintw(local_win, " E: ");
    wattroff(local_win, A_BOLD);
    wattron(local_win, A_ITALIC);
    wprintw(local_win, "Refresh");
    wattroff(local_win, A_ITALIC);

    // Dibujar línea vertical separadora
    mvwvline(local_win, LINES-4, COLS*7/10, 0, 3);

    // ========== INDICADORES DE ESTADO ==========
    
    // Indicador ON/OFF
    if(local_eqSet.isactive == true){
        string button = "[ON]";
        wattron(local_win, COLOR_PAIR(2) | A_BOLD);
        mvwprintw(local_win, height-4, width*7/10-button.size()-1, button.data());
        wattroff(local_win, COLOR_PAIR(2) | A_BOLD);
    } else {
        string button = "[OFF]";
        wattron(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM);
        mvwprintw(local_win, height-4, COLS*7/10-button.size()-1, button.data());
        wattroff(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM);
    }
    
    // Indicador ACTIVE/UNACTIVE
    if(local_eqSet.isactive == true){
        string button = "[ACTIVE]";
        wattron(local_win, COLOR_PAIR(2) | A_BOLD);
        mvwprintw(local_win, height-3, width*7/10-button.size()-1, button.data());
        wattroff(local_win, COLOR_PAIR(2) | A_BOLD);
    } else {
        string button = "[UNACTIVE]";
        wattron(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM);
        mvwprintw(local_win, height-3, COLS*7/10-button.size()-1, button.data());
        wattroff(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM);
    }
    
    // Indicador SAVED/UNSAVED
    if(local_eqSet.unsaved == true){
        string button = "[UNSAVED]";
        wattron(local_win, COLOR_PAIR(2) | A_BOLD);
        mvwprintw(local_win, height-2, COLS*7/10-button.size()-1, button.data());
        wattroff(local_win, COLOR_PAIR(2) | A_BOLD);
    } else {
        string button = "[SAVED]";
        wattron(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM);
        mvwprintw(local_win, height-2, COLS*7/10-button.size()-1, button.data());
        wattroff(local_win, COLOR_PAIR(3) | A_BOLD | A_DIM);
    }

    // Refrescar ventana para mostrar cambios
    wrefresh(local_win);
}

// ============================================================================
// clean_window - Limpia las líneas de información de la ventana principal
// ============================================================================
void clean_window(WINDOW *local_win){
    int width, height;
    getmaxyx(local_win, height, width);
    
    // Limpiar las 3 líneas inferiores (height-2, height-3, height-4)
    for(int j=1; j<width-1; j++){
        mvwprintw(local_win, height-2, j, " ");
        mvwprintw(local_win, height-3, j, " ");
        mvwprintw(local_win, height-4, j, " ");
    }
}

// ============================================================================
// update_control - Actualiza el gráfico de una barra de control
// ============================================================================
void update_control(WINDOW *local_win, float value){
    // Convertir valor de dB [-20, +20] a rango visual [0, 40] sumando 20
    value = value + 20;  // Ahora value está en rango [0, 40]
    
    int percent;
    int width, height;
    string str;
    
    // Obtener dimensiones de la ventana
    getmaxyx(local_win, height, width);
    
    // Convertir a porcentaje: (value * 100) / 40
    percent = static_cast<int>(value * 100 / 40);

    // Dibujar gráfico de barras usando caracteres de tablero
    wattron(local_win, COLOR_PAIR(5) | A_BOLD);
    for(int i=0; i<percent*(height-4)/100; i++){
        mvwaddch(local_win, height-i-3, 1, ACS_CKBOARD);
        mvwaddch(local_win, height-i-3, 2, ACS_CKBOARD);
        mvwaddch(local_win, height-i-3, 3, ACS_CKBOARD);
    }
    wattroff(local_win, COLOR_PAIR(5) | A_BOLD);
    
    // Limitar porcentaje a rango válido [1, 99]
    if(percent >= 100) percent = 99;
    if(percent <= 0) percent = 1;
    
    // Convertir porcentaje a ganancia visual [-50%, +50%]
    percent -= 50;
    percent = percent * 2;
    
    // Preparar string con signo y valor
    if(percent < 0){
        str = to_string(percent);
    } else {
        str = "+" + to_string(percent);
    }
    
    // Rellenar con espacios para alineación
    if(str.size() < 3) str += "%%";
    
    // Mostrar valor en porcentaje
    wattron(local_win, COLOR_PAIR(4) | A_BOLD);
    mvwprintw(local_win, height-2, 1, str.data());
    wattroff(local_win, COLOR_PAIR(4) | A_BOLD);
    
    // Refrescar ventana para mostrar cambios
    wrefresh(local_win);
}

// ============================================================================
// clean_control - Limpia el contenido visual de una barra
// ============================================================================
void clean_control(WINDOW *local_win){
    int width, height;
    getmaxyx(local_win, height, width);
    
    // Borrar los caracteres del gráfico (columnas 1-3, filas height-2 hasta altura-3)
    for(int i=0; i<height-3; i++){
        mvwprintw(local_win, height-i-2, 1, " ");
        mvwprintw(local_win, height-i-2, 2, " ");
        mvwprintw(local_win, height-i-2, 3, " ");
    }
    
    // Redibujar línea separadora
    mvwhline(local_win, 2, 1, 0, 3);
}

// ============================================================================
// destroy_win - Destruye una ventana ncurses
// ============================================================================
void destroy_win(WINDOW *local_win){
    // Liberar la memoria asociada a la ventana
    delwin(local_win);
}