#pragma once

#include "CommandParseError.h"
#include "ICommand.h"
#include "ShapeData.h"
#include <memory>
#include <sstream>

ShapeType ReadShapeType(const std::string& str);
std::unique_ptr<ICommand> ReadCommand(std::stringstream& ss);
void ReadConcreteShapeData(std::stringstream& ss, ShapeData& args);
std::unique_ptr<IShapeGeometry> CreateGeometry(const ShapeData& args);

std::string ReadStringStrictly(std::stringstream& ss);
Point ReadPoint(std::stringstream& ss);
Color ReadColor(std::stringstream& ss);
double ReadDouble(std::stringstream& ss);
