#include <iostream> //std::cin, std::cout
#include <fstream>  //std::ifstream
#include <string>
#include <map>
#include <ao/ao.h>  //audio library

#include "melobeep.h"
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

  Melobeep m("claw.txt");

  m.output = new AudioOutput(2);
  m.output->write2output();
  delete m.output;
  m.output = new MidiOutput(4);
  m.output->write2output();
  delete m.output;
  m.output = new WavOutput(6);
  m.output->write2output(); 
  delete m.output;

  m.song = new Song();
  Note n(false, 'c', 1, 4, true, false, false, "ahoj");
  m.song->melody.push_back(n);
  cout << m.song->melody[0].getNote() << endl;

  return 0;
}