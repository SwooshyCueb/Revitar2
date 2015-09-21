/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Copyright (C) 2004 C. Lawrence Zitnick III <larryz@microsoft.com>
 * Copyright (C) 2010 Hermann Seib <him@hermannseib.com>
 * Copyright (C) 2014 Asseca <http://www.asseca.org/revitar.html>
 * Copyright (C) 2015 Markus Kitsinger (SwooshyCueb) <root@swooshalicio.us>
 */

#ifndef __REVITAR_H__
#define __REVITAR_H__


#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "pluginterfaces/vst2.x/vstfxstore.h"
#include "vstgui.sf/vstgui/vstgui.h"

#include <string.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>


//#define		TUNE_NOW

enum
{
	kGain,
	kBodyGain,
	kPickVolume,
	kTuning,                        /* tuning (0=-100,0.5=0,1=100 cents) */
	kBridgeDamping,
	kStringDamping,
	kVibratoAmplit,
	kVibratoRate,
	kSympathetic,
	kSlap,
	kPickSpeed,
	kChordRate,
	kStringType,
	kPalmDamp,
	kSlideRate,

	kChordOnOff,
	kSustain,
	kMono,
	kChord,
	kChordAbs,
	kChordSwitch,
	kBodySwitch,
	kPickNoise,                         /* was kPickSwitch                   */
    kReserved1,                         /* was kDS0                          */
	kChordNote0,
	kChordNote1,
	kChordNote2,
	kChordNote3,
	kChordNote4,
	kChordNote5,
	kPickPos,
	kPickupPos,
	kPickWidth,
	kPickStiffness,
	kReserved2,                         /* was kGuitarBottom                 */
	kReserved3,                         /* was kGuitarTop                    */
	kPalmPos,
	kAbsRel,
	kStopSwitch,

    kChordNotes,
    kChordAbsNotes = kChordNotes + NUM_CHORDS * NUM_CHORD_NOTES,
	kNumParams = kChordAbsNotes + NUM_CHORDS * NUM_CHORD_NOTES,
	
	kMeter,
	kAbout,
    kPickSwitch,
	kDS0,
    kGuitarBottom,
    kGuitarTop,

	kNumPrograms = 48,
	kNumOutputs = 2,
	kNumFrequencies = 128,	// 128 midi notes

    kVersion = 2001,                    /* current version (2.001)           */

};


class Revitar;

class RevitarProgram
{
friend class Revitar;
public:
	RevitarProgram();
	~RevitarProgram() {}
	void setChord(float chordNote[NUM_CHORDS][NUM_CHORD_NOTES]);
	void setAbsChord(float chordNote[NUM_CHORDS][NUM_CHORD_NOTES]);

private:	
	float fKnob[NUM_KNOBS];
	float fButton;
	float fMono;
	float fChordOnOff;
	float fSustain;
        float fPickNoise;
	float fChordSwitch;
	float fBodySwitch;
	float fChordNote[NUM_CHORDS][NUM_CHORD_NOTES];
	float fChordAbsNote[NUM_CHORDS][NUM_CHORD_NOTES];
	float fPickupPos;
	float fPickPos;
	float fGLove;
	float fMeter;
	float fPickWidth;
	float fPickStiffness;
	float fChordIdx;
	float fChordAbsIdx;
	float fPalmSlider;
	float fAbsRel;
	float fStopSwitch;
	char name[128];
};

class Revitar : public AudioEffectX
{
public:
	Revitar(audioMasterCallback audioMaster);
	~Revitar();

	virtual void process(float **inputs, float **outputs, VstInt32 sampleframes);
	virtual void processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames);
	virtual VstInt32 processEvents(VstEvents* events);
	virtual void setProgram(VstInt32 program);
	virtual void setProgramName(char *name);
	virtual void getProgramName(char *name);
	virtual void setParameter(VstInt32 index, float value);
	virtual float getParameter(VstInt32 index);
	virtual void getParameterLabel(VstInt32 index, char *label);
	virtual void getParameterDisplay(VstInt32 index, char *text);
	virtual void getParameterName(VstInt32 index, char *text);
	virtual bool canParameterBeAutomated (VstInt32 index);
	virtual bool getParameterProperties (VstInt32 index, VstParameterProperties* p);
	virtual float DECLARE_VST_DEPRECATED (getVu) ();
	virtual void suspend();
	virtual void resume();
	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion () {return 1;}
	virtual VstInt32 canDo (char* text);
	virtual bool getOutputProperties (VstInt32 index, VstPinProperties* properties);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
	virtual bool DECLARE_VST_DEPRECATED (copyProgram) (VstInt32 destination);
	virtual void setSampleRate(float sampleRate);
	virtual void setBlockSize(VstInt32 blockSize);

	void handleMIDIData();
    void handleMIDINoteOn(int channel, int note, int velocity);
    void handleMIDINoteOff(int channel, int note, int velocity);
    void handleController(int channel, int ctrl, int value);
    void handleProgramChange(int channel, int value);
    void handlePitchWheel(int channel, int value);
	void setResonances();

	float m_ChordDisplayUpdate;
	float m_ChordDisplay[MAX_NUM_POLY][NUM_POINTS][CHORD_DISPLAY_HEIGHT];

	void setDisplayPickParameters(float pickWidth, float pickStiffness);
    void setDisplayChordNote(int nNote, int nFret);
	float computeBodyResonance(float resPickUp);
	void updateNewNotes(int polyIdx);
	void turnOffNotes(int polyIdx);
	void turnOffNote(int polyIdx);
	void doPluck(int polyIdx);
	void saveMIDIControls();

    bool LoadBank(void *data, size_t len);
    bool LoadBank(char *name);
    bool SaveBank(char *name);

    short GetTune(int polyIdx) { return m_Tune[polyIdx]; }

//	FILE *m_OutFile;
	int m_MIDIControl[NUM_MIDI_CC];
    unsigned char m_MIDICtrlVal[128];
	int m_LastMIDICC;
        



private:
	void initProcess();
	void noteOn(long note, long velocity, long delta, int polyIdx, int hammer);
	float computeRate(int polyIdx, long note, float tune);
	void turnOff(int delta);
	void setPresets();
	void refreshNotes();

	float m_ObjectPos[MAX_NUM_POLY][NUM_POINTS];
	float m_ObjectVel[MAX_NUM_POLY][NUM_POINTS];

	RevitarProgram *programs;           /* list of programs                  */
    RevitarProgram prog;                /* current program                   */

    float fKnobLast[NUM_KNOBS];
	float vu;
	int m_VuDisplayed;
	long currentNote;

	int m_MIDIData[MAX_MIDI_DATA][4];
	int m_NumMIDIData;

	float m_SampleRate;
    float m_SampleRateMult;
	int m_NumNoteOffs[kNumFrequencies];
	int m_NoteOn[MAX_NUM_POLY];
	int m_NoteTurningOff[MAX_NUM_POLY];
	int m_NoteOffFade[MAX_NUM_POLY];
	int m_TypeOffset[MAX_NUM_POLY];
	int m_TypeOffsetPull[MAX_NUM_POLY];
	float m_Rate[MAX_NUM_POLY];
	float m_DefaultRate[MAX_NUM_POLY];
	float m_RateReal[MAX_NUM_POLY];
	int m_PullCt[MAX_NUM_POLY][NUM_POINTS][2];
	int m_ComputeCt[MAX_NUM_POLY];
	float m_Random[NUM_SAMPLES];
	int m_RandomCt;
	float m_PickNoise;
	float m_PickNoiseRate;
	float m_PickNoiseWave[15];
	float m_PickAmp;
	float m_Velocity[MAX_NUM_POLY];
	float m_MIDIVelocity[MAX_NUM_POLY];
	float m_Pitch[MAX_NUM_POLY];
	float m_PitchUp[MAX_NUM_POLY];
	float m_PitchDown[MAX_NUM_POLY];
	float m_DampBridge[MAX_NUM_POLY];
	float m_DampString;
	float m_DampPalm[MAX_NUM_POLY];
	float m_PickUpDiff[MAX_NUM_POLY][2];
	float m_Left[MAX_NUM_POLY];
	float m_Right[MAX_NUM_POLY];
	float m_LeftDel[MAX_NUM_POLY];
	float m_RightDel[MAX_NUM_POLY];
	float m_LeftCt[MAX_NUM_POLY];
	float m_RightCt[MAX_NUM_POLY];
	float m_FreqRate[NUM_POINTS];
	float m_PickStiff[NUM_POINTS];
	float m_CurrentBendRate;
	float m_CurrentPan;
	float m_CurrentBendDest;
	float m_StartBend[MAX_NUM_POLY];
	float m_Vibrato[MAX_NUM_POLY];
	bool m_SlideFinished[MAX_NUM_POLY];
	int m_Down;
	int m_NoteQueue[MAX_NUM_POLY][MAX_NOTE_QUEUE][4];
	int m_NoteCt[MAX_NUM_POLY];

	int m_Pluck[MAX_NUM_POLY];
	int m_PickUp[MAX_NUM_POLY];
	long m_Note[MAX_NUM_POLY];

	float m_PickPos[MAX_NUM_POLY];
	float m_PickAdd[MAX_NUM_POLY];
	
	int m_NumPoints[MAX_NUM_POLY];
	float freqtab[kNumFrequencies];

	int m_SampleCt;

	int m_LoveCt;
	int m_LoveDt;
	int m_LoveDs;


#ifdef	TUNE_NOW
	int m_ZeroCt;
	int m_ZeroTotal;
	int m_ZeroCt2;
	float m_LastZero;
	int m_ZeroCtGood;
	int m_TestsDone;
#endif

	int m_GLove;
	int m_GCount;

	float m_VibratoAmp;
	float m_VibratoAdd;
	float m_VibratoPos;
	int   m_VibratoCt;
	float m_VibratoCtAll;
	int m_VibratoUp;

	float resonDrag[NUM_BODIES];
	float resonPos[NUM_RESON];
	float resonVel[NUM_RESON];
	float resonRate[NUM_BODIES][NUM_RESON];
	float resonVol[NUM_BODIES][NUM_RESON];

	float m_Slap;

	int m_CurrentChordNote;
	int m_FirstChordNote;
	int m_FirstChordNotes[MAX_NUM_POLY];
	int m_CurrentChordNotes[MAX_NUM_POLY];

	int m_MonoNote;
	int m_Pick[MAX_NUM_POLY];
	int m_NumPoly;

	int m_BodyIdx;
	int m_LastBodyIdx;
	int m_BodyCt;
	float m_BodyDel;
	float m_LastResOut;
	bool m_ForceOff;
	bool m_ShutDown;
    bool m_InSetProgram;

	int m_SlideNoteCt[MAX_NUM_POLY];
	int m_CurrentSlideNoteCt[MAX_NUM_POLY];
	int m_SlideNote[MAX_NUM_POLY][MAX_NUM_NOTES];
	int m_SlideLength[MAX_NUM_POLY][MAX_NUM_NOTES];
	int m_MIDINote[MAX_NUM_POLY];

    short m_DefaultTune[MAX_NUM_POLY];  /* open string tuning; defaults to   */
    short m_Tune[MAX_NUM_POLY];         /* the MIDI notes for EADGBE         */
                                        /* (40,45,50,55,59,64)               */

	float m_CurrentBPM;
    float m_BendRange;                  /* pitch bend range in cents         */
    int   m_ControllerBits;             /* 7 for 7-bit controllers, or       */
                                        /* 14 for 14-bit controllers         */

};

#endif // __REVITAR_H__
