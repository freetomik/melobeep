#include <cmath>    //for sharp computation

#include "notes.h"

using namespace std;

NotePitch::NotePitch(bool s, char n, short o)
//initialization list:
  : sharp(s),
    note(n),
    octave(o)
//constructor body:
{
  pitch = freqTable.at(note);
  if(octave == -1) pitch /= 2;
  else if(octave == 1) pitch *= 2;
  else if(octave == 2) pitch *= 4;
  else if(octave == 3) pitch *= 8;
  if(sharp) pitch *= pow(2, 1/12.0);
}

bool NotePitch::getSharp() {
  return sharp;
}
char NotePitch::getNote() {
  return note;
}
short NotePitch::getOctave() {
  return octave;
}
double NotePitch::getPitch() {
  return pitch;
}

NoteDuration::NoteDuration(unsigned short v, bool d, bool d2, bool t)
  : value(v),
    dot(d),
    dot2(d2),
    triplet(t),
    duration(500),
    tempo(120)
{
  //compute duration
}

unsigned short NoteDuration::getValue() {
  return value;
}
bool NoteDuration::getDot() {
  return dot;
}
bool NoteDuration::getDot2() {
  return dot2;
}
bool NoteDuration::getTriplet() {
  return triplet;
}


string Note::getWord() {
  return this->word;
}

// http://mp3.deepsound.net/eng/samples_calculs.php
// 2=60   -> (4/2)*60=120
// 8=270  -> (4/8)*270=135
// 2.=40  -> (4/(2*(2/3)))*40=120
ValuedTempo::ValuedTempo(unsigned short v, bool d, bool d2, bool t)
  : duration(v, d, d2, t)
{
  double value = 4, ratio = 1;
  if(duration.getDot()) {
    value = duration.getValue() * 3/2;
    if(duration.getDot2())
      value = duration.getValue() * 7/4;
  }
  else  //dot a triplet se svymi ucinky vzajemne rusi
    if(duration.getTriplet())
      value *= 2/3;
  ratio = 4 / value;
  BPM = ratio * tempo;
}