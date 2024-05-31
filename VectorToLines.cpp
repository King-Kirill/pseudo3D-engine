#include "VectorToLines.h"

VectorToLines::VectorToLines(LineMap& newLine, std::vector<LineMap*>& newVectorLines)
{
	this->line = newLine;
	this->vectorLines = newVectorLines;
}

VectorToLines::VectorToLines()
{
}
