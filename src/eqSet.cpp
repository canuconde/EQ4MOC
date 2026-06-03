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
#include <filesystem>
#include <fstream>
#include <iomanip>
#include "eqSet.hpp"

#define MOC_EQSET_DIR  ".moc/eqsets"

// Constantes para validación de rango de EQ
#define EQ_MIN_VALUE   -20.0f  ///< Mínima ganancia permitida (-20 dB)
#define EQ_MAX_VALUE    20.0f  ///< Máxima ganancia permitida (+20 dB)
#define EQ_BAND_COUNT   10     ///< Número de bandas de ecualizador

using namespace std;
using namespace std::filesystem;

/**
 * Constructor del preset de ecualizador
 * 
 * Extrae el directorio padre y el nombre del archivo de la ruta proporcionada.
 * Ejemplo: "/home/user/.moc/eqsets/mypreset" -> 
 *   - moc_dir = "/home/user/.moc/eqsets/"
 *   - name = "mypreset"
 */
eqSet::eqSet(const string &file_path){
    moc_dir = path(file_path).remove_filename();
    name = path(file_path).filename();
}

/**
 * Guarda el preset actual en su archivo
 * 
 * El archivo tiene el siguiente formato:
 * ```
 * #This file was generated automatically by EQ4MOC.
 * EQSET
 * 0	0.00
 * 60	1.46	-0.00
 * 170	1.42	-0.00
 * ...
 * ```
 * 
 * @return true si la operación fue exitosa, false si hay error de I/O
 */
bool eqSet::save(){
    string tmp_filename;
    tmp_filename = moc_dir;
    tmp_filename.append(name);
    path tmp_filepath = tmp_filename;
    
    // Abrir archivo en modo escritura
    ofstream eqset_file(tmp_filepath);
    if(!eqset_file.is_open()) {
        cerr << "Error: No se puede abrir archivo para escritura: " << tmp_filepath << endl;
        return false;
    }
    
    // Cabecera del archivo
    eqset_file << "#This file was generated automatically by EQ4MOC." << endl;
    eqset_file << "EQSET" << endl;
    
    // Banda maestra (índice 0) - Solo frecuencia y Q
    eqset_file << band[0].bandfreq << "\t" 
               << setprecision(2) << fixed << band[0].bandwidth << endl;
    
    // Bandas individuales (índices 1-10) - Frecuencia, Q y Amplitud
    for(int i=1; i<11; i++){
        eqset_file << band[i].bandfreq << "\t" 
                   << setprecision(2) << fixed << band[i].bandwidth << "\t"
                   << setprecision(2) << fixed << band[i].bandamp << endl;
    }
    eqset_file.close();
    
    unsaved = false;  // Marcar como guardado
    return true;
}

/**
 * Guarda el preset con un nuevo nombre de archivo
 * 
 * Útil para crear nuevos presets basados en uno existente sin modificar
 * el preset original.
 * 
 * @param new_filename Nuevo nombre del archivo (sin ruta)
 * @return true si se guardó exitosamente, false en caso de error
 */
bool eqSet::save_as(const string &new_filename){
    // Validación: evitar nombres vacíos
    if(new_filename.empty()) {
        cerr << "Error: El nombre del archivo no puede estar vacío" << endl;
        return false;
    }
    
    name = new_filename;
    if(!save()) return false;
    return true;
}

/**
 * Establece el valor de amplitud (ganancia en dB) para una banda específica
 * 
 * Los valores se limitan automáticamente al rango [-20, +20] dB.
 * 
 * @param bandnumber Número de banda (0-10)
 *        - 0 = banda maestra
 *        - 1-10 = bandas de ecualizador
 * @param value Ganancia deseada en dB
 * 
 * @return true si la operación fue válida, false si el índice está fuera de rango
 * 
 * @note Establece automaticamente el flag 'unsaved' en true
 */
bool eqSet::setbandvalue(const int &bandnumber, const float &value){
    // Validación de rango de banda
    if(bandnumber < 0 || bandnumber > EQ_BAND_COUNT) {
        cerr << "Error: Número de banda inválido: " << bandnumber 
             << " (debe estar entre 0 y " << EQ_BAND_COUNT << ")" << endl;
        return false;
    }
    
    // Limitar valor al rango válido [-20, +20] dB (clamping)
    float clamped_value = (value < EQ_MIN_VALUE) ? EQ_MIN_VALUE : 
                          ((value > EQ_MAX_VALUE) ? EQ_MAX_VALUE : value);
    
    band[bandnumber].bandamp = clamped_value;
    unsaved = true;
    return true;
}

/**
 * Obtiene el valor de amplitud (ganancia en dB) de una banda específica
 * 
 * @param bandnumber Número de banda (0-10)
 * @return Ganancia en dB de la banda solicitada, o 0.0f si el índice es inválido
 */
float eqSet::getbandvalue(const int &bandnumber){
    // Validación: evitar acceso fuera de rango
    if(bandnumber < 0 || bandnumber > EQ_BAND_COUNT) {
        cerr << "Error: Número de banda inválido: " << bandnumber 
             << " (debe estar entre 0 y " << EQ_BAND_COUNT << ")" << endl;
        return 0.0f;
    }
    
    return band[bandnumber].bandamp;
}