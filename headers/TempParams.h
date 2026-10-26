#ifndef TempParams_hpp
#define TempParams_hpp

#include "GridDots.h"
#include "SquareMap.h"
#include <vector>
#include "VectorToLines.h"
#include "LineMap.h"
#include "GameCharacter.h"
#include "SectorMap.h"

//массив точек для отрисовки карты
extern GridDots** gridDotsArray;

//массив нормализованых квадратов для хранения отрисованных расторизованных векторов
extern SquareMap** squareMapArray;

//массив содержащий массивы сообщающихся линий (секторов)
extern std::vector<SectorMap*> sectorsArray;

//массив для хранения обычных векторов (пока не расторизованных)
extern std::vector<LineMap*> vectorsMapArray;

//предыдущий массив для хранения обычных векторов (пока не расторизованных)
extern std::vector<LineMap*> previousVectorsMapArray;

//массив персонажей игры
extern std::vector<GameCharacter*> characters;

//вывод в консоль при отрисовке луча
extern double currPerpWallDist;
extern double hitFromSquareDot;
extern int hit_on_map_Y;
extern int hit_on_map_X;
extern double dot_hit_x;
extern double dot_hit_y;
extern double total_hit_x;
extern double total_hit_y;
extern double totalPerpWallDist;
extern double actual_hit_x;
extern double actual_hit_y;
extern double amount_of_lines;
extern double line_start_x;
extern double line_start_y;
extern double line_end_x;
extern double line_end_y;

//число пи
extern double PI;

//количество стобцов и строк для отрисовки карты
extern int gridRows;
extern int gridCols;

//цветовая палитра отрисовываемой линии
extern int currLineColorR;
extern int currLineColorG;
extern int currLineColorB;

//цветовая палитра текущего персонажа
extern int currCharColorR;
extern int currCharColorG;
extern int currCharColorB;

//буферы записи для инпут полей
extern int const bufSize;
extern char buf[];

//параметры отрисовываемой карты
extern double dotRadius;
extern double squareHeight;
extern double squareWidth;
extern double prevSquareWidth;
extern double prevSquareHeight;

//переменные таймера для синхронизации кадров
extern Uint32 lastTick;
extern Uint32 currTick;
extern Uint32 deltaTime;

//переменные скрола/зума карты
extern double amountOfDotsHeight;
extern double amountOfDotsWidth;
extern double zoomEpsW;
extern double zoomEpsH;
extern double prevZoomEpsW;
extern double prevZoomEpsH;

//параметры создания персонажей
extern std::string currCharRole;
extern std::string currCharName;

//направление персонажа/игрока
extern double currDirX;
extern double currDirY;

//обработчик нажатия клавиш
extern char key_pressed;
extern char key_pressed_rotation;

//старт таймера
extern Uint32 pressBtnTick;
extern Uint32 releaseBtnTick;

//флаги обработки
extern bool placeCharacter;
extern bool mouseButtonIsPressed;
extern bool mapMoove;
extern bool lineDrawing;
extern bool drawinWhileMoove;
extern bool mainCharacterCreated;

extern double step_size;

//текущая позиция отрисовываемого персонажа
extern double characterX;
extern double characterY;

//переменные для обозначения положения мыши для приближения/отдаления карты
extern double scrollDotsW;
extern double scrollDotsH;
extern int prevMouseX;
extern int prevMouseY;
extern int prevZoomSector;
// 1
extern double firstZone_startX;
extern double firstZone_startY;
extern double firstZone_endX;
extern double firstZone_endY;
// 2
extern double secondZone_startX;
extern double secondZone_startY;
extern double secondZone_endX;
extern double secondZone_endY;
// 3
extern double thirdZone_startX;
extern double thirdZone_startY;
extern double thirdZone_endX;
extern double thirdZone_endY;
// 4
extern double fourthZone_startX;
extern double fourthZone_startY;
extern double fourthZone_endX;
extern double fourthZone_endY;
// 5
extern double fifthZone_startX;
extern double fifthZone_startY;
extern double fifthZone_endX;
extern double fifthZone_endY;
// 6
extern double sixZone_startX;
extern double sixZone_startY;
extern double sixZone_endX;
extern double sixZone_endY;
// 7
extern double sevenZone_startX;
extern double sevenZone_startY;
extern double sevenZone_endX;
extern double sevenZone_endY;
// 8
extern double eightZone_startX;
extern double eightZone_startY;
extern double eightZone_endX;
extern double eightZone_endY;
// 9
extern double ninethZone_startX;
extern double ninethZone_startY;
extern double ninethZone_endX;
extern double ninethZone_endY;

//игрок и угол обзора
extern GameCharacter* player;

//параметры игрока
extern double charRadius;
extern double step_eps;

//переменная переключения между оканми движка
extern std::string selected_engine_window;
extern int draw_step;

//разница между текущими координатами игрока и координатами на мини карте
extern double mini_map_differ_x;
extern double mini_map_differ_y;

//погрешность для сдвига при перемещениях на мини карте
extern double mini_map_epsilon_x;
extern double mini_map_epsilon_y;

//кастомные методы очистки памяти массивов указателей
void clearCharacters();
void clearSquareMapArray();
void initSquareMapArray();
void clearGridDotsArray();

//шаблонные методы очистки памяти
template<typename T>
void clearVectorArray(std::vector<T>& vec);

template<typename T>
void clearUnusedMemmory_simpleArr(std::vector<T>& vec);

template<typename T>
void clearUnusedMemmory_ArrInArr(std::vector<std::vector<T>>& vec);

template<typename T>
inline void clearVectorArray(std::vector<T>& vec)
{
	std::vector<T>().swap(vec);
	std::cout << "vectors arr has been cleared, capacity: " << vec.capacity() << std::endl;
}

template<typename T>
inline void clearUnusedMemmory_simpleArr(std::vector<T>& vec)
{
	vec.resize(vec.size());
	vec.shrink_to_fit();

	std::cout << "unused memmory has been cleared, capacity: " << vec.capacity() << " is equal to size: " << vec.size() << std::endl;
}

template<typename T>
inline void clearUnusedMemmory_ArrInArr(std::vector<std::vector<T>>& vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        std::vector<T>& newVec = vec[i];

        newVec.resize(newVec.size());
        newVec.shrink_to_fit();
    }

    vec.resize(vec.size());
    vec.shrink_to_fit();

    std::cout << "unused memmory has been cleared, capacity: " << vec.capacity() << " is equal to size: " << vec.size() << std::endl;
}

#endif /* TempParams_hpp */