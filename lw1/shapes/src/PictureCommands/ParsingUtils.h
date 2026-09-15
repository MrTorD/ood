#pragma once

#include "CommandData.h"
#include "Errors/CommandParseError.h"
#include <sstream>
#include <memory>
#include "Commands/ICommand.h"

ShapeType ReadShapeType(const std::string& str);
std::unique_ptr<ICommand> ReadCommand(std::stringstream& ss);
void ReadCommandData(std::stringstream& ss, CommandData& args);
std::unique_ptr<IShapeGeometry> CreateGeometry(CommandData args);

std::string ReadStringStrictly(std::stringstream& ss);
Point ReadPoint(std::stringstream& ss);
Color ReadColor(std::stringstream& ss);
double ReadDouble(std::stringstream& ss);
