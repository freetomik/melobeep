#ifndef OUTPUT_H
#define OUTPUT_H

class Output
{
public:
  Output();
  virtual ~Output();
  virtual void write2output() = 0;
  // void write2output();
};

class MidiOutput : public Output
{
  int channels;
public:
  MidiOutput(int c):
    channels(c)
  {}
  ~MidiOutput(){}
  void write2output();
};

class WavOutput : public Output
{
  int channels;
public:
  WavOutput(int c):
    channels(c)
  {}
  ~WavOutput(){}
  void write2output();
};

class AudioOutput : public Output
{
  int channels;
public:
  AudioOutput(int c):
    channels(c)
  {}
  ~AudioOutput(){}
  void write2output();
};

#endif