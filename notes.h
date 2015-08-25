#ifndef NOTES_H
#define NOTES_H

#include <string>
#include <vector>
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

public:
  NotePitch();  //kvuli inicializaci pole pitches v Chord
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
  NoteDuration();   //kvuli inicializaci pole durations v Ligature
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
  Note():   //kvuli inicializaci pole notes v DifferentDurationsChord
    NoteDuration(),
    NotePitch()
  {}
  Note(unsigned short v,
       bool d,
       bool d2,
       bool t,
       bool s,
       char n,
       short o,
       std::string w):
    NoteDuration(v, d, d2, t),
    NotePitch(s, n, o),
    word(w)
  {}

  std::array<NoteDuration*, 9> ligature;
  std::array<NotePitch*, 6> chord;
  std::array<Note*, 6> ddchord;  //different durations chord

  std::string getWord();

  ~Note(){}
};

struct Rest : public Note
{
  Rest(unsigned short v,
       bool d,
       bool d2,
       bool t):
    Note(v, d, d2, t, false, '-', -2, "")
  {}
  ~Rest(){}
};

struct Tempo
{
  double BPM;               //beats per minute
  unsigned short position;  //after which note tempo begins

  Tempo(){}
  ~Tempo(){}
};

//8..t=280
struct ValuedTempo : public Tempo
{
  unsigned short tempo;           //280
  NoteDuration duration;          //8..t

  ValuedTempo(){}
  ValuedTempo(unsigned short v, bool d, bool d2, bool t);

  ~ValuedTempo(){}
};

class Song
{
// private:
public:
  std::vector<Note> melody;
  std::vector<Tempo> tempos;
public:
  Song(){}
  ~Song(){}
};

#endif