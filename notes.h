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
  short octave;             //from -1(great octave) to 3

  unsigned short pitch;     //in Hz

  NotePitch(bool s, char n, short o);

  bool getSharp();
  char getNote();
  short getOctave();
  double getPitch();

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
  
  double duration;    //in ms
  unsigned short tempo;

public:
  NoteDuration(unsigned short v, bool d, bool d2, bool t);

  unsigned short getValue();
  bool getDot();
  bool getDot2();
  bool getTriplet();

  ~NoteDuration(){}
};

//e.g. 16..#a2t[Hello ]
class Note : public NoteDuration, public NotePitch  //multiple inheritance, +500 EXP
{
protected:
  std::string word;

public:
  Note(bool s,
       char n,
       short o,
       unsigned short v,
       bool d,
       bool d2,
       bool t,
       std::string w):
    NoteDuration(v, d, d2, t),
    NotePitch(s, n, o),
    word(w)
  {}

  std::string getWord();

  ~Note(){}
};

struct Rest : public Note
{
  Rest(unsigned short v,
       bool d,
       bool d2,
       bool t):
    Note(v, d, d2, t, false, '-', 0, 0)
  {}
  ~Rest(){}
};

//struct a class je v C++ skoro to same(struct ma vse public)
// e.g. 1+2+4.t+8..t+16..c
struct Ligature : public Note
{
  std::array<NoteDuration, 9> durations;
};

// e.g. 4<d #f a> (4..t+16.t<d #f1 a2>)
struct Chord : public Note
{
  //max 7 notes in 13th chord, first note of chord is chord itself(inherited from Note)
  std::array<NotePitch, 6> pitches;
};

// e.g. <8a 4#c 2e> (<8.a 4t#c1 2.+8e>)
struct DifferentDurationsChord : public Note
{
  std::array<Note, 6> notes;
};

struct Tempo
{
  unsigned short tempo;
  unsigned short position;  //after which note tempo begins
  double BPM;

  Tempo(){}
  ~Tempo(){}
};

struct ValuedTempo : public Tempo
{
  NoteDuration duration;          //1.part of 8.=280

  ValuedTempo(unsigned short v, bool d, bool d2, bool t);

  ~ValuedTempo(){}
};

#endif