#include <cmath>    //for sharp computation

#include "notes.h"

using namespace std;


void NotePitch::setSharp() {
  sharp = true;
}
void NotePitch::setNote(char note) {
  this->note = note;
}
void NotePitch::setOctave(char octave) {
  this->octave = octave;
}
double NotePitch::getPitch() {
  return this->pitch;
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


void NoteDuration::setDuration(char duration) {
  this->duration = duration;
}
void NoteDuration::setDot() {
  dot = true;
}
void NoteDuration::setDot2() {
  dot2 = true;
}
void NoteDuration::setTriplet() {
  triplet = true;
}
double NoteDuration::computeDuration()
{
  return 100.0;
}


void Note::setWord(string word) {
  this->word = word;
  // strcpy(this->word, word);
}
string Note::getWord() {
  return this->word;
}

unsigned short Tempo::calculateBPM()
{
  
}
// 2=60   -> (4/2)*60=120
// 8=270  -> (4/8)*270=135
// 2.=40  -> (4/(2*(2/3)))*40=120