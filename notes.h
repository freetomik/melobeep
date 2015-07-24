#ifndef NOTES_H
#define NOTES_H

#include <string>
#include <array>
#include <map>

typedef std::map<char, double> MNoteFreq;
//C++11 initialization of a map, +100 EXP
const MNoteFreq freqTable {
  {'c', 130.8127826503},
  {'d', 146.8323839587},
  {'e', 164.8137784564},
  {'f', 174.6141157165},
  {'g', 195.9977179909},
  {'a', 220.0000000000},
  {'h', 246.9416506281}
};

// for notes in chord type no.1: 4<a1 #c1 e1>
class NotePitch
{
protected:
  bool sharp;
  char note;
  short octave;

  unsigned short pitch;     //in Hz

public:  
  // initialization list, +200 EXP
  NotePitch():
    sharp(false),
    octave(0)
  {}

  void setSharp();
  void setNote(char note);
  void setOctave(char octave);
  double getPitch();

  void computePitch();

  ~NotePitch(){}
};

// for parts of ligature(tie)
// e.g. 1c+2+4.+8..+16..t
class NoteDuration
{
protected:
  unsigned short value;  //1, 2, 4, 8, 16, 32(, ...)
  bool dot, dot2;
  bool triplet;
  
  unsigned int duration;    //in ms
  unsigned short tempo;

public:
  NoteDuration():
    value(4),
    dot(false),
    dot2(false),
    triplet(false),
    duration(500),
    tempo(120)
  {}
  void setDuration(char duration);
  void setDot();
  void setDot2();
  void setTriplet();

  double computeDuration();

  ~NoteDuration(){}
};

//e.g. 16..#a2t[Hello ]
class Note : public NoteDuration, public NotePitch  //multiple inheritance, +500 EXP
{
protected:
  std::string word;

public:
  Note():
    word("")
  {}

  void setWord(std::string word);
  std::string getWord();

  ~Note(){}
};

// e.g. 1c+2+4.+8..+16..t
// class Ligature : public Note
// {
// private:
//   std::array<NoteDuration, 10> durations;

// public:
//   Ligature();
//   ~Ligature(){}
// };

//struct a class je v C++ skoro to same(struct ma vse public)
struct Ligature : public Note
{
  std::array<NoteDuration, 10> durations;
};

// e.g. 4<d #f a> (4..+16.<d #f1 a2>t)
struct Chord : public Note
{
  std::array<NotePitch, 7> pitches;  //max 7 notes in 13th chord
};

// e.g. <8a 4#c 2e> (<8.a 4#c1t 2e2+8>)
struct DifferentDurationsChord : public Note
{
  std::array<Note, 7> notes;
};

class Tempo
{
private:
  NoteDuration value;       //1.part of 8.=280
  unsigned short BPM;       //2.part of 8.=280
  unsigned short position;  //after which note tempo begins
public:
  Tempo():
    BPM(120)
  {}

  unsigned short calculateBPM();

  ~Tempo(){}
};

#endif