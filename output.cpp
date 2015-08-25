#include <iostream>

#include "output.h"

using namespace std;

Output::Output(){}
Output::~Output(){}
void Output::write2output() {
  cout << "Output" << endl;
}
void MidiOutput::write2output() {
  cout << "MidiOutput, channels: " << channels << endl;
}
void WavOutput::write2output() {
  cout << "WavOutput, channels: " << channels << endl;
}
void AudioOutput::write2output() {
  cout << "AudioOutput, channels: " << channels << endl;
}