#include "input.h"

using namespace std;


string printTokenType(ETokenType t) {
  string tokenTypeString = "";
  switch(t) {
    case NOTHING:
      tokenTypeString = "NOTHING";
      break;
    case NUMBER:
      tokenTypeString = "NUMBER";
      break;
    case STRING:
      tokenTypeString = "STRING";
      break;
    case TSHARP:
      tokenTypeString = "TSHARP";
      break;
    case TDOT:
      tokenTypeString = "TDOT";
      break;
    case DASH:
      tokenTypeString = "DASH";
      break;
    case PLUS:
      tokenTypeString = "PLUS";
      break;
    case EQUALS:
      tokenTypeString = "EQUALS";
      break;
    case RIGHT_ANGLE_BRACKET:
      tokenTypeString = "RIGHT_ANGLE_BRACKET";
      break;
    case LEFT_ANGLE_BRACKET:
      tokenTypeString = "LEFT_ANGLE_BRACKET";
      break;
    case RIGHT_SQUARE_BRACKET:
      tokenTypeString = "RIGHT_SQUARE_BRACKET";
      break;
    case LEFT_SQUARE_BRACKET:
      tokenTypeString = "LEFT_SQUARE_BRACKET";
      break;
    case RIGHT_PARENTHESIS:
      tokenTypeString = "RIGHT_PARENTHESIS";
      break;
    case LEFT_PARENTHESIS:
      tokenTypeString = "LEFT_PARENTHESIS";
      break;
    case WHITESPACE:
      tokenTypeString = "WHITESPACE";
      break;
    case NEWLINE:
      tokenTypeString = "NEWLINE";
      break;
    case END_OF_FILE:
      tokenTypeString = "END_OF_FILE";  
      break;
    }
    return tokenTypeString;
}

ETokenType getNextToken(ifstream &sourceFile, string &data) {
  int i;
  char c;
  data = "";
  ETokenType TokenType = NOTHING;

  while(sourceFile.get(c)) {
    i = 0;
    if(isdigit(c)) {
      data += c;
      TokenType = NUMBER;
      do {
        sourceFile.get(c);
        data += c;
        i++;
        if(i > 50) return END_OF_FILE;  //workaround pro soubor zakonceny znakem a EOFem bez konce radku mezi nimi, cykli to
      } while(isdigit(c));
      if(c != EOF) {
        data.pop_back();
        sourceFile.unget();
      }
      return TokenType;
    }
    if(isalpha(c)) {
      data += c;
      TokenType = STRING;
      do {
        sourceFile.get(c);
        data += c;
        i++;
        if(i > 50) return END_OF_FILE;
      } while(isalpha(c));
      if(c != EOF) {
        data.pop_back();
        sourceFile.unget();
      }
      return TokenType;
    }
    switch(c) {
      case ' ':
      case '\t':
        TokenType = WHITESPACE;
        do {
          sourceFile.get(c);
        } while(c == ' ' || c == '\t');
        if(c != EOF)
          sourceFile.unget();
        break;
      case '\r':
      case '\n':
        TokenType = NEWLINE;
        break;
      default:
        TokenType = CharTokenType.at(c);
    }
    return TokenType;
  }

  TokenType = END_OF_FILE;

  return TokenType;
}

int parse() {
  return 42;
}