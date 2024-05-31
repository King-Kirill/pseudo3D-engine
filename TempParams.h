#ifndef TempParams_hpp
#define TempParams_hpp

#include "GridDots.h"
#include "SquareMap.h"
#include <vector>
#include "VectorToLines.h"
#include "LineMap.h"

extern GridDots** gridDotsArray;
extern SquareMap** squareMapArray;

extern std::vector<std::vector<VectorToLines>> sectorLine_Pointers;

extern std::vector<LineMap> vectorsMapArray;
extern std::vector<LineMap> previousVectorsMapArray;

extern int gridRows;
extern int gridCols;

extern int currLineColorR;
extern int currLineColorG;
extern int currLineColorB;

extern bool mouseButtonIsPressed;

void clearSquareMapArray();
void initSquareMapArray();

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