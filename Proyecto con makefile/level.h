/***************************************************************************//**
  @file     level.c
  @brief    Funciones de inicializacion, manejo y dibujado de aquello relacionado al nivel
 ******************************************************************************/

#ifndef TPFINAL_LEVEL_H
#define TPFINAL_LEVEL_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "render.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MAXPLAYERNAME 11

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Se detalla en memoria dinamica cual es cada uno de los bloques que hay en el nivel especificado
 * @param level puntero a la estructura level_t donde se reservara memoria dinamica y se guardaran todos los bloques
 * @param id aqui se expecifica el nivel del cual se debera cargar el mapa
 * @return Devuelve 0 si no hubo ningun error y 1 si hubo alguno
*/
int cargarMapa(level_t* level , int id);

/**
 * @brief Libera el espacio en memoria reservado dinamicamente por cargarMapa
 * @param gameState Puntero al estado del juego desde donde se accede a todos los bloques
*/
void destroyMap(estadoJuego_t* gameState);

/**
 * @brief A partir de los bloques cargados anteriormente se encarga de dibujar el nivel, en allegro y en la raspi
 * @param *gameState puntero al estado del juego del cual consigue la posicion y las imagenes de aquello que hay que dibujar
*/
void drawLevel(estadoJuego_t *gameState);

/**
 * @brief Se ponen los valores iniciales del tiempo, las monedas, el puntaje y el primer nivel
 * @param gameUI puntero a gameUI_t estructura donde se escribe lo anterior
*/
void initUI(gameUI_t* gameUI);

/**
 * @brief Tomando la informacion de cargarmapa, se inicializa bloques y enemigos con su respectiva posicion, velocidad, tamaño.
 * Tambien se hace un backup de esta info
 * @param gameState Puntero a estadoJuego_t, donde se toma la informacion dicha anteriormente y luego se guarda lo nuevo
 * @return Devuelve 0 si no hubo ningun error y 1 si hubo alguno
*/
int initEntities(estadoJuego_t* gameState);

/**
 * @brief Dibuja las vidas restantes en pantalla en caso de perder una. Esto se realiza en modo allegro y tambien en raspi.
 * @param gameState puntero a estadoJuego_t donde obtiene la informacion para realizar el dibujado
*/
void drawRetryScreen(estadoJuego_t* gameState);

/**
 * @brief Se avisa al jugador que perdio y se muestra su puntaje final. Luego se avisa en el caso de haberse realizado un HIGH SCORE
 * @param gameState puntero a estadoJuego_t donde obtiene la informacion para realizar el dibujado
*/
void drawGameOverScreen(estadoJuego_t* gameState);

/**
 * @brief Dibuja el menu de pausa. Esta funcion solo esta disponible para el modo allegro, para la raspi es una funcion nula
 * @param gameState puntero a estadoJuego_t donde obtiene la informacion para realizar el dibujado
*/
void drawPause(estadoJuego_t *gameState);

/**
 * @brief Se libera el espacio reservado para los bloques y los enemigos, tanto en el original como el backup
 * @param gameState puntero a estadoJuego_t de donde se obtienen los punteros para liberarlos
*/
void destroyEntities(estadoJuego_t * gameState);

/**
 * @brief Se reinician todas los bloques y enemigos ya inicializados en el juego mediante el uso del backup creado por initBackupEntities
 *  Funcion privada que es llamada por initEntities
 * @param puntero a estadoJuego_t de donde se obtiene el estado actual y el default del juego
*/
void resetEntitiesState(estadoJuego_t* gameState);

/**
 * @brief En el caso de ganar el nivel, se avisa y se dibuja el score actual. Para modo Allegro tambien se dibujan las vidas
 * @param gameState puntero a estadoJuego_t donde obtiene la informacion para realizar el dibujado
*/
void drawNextLevelScreen(estadoJuego_t *gameState);

/**
 * @brief Pone la posicion de la ola en 0
*/
void resetWavePosition(void);

/**
 * @brief Se compara el score actual con la tabla de highscores para saber si se realizo un nuevo highscore
 * @param gameState puntero a estadoJuego_t del cual se obtienen los highscores
 * @return Devuelve 0 en el caso de no ser highscore y 1 en el caso de serlo
*/
int wasNewHighScoreAchieved(estadoJuego_t* gameState);

/**
 * @brief Se guarda el nuevo highscore en su respectiva posicion en el archivo .txt que los contiene
 * @param gameState puntero a estadoJuego_t del cual se obtienen los highscores
*/
void saveNewHighScore(estadoJuego_t* gameState);

#if MODOJUEGO == RASPI

/**
 * @brief En el caso de haberse realizado un high score, esta funcion lo avisa en el modo RASPI.
 * @param gameState puntero a estadoJuego_t donde obtiene la informacion para realizar el dibujado
*/
void drawGameOverScreenHighScore(estadoJuego_t* gameState);

#endif

#endif //TPFINAL_LEVEL_H
