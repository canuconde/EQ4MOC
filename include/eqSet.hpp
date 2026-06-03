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
#include <filesystem>

using namespace std;
using namespace std::filesystem;

/**
 * @class eqSet
 * @brief Representa un preset de ecualizador con 10 bandas de frecuencia.
 * 
 * Gestiona los parámetros de amplitud (ganancia en dB) para cada banda de frecuencia.
 * Las frecuencias soportadas van desde 60 Hz hasta 16 kHz.
 * 
 * Los archivos se guardan/cargan automáticamente en formato MOC (.moc/eqsets/).
 * Cada preset puede ser marcado como activo (en uso) y contiene un flag de cambios sin guardar.
 */
class eqSet{
private:
    /// @brief Estructura que define una banda de ecualizador
    struct band_type{
        int bandfreq;      ///< Frecuencia central en Hz (60, 170, 310, 600, 1000, 3000, 6000, 12000, 14000, 16000)
        float bandwidth;   ///< Ancho de banda Q (factor de calidad: típicamente ~1.4-2.1)
        float bandamp;     ///< Amplitud/ganancia en dB (-20 a +20 dB)
    };
    
    /// @brief Amplificación principal (3 canales, actualmente no utilizado, reservado para expansión futura)
    float amp[3]={0,0.00,0.00};
    
    /// @brief Array de 11 bandas totales:
    /// - Índice 0: banda maestra/general
    /// - Índices 1-10: las 10 bandas de ecualizador independientes
    /// 
    /// Inicialmente, todos los valores de amplitud están en 0 dB (neutro/sin cambios)
    band_type band[11]={0,0.00,0.00,
                        60,1.46,-0.00,
                        170,1.42,-0.00,
                        310,1.42,-0.00,
                        600,1.42,-0.00,
                        1000,1.42,-0.00,
                        3000,1.44,-0.00,
                        6000,1.50,-0.00,
                        12000,1.76,-0.00,
                        14000,1.90,-0.00,
                        16000,2.08,-0.00};

public:
    /// @brief Constructor que inicializa un preset desde una ruta de archivo
    /// @param file_path Ruta completa del archivo del preset (ej: /home/user/.moc/eqsets/mypreset)
    eqSet(const string &file_path);
    
    /// @brief Nombre del preset (nombre del archivo)
    string name;
    
    /// @brief Directorio donde se almacena el preset
    path moc_dir;
    
    /// @brief Indica si hay cambios sin guardar en el preset actual
    bool unsaved=false;
    
    /// @brief Indica si este preset está activo/en uso en MOC en este momento
    bool isactive=false;
    
    /**
     * @brief Establece el valor de ganancia para una banda específica
     * 
     * @param bandnumber Número de banda (1-10)
     *        - 1 = 60 Hz, 2 = 170 Hz, 3 = 310 Hz, 4 = 600 Hz, 5 = 1 kHz
     *        - 6 = 3 kHz, 7 = 6 kHz, 8 = 12 kHz, 9 = 14 kHz, 10 = 16 kHz
     * @param value Ganancia en dB (-20.0 a +20.0)
     * @return true si tuvo éxito, false en caso contrario
     * 
     * @note Los valores se limitan automáticamente al rango [-20, +20] dB
     * @note Establece automaticamente el flag 'unsaved' en true
     */
    bool setbandvalue(const int &bandnumber,const float &value);
    
    /**
     * @brief Obtiene el valor de ganancia de una banda específica
     * @param bandnumber Número de banda (1-10)
     * @return Ganancia en dB del preset actual
     */
    float getbandvalue(const int &bandnumber);
    
    /**
     * @brief Guarda el preset en su archivo original
     * 
     * Formato del archivo:
     * - Línea 1: comentario (#This file was generated automatically by EQ4MOC.)
     * - Línea 2: marcador (EQSET)
     * - Línea 3: banda maestra (frecuencia, Q)
     * - Líneas 4-13: 10 bandas (frecuencia, Q, amplitud en dB)
     * 
     * @return true si se guardó exitosamente, false en caso de error de I/O
     * @note Establece automaticamente 'unsaved' en false después de guardar
     */
    bool save();
    
    /**
     * @brief Guarda el preset con un nuevo nombre de archivo
     * Útil para crear nuevos presets basados en uno existente
     * 
     * @param new_filename Nuevo nombre del archivo (sin ruta, ej: "mypreset")
     * @return true si se guardó exitosamente, false en caso de error
     * 
     * @warning Si el nombre está vacío, devuelve false
     */
    bool save_as(const string &new_filename);
};