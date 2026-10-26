#include "../headers/TempParams.h"
#include "../headers/ConstParams.h"

//инициализация основных массивов
GridDots** gridDotsArray = NULL;
SquareMap** squareMapArray = NULL;

std::vector<SectorMap*> sectorsArray;

std::vector<LineMap*> vectorsMapArray;
std::vector<LineMap*> previousVectorsMapArray;

std::vector<GameCharacter*> characters;

//вывод в консоль при отрисовке луча
double currPerpWallDist;
double hitFromSquareDot;
int hit_on_map_Y;
int hit_on_map_X;
double dot_hit_x;
double dot_hit_y;
double total_hit_x;
double total_hit_y;
double totalPerpWallDist;
double actual_hit_x;
double actual_hit_y;
double amount_of_lines;
double line_start_x;
double line_start_y;
double line_end_x;
double line_end_y;

//инициализация числа пи
double PI = 3.14159;

//инициализация кол-ва строк и столбцов для отрисовки карты
int gridRows = squares_amount_x;
int gridCols = squares_amount_y;

//координаты изменения параметров grid сетки
double dotRadius = 1;
double squareHeight = ceil_h;
double squareWidth = ceil_w;
double prevSquareWidth;
double prevSquareHeight;

//время кадра
Uint32 lastTick;
Uint32 currTick;
Uint32 deltaTime;

//моиторинг зума
double amountOfDotsHeight = squares_amount_y;
double amountOfDotsWidth = squares_amount_x;
double zoomEpsW;
double zoomEpsH;
double prevZoomEpsW;
double prevZoomEpsH;
double scrollDotsW = amountOfDotsWidth;
double scrollDotsH = amountOfDotsHeight;
bool mapMoove;
bool lineDrawing;
bool drawinWhileMoove;
int prevMouseX;
int prevMouseY;
int prevZoomSector;
Uint32 pressBtnTick;
Uint32 releaseBtnTick;


//обработка характеристик персонажей
double charRadius = 5;
double step_eps = 0.006;
std::string currCharRole;
std::string currCharName;
double currDirX;
double currDirY;
char key_pressed;
char key_pressed_rotation;

double step_size = 0.4;

//переменные игрока
bool placeCharacter;
bool playerIsPlaced;
bool mainCharacterCreated;
double characterX;
double characterY;

//области наведения мыши
// 1
double firstZone_startX = 0;
double firstZone_startY = 0;
double firstZone_endX = (img_w - dev_zone_w) / 3;
double firstZone_endY = img_h / 3;
// 2
double secondZone_startX = (img_w - dev_zone_w) / 3;
double secondZone_startY = 0;
double secondZone_endX = 2 * ((img_w - dev_zone_w) / 3);
double secondZone_endY = img_h / 3;
// 3
double thirdZone_startX = 2 * ((img_w - dev_zone_w) / 3);
double thirdZone_startY = 0;
double thirdZone_endX = (img_w - dev_zone_w);
double thirdZone_endY = img_h / 3;
// 4
double fourthZone_startX = 0;
double fourthZone_startY = img_h / 3;
double fourthZone_endX = (img_w - dev_zone_w) / 3;
double fourthZone_endY = 2 * (img_h / 3);
// 5
double fifthZone_startX = (img_w - dev_zone_w) / 3;
double fifthZone_startY = img_h / 3;
double fifthZone_endX = 2 * ((img_w - dev_zone_w) / 3);
double fifthZone_endY = 2 * (img_h / 3);
// 6
double sixZone_startX = 2 * ((img_w - dev_zone_w) / 3);
double sixZone_startY = img_h / 3;
double sixZone_endX = (img_w - dev_zone_w);
double sixZone_endY = 2 * (img_h / 3);
// 7
double sevenZone_startX = 0;
double sevenZone_startY = 2 * (img_h / 3);
double sevenZone_endX = (img_w - dev_zone_w) / 3;
double sevenZone_endY = img_h;
// 8
double eightZone_startX = (img_w - dev_zone_w) / 3;
double eightZone_startY = 2 * (img_h / 3);
double eightZone_endX = 2 * ((img_w - dev_zone_w) / 3);
double eightZone_endY = img_h;
// 9
double ninethZone_startX = 2 * ((img_w - dev_zone_w) / 3);
double ninethZone_startY = 2 * (img_h / 3);
double ninethZone_endX = (img_w - dev_zone_w);
double ninethZone_endY = img_h;

//игрок
GameCharacter* player = nullptr;

int currLineColorR = 255;
int currLineColorG = 255;
int currLineColorB = 255;

int currCharColorR = 255;
int currCharColorG = 255;
int currCharColorB = 255;

int const bufSize = 20;
char buf[bufSize] = "";

bool mouseButtonIsPressed = false;

//переменная переключения между оканми движка
std::string selected_engine_window = "map";
int draw_step = ((img_w - dev_zone_w) / additional_window_w);

//разница между текущими координатами игрока и координатами на мини карте
double mini_map_differ_x = 0.0;
double mini_map_differ_y = 0.0;

//погрешность для сдвига при перемещениях на мини карте
double mini_map_epsilon_x = 0.0;
double mini_map_epsilon_y = 0.0;

void clearCharacters()
{
    if (characters.size() != 0)
    {
        for (int i = 0; i < characters.size(); i++)
        {
            delete characters[i];
        }
        characters.clear();
        std::vector<GameCharacter*>().swap(characters);
    }
    else
    {
        std::cout << "ERROR: Not initialized array" << std::endl;
    }
}

void clearSquareMapArray()
{
    if (squareMapArray != nullptr)
    {
        for (int i = 0; i < squares_amount_y - 1; ++i) {
            if (squareMapArray[i] != nullptr) {
				// очистка векторов внутри каждого элемента массива
				squareMapArray[i]->linesList.clear();
                
                delete[] squareMapArray[i];
                squareMapArray[i] = nullptr; // на всякий случай
            }
        }
        delete[] squareMapArray;
        squareMapArray = nullptr;
    }
    else
    {
        std::cout << "ERROR: not initialized array" << std::endl;
    }
}

void initSquareMapArray()
{
    int height = squares_amount_y;
    int width = squares_amount_x;

    if (height <= 0 || width <= 0)
    {
        std::cout << "ERROR: sizes of squareMapArray has not been set" << std::endl;
    }
    else
    {
        squareMapArray = new SquareMap * [height - 1];

        for (int i = 0; i < height - 1; i++)
        {
            squareMapArray[i] = new SquareMap[width - 1];

            for (int j = 0; j < width - 1; j++)
            {
                squareMapArray[i][j] = SquareMap();
            }
        }
    }
}

void clearGridDotsArray()
{
    for (int i = 0; i < gridCols; ++i) {
        delete[] gridDotsArray[i];
    }
    delete[] gridDotsArray;
    gridDotsArray = nullptr;
}