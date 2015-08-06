#include <iostream> //std::cin, std::cout
#include <fstream>  //std::ifstream
#include <string>
#include <map>
#include <ao/ao.h>  //audio library

#include "input.h"
#include "notes.h"
#include "output.h"

using namespace std;

int main(int argc, char **argv)
{
  // MNoteFreq freqTable;
  // MNoteFreq::const_iterator n;
  // for (n = freqTable.begin(); n != freqTable.end(); ++n) {
  // cout << n->first
  //        << ':'
  //        << n->second
  //        << " Hz"
  //        << endl;
  // }

  // Tempo t;
  // cout << t.value.value << endl
  //      << t.value.duration << endl
  //      << t.value.tempo << endl;

  // string data;
  // ETokenType token;
  // ifstream sourceFile(argv[1]);

  // while(token != END_OF_FILE) {
  //   token = getNextToken(sourceFile, data);
  //   cout << printTokenType(token) << ", " << data << endl;
  // } 

  // sourceFile.close();

  // m.output = new AudioOutput(2);
  // m.output->write2output();
  // delete m.output;
  // m.output = new MidiOutput(4);
  // m.output->write2output();
  // delete m.output;
  // m.output = new WavOutput(6);
  // m.output->write2output(); 
  // delete m.output;

  Song song;
  Note n(4, true, false, false, false, 'c', 1, "ahoj");
  song.melody.push_back(n);
  cout << song.melody[0].getNote() << endl;

  Rest r(8, true, false, false);  
  song.melody.push_back(r);
  cout << song.melody[1].getNote() << endl;

  Ligature l(16, true, false, false, false, 'c', 1, "ahoj");
  song.melody.push_back(l);
  cout << song.melody[2].getValue() << endl;
  // NoteDuration nd1(16, 0, 0, 0);
  // NoteDuration nd2(32, 1, 0, 1);
  // NoteDuration nd3(8, 1, 1, 0);
  // NoteDuration nd4(1, 1, 1, 1);
  // song.melody[2].durations.push_back(nd1);
  // song.melody[2].durations.push_back(nd2);
  // song.melody[2].durations.push_back(nd3);
  // song.melody[2].durations.push_back(nd4);

  //nechce to najit durations, asi proto, ze pole melody je typu Note, a ten v sobe durations nema
  // mozne reseni: vytvorit jeste jeden nadrazeny typ, napr. MelodyObject
  // tohoto typu bude pole melody a od tohoto typu budou dedit vsechny noty

  return 0;
}