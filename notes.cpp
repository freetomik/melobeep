#include <cmath>    //for sharp computation

#include "notes.h"

using namespace std;


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
void NotePitch::computePitch()
{
  pitch = freqTable.at(note);
  if(octave == -1) pitch /= 2;
  else if(octave == 1) pitch *= 2;
  else if(octave == 2) pitch *= 4;
  else if(octave == 3) pitch *= 8;
  if(sharp) pitch *= pow(2, 1/12.0);
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
double NoteDuration::computeDuration()
{
  return 100.0;
}


string Note::getWord() {
  return this->word;
}

// http://mp3.deepsound.net/eng/samples_calculs.php
// 2=60   -> (4/2)*60=120
// 8=270  -> (4/8)*270=135
// 2.=40  -> (4/(2*(2/3)))*40=120
unsigned short ValuedTempo::calculateBPM()
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
  return ratio * tempo;
}