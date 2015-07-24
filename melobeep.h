#ifndef MELOBEEP_H
#define MELOBEEP_H

#include <string>
#include <vector>

#include "notes.h"

class Song
{
private:
  std::vector<Note> melody;
  std::vector<Tempo> tempos;
public:
  Song();
  ~Song(){}
};

class Melobeep
{
private:
  std::string fileName;
  Song song; 
public:
  Melobeep(std::string fileName);

  int read(std::string inputFileName);
  int write2wav(std::string outputFileName);
  int write2midi(std::string outputFileName);
  int write2lilypond(std::string outputFileName);

  ~Melobeep(){}
};

#endif