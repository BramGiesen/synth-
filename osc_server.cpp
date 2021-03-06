#include "osc_server.h"


LocalOSC::LocalOSC(FmSynth& fm) : fmSynthRef(fm)
{

}

LocalOSC::~LocalOSC()
{

}

//function that recieves the OSC messages
int LocalOSC::realcallback(const char *path,const char *types,lo_arg **argv,int argc)
{
string msgpath=path;

  if(!msgpath.compare("/MIDICC")){ // OSC to set FM settings of the Synth
     string paramname=(char *)argv[0];
     int modDepth = argv[1]->i;
     int modRatio = argv[2]->i;

     }
  if(!msgpath.compare("/noteOn")){ // OSC for midi pitch and velocity
     int channel = argv[0]->i;
     int pitch = argv[1]->i;
     int velocity = argv[2]->i;

     /* this part of the function prevents noteOffs beeing send after there is a new key pressed,
     so if you release a key a noteOff is send but if press a new key and then you release the old key there is no
     note of send to the ADSR because that would mute the new key even when it is pressed
     */
     if (pitch == midiSend && !first)
     {
     setMidiValue(pitch);
     setNoteOnOff(velocity);
     }
     else
     {
       if(velocity > 0){
       setMidiValue(pitch);
       }
     }

     if (first)
     {
       setMidiValue(pitch);
       setNoteOnOff(velocity);
       first =  false;
     }

     if (velocity == 0)
     {
       first = true;
     }

   }

  return 0;
} // realcallback()

// MIDI setters
void LocalOSC::setMidiValue(int newMidiValue)
{
  midiSend = newMidiValue;
}

void LocalOSC::setNoteOnOff(int newVelocity)
{
  velocity=newVelocity;
}

// MIDI getters
void LocalOSC::getMidiValue()
{
  fmSynthRef.setMidiPitch(midiSend);
  // return midiSend;
}

void LocalOSC::getNoteOnOff()
{
  gate = (velocity > 0) ? 1 : 0;
  fmSynthRef.setADSRgate(gate);
}

void LocalOSC::getMIDIinfo()
{
  while ( programRunning ) {
    getMidiValue();
    getNoteOnOff();
  }
}
