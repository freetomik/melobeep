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

  // initialization list, +200 EXP
  NotePitch(bool s,
            char n,
            short o):
    sharp(s),
    note(n),
    octave(o)
  {}

  bool getSharp();
  char getNote();
  short getOctave();
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
  NoteDuration(unsigned short v,
               bool d,
               bool d2,
               bool t):
    value(v),
    dot(d),
    dot2(d2),
    triplet(t),
    duration(500),
    tempo(120)
  {}

  unsigned short getValue();
  bool getDot();
  bool getDot2();
  bool getTriplet();

  double computeDuration();

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

struct Rest : Note
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
// e.g. 1c+2+4.+8..+16..t
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

struct Tempo
{
  unsigned short tempo;           //2.part of 8.=280
  unsigned short position;  //after which note tempo begins
  unsigned short BPM;

  Tempo(){}
  ~Tempo(){}
};

struct ValuedTempo : Tempo
{
  NoteDuration duration;          //1.part of 8.=280

  ValuedTempo(unsigned short v,
              bool d,
              bool d2,
              bool t):
    duration(v, d, d2, t)
  {}

  unsigned short calculateBPM();

  ~ValuedTempo(){}
};

#endif