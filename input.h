#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <vector>
#include <map>
#include <fstream>

#include "notes.h"
#include "output.h"

enum State { START, WHITESEP, _16, _32, DURATION, DOT, DOT2, SHARP, NOTE,
             OCTAVE, TRIPLET, REST };

typedef enum TokenType {
  NOTHING, NUMBER, STRING, TSHARP, TDOT, DASH, PLUS, EQUALS,
  RIGHT_ANGLE_BRACKET, LEFT_ANGLE_BRACKET, RIGHT_SQUARE_BRACKET, LEFT_SQUARE_BRACKET, RIGHT_PARENTHESIS, LEFT_PARENTHESIS,
  WHITESPACE, NEWLINE, END_OF_FILE
}ETokenType;

const std::map<char, TokenType> CharTokenType
{
  {'#', TSHARP},
  {'.', TDOT},
  {'-', DASH},
  {'+', PLUS},
  {'=', EQUALS},
  {'(', LEFT_PARENTHESIS},
  {')', RIGHT_PARENTHESIS},
  {'[', LEFT_ANGLE_BRACKET},
  {']', RIGHT_ANGLE_BRACKET},
  {'<', LEFT_ANGLE_BRACKET},
  {'>', RIGHT_ANGLE_BRACKET}
};

std::string printTokenType(ETokenType t);
ETokenType getNextToken(std::ifstream &sourceFile, std::string &data);
int parse();

#endif