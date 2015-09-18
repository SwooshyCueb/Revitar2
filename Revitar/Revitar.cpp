/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Copyright (C) 2004 C. Lawrence Zitnick III <larryz@microsoft.com>
 * Copyright (C) 2010 Hermann Seib <him@hermannseib.com>
 * Copyright (C) 2015 Markus Kitsinger (SwooshyCueb) <root@swooshalicio.us>
 */

#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <windows.h>
#include <winreg.h>

#include "public.sdk/source/vst2.x/aeffeditor.h"


#include "RevEditor.h"
#include "Revitar.h"

//FILE *m_OutFile;

bool oome = false;

/*****************************************************************************/
/* createEffectInstance : creates an effect instance                         */

/*****************************************************************************/

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
    // Create the AudioEffect
    AudioEffect* effect = new Revitar(audioMaster);
    if (effect && oome) {
        delete effect;
        effect = NULL;
    }

    return effect;
}


/*===========================================================================*/
/* Revitar class members                                                     */
/*===========================================================================*/

/*****************************************************************************/
/* Revitar : constructor                                                     */

/*****************************************************************************/

Revitar::Revitar(audioMasterCallback audioMaster)
: AudioEffectX(audioMaster, kNumPrograms, kNumParams) {
    //  m_OutFile = NULL;
    //  m_OutFile = fopen("outTune.txt", "w");

    setUniqueID('cmr2');
    cEffect.version = kVersion;
    setNumInputs(0); // no inputs
    setNumOutputs(kNumOutputs); // 2 outputs
    canProcessReplacing();
    DECLARE_VST_DEPRECATED(hasVu) ();
    DECLARE_VST_DEPRECATED(hasClip) (false);
    isSynth();

    programs = new RevitarProgram[kNumPrograms];

    if (programs)
        setPresets();
    else
        oome = true;
    if (audioMaster) {
        editor = new RevEditor(this);
        if (!editor)
            oome = true;
    }

    /*---------------------------------------------------------------------------*/
    /* Create/read registry values                                               */
    /*---------------------------------------------------------------------------*/
    int i;
    DWORD dwDummy;
    DWORD sizeWord;
    HKEY reg;
    HKEY regMIDI;

    reg = NULL;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\CutterMusic\\Revo2", 0, "REG_DWORD",
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &reg, &dwDummy);

#if WITH_REGISTRATION
    int c, m;
    char panum[1024];
    if (dwDummy == REG_CREATED_NEW_KEY) {
        sprintf(panum, "");
        RegSetValueEx(reg, "regNum", 0, REG_SZ, (LPBYTE) panum, sizeof (panum));
    } else {
        if (RegQueryValueEx(reg, "regNum", 0, &dwDummy, NULL, &sizeWord) == ERROR_SUCCESS)
            RegQueryValueEx(reg, "regNum", 0, &dwDummy, (LPBYTE) & panum, &sizeWord);

        for (c = 0, i = 7; i >= 0; i--) {
            c += panum[i] - 48;
            c *= 10;
        }
        c /= 10;

        m = panum[8] - 48;
        m += 10 * (panum[9] - 48);

        m_GLove = 0;

        if (c % 57 == m)
            m_GLove = 1;

        if ((panum[7] - 48) != 6)
            m_GLove = 0;

        if ((panum[6] - 48) != 9)
            m_GLove = 0;
    }

    RegCloseKey(reg);

    if (!GLoveGood()) {
        cEffect.numParams = 0;
        numParams = 0;
    }
#else
    // Lets remove the need to register.  It's free now!
    m_GLove = 1;
#endif

    m_BendRange = DEFAULT_BEND_CENTS; /* set up default pitch bend range   */
    DWORD dwPbCents = (DWORD) m_BendRange;
    sizeWord = sizeof (DWORD);
    if (RegQueryValueEx(reg, "PitchBendCents", 0, &dwDummy, (LPBYTE) & dwPbCents, &sizeWord) == ERROR_SUCCESS) {
        if (dwPbCents < 1)
            dwPbCents = 1;
        else if (dwPbCents > 1200) /* 1 octave maximum                       */
            dwPbCents = 1200;
        m_BendRange = (float) dwPbCents;
    } else
        RegSetValueEx(reg, "PitchBendCents", 0, REG_DWORD, (LPBYTE) & dwPbCents, sizeof (DWORD));

    m_ControllerBits = 7;
    DWORD dwBits = 7;
    sizeWord = sizeof (DWORD);
    if (RegQueryValueEx(reg, "ControllerBits", 0, &dwDummy, (LPBYTE) & dwBits, &sizeWord) == ERROR_SUCCESS) {
        if (dwBits <= 7)
            m_ControllerBits = 7;
        else if (dwBits <= 14)
            m_ControllerBits = 14;
    } else
        RegSetValueEx(reg, "ControllerBits", 0, REG_DWORD, (LPBYTE) & dwBits, sizeof (DWORD));

    regMIDI = NULL;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\CutterMusic\\Revo2\\MIDI", 0, "REG_DWORD",
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &regMIDI, &dwDummy);

    if (dwDummy == REG_CREATED_NEW_KEY) {
        m_MIDIControl[ 0] = NO_INFORMATION;
        m_MIDIControl[ 1] = kKnobPalmDamp;
        m_MIDIControl[ 2] = kKnobSlideRate;
        m_MIDIControl[ 3] = kKnobVibratoRate;
        m_MIDIControl[ 4] = kKnobVibratoAmplit;
        m_MIDIControl[ 5] = kPickPosition;
        m_MIDIControl[ 6] = kPickUp;
        m_MIDIControl[ 7] = kKnobBridgeDamping;
        m_MIDIControl[ 8] = kKnobStringDamping;
        m_MIDIControl[ 9] = kKnobSlap;
        m_MIDIControl[10] = NO_INFORMATION;
        m_MIDIControl[11] = kKnobPickSpeed;
        m_MIDIControl[12] = kKnobStringType;
        m_MIDIControl[13] = kKnobTuning;
        m_MIDIControl[14] = kKnobBodyGain;
        m_MIDIControl[15] = kKnobPickVolume;
        m_MIDIControl[16] = kKnobGain;
        m_MIDIControl[17] = kKnobChordRate;
        m_MIDIControl[18] = kPalmSlider;
        m_MIDIControl[19] = kPickStiffness;
        m_MIDIControl[20] = kPickWidth;
        m_MIDIControl[21] = kKnobSympathetic;
        m_MIDIControl[22] = kBodySwitch;
        for (i = 23; i < NUM_MIDI_CC; i++)
            m_MIDIControl[i] = NO_INFORMATION;

        RegSetValueEx(regMIDI, "MIDICC", 0, REG_SZ, (LPBYTE) m_MIDIControl, NUM_MIDI_CC * sizeof (int));
    } else {
        if (RegQueryValueEx(regMIDI, "MIDICC", 0, &dwDummy, NULL, &sizeWord) == ERROR_SUCCESS)
            RegQueryValueEx(regMIDI, "MIDICC", 0, &dwDummy, (LPBYTE) & m_MIDIControl, &sizeWord);
    }

    RegCloseKey(regMIDI);

    initProcess();
    suspend();
}

/*****************************************************************************/
/* ~Revitar : destructor                                                     */

/*****************************************************************************/

Revitar::~Revitar() {
    if (programs)
        delete[] programs;
}

/*****************************************************************************/
/* saveMIDIControls : saves the current set of MIDI controllers              */

/*****************************************************************************/

void Revitar::saveMIDIControls() {
    DWORD dwDummy;
    HKEY regMIDI;

    regMIDI = NULL;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\CutterMusic\\Revo2\\MIDI", 0, "REG_DWORD",
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &regMIDI, &dwDummy);

    if (regMIDI != NULL) {
        RegSetValueEx(regMIDI, "MIDICC", 0, REG_SZ, (LPBYTE) m_MIDIControl, NUM_MIDI_CC * sizeof (int));

        RegCloseKey(regMIDI);
    }

} // saveMIDIControls

/*****************************************************************************/
/* resume : called from host when the PlugIn is put to work                  */

/*****************************************************************************/

void Revitar::resume() {
    DECLARE_VST_DEPRECATED(wantEvents) ();
}

/*****************************************************************************/
/* setResonances : sets the resonances (need a better comment :-)            */

/*****************************************************************************/

void Revitar::setResonances() {
    int i, j;
    float sum;

    for (i = 0; i < NUM_RESON; i++) {
        resonPos[i] = 0.0f;
        resonVel[i] = 0.0f;
    }

    for (j = 0; j < NUM_BODIES; j++) {
        resonDrag[j] = 1.0f;
    }

    for (j = 0; j < NUM_BODIES; j += 2) {
        // Acoustic 1
        resonRate[j][0] = 77.0f;
        resonRate[j][1] = 117.0f;
        resonRate[j][2] = 126.0f;
        resonRate[j][3] = 137.0f;
        resonRate[j][4] = 148.0f;
        resonRate[j][5] = 190.0f;
        resonRate[j][6] = 221.0f;
        resonRate[j][7] = 285.0f;
        resonRate[j][8] = 314.0f;
        resonRate[j][9] = 372.0f;
        resonRate[j][10] = 375.0f;
        resonRate[j][11] = 382.0f;
        resonRate[j][12] = 406.0f;
        resonRate[j][13] = 438.0f;
        resonRate[j][14] = 481.0f;
        resonRate[j][15] = 511.0f;
        resonRate[j][16] = 527.0f;
        resonRate[j][17] = 530.0f;
        resonRate[j][18] = 609.0f;
        resonRate[j][19] = 618.0f;
    }

    for (j = 1; j < NUM_BODIES; j += 2) {
        // Acoustic 2
        resonRate[j][0] = 59.0f;
        resonRate[j][1] = 103.0f;
        resonRate[j][2] = 188.0f;
        resonRate[j][3] = 202.0f;
        resonRate[j][4] = 223.0f;
        resonRate[j][5] = 231.0f;
        resonRate[j][6] = 262.0f;
        resonRate[j][7] = 315.0f; // end of the important modes
        resonRate[j][8] = 385.0f;
        resonRate[j][9] = 481.0f;
        resonRate[j][10] = 749.0f;
        resonRate[j][11] = 372.0f;
        resonRate[j][12] = 416.0f;
        resonRate[j][13] = 448.0f;
        resonRate[j][14] = 471.0f;
        resonRate[j][15] = 501.0f;
        resonRate[j][16] = 517.0f;
        resonRate[j][17] = 520.0f;
        resonRate[j][18] = 619.0f;
        resonRate[j][19] = 628.0f;
    }

    for (i = 0; i < NUM_RESON; i++) {
        resonRate[0][i] *= 1.1f;
        resonRate[1][i] *= 1.4f;
        resonRate[2][i] *= 1.35f;
        resonRate[3][i] *= 1.45f;
        resonRate[4][i] *= 1.7f;
        resonRate[5][i] *= 1.7f;
    }

    resonDrag[0] *= 1.1f;
    resonDrag[1] *= 1.4f;
    resonDrag[2] *= 3.4f;
    resonDrag[3] *= 4.5f;
    resonDrag[4] *= 9.8f;
    resonDrag[5] *= 33.2f;

    /*
    // Electric reg
    resonRate[0] = 55.0;
    resonRate[1] = 160.0;
    resonRate[2] = 372.0;
    resonRate[3] = 472.0;

    // Electric irreg
    resonRate[0] = 46.0;
    resonRate[1] = 132.0;
    resonRate[2] = 221.0;
  
    // Acoustic 2
    resonRate[0] = 59.0;
    resonRate[1] = 103.0;
    resonRate[2] = 188.0;
    resonRate[3] = 202.0;
    resonRate[4] = 223.0;
    resonRate[5] = 231.0;
    resonRate[6] = 262.0;
    resonRate[7] = 315.0;  // end of the important modes
    resonRate[8] = 385.0;
    resonRate[9] = 481.0;
    resonRate[10] = 749.0;
     */

    resonVol[0][0] = 1.0f;
    resonVol[0][1] = 0.9f;
    resonVol[0][2] = 0.06f;
    resonVol[0][3] = 0.7f;
    resonVol[0][4] = 0.06f;
    resonVol[0][5] = 0.4f;
    resonVol[0][6] = 0.8f;
    resonVol[0][7] = 0.9f;
    resonVol[0][8] = 0.02f;
    resonVol[0][9] = 0.02f;
    resonVol[0][10] = 0.04f;
    resonVol[0][11] = 0.03f;
    resonVol[0][12] = 0.02f;
    resonVol[0][13] = 0.01f;
    resonVol[0][14] = 0.01f;
    resonVol[0][15] = 0.005f;
    resonVol[0][16] = 0.003f;
    resonVol[0][17] = 0.001f;
    resonVol[0][18] = 0.002f;
    resonVol[0][19] = 0.003f;

    i = 4;
    resonVol[i][0] = 0.8f;
    resonVol[i][1] = 0.9f;
    resonVol[i][2] = 0.1f;
    resonVol[i][3] = 0.1f;
    resonVol[i][4] = 0.8f;
    resonVol[i][5] = 0.7f;
    resonVol[i][6] = 0.8f;
    resonVol[i][7] = 0.6f;
    resonVol[i][8] = 0.8f;
    resonVol[i][9] = 0.2f;
    resonVol[i][10] = 0.4f;
    resonVol[i][11] = 0.8f;
    resonVol[i][12] = 0.2f;
    resonVol[i][13] = 0.7f;
    resonVol[i][14] = 0.3f;
    resonVol[i][15] = 0.05f;
    resonVol[i][16] = 0.3f;
    resonVol[i][17] = 0.1f;
    resonVol[i][18] = 0.2f;
    resonVol[i][19] = 0.1f;

    i = 2;
    resonVol[i][0] = 0.3f;
    resonVol[i][1] = 0.4f;
    resonVol[i][2] = 0.1f;
    resonVol[i][3] = 0.4f;
    resonVol[i][4] = 0.2f;
    resonVol[i][5] = 0.3f;
    resonVol[i][6] = 0.6f;
    resonVol[i][7] = 0.6f;
    resonVol[i][8] = 0.8f;
    resonVol[i][9] = 0.8f;
    resonVol[i][10] = 0.0f;
    resonVol[i][11] = 0.0f;
    resonVol[i][12] = 0.1f;
    resonVol[i][13] = 0.0f;
    resonVol[i][14] = 0.1f;
    resonVol[i][15] = 0.0f;
    resonVol[i][16] = 0.0f;
    resonVol[i][17] = 0.1f;
    resonVol[i][18] = 0.2f;
    resonVol[i][19] = 0.1f;

    i = 3;
    resonVol[i][0] = 0.1f;
    resonVol[i][1] = 0.01f;
    resonVol[i][2] = 0.2f;
    resonVol[i][3] = 0.8f;
    resonVol[i][4] = 0.9f;
    resonVol[i][5] = 0.2f;
    resonVol[i][6] = 0.09f;
    resonVol[i][7] = 0.4f;
    resonVol[i][8] = 0.08f;
    resonVol[i][9] = 0.8f;
    resonVol[i][10] = 0.04f;
    resonVol[i][11] = 0.1f;
    resonVol[i][12] = 0.08f;
    resonVol[i][13] = 0.17f;
    resonVol[i][14] = 0.03f;
    resonVol[i][15] = 0.05f;
    resonVol[i][16] = 0.03f;
    resonVol[i][17] = 0.04f;
    resonVol[i][18] = 0.02f;
    resonVol[i][19] = 0.01f;

    i = 1;
    resonVol[i][0] = 0.1f;
    resonVol[i][1] = 0.9f;
    resonVol[i][2] = 0.1f;
    resonVol[i][3] = 0.1f;
    resonVol[i][4] = 0.3f;
    resonVol[i][5] = 0.7f;
    resonVol[i][6] = 0.8f;
    resonVol[i][7] = 0.1f;
    resonVol[i][8] = 0.08f;
    resonVol[i][9] = 0.09f;
    resonVol[i][10] = 0.04f;
    resonVol[i][11] = 0.03f;
    resonVol[i][12] = 0.008f;
    resonVol[i][13] = 0.07f;
    resonVol[i][14] = 0.005f;
    resonVol[i][15] = 0.05f;
    resonVol[i][16] = 0.003f;
    resonVol[i][17] = 0.01f;
    resonVol[i][18] = 0.002f;
    resonVol[i][19] = 0.001f;

    i = 5;
    resonVol[i][0] = 0.3f;
    resonVol[i][1] = 0.2f;
    resonVol[i][2] = 0.1f;
    resonVol[i][3] = 0.4f;
    resonVol[i][4] = 0.2f;
    resonVol[i][5] = 0.7f;
    resonVol[i][6] = 0.6f;
    resonVol[i][7] = 0.6f;
    resonVol[i][8] = 0.8f;
    resonVol[i][9] = 0.2f;
    resonVol[i][10] = 0.4f;
    resonVol[i][11] = 0.3f;
    resonVol[i][12] = 0.1f;
    resonVol[i][13] = 0.7f;
    resonVol[i][14] = 0.1f;
    resonVol[i][15] = 0.05f;
    resonVol[i][16] = 0.3f;
    resonVol[i][17] = 0.1f;
    resonVol[i][18] = 0.2f;
    resonVol[i][19] = 0.1f;
    /*
    i = 6;
    resonVol[i][0] = 0.7;
    resonVol[i][1] = 0.2;
    resonVol[i][2] = 0.9;
    resonVol[i][3] = 0.2;
    resonVol[i][4] = 0.4;
    resonVol[i][5] = 0.7;
    resonVol[i][6] = 0.1;
    resonVol[i][7] = 0.1;
    resonVol[i][8] = 0.8;
    resonVol[i][9] = 0.2;
    resonVol[i][10] = 0.2;
    resonVol[i][11] = 0.1;
    resonVol[i][12] = 0.1;
    resonVol[i][13] = 0.07;
    resonVol[i][14] = 0.09;
    resonVol[i][15] = 0.05;
    resonVol[i][16] = 0.03;
    resonVol[i][17] = 0.01;
    resonVol[i][18] = 0.02;
    resonVol[i][19] = 0.03;

    i = 7;
    resonVol[i][0] = 0.1;
    resonVol[i][1] = 0.4;
    resonVol[i][2] = 0.9;
    resonVol[i][3] = 0.8;
    resonVol[i][4] = 0.2;
    resonVol[i][5] = 0.7;
    resonVol[i][6] = 0.5;
    resonVol[i][7] = 0.6;
    resonVol[i][8] = 0.8;
    resonVol[i][9] = 0.3;
    resonVol[i][10] = 0.04;
    resonVol[i][11] = 0.01;
    resonVol[i][12] = 0.01;
    resonVol[i][13] = 0.07;
    resonVol[i][14] = 0.03;
    resonVol[i][15] = 0.05;
    resonVol[i][16] = 0.03;
    resonVol[i][17] = 0.1;
    resonVol[i][18] = 0.06;
    resonVol[i][19] = 0.1;
  
    i = 8;
    resonVol[i][0] = 0.02;
    resonVol[i][1] = 0.61;
    resonVol[i][2] = 0.81;
    resonVol[i][3] = 0.91;
    resonVol[i][4] = 0.2;
    resonVol[i][5] = 0.3;
    resonVol[i][6] = 0.1;
    resonVol[i][7] = 0.4;
    resonVol[i][8] = 0.1;
    resonVol[i][9] = 0.2;
    resonVol[i][10] = 0.04;
    resonVol[i][11] = 0.1;
    resonVol[i][12] = 0.08;
    resonVol[i][13] = 0.07;
    resonVol[i][14] = 0.08;
    resonVol[i][15] = 0.05;
    resonVol[i][16] = 0.03;
    resonVol[i][17] = 0.07;
    resonVol[i][18] = 0.04;
    resonVol[i][19] = 0.01;
    
    i = 9;
    resonVol[i][0] = 0.05;
    resonVol[i][1] = 0.81;
    resonVol[i][2] = 0.61;
    resonVol[i][3] = 0.53;
    resonVol[i][4] = 0.42;
    resonVol[i][5] = 0.07;
    resonVol[i][6] = 0.01;
    resonVol[i][7] = 0.06;
    resonVol[i][8] = 0.08;
    resonVol[i][9] = 0.2;
    resonVol[i][10] = 0.4;
    resonVol[i][11] = 0.1;
    resonVol[i][12] = 0.08;
    resonVol[i][13] = 0.07;
    resonVol[i][14] = 0.3;
    resonVol[i][15] = 0.05;
    resonVol[i][16] = 0.06;
    resonVol[i][17] = 0.08;
    resonVol[i][18] = 0.05;
    resonVol[i][19] = 0.09;
     */

    for (j = 0; j < NUM_BODIES; j++) {
        sum = 0.0f;

        for (i = 0; i < NUM_RESON; i++) {
            resonRate[j][i] /= 1000.0f;
            sum += resonVol[j][i];
        }

        for (i = 0; i < NUM_RESON; i++)
            resonVol[j][i] /= sum;
    }

} // setResonances

/*****************************************************************************/
/* initProcess : called to initialize the PlugIn after instantiation         */

/*****************************************************************************/

void Revitar::initProcess() {
    long i;
    int idx;
    float slope, offset;

    m_SampleRate = 44100.f;
    m_SampleRateMult = 1.;

    m_InSetProgram = false;

    // make frequency (Hz) table
    float k = 1.059463094359f; // 12th root of 2
    float a = 6.875f; // a
    a *= k; // b
    a *= k; // bb
    a *= k; // c, frequency of midi note 0
    for (i = 0; i < kNumFrequencies; i++) // 128 midi notes
    {
        freqtab[i] = (float) a;
        a *= k;
    }

    for (i = 0; i < kNumFrequencies; i++)
        m_NumNoteOffs[i] = 0;

    for (i = 0; i < NUM_KNOBS; i++)
        fKnobLast[i] = 0.0;

    m_NumPoly = 6;

    slope = 0.000045334114571f;
    offset = -0.000044163570764f;

    for (i = 0; i < NUM_POINTS; i++)
        m_FreqRate[i] = slope * (float) i + offset;

    for (idx = 0; idx < MAX_NUM_POLY; idx++)
        m_NumPoints[idx] = NUM_POINTS;

    /* create a set of filtered noise    */
    for (i = 0; i < NUM_SAMPLES; i++) {
        m_Random[i] = 0.8f * ((float) rand() / (float) RAND_MAX);
        if (i % 2)
            m_Random[i] = -m_Random[i];
    }
    for (i = 0; i < NUM_SAMPLES; i++) {
        m_Random[i] = 0.4f * m_Random[i] +
                0.2f * m_Random[(i + 1) % NUM_SAMPLES] +
                0.2f * m_Random[(i - 1 + NUM_SAMPLES) % NUM_SAMPLES] +
                0.1f * m_Random[(i + 2) % NUM_SAMPLES] +
                0.1f * m_Random[(i - 2 + NUM_SAMPLES) % NUM_SAMPLES];
    }
    m_RandomCt = 0;
    /* set up EADGBE default tuning      */
    m_DefaultTune[0] = m_Tune[0] = 40;
    m_DefaultTune[1] = m_Tune[1] = 45;
    m_DefaultTune[2] = m_Tune[2] = 50;
    m_DefaultTune[3] = m_Tune[3] = 55;
    m_DefaultTune[4] = m_Tune[4] = 59;
    m_DefaultTune[5] = m_Tune[5] = 64;

    for (idx = 0; idx < MAX_NUM_POLY; idx++) {
        m_DefaultRate[idx] = computeRate(idx, m_Tune[idx], 0.5f);
        m_Note[idx] = m_MIDINote[idx] = m_Tune[idx];

        m_SlideFinished[idx] = false;
        m_Rate[idx] = m_RateReal[idx] = m_DefaultRate[idx];
        m_NoteOn[idx] = false;
        m_NoteTurningOff[idx] = false;
        m_PickUpDiff[idx][0] = 0.0f;
        m_PickUpDiff[idx][1] = 0.0f;
        m_StartBend[idx] = 1.0f;
        m_Vibrato[idx] = 1.0f;
        m_Pluck[idx] = 0;
        m_NoteOffFade[idx] = 0;
        m_TypeOffset[idx] = 0;
        m_TypeOffsetPull[idx] = 0;
        m_ComputeCt[idx] = 0;
        m_Velocity[idx] = 0.0f;
        m_PickUpDiff[idx][0] = 0.0f;
        m_PickUpDiff[idx][1] = 0.0f;
        m_Left[idx] = 1.0f;
        m_Right[idx] = 1.0f;
        m_LeftDel[idx] = 0.0f;
        m_RightDel[idx] = 0.0f;
        m_LeftCt[idx] = 0.0f;
        m_RightCt[idx] = 0.0f;
        m_NoteCt[idx] = 0;
        m_Pick[idx] = 0;
        m_MIDIVelocity[idx] = 0.0;
        m_PickStiff[idx] = 0.5f;
        m_FirstChordNotes[idx] = NO_INFORMATION;
        m_SlideNoteCt[idx] = 0;
        m_CurrentSlideNoteCt[idx] = 0;

        for (i = 0; i < NUM_POINTS; i++) {
            m_ObjectPos[idx][i] = 0.0;
            m_ObjectVel[idx][i] = 0.0;
            m_PullCt[idx][i][0] = 0;
            m_PullCt[idx][i][1] = 0;
        }

        m_PickUp[idx] = m_NumPoints[idx] - ((int) ((float) ((m_NumPoints[idx] - 4) / 2) * prog.fPickUp) + 2);
        m_DampBridge[idx] = prog.fKnob[kKnobBridgeDamping] * 0.2f + 0.1f;
        m_DampBridge[idx] = (float) (m_DampBridge[idx] * m_DampBridge[idx]);
        m_DampBridge[idx] *= (float) m_NumPoints[idx] / NUM_POINTS;
    }


#ifdef  TUNE_NOW
    m_ZeroCt = 0;
    m_ZeroCt2 = 0;
    m_ZeroTotal = 0;
#endif

    m_VibratoAmp = 0.0001f;
    m_VibratoCt = 0;
    m_VibratoPos = 0.0f;
    m_VibratoCtAll = 0.0f;
    m_VibratoUp = 0;


    m_CurrentPan = 0.5f;
    m_CurrentBendDest = 1.0f;
    m_CurrentBendRate = 1.0f;

    setResonances();

    m_VuDisplayed = 0;

    m_LoveCt = -65 * 44100;
    m_LoveDt = 1;
    m_LoveDs = 100;

    m_ChordDisplayUpdate = 0;
    m_Down = 1;
    m_CurrentChordNote = NO_INFORMATION;
    m_FirstChordNote = NO_INFORMATION;
    m_MonoNote = NO_INFORMATION;
    m_NumMIDIData = 0;
    m_PickNoise = 0;
    m_ForceOff = false;
    m_SampleCt = 0;
    m_ShutDown = true;
    m_LastBodyIdx = 0;
    m_BodyCt = 0;
    m_BodyDel = 0.0;
    m_LastResOut = 0.0;
    m_PickAmp = 0.0f;
    m_LastMIDICC = NO_INFORMATION;
    m_CurrentBPM = 120.f;

    m_PickNoiseWave[14] = -0.05f;
    m_PickNoiseWave[13] = -0.1f;
    m_PickNoiseWave[12] = 0.1f;
    m_PickNoiseWave[11] = 0.8f;
    m_PickNoiseWave[10] = 0.9f;
    m_PickNoiseWave[9] = 0.2f;
    m_PickNoiseWave[8] = 0.0f;
    m_PickNoiseWave[7] = -0.15f;
    m_PickNoiseWave[6] = -0.3f;
    m_PickNoiseWave[5] = -0.1f;
    m_PickNoiseWave[4] = 0.11f;
    m_PickNoiseWave[3] = 0.2f;
    m_PickNoiseWave[2] = 0.1f;
    m_PickNoiseWave[1] = 0.05f;
    m_PickNoiseWave[0] = -0.01f;
    /* assure that any ctrl IS a change  */
    memset(m_MIDICtrlVal, 0xff, sizeof (m_MIDICtrlVal));

    m_GCount = 41000 * 10;
}

/*****************************************************************************/
/* setProgram : loads a(nother) program                                      */

/*****************************************************************************/

void Revitar::setProgram(VstInt32 program) {
    RevitarProgram * ap = &programs[program];

    m_InSetProgram = true;

    curProgram = program;
    prog = *ap;
    /* afterprocessing                   */
    turnOff(0);

    if (editor) {
        for (VstInt32 i = kKnobGain; i < kChordNotes; i++)
            ((AEffGUIEditor*) editor)->setParameter(i, getParameter(i));
    }

    m_InSetProgram = false;
}

/*****************************************************************************/
/* setProgramName : sets the current program's name                          */

/*****************************************************************************/

void Revitar::setProgramName(char *name) {
    strcpy(prog.name, name);
    strcpy(programs[curProgram].name, name);
}

/*****************************************************************************/
/* getProgramName : retrieves the current program's name                     */

/*****************************************************************************/

void Revitar::getProgramName(char *name) {
    if (!strcmp(prog.name, "Init"))
        sprintf(name, "%s %d", prog.name, curProgram + 1);
    else
        strcpy(name, prog.name);
}

/*****************************************************************************/
/* suspend : called when the PlugIn is suspended from processing             */

/*****************************************************************************/

void Revitar::suspend() {
}

/*****************************************************************************/
/* getVu : returns old-style VU meter content                                */

/*****************************************************************************/

float Revitar::DECLARE_VST_DEPRECATED(getVu) () {
    m_VuDisplayed = 1;

    return vu;
}

/*****************************************************************************/
/* setRegText : sets registration number                                     */

/*****************************************************************************/

void Revitar::setRegText(char *text) {
    //#define PASSWORD
#ifdef PASSWORD

    FILE *pwdFile = fopen("presets.txt", "w");
    if (pwdFile) {
        fprintf(pwdFile, "%s\n", text);

        char out[256];
        int ct, mod;
        int j;

        j = 0;
        ct = 0;
        while (text[j] != 0) {
            ct += (int) text[j];
            j++;
        }

        ct += 69854264;
        mod = ct % 57;

        for (j = 0; j < 8; j++) {
            out[j] = ct % 10 + 48;
            ct /= 10;
        }

        out[8] = mod % 10 + 48;
        mod /= 10;
        out[9] = mod % 10 + 48;

        out[10] = 0;

        fprintf(pwdFile, "%s\n", out);
        fflush(pwdFile);
        fclose(pwdFile);
    }

    sprintf(text, "%s", out);

    // 6165589603

#endif


#if WITH_REGISTRATION
    HKEY reg;
    DWORD dwDummy;
    int m, i;
    int c;

    reg = NULL;
    RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\CutterMusic\\Revo2", 0, "REG_DWORD",
            REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &reg, &dwDummy);

    if (reg != NULL)
        RegSetValueEx(reg, "regNum", 0, REG_SZ, (LPBYTE) text, strlen(text) + 1);

    RegCloseKey(reg);

    for (c = 0, i = 7; i >= 0; i--) {
        c += text[i] - 48;
        c *= 10;
    }
    c /= 10;

    m = text[8] - 48;
    m += 10 * (text[9] - 48);

    m_GLove = 0;

    if (c % 57 == m) {
        m_GLove = 1;
        cEffect.numParams = kNumParams;
        numParams = kNumParams;
    }

    if ((text[7] - 48) != 6) {
        m_GLove = 0;
        cEffect.numParams = 0;
        numParams = 0;
    }

    if ((text[6] - 48) != 9) {
        m_GLove = 0;
        cEffect.numParams = 0;
        numParams = 0;
    }
#endif

}

/*****************************************************************************/
/* setDisplayPickParameters : called from editor with new pick settings      */

/*****************************************************************************/

void Revitar::setDisplayPickParameters(float pickWidth, float pickStiffness) {
    setParameterAutomated(kPickWidth, pickWidth);
    setParameterAutomated(kPickStiffness, pickStiffness);

#if 0 // not in VST SDK 2.4 any more
    if (editor)
        editor->postUpdate();
#endif
}

/*****************************************************************************/
/* setDisplayChordNote : called from editor with new chord note setting      */

/*****************************************************************************/

void Revitar::setDisplayChordNote(int nNote, int nFret) {
    bool bAbsRel = (prog.fAbsRel > 0.f);
    /* calculate current chord#          */
    int chordIdx = (int) (bAbsRel ? prog.fChordIdx : prog.fChordAbsIdx);
    /* calculate parameter number for it */
    int index = (bAbsRel ? kChordNotes : kChordAbsNotes) + chordIdx * NUM_CHORD_NOTES;
    /* then set the new value there      */
    float value = (float) nFret / 24.f;
    setParameterAutomated(index + nNote, value);
    /* also update the "real" chord      */
    if (audioMaster)
        audioMaster(&cEffect, audioMasterAutomate, kChordNote0 + nNote, 0, 0, value);

#if 0 // not in VST SDK 2.4 any more
    if (editor)
        editor->postUpdate();
#endif
}

/*****************************************************************************/
/* setParameter : called to set one of the parameters                        */

/*****************************************************************************/

void Revitar::setParameter(VstInt32 index, float value) {
    RevitarProgram * ap = &programs[curProgram];
    bool bAbsRel = (prog.fAbsRel > 0.f);
    int chordIdx = (int) (bAbsRel ? prog.fChordIdx : prog.fChordAbsIdx);

    switch (index) {
        case kKnobGain:
        case kKnobBodyGain:
        case kKnobPickVolume:
        case kKnobTuning:
        case kKnobBridgeDamping:
        case kKnobStringDamping:
        case kKnobVibratoAmplit:
        case kKnobVibratoRate:
        case kKnobSympathetic:
        case kKnobSlap:
        case kKnobPickSpeed:
        case kKnobChordRate:
        case kKnobStringType:
        case kKnobPalmDamp:
        case kKnobSlideRate:
            prog.fKnob[index] = ap->fKnob[index] = value;
            break;

        case kMono:
            prog.fMono = ap->fMono = value;
            m_MonoNote = NO_INFORMATION;
            turnOff(0);
            if (prog.fChordOnOff > 0.f)
                prog.fMono = ap->fMono = 0.f;
            break;
        case kChordOnOff:
            prog.fChordOnOff = ap->fChordOnOff = value;
            if (prog.fChordOnOff > 0.f)
                prog.fMono = ap->fMono = 0.f;
            break;
        case kSustain: prog.fSustain = ap->fSustain = value;
            break;
        case kPickNoise: prog.fPickNoise = ap->fPickNoise = value;
            break;
        case kChord:
            if (bAbsRel)
                prog.fChordIdx = ap->fChordIdx = value;
            else
                prog.fChordAbsIdx = ap->fChordAbsIdx = value;
            break;
        case kChordAbs:
            if (!bAbsRel)
                prog.fChordIdx = ap->fChordIdx = value;
            else
                prog.fChordAbsIdx = ap->fChordAbsIdx = value;
            break;
        case kChordSwitch: prog.fChordSwitch = ap->fChordSwitch = value;
            break;
        case kBodySwitch: prog.fBodySwitch = ap->fBodySwitch = value;
            break;
        case kChordNote0:
        case kChordNote1:
        case kChordNote2:
        case kChordNote3:
        case kChordNote4:
        case kChordNote5:
            if (bAbsRel) {
                prog.fChordNote[chordIdx][index - kChordNote0] =
                        ap->fChordNote[chordIdx][index - kChordNote0] = value;
                chordIdx = kChordNotes + (chordIdx * NUM_CHORD_NOTES);
            } else {
                prog.fChordAbsNote[chordIdx][index - kChordNote0] =
                        ap->fChordAbsNote[chordIdx][index - kChordNote0] = value;
                chordIdx = kChordAbsNotes + (chordIdx * NUM_CHORD_NOTES);
            }
            if (!m_InSetProgram) {
                chordIdx += (index - kChordNote0);
                if (audioMaster)
                    audioMaster(&cEffect, audioMasterAutomate, chordIdx, 0, 0, value);
            }
            break;
        case kPickPosition: prog.fPickPosition = ap->fPickPosition = value;
            break;
        case kPickUp: prog.fPickUp = ap->fPickUp = value;
            break;
        case kPickStiffness: prog.fPickStiffness = ap->fPickStiffness = value;
            break;
        case kPickWidth: prog.fPickWidth = ap->fPickWidth = value;
            break;
        case kPalmSlider: prog.fPalmSlider = ap->fPalmSlider = value;
            break;
        case kAbsRel: prog.fAbsRel = ap->fAbsRel = value;
            break;
        case kStopSwitch: prog.fStopSwitch = ap->fStopSwitch = value;
            break;

        default:
            if ((index >= kChordNotes) && (index < kChordAbsNotes)) {
                int i = index - kChordNotes;
                int fixChordIdx = i / NUM_CHORD_NOTES;
                i = i % NUM_CHORD_NOTES;
                prog.fChordNote[fixChordIdx][i] = ap->fChordNote[fixChordIdx][i] = value;
                if ((!m_InSetProgram) && (bAbsRel) && (chordIdx == fixChordIdx)) {
                    if (audioMaster)
                        audioMaster(&cEffect, audioMasterAutomate, kChordNote0 + i, 0, 0, value);
                    if (editor)
                        ((AEffGUIEditor*) editor)->setParameter(kChordNote0 + i, value);
                }
            } else if ((index >= kChordAbsNotes) && (index < kNumParams)) {
                int i = index - kChordAbsNotes;
                int fixChordIdx = i / NUM_CHORD_NOTES;
                i = i % NUM_CHORD_NOTES;
                prog.fChordAbsNote[fixChordIdx][i] = ap->fChordAbsNote[fixChordIdx][i] = value;
                if ((!m_InSetProgram) && (!bAbsRel) && (chordIdx == fixChordIdx)) {
                    if (audioMaster)
                        audioMaster(&cEffect, audioMasterAutomate, kChordNote0 + i, 0, 0, value);
                    if (editor)
                        ((AEffGUIEditor*) editor)->setParameter(kChordNote0 + i, value);
                }
            }
            break;
    }

    /*
    if(index == kStopSwitch)
      {
      fprintf(m_OutFile, "start chord dump\n\n");
      for(int i=0;i<24;i++)
      {
      fprintf(m_OutFile, "chordNote[i][0] = %d.f;\n", 
      (int) floor(24.f*fChordAbsNote[i][0] + 0.0001));
      fprintf(m_OutFile, "chordNote[i][1] = %d.f;\n", 
      (int) floor(24.f*fChordAbsNote[i][1] + 0.0001));
      fprintf(m_OutFile, "chordNote[i][2] = %d.f;\n", 
      (int) floor(24.f*fChordAbsNote[i][2] + 0.0001));
      fprintf(m_OutFile, "chordNote[i][3] = %d.f;\n", 
      (int) floor(24.f*fChordAbsNote[i][3] + 0.0001));
      fprintf(m_OutFile, "chordNote[i][4] = %d.f;\n", 
      (int) floor(24.f*fChordAbsNote[i][4] + 0.0001));
      fprintf(m_OutFile, "chordNote[i][5] = %d.f;\n", 
      (int) floor(24.f*fChordAbsNote[i][5] + 0.0001));
      fprintf(m_OutFile, "i++;\n\n");
      }
  
    fflush(m_OutFile);
     */

    if (editor) {
#if 0 // not in VST SDK 2.4 any more
        editor->postUpdate();
#endif
        ((AEffGUIEditor*) editor)->setParameter(index, value);
    }
}

/*****************************************************************************/
/* getParameter : retrieves one of the VSTi parameters                       */

/*****************************************************************************/

float Revitar::getParameter(VstInt32 index) {
    float v = 0;
    bool bAbsRel = (prog.fAbsRel > 0.f);
    int chordIdx = (int) (bAbsRel ? prog.fChordIdx : prog.fChordAbsIdx);

    switch (index) {
        case kKnobGain:
        case kKnobBodyGain:
        case kKnobPickVolume:
        case kKnobTuning:
        case kKnobBridgeDamping:
        case kKnobStringDamping:
        case kKnobVibratoAmplit:
        case kKnobVibratoRate:
        case kKnobSympathetic:
        case kKnobSlap:
        case kKnobPickSpeed:
        case kKnobChordRate:
        case kKnobStringType:
        case kKnobPalmDamp:
        case kKnobSlideRate:
            v = prog.fKnob[index];
            break;
        case kMono: v = prog.fMono;
            break;
        case kChordOnOff: v = prog.fChordOnOff;
            break;
        case kSustain: v = prog.fSustain;
            break;
        case kPickNoise: v = prog.fPickNoise;
            break;
        case kChord: v = (bAbsRel) ? prog.fChordIdx: prog.fChordAbsIdx;
            break;
        case kChordAbs: v = (!bAbsRel) ? prog.fChordIdx: prog.fChordAbsIdx;
            break;
        case kChordSwitch: v = prog.fChordSwitch;
            break;
        case kBodySwitch: v = prog.fBodySwitch;
            break;
        case kChordNote0:
        case kChordNote1:
        case kChordNote2:
        case kChordNote3:
        case kChordNote4:
        case kChordNote5:
            if (bAbsRel)
                v = prog.fChordNote[chordIdx][index - kChordNote0];
            else
                v = prog.fChordAbsNote[chordIdx][index - kChordNote0];
            break;
        case kPickPosition: v = prog.fPickPosition;
            break;
        case kPickUp: v = prog.fPickUp;
            break;
        case kPickWidth: v = prog.fPickWidth;
            break;
        case kPickStiffness: v = prog.fPickStiffness;
            break;
        case kPalmSlider: v = prog.fPalmSlider;
            break;
        case kAbsRel: v = prog.fAbsRel;
            break;
        case kStopSwitch: v = prog.fStopSwitch;
            break;

        default:
            if ((index >= kChordNotes) && (index < kChordAbsNotes)) {
                int i = index - kChordNotes;
                chordIdx = i / NUM_CHORD_NOTES;
                i = i % NUM_CHORD_NOTES;
                v = prog.fChordNote[chordIdx][i];
            } else if ((index >= kChordAbsNotes) && (index < kNumParams)) {
                int i = index - kChordAbsNotes;
                chordIdx = i / NUM_CHORD_NOTES;
                i = i % NUM_CHORD_NOTES;
                v = prog.fChordAbsNote[chordIdx][i];
            }
            break;
    }

    return v;
}

/*****************************************************************************/
/* getParameterName : retrieves a parameter name                             */

/*****************************************************************************/

void Revitar::getParameterName(VstInt32 index, char *label) {
    switch (index) {
        case kKnobGain: strcpy(label, " Gain ");
            break;
        case kKnobBodyGain: strcpy(label, " Body Gain ");
            break;
        case kKnobPickVolume: strcpy(label, " Pick Volume ");
            break;
        case kKnobTuning: strcpy(label, " Tune ");
            break;
        case kKnobBridgeDamping: strcpy(label, " Bridge Damp ");
            break;
        case kKnobStringDamping: strcpy(label, " String Damp ");
            break;
        case kKnobVibratoAmplit: strcpy(label, " Vibrato Amp ");
            break;
        case kKnobVibratoRate: strcpy(label, " Vibrato Rate ");
            break;
        case kKnobSympathetic: strcpy(label, " Sympathetic ");
            break;
        case kKnobSlap: strcpy(label, " Slap ");
            break;
        case kKnobPickSpeed: strcpy(label, " Pick Speed ");
            break;
        case kKnobChordRate: strcpy(label, " Chord Rate ");
            break;
        case kKnobStringType: strcpy(label, " String Type ");
            break;
        case kKnobPalmDamp: strcpy(label, " Palm Damp ");
            break;
        case kKnobSlideRate: strcpy(label, " Slide Rate ");
            break;

        case kMono: strcpy(label, " Mono ");
            break;
        case kSustain: strcpy(label, " Sustain ");
            break;
        case kChordOnOff: strcpy(label, " Chord Play On/Off ");
            break;
        case kChord: strcpy(label, " Chord Type ");
            break;
        case kChordAbs: strcpy(label, " Chord Type ");
            break;
        case kChordSwitch: strcpy(label, " Strum Direction ");
            break;
        case kBodySwitch: strcpy(label, " Body Type ");
            break;
        case kPickNoise: strcpy(label, " Pluck Noise ");
            break;
        case kChordNote0:
        case kChordNote1:
        case kChordNote2:
        case kChordNote3:
        case kChordNote4:
        case kChordNote5: sprintf(label, " Chord Note %d ", index - kChordNote0);
            break;

        case kPalmSlider: strcpy(label, " Palm Position ");
            break;
        case kPickPosition: strcpy(label, " Pick Position ");
            break;
        case kPickUp: strcpy(label, " Pick Up Position ");
            break;
        case kPickWidth: strcpy(label, " Pick Width ");
            break;
        case kPickStiffness: strcpy(label, " Pick Stiffness ");
            break;
        case kAbsRel: strcpy(label, " Absolute/Relative Position ");
            break;
        case kStopSwitch: strcpy(label, " Stop Strings ");
            break;

        default:
            if ((index >= kChordNotes) && (index < kChordAbsNotes)) {
                int i = index - kChordNotes;
                int chordIdx = i / NUM_CHORD_NOTES;
                i = i % NUM_CHORD_NOTES;
                sprintf(label, " Chord %d Note %d ", chordIdx, i);
            } else if ((index >= kChordAbsNotes) && (index < kNumParams)) {
                int i = index - kChordAbsNotes;
                int chordIdx = i / NUM_CHORD_NOTES;
                i = i % NUM_CHORD_NOTES;
                sprintf(label, " Abs Chord %d Note %d ", chordIdx, i);
            } else
                sprintf(label, "#%d", index);
            break;
            break;
    }
}

/*****************************************************************************/
/* getParameterDisplay : returns a parameter value in text format            */

/*****************************************************************************/

void Revitar::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kKnobGain:
        case kKnobBodyGain:
        case kKnobPickVolume:
        case kKnobTuning:
        case kKnobBridgeDamping:
        case kKnobStringDamping:
        case kKnobVibratoAmplit:
        case kKnobVibratoRate:
        case kKnobSympathetic:
        case kKnobSlap:
        case kKnobPickSpeed:
        case kKnobChordRate:
        case kKnobStringType:
        case kKnobPalmDamp:
        case kKnobSlideRate:
            float2string(prog.fKnob[index], text, kVstMaxParamStrLen);
            break;

        case kChordOnOff: float2string(prog.fChordOnOff, text, kVstMaxParamStrLen);
            break;
        case kMono: float2string(prog.fMono, text, kVstMaxParamStrLen);
            break;
        case kSustain: float2string(prog.fSustain, text, kVstMaxParamStrLen);
            break;
        case kPickNoise: float2string(prog.fPickNoise, text, kVstMaxParamStrLen);
            break;
        case kChord: float2string(prog.fChordIdx, text, kVstMaxParamStrLen);
            break;
        case kChordAbs: float2string(prog.fChordAbsIdx, text, kVstMaxParamStrLen);
            break;
        case kChordSwitch: float2string(prog.fChordSwitch, text, kVstMaxParamStrLen);
            break;
        case kBodySwitch: float2string(prog.fBodySwitch, text, kVstMaxParamStrLen);
            break;
        case kChordNote0:
        case kChordNote1:
        case kChordNote2:
        case kChordNote3:
        case kChordNote4:
        case kChordNote5:
        {
            float v;
            bool bAbsRel = (prog.fAbsRel > 0.f);
            int chordIdx = (int) (bAbsRel ? prog.fChordIdx : prog.fChordAbsIdx);
            if (bAbsRel)
                v = prog.fChordNote[chordIdx][index - kChordNote0] * 24.f;
            else
                v = prog.fChordAbsNote[chordIdx][index - kChordNote0] * 24.f;
            if (v > -0.01f)
                sprintf(text, "%d", (int) v);
            else
                strcpy(text, "Off");
        }
            break;

        case kPalmSlider: float2string(prog.fPalmSlider, text, kVstMaxParamStrLen);
            break;
        case kPickPosition: float2string(prog.fPickPosition, text, kVstMaxParamStrLen);
            break;
        case kPickUp: float2string(prog.fPickUp, text, kVstMaxParamStrLen);
            break;
        case kPickWidth: float2string(prog.fPickWidth, text, kVstMaxParamStrLen);
            break;
        case kPickStiffness: float2string(prog.fPickStiffness, text, kVstMaxParamStrLen);
            break;
        case kAbsRel: float2string(prog.fAbsRel, text, kVstMaxParamStrLen);
            break;
        case kStopSwitch: float2string(prog.fStopSwitch, text, kVstMaxParamStrLen);
            break;

        default:
            if ((index >= kChordNotes) && (index < kChordAbsNotes)) {
                int i = index - kChordNotes;
                int chordIdx = i / NUM_CHORD_NOTES;
                i = i % NUM_CHORD_NOTES;
                float v = prog.fChordNote[chordIdx][i] * 24.f;
                if (v > -0.01f)
                    sprintf(text, "%d", (int) v);
                else
                    strcpy(text, "Off");
            } else if ((index >= kChordAbsNotes) && (index < kNumParams)) {
                int i = index - kChordAbsNotes;
                int chordIdx = i / NUM_CHORD_NOTES;
                i = i % NUM_CHORD_NOTES;
                float v = prog.fChordAbsNote[chordIdx][i] * 24.f;
                if (v > -0.01f)
                    //      float2string (v, text, kVstMaxParamStrLen);
                    sprintf(text, "%d", (int) v);
                else
                    strcpy(text, "Off");
            } else
                *text = '\0';
            break;
    }
}

/*****************************************************************************/
/* getParameterLabel : returns the label for a parameter                     */

/*****************************************************************************/

void Revitar::getParameterLabel(VstInt32 index, char *label) {
    switch (index) {
        case kKnobGain: strcpy(label, " Gain ");
            break;
        case kKnobBodyGain: strcpy(label, " Body Gain ");
            break;
        case kKnobPickVolume: strcpy(label, " Pick Volume ");
            break;
        case kKnobTuning: strcpy(label, " Tune ");
            break;
        case kKnobBridgeDamping: strcpy(label, " Bridge Damp ");
            break;
        case kKnobStringDamping: strcpy(label, " String Damp ");
            break;
        case kKnobVibratoAmplit: strcpy(label, " Vibrato Amp ");
            break;
        case kKnobVibratoRate: strcpy(label, " Vibrato Rate ");
            break;
        case kKnobSympathetic: strcpy(label, " Sympathetic ");
            break;
        case kKnobSlap: strcpy(label, " Slap ");
            break;
        case kKnobPickSpeed: strcpy(label, " Pick Speed ");
            break;
        case kKnobChordRate: strcpy(label, " Chord Rate ");
            break;
        case kKnobStringType: strcpy(label, " String Type ");
            break;
        case kKnobPalmDamp: strcpy(label, " Palm Damp ");
            break;
        case kKnobSlideRate: strcpy(label, " Slide Rate ");
            break;

        case kMono: strcpy(label, " Mono/Poly ");
            break;
        case kSustain: strcpy(label, " Sustain ");
            break;
        case kChordOnOff: strcpy(label, " Chord Play On/Off ");
            break;
        case kChord: strcpy(label, " Chord Type ");
            break;
        case kChordAbs: strcpy(label, " Chord Type ");
            break;
        case kChordSwitch: strcpy(label, " Strum Direction ");
            break;
        case kBodySwitch: strcpy(label, " Body Type ");
            break;
        case kPickNoise: strcpy(label, " Pluck Noise Full/Reduced ");
            break;
        case kChordNote0:
        case kChordNote1:
        case kChordNote2:
        case kChordNote3:
        case kChordNote4:
        case kChordNote5: strcpy(label, (prog.fAbsRel > 0.f) ? " Fret# ": " Note ");
            break;
        case kPalmSlider: strcpy(label, " Palm Position ");
            break;
        case kPickPosition: strcpy(label, " Pick Position ");
            break;
        case kPickUp: strcpy(label, " Pick Up Position ");
            break;
        case kPickWidth: strcpy(label, " Pick Width ");
            break;
        case kPickStiffness: strcpy(label, " Pick Stiffness ");
            break;
        case kAbsRel: strcpy(label, " Absolute/Relative Position ");
            break;
        case kStopSwitch: strcpy(label, " Stop Strings ");
            break;

        default:
            if ((index >= kChordNotes) && (index < kChordAbsNotes))
                strcpy(label, " Fret# ");
            else if ((index >= kChordAbsNotes) && (index < kNumParams))
                strcpy(label, " Note ");
            else
                *label = '\0';
            break;
    }
}

/*****************************************************************************/
/* canParameterBeAutomated : returns whether a parameter can be automated    */

/*****************************************************************************/

bool Revitar::canParameterBeAutomated(VstInt32 index) {
    switch (index) {
            //case kReserved0 :                     /* unused parameters can't be.       */
        case kReserved1:
        case kReserved2:
        case kReserved3:
            return false;
        default:
            if ((index >= 0) && (index < kNumParams))
                return true;
            return false;
    }
}

/*****************************************************************************/
/* getParameterProperties : returns a parameter's properties                 */

/*****************************************************************************/

bool Revitar::getParameterProperties(VstInt32 index, VstParameterProperties* p) {
    if ((index < 0) || (index >= kNumParams) || (!p))
        return false;

    return false; // not coded yet
}


/*****************************************************************************/
/* process : processes an audio buffer                                       */
/*****************************************************************************/

// This is the function that does all the synthesis, and is the most confusing
// so I'll try to comment a little.

void Revitar::process(float **inputs, float **outputs, VstInt32 sampleframes) {
    int polyIdx;
    int sampleTemp = sampleframes;
    float val0, val1;
    int i, j;
    float volAdd;
    float out[2], vol;
    float vibrato;
    int numPoints;
    float maxlvl;
    float amountVol;
    float amountVolAdd;
    float velPluck, posPluck;
    float outPrior;
    float outTotal[2];
    float resPickUp, resRate;

    // deal with underflow problems - if you don't do this everything gets really slllllooowwww.
    if (m_ShutDown == false && m_SampleCt > 2000) {
        float sum = 0.0f;

        for (polyIdx = 0; polyIdx < m_NumPoly; polyIdx++) {
            float sumNote = 0.0f;

            for (i = 1; i < m_NumPoints[polyIdx]; i++) {
                sumNote += (float) fabs(m_ObjectVel[polyIdx][i]);
                sumNote += (float) fabs(m_ObjectPos[polyIdx][i]);

                m_ObjectVel[polyIdx][i] += 0.000001f;
                m_ObjectPos[polyIdx][i] += 0.000001f;
            }

            sum += sumNote / (float) (m_NumPoints[polyIdx] - 1);

            for (i = m_NumPoints[polyIdx]; i < NUM_POINTS; i++) {
                m_ObjectVel[polyIdx][i] = 0.000001f;
                m_ObjectPos[polyIdx][i] = 0.000001f;
            }
        }

        float sumRes = 0.0f;

        for (i = 0; i < NUM_RESON; i++) {
            sumRes += (float) fabs(resonPos[i]);
            sumRes += (float) fabs(resonVel[i]);
            resonPos[i] += 0.000001f;
            resonVel[i] += 0.000001f;
        }

        sum += sumRes / (float) (NUM_RESON);

        for (polyIdx = 0; polyIdx < m_NumPoly; polyIdx++) {
            sum += (float) m_NoteCt[polyIdx];
            sum += (float) m_Pluck[polyIdx];
            sum += (float) m_Pick[polyIdx];
            sum += (float) m_NoteOffFade[polyIdx];
        }

        if (sum < 0.005)
            m_ForceOff = true;

        if (sum < 0.001)
            m_ShutDown = true;

        m_SampleCt = 0;
    }

    // Get all the input parameters from the knobs...
    m_BodyIdx = (int) ((prog.fBodySwitch + 0.01)*(float) (NUM_BODIES - 1));
    maxlvl = 0.0f;

    float* out1 = outputs[0];
    float* out2 = outputs[1];

    vol = 0.7f * fKnobLast[0];
    amountVol = fKnobLast[1];

    volAdd = 0.7f * prog.fKnob[kKnobGain];
    amountVolAdd = prog.fKnob[kKnobBodyGain];

    volAdd = (volAdd - vol) / (float) (std::max(1, sampleTemp));
    amountVolAdd = (amountVolAdd - amountVol) / (float) (std::max(1, sampleTemp));

    if (!prog.fKnob[kKnobSlap])
        m_Slap = -5.0f;
    else
        m_Slap = -2.5f * (1.0f - prog.fKnob[kKnobSlap] + 0.1f);

    for (polyIdx = 0; polyIdx < m_NumPoly; polyIdx++)
        m_DampPalm[polyIdx] = 0.1f *
            prog.fKnob[kKnobPalmDamp] *
            prog.fKnob[kKnobPalmDamp] *
            prog.fKnob[kKnobPalmDamp] *
            prog.fKnob[kKnobPalmDamp];

    m_DampString = 0.00035f *
            prog.fKnob[kKnobStringDamping] *
            prog.fKnob[kKnobStringDamping] *
            prog.fKnob[kKnobStringDamping] +
            0.0000001f;
    float minDampString = 0.00045f + 0.0000001f;


    sampleframes = sampleTemp;
    m_SampleCt += sampleTemp;
    // For every sample
    while (--sampleframes >= 0) {
        out[0] = 0.0f;
        out[1] = 0.0f;
        outTotal[0] = 0.0f;
        outTotal[1] = 0.0f;
        resPickUp = 0.0f;

        // Handle Vibrato (pitch change.)
        if (m_VibratoUp) {
            m_VibratoAmp *= 1.001f;

            if (m_VibratoAmp > 1.0f) {
                m_VibratoAmp = 1.0f;
                m_VibratoUp = false;
            }
        } else {
            if (m_VibratoAmp > 0.01f)
                m_VibratoAmp *= 0.99999f;
            else if (m_VibratoAmp > 0.001f)
                m_VibratoAmp *= 0.999f;
        }

        m_VibratoCt++;
        if (m_VibratoCt >= 500) {
            m_VibratoCt = 0;
            m_VibratoCtAll += (0.3f * prog.fKnob[kKnobVibratoRate] + 0.4f) *
                    (0.3f * prog.fKnob[kKnobVibratoRate] + 0.4f) +
                    0.001f;
            vibrato = (float) cos(m_VibratoCtAll + 3.14);
            vibrato += 1.0;
            vibrato /= 2.0;

            //  if(vibrato < 0.0f)
            //      vibrato = (float) -sqrt(-vibrato);
            //  else
            //      vibrato = (float) sqrt(vibrato);

            vibrato *= 0.059463094359f *
                    prog.fKnob[kKnobVibratoAmplit] *
                    prog.fKnob[kKnobVibratoAmplit];

            m_VibratoAdd = (vibrato - m_VibratoPos) / 500.0f;
        }

        m_VibratoPos += m_VibratoAdd;

        vibrato = m_VibratoAmp * m_VibratoPos;
        vibrato++;

        handleMIDIData();

        val0 = m_CurrentBendDest - m_CurrentBendRate;
        if (fabsf(val0) > 0.0001f)
            m_CurrentBendRate += 0.0004f * val0;

        //  vibrato *= m_CurrentBendRate;

        if (m_ShutDown == false) {
            for (polyIdx = 0; polyIdx < m_NumPoly; polyIdx++) {
                if (!m_NoteOn[polyIdx])
                    m_Vibrato[polyIdx] = 1.0f;
                else if (m_NoteTurningOff[polyIdx]) {
                    val0 = 1.0f - m_Vibrato[polyIdx];
                    if (fabsf(val0) > 0.0001f)
                        m_Vibrato[polyIdx] += 0.01f * val0;
                } else
                    m_Vibrato[polyIdx] = vibrato;


                m_ComputeCt[polyIdx]++;
                numPoints = m_NumPoints[polyIdx];

                // handle new notes coming in
                if (m_NoteCt[polyIdx] > 0 && m_NoteQueue[polyIdx][0][0] == 0)
                    updateNewNotes(polyIdx);

                for (i = 0; i < m_NoteCt[polyIdx]; i++)
                    if (m_NoteQueue[polyIdx][i][0] >= 0)
                        m_NoteQueue[polyIdx][i][0]--;

                // handle turning off notes
                if (m_NoteTurningOff[polyIdx])
                    turnOffNotes(polyIdx);

                float dval0 = m_Rate[polyIdx] * m_Vibrato[polyIdx] * m_CurrentBendRate;
                if (m_StartBend[polyIdx] > 1.000001f) {
                    m_StartBend[polyIdx] += 0.009f * m_Rate[polyIdx] * (1.0f - m_StartBend[polyIdx]);
                    dval0 *= m_StartBend[polyIdx];
                }
                // this must not get out of control!
                if (fabsf(dval0) > 0.999f)
                    dval0 = (float) _copysign(0.999, dval0);

                if (m_Pluck[polyIdx] &&
                        m_PullCt[polyIdx][m_Pluck[polyIdx]][0] > 3 * m_PullCt[polyIdx][m_Pluck[polyIdx]][1] / 4) {
                    velPluck = m_ObjectVel[polyIdx][m_Pluck[polyIdx]];
                    posPluck = m_ObjectPos[polyIdx][m_Pluck[polyIdx]];
                }

                float dval2 = 1.0f - m_DampString;
                if (m_NoteTurningOff[polyIdx] &&
                        prog.fSustain < 0.5f &&
                        m_DampString < minDampString)
                    dval2 = 1.0f - minDampString;

                // This is the part of the code that actually runs the string simulation.
                float *p1 = &m_ObjectPos[polyIdx][0];
                float *v1 = &m_ObjectVel[polyIdx][0];
                float disX;
                float acc = 0.0f;

                // For every point on the string...
                // Low notes need more points than high notes.
                // If you want to make a really good bass guitar I would add a lot more
                // points to get a realistic sound.
                for (int tmp0 = 0; tmp0 < numPoints - 1;) {
                    // This for loop is unrolled 4 times to increase speed.
                    // If you want to optimize my code this is the place to do it.
                    //  acc is the acceleration of the points.
                    //  v1 is the velocity
                    //  p1 is it's position
                    //  This is a basic spring mass model- roughly.
                    //  Damping has been included (in V2.0, it was a separate loop)

                    disX = p1[tmp0] - p1[tmp0 + 1];

                    v1[tmp0] = (v1[tmp0] + dval0 * (acc - disX)) * dval2;
                    p1[tmp0] += dval0 * v1[tmp0];

                    acc = disX;

                    ++tmp0;


                    disX = p1[tmp0] - p1[tmp0 + 1];

                    v1[tmp0] = (v1[tmp0] + dval0 * (acc - disX)) * dval2;
                    p1[tmp0] += dval0 * v1[tmp0];

                    acc = disX;

                    ++tmp0;

                    disX = p1[tmp0] - p1[tmp0 + 1];

                    v1[tmp0] = (v1[tmp0] + dval0 * (acc - disX)) * dval2;
                    p1[tmp0] += dval0 * v1[tmp0];

                    acc = disX;

                    ++tmp0;

                    disX = p1[tmp0] - p1[tmp0 + 1];

                    v1[tmp0] = (v1[tmp0] + dval0 * (acc - disX)) * dval2;
                    p1[tmp0] += dval0 * v1[tmp0];

                    acc = disX;

                    ++tmp0;
                }

                // Make sure the end points have 0 position and velocity
                p1[0] = 0.0f;
                v1[0] = 0.0f;
                p1[numPoints - 1] = 0.0f;
                v1[numPoints - 1] = 0.0f;

                // Update string display
                if ((!(sampleframes % 70)) &&
                        (getEditor()->isOpen())) {
                    for (i = 0; i < numPoints; i++) {
                        j = (int) ((float) CHORD_DISPLAY_HEIGHT * p1[i] / 4.0f);
                        j += CHORD_DISPLAY_HEIGHT / 2;
                        j = std::min(CHORD_DISPLAY_HEIGHT - 1, std::max(0, j));

                        m_ChordDisplay[polyIdx][i][j]++;
                    }
                    for (; i < NUM_POINTS; i++)
                        m_ChordDisplay[polyIdx][i][CHORD_DISPLAY_HEIGHT / 2]++;

                    m_ChordDisplayUpdate++;
                }

                // compute slap effect
                for (i = 40; i < numPoints - 1; i += 10) {
                    int flip = numPoints - 1 - i;
                    if (p1[flip] < m_Slap) {
                        p1[flip] += 0.5f * (m_Slap - p1[flip]);
                        v1[flip] *= 0.5f;
                    }
                }

                // Handle palm damping
                int dampIdx = numPoints / 2 +
                        (int) (((double) (numPoints / 2 - 3)) * (1.0 - prog.fPalmSlider));
                float palmDamp = m_DampPalm[polyIdx];
                v1[dampIdx] -= palmDamp * v1[dampIdx];

                if (prog.fStopSwitch > 0.f || m_ForceOff) {
                    palmDamp = 0.08f;
                    v1[dampIdx] -= palmDamp * v1[dampIdx];
                    dampIdx--;
                    v1[dampIdx] -= palmDamp * v1[dampIdx];
                }

                // Handle plucking
                if (prog.fPickNoise > 0.5f && m_Pluck[polyIdx] &&
                        m_PullCt[polyIdx][m_Pluck[polyIdx]][0] > 3 * m_PullCt[polyIdx][m_Pluck[polyIdx]][1] / 4) {
                    v1[m_Pluck[polyIdx]] = velPluck;
                    p1[m_Pluck[polyIdx]] = posPluck;
                }

                // Handle sliding
                if (m_CurrentSlideNoteCt[polyIdx] < m_SlideNoteCt[polyIdx]) {
                    int noteIdx = m_CurrentSlideNoteCt[polyIdx];
                    m_SlideLength[polyIdx][noteIdx]--;

                    if (m_SlideLength[polyIdx][noteIdx] <= 0) {
                        m_Note[polyIdx] = m_SlideNote[polyIdx][noteIdx];

                        m_Rate[polyIdx] = m_RateReal[polyIdx] =
                                computeRate(polyIdx, m_Note[polyIdx], prog.fKnob[kKnobTuning]);

                        m_CurrentSlideNoteCt[polyIdx]++;
                    }

                    if (m_SlideLength[polyIdx][noteIdx] <= 200) {
                        if (p1[6] < 0.0f) {
                            p1[6] = 0.0f;
                            v1[6] = 0.0f;
                        }
                    }
                }

                if (m_Pluck[polyIdx])
                    doPluck(polyIdx);

                // Bridge damping - changes the sound of the string and make it more "natural".
                // In practice this slightly detunes the harmonics making for a richer sound.
                v1[1] -= m_DampBridge[polyIdx] * v1[1];
                v1[numPoints - 2] -= m_DampBridge[polyIdx] * v1[numPoints - 2];

                // Need to sample three points on the string for the pickup. 
                // If only one is used weird high frequency are picked up - not sure why.
                val0 = 1.0f * p1[m_PickUp[polyIdx]];
                val0 += 0.5f * p1[m_PickUp[polyIdx] - 1];
                val0 += 0.5f * p1[m_PickUp[polyIdx] + 1];

                if (m_PickUpDiff[polyIdx][0]) {
                    val0 += m_PickUpDiff[polyIdx][0];
                    m_PickUpDiff[polyIdx][0] *= 0.95f;

                    if (fabs(m_PickUpDiff[polyIdx][0]) < 0.0001f)
                        m_PickUpDiff[polyIdx][0] = 0.0f;
                }

                val1 = 1.0f * p1[m_PickUp[polyIdx] + 1];
                val1 += 0.5f * p1[m_PickUp[polyIdx]];
                val1 += 0.5f * p1[m_PickUp[polyIdx] + 2];

                if (m_PickUpDiff[polyIdx][1]) {
                    val1 += m_PickUpDiff[polyIdx][1];
                    m_PickUpDiff[polyIdx][1] *= 0.95f;

                    if (fabs(m_PickUpDiff[polyIdx][1]) < 0.0001f)
                        m_PickUpDiff[polyIdx][1] = 0.0f;
                }

                if (m_LeftCt[polyIdx]) {
                    m_Left[polyIdx] += m_LeftDel[polyIdx];
                    m_LeftCt[polyIdx]++;
                }

                out[0] += 0.1f * m_Left[polyIdx] * val0;

                if (m_RightCt[polyIdx]) {
                    m_Right[polyIdx] += m_RightDel[polyIdx];
                    m_RightCt[polyIdx]++;
                }

                out[1] += 0.1f * m_Right[polyIdx] * val1;

                //  float resRateHigh =  56.0f/((float) m_Note[polyIdx]);
                resRate = 1.0f - ((float) m_Note[polyIdx] - 46.0f) / 54.0f;

                if (resRate > 1.0f)
                    resRate = 1.0f;
                else if (resRate < 0.0f)
                    resRate = 0.0f;

                float resRateDel = 4.0f * (prog.fKnob[kKnobBodyGain] - 0.5f) + 1.0f;
                if (resRateDel < 0.0f)
                    resRateDel = 0.0f;

                resRate *= resRateDel;

                float resPickUpSum = 0.0f;

                // Compute the resonance that will be transfered to the guitar body.
                resPickUpSum += resRate * p1[9];
                resPickUpSum += resRate * p1[10];
                resPickUpSum += resRate * p1[11];
                resPickUpSum += resRate * p1[12];
                resPickUp += resPickUpSum / 4.0f;

            }

            // Compute the sound of the guitar body
            // To be honest I never got this right.  
            float resOut = computeBodyResonance(resPickUp);
            float pickResOut = resOut;

            pickResOut *= amountVol;

            if (m_PickNoise > 0.0f) {
                m_PickNoise -= m_PickNoiseRate;

                pickResOut += m_PickAmp *
                        (0.5f * (float) fabs(m_Random[m_RandomCt]) + 2.0f) *
                        m_PickNoiseWave[(int) m_PickNoise];

                m_RandomCt++;
                if (m_RandomCt >= NUM_SAMPLES)
                    m_RandomCt = 0;
            }

            val0 = pickResOut + 1.5f * out[0];
            val1 = pickResOut + 1.5f * out[1];

            val0 *= vol;
            val1 *= vol;

            outTotal[0] = val0;
            outTotal[1] = val1;

            if (maxlvl < val0)
                maxlvl = val0;

            outPrior = 0.0f;
            for (i = 0; i < m_NumPoly; i++)
                outPrior += m_ObjectPos[i][1];

            // This adds the vibrations of the body back into the stings.  It also allows the string
            // to talk to each other - provides richer sound.
            for (i = 0; i < m_NumPoly; i++) {
                m_ObjectPos[i][0] = 0.6f *
                        prog.fKnob[kKnobSympathetic] *
                        (outPrior - m_ObjectPos[i][1]) / (float) m_NumPoly;
            }

            *out1 += outTotal[0];
            *out2 += outTotal[1];
        }

#if WITH_REGISTRATION
        // This just does the annoying sound cutoff if it's not registered/purchased.
        if (!GLoveGood()) {
            if (m_GCount < 0) {
                float deloff = ((float) m_GCount + 10000.f) / 10000.f;
                deloff *= deloff;
                deloff *= deloff;
                deloff *= deloff;
                deloff *= deloff;

                *out1 *= deloff;
                *out2 *= deloff;

                if (m_GCount < -20000)
                    m_GCount = (int) ((1.0f + m_Random[m_RandomCt]) * 15.f * 44100.f);
            }
            m_GCount--;
        }
#endif

        out1++;
        out2++;

        vol += volAdd;
        amountVol += amountVolAdd;
    }

    if (maxlvl >= 0.95f || (!m_VuDisplayed && vu >= 0.85f))
        vu = 0.95f;
    else
        vu += 0.2f * (maxlvl - vu);

    m_VuDisplayed = 0;

    memcpy(fKnobLast, prog.fKnob, NUM_KNOBS * sizeof (fKnobLast[0]));
}

/*****************************************************************************/
/* doPluck :                                                                 */

/*****************************************************************************/

void Revitar::doPluck(int polyIdx) {
#if 0
    // this is already checked outside the function!
    if (m_Pluck[polyIdx])
#endif
    {
        int i = m_Pluck[polyIdx];

        if (m_Pick[polyIdx]) {
            m_PickPos[polyIdx] += m_PickAdd[polyIdx];

            if (m_ObjectPos[polyIdx][i] < m_PickPos[polyIdx]) {
                if (m_PullCt[polyIdx][i][0] < m_TypeOffsetPull[polyIdx]) {
                    m_ObjectPos[polyIdx][i] += m_PickStiff[polyIdx]*((m_PickPos[polyIdx] - m_ObjectPos[polyIdx][i]));
                    m_ObjectVel[polyIdx][i] *= 0.999f;

                    int pickOffset0 = (int) (prog.fPickWidth * 10.5f);
                    int pickOffset1 = pickOffset0 / 2;
                    pickOffset0 = pickOffset0 - pickOffset1;
                    if (pickOffset0 > 0) {
                        m_ObjectPos[polyIdx][i + pickOffset0] += m_PickStiff[polyIdx]*((m_PickPos[polyIdx] - m_ObjectPos[polyIdx][i + pickOffset0]));
                        m_ObjectVel[polyIdx][i + pickOffset0] *= 0.999f;

                        m_ObjectPos[polyIdx][i - pickOffset1] += m_PickStiff[polyIdx]*((m_PickPos[polyIdx] - m_ObjectPos[polyIdx][i - pickOffset1]));
                        m_ObjectVel[polyIdx][i - pickOffset1] *= 0.999f;
                    }
                }

                if (m_PullCt[polyIdx][i][0] >
                        m_TypeOffsetPull[polyIdx] + (int) (50.0f * (1.0f - prog.fKnob[kKnobPickSpeed]))) {
                    m_TypeOffsetPull[polyIdx] +=
                            m_TypeOffset[polyIdx] +
                            (int) (0.4f * m_TypeOffset[polyIdx] * m_Random[m_RandomCt]);
                    m_RandomCt++;
                    if (m_RandomCt >= NUM_SAMPLES)
                        m_RandomCt = 0;
                }
            }
        }

        m_PullCt[polyIdx][i][0]++;

        if (m_PullCt[polyIdx][i][0] == m_PullCt[polyIdx][i][1]) {
            if (m_Pick[polyIdx]) {
                m_VibratoAmp = 0.0001f;
                m_VibratoCt = -1500;
                m_VibratoUp = true;
                m_VibratoAdd = 0.0f;
                m_VibratoPos = 0.0f;
                m_VibratoCtAll = 0.0f;
                m_StartBend[polyIdx] = 1.03f;
                m_PickNoise = 15.0f * (prog.fPickNoise > 0.5f);
                //m_PickNoise = 0.0;
                m_PickNoiseRate = (0.7f + 1.0f * m_Random[m_RandomCt]) * (prog.fPickNoise > 0.5f);
                m_PickNoiseRate *= (0.2f + std::max(0.0f, 0.9f * (prog.fPickStiffness + 0.2f) - 0.5f * (1.0f - prog.fPickWidth))) * (prog.fPickNoise > 0.5f);
                //m_PickNoiseRate = 0.0;
                m_PickAmp = ((0.3f + 2.5f * m_Random[m_RandomCt] * m_Random[m_RandomCt]) *
                        prog.fKnob[kKnobPickVolume] *
                        m_Velocity[polyIdx] *
                        m_Velocity[polyIdx]) * (prog.fPickNoise > 0.5f);
                //m_PickAmp = 0.0;
                m_RandomCt++;
                if (m_RandomCt >= NUM_SAMPLES)
                    m_RandomCt = 0;

                if (m_Pick[polyIdx] == 2)
                    m_PickAmp = 0.0;
            }

            m_Pick[polyIdx] = 0;
            m_Pluck[polyIdx] = 0;
        }
    }
} // doPluck

/*****************************************************************************/
/* turnOffNotes : processes turning off notes                                */

/*****************************************************************************/

void Revitar::turnOffNotes(int polyIdx) {
#if 0
    // this is already checked before calling the function!
    if (m_NoteTurningOff[polyIdx])
#endif
    {
        if (prog.fSustain == 0.f)
            m_DampPalm[polyIdx] = 0.1f;

        if (--m_NoteOffFade[polyIdx] == 0) {
            m_ComputeCt[polyIdx] = 0;
            m_NoteTurningOff[polyIdx] = false;
            m_NoteOn[polyIdx] = false;
        }
    }
} // turnOffNotes

/*****************************************************************************/
/* updateNewNotes :                                                          */

/*****************************************************************************/

void Revitar::updateNewNotes(int polyIdx) {
#if 0
    // this is already checked outside the function:
    // if (m_NoteCt[polyIdx] > 0 && m_NoteQueue[polyIdx][0][0] == 0)
    if (m_SlideFinished[polyIdx] == false && m_NoteCt[polyIdx] > 0 && m_NoteQueue[polyIdx][0][0] == 0)
#else
    // so let's optimize a bit:
    if (m_SlideFinished[polyIdx] == false)
#endif
    {
        m_MIDINote[polyIdx] = m_NoteQueue[polyIdx][0][1];

        int maxSlide = (int) ((prog.fKnob[kKnobSlideRate] - 0.2f) *
                (float) SLIDE_LENGTH *
                (1.0f + 0.5f * m_Random[m_RandomCt]));
        if (maxSlide < 0)
            maxSlide = 0;

        m_RandomCt++;
        if (m_RandomCt >= NUM_SAMPLES)
            m_RandomCt = 0;

        int totalLength = 0;
        m_SlideNoteCt[polyIdx] = 0;
        m_CurrentSlideNoteCt[polyIdx] = 0;

        if (m_NoteQueue[polyIdx][0][3] == 0) {
            int startNote = m_Note[polyIdx];
            int endNote = m_NoteQueue[polyIdx][0][1];

            if (startNote < endNote) {
                for (int j = startNote + 1; j <= endNote; j++) {
                    int idx = m_SlideNoteCt[polyIdx];
                    m_SlideNote[polyIdx][idx] = j;

                    float del = (float) pow(1.059f, (float) (j - 1 - startNote));
                    m_SlideLength[polyIdx][idx] = maxSlide;
                    m_SlideLength[polyIdx][idx] = (int) ((float) m_SlideLength[polyIdx][idx] * del);
                    totalLength += m_SlideLength[polyIdx][idx];

                    m_SlideNoteCt[polyIdx]++;
                }
            } else {
                for (int j = startNote - 1; j >= endNote; j--) {
                    int idx = m_SlideNoteCt[polyIdx];
                    m_SlideNote[polyIdx][idx] = j;

                    float del = (float) pow(1.059f, (float) (j - endNote));
                    m_SlideLength[polyIdx][idx] = maxSlide;
                    m_SlideLength[polyIdx][idx] = (int) ((float) m_SlideLength[polyIdx][idx] * del);

                    totalLength += m_SlideLength[polyIdx][idx];

                    m_SlideNoteCt[polyIdx]++;
                }
            }
        }

        m_SlideFinished[polyIdx] = true;

        m_NoteQueue[polyIdx][0][0] = totalLength;

        if ((prog.fMono == 0.f && prog.fChordOnOff == 0.f) ||
                (prog.fKnob[kKnobSlideRate] < 0.2)) {
            m_NoteQueue[polyIdx][0][0] = 0;
            m_SlideNoteCt[polyIdx] = 0;
        }
    }


    if (m_NoteCt[polyIdx] > 0 && m_NoteQueue[polyIdx][0][0] == 0) {
        m_Note[polyIdx] = m_NoteQueue[polyIdx][0][1];
        m_MIDINote[polyIdx] = m_NoteQueue[polyIdx][0][1];
        m_Velocity[polyIdx] = (float) m_NoteQueue[polyIdx][0][2] / 128.0f;

        m_SlideFinished[polyIdx] = false;
        m_ForceOff = false;

        m_RateReal[polyIdx] = computeRate(polyIdx, m_Note[polyIdx], prog.fKnob[kKnobTuning]);
        m_RateReal[polyIdx] *= 1.0f + 0.0001f * m_Random[m_RandomCt];

        m_RandomCt++;
        if (m_RandomCt >= NUM_SAMPLES)
            m_RandomCt = 0;

        m_Rate[polyIdx] = m_RateReal[polyIdx];

        m_Pick[polyIdx] = m_NoteQueue[polyIdx][0][3];

        for (int i = 0; i < m_NoteCt[polyIdx] - 1; i++) {
            m_NoteQueue[polyIdx][i][0] = m_NoteQueue[polyIdx][i + 1][0];
            m_NoteQueue[polyIdx][i][1] = m_NoteQueue[polyIdx][i + 1][1];
            m_NoteQueue[polyIdx][i][2] = m_NoteQueue[polyIdx][i + 1][2];
            m_NoteQueue[polyIdx][i][3] = m_NoteQueue[polyIdx][i + 1][3];
        }

        m_NoteCt[polyIdx]--;

        m_ComputeCt[polyIdx] = 0;

        m_NoteOffFade[polyIdx] = 0;
        m_NoteTurningOff[polyIdx] = false;

        if (m_CurrentPan > 0.5f) {
            m_RightDel[polyIdx] = (1.0f - m_Right[polyIdx]) / 100.0f;
            m_LeftDel[polyIdx] = ((1.0f - 2.0f * (m_CurrentPan - 0.5f)) - m_Left[polyIdx]) / 100.0f;
        } else {
            m_RightDel[polyIdx] = (2.0f * m_CurrentPan - m_Right[polyIdx]) / 100.0f;
            m_LeftDel[polyIdx] = (1.0f - m_Left[polyIdx]) / 100.0f;
        }

        m_RightCt[polyIdx] = -100;
        m_LeftCt[polyIdx] = -100;

        m_Pluck[polyIdx] = m_NumPoints[polyIdx] - ((int) ((float) ((m_NumPoints[polyIdx] - 10) / 2) * prog.fPickPosition) + 5);
        if (m_Random[m_RandomCt] > 0.0)
            m_Pluck[polyIdx]++;

        m_PickUpDiff[polyIdx][0] = m_ObjectPos[polyIdx][m_PickUp[polyIdx]];
        m_PickUpDiff[polyIdx][0] += 0.5f * m_ObjectPos[polyIdx][m_PickUp[polyIdx] - 1];
        m_PickUpDiff[polyIdx][0] += 0.5f * m_ObjectPos[polyIdx][m_PickUp[polyIdx] + 1];

        m_PickUpDiff[polyIdx][1] = m_ObjectPos[polyIdx][m_PickUp[polyIdx] + 1];
        m_PickUpDiff[polyIdx][1] += 0.5f * m_ObjectPos[polyIdx][m_PickUp[polyIdx]];
        m_PickUpDiff[polyIdx][1] += 0.5f * m_ObjectPos[polyIdx][m_PickUp[polyIdx] + 2];

        m_PickUp[polyIdx] = ((int) ((float) ((m_NumPoints[polyIdx] - 6) / 2) * prog.fPickUp) + 3);

        m_PickUpDiff[polyIdx][0] -= m_ObjectPos[polyIdx][m_PickUp[polyIdx]];
        m_PickUpDiff[polyIdx][0] -= 0.5f * m_ObjectPos[polyIdx][m_PickUp[polyIdx] - 1];
        m_PickUpDiff[polyIdx][0] -= 0.5f * m_ObjectPos[polyIdx][m_PickUp[polyIdx] + 1];

        m_PickUpDiff[polyIdx][1] -= m_ObjectPos[polyIdx][m_PickUp[polyIdx] + 1];
        m_PickUpDiff[polyIdx][1] -= 0.5f * m_ObjectPos[polyIdx][m_PickUp[polyIdx]];
        m_PickUpDiff[polyIdx][1] -= 0.5f * m_ObjectPos[polyIdx][m_PickUp[polyIdx] + 2];

        int length = (int) ((500.0f +
                (1.0f - (0.5f * prog.fKnob[kKnobPickSpeed] + 0.5f)) *
                (1.0f - (0.5f * prog.fKnob[kKnobPickSpeed] + 0.5f)) *
                (1.0f - (0.5f * prog.fKnob[kKnobPickSpeed] + 0.5f)) *
                20000.0f) * (prog.fPickNoise > 0.5f));
        length += (int) ((1.0f - m_Velocity[polyIdx]) * 2000.0f); // + (50.0f * ((prog.fPickNoise > 0.5f) - 1.0f));
        length += (int) (((float) length * (0.5f * m_Random[m_RandomCt]))); // * (prog.fPickNoise > 0.5f));
        m_RandomCt++;
        if (m_RandomCt >= NUM_SAMPLES)
            m_RandomCt = 0;

        m_TypeOffset[polyIdx] = (int) (length *
                (0.6f * prog.fKnob[kKnobStringType] + 0.2f) *
                (0.6f * prog.fKnob[kKnobStringType] + 0.2f));

        m_PullCt[polyIdx][m_Pluck[polyIdx]][0] = 0;
        m_PullCt[polyIdx][m_Pluck[polyIdx]][1] = length;

        m_ObjectPos[polyIdx][0] = 0.0;
        m_ObjectVel[polyIdx][0] = 0.0;

        m_TypeOffsetPull[polyIdx] = m_TypeOffset[polyIdx];

        m_PickPos[polyIdx] = -1.0f;
        m_PickAdd[polyIdx] = (1.0f + m_Velocity[polyIdx]*2.0f) / (float) length;

        m_DampBridge[polyIdx] = prog.fKnob[kKnobBridgeDamping] * 0.2f + 0.1f;
        m_DampBridge[polyIdx] = (float) (m_DampBridge[polyIdx] * m_DampBridge[polyIdx]);
        m_DampBridge[polyIdx] *= (float) m_NumPoints[polyIdx] / NUM_POINTS;

        float stiff;
        stiff = prog.fPickStiffness + (1.0f - m_Velocity[polyIdx])*0.5f;
        if (stiff > 1.0f)
            stiff = 1.0f;

        m_PickStiff[polyIdx] = (0.1f +
                0.7f *
                (0.5f * (1.0f - stiff) + 0.5f) *
                (0.5f * (1.0f - stiff) + 0.5f));
    }

} // updateNewNotes

/*****************************************************************************/
/* computeBodyResonance : computes the body resonance ;-)                    */
/*****************************************************************************/
// As I said before I never got this right.
// I'm sure someone could do better.

float Revitar::computeBodyResonance(float resPickUp) {
    int i;
    float *p0 = &resonPos[0];
    float resTotalPos = 0.0f;

    for (i = 0; i < NUM_RESON; i++) {
        resTotalPos += resonVol[m_BodyIdx][i] * (*p0);
        p0++;
    }

    float resDrag = 0.0015f * resonDrag[m_BodyIdx];
    float delKnob1 = std::max(0.0f, prog.fKnob[kKnobBodyGain] - 0.6f);
    float maxIn = 0.8f - delKnob1;
    float acc = (std::max(-maxIn, std::min(maxIn, (resPickUp))) - resTotalPos);
    //  float acc = (resPickUp) - resTotalPos;
    //  float range = fabs(resTotalPos);
    //  range = min(1.0, max(0.0, range - 0.3));
    //  range *= range;
    //  float resDragPos = 0.001f;// + range*delKnob1*delKnob1;

    p0 = &resonPos[0];
    float *v0 = &resonVel[0];
    float *r0 = &resonRate[m_BodyIdx][0];
    float *a0 = &resonVol[m_BodyIdx][0];
    float resRate = 0.045f;
    float resOut = 0.0f;
    float resAccRate = resRate*acc;

    // For each resonate frequency of the guitar body compute its velocity and position.
    for (i = 0; i < NUM_RESON; i++) {
        v0[i] += resAccRate * r0[i] - resDrag * v0[i];
        p0[i] += resRate * r0[i] * v0[i] - 0.001f * p0[i];
        resOut += a0[i] * p0[i];
    }

    if (m_BodyIdx == 0)
        resOut *= 2.0f;
    else if (m_BodyIdx == 5)
        resOut *= 1.5f;

    if (m_BodyIdx != m_LastBodyIdx) {
        m_BodyCt = BODY_FADE;
        m_BodyDel = m_LastResOut - resOut;
    }

    if (m_BodyCt > 0) {
        resOut += (float) m_BodyCt * m_BodyDel / (float) BODY_FADE;
        m_BodyCt--;
    }

    m_LastBodyIdx = m_BodyIdx;
    m_LastResOut = resOut;

    return 1.2f * resOut;

} // computeBodyResonance


/*****************************************************************************/
/* processReplacing : processes a buffer, replacing buffer contents          */

/*****************************************************************************/

void Revitar::processReplacing(float **inputs, float **outputs, VstInt32 sampleframes) {
#if 1
    // this does precisely the same, but faster
    memset(outputs[0], 0, sampleframes * sizeof (float));
    memset(outputs[1], 0, sampleframes * sizeof (float));
#else
    // than this.
    int sampleTemp = sampleframes;
    float* out1 = outputs[0];
    float* out2 = outputs[1];
    while (--sampleframes >= 0) {
        *out1++ = 0;
        *out2++ = 0;
    }
    sampleframes = sampleTemp;
    // ... which, BTW, is not perfect anyway, since doing --sampleTemp
    // would remove the need to reset sampleFrames afterwards.
#endif

    process(inputs, outputs, sampleframes);
}

/*****************************************************************************/
/* processEvents : process incoming MIDI events                              */

/*****************************************************************************/

VstInt32 Revitar::processEvents(VstEvents* ev) {
    for (long i = 0; i < ev->numEvents; i++) {
        if ((ev->events[i])->type != kVstMidiType)
            continue;
        VstMidiEvent* event = (VstMidiEvent*) ev->events[i];
        char* midiData = event->midiData;
        m_MIDIData[m_NumMIDIData][0] = event->deltaFrames;
        m_MIDIData[m_NumMIDIData][1] = (int) midiData[0];
        m_MIDIData[m_NumMIDIData][2] = (int) midiData[1] & 0x7f;
        m_MIDIData[m_NumMIDIData][3] = (int) midiData[2] & 0x7f;
        m_NumMIDIData++;

        if (m_NumMIDIData >= MAX_MIDI_DATA)
            m_NumMIDIData = MAX_MIDI_DATA - 1;
    }
    return 1; // want more
}

/*****************************************************************************/
/* turnOff : turns off all playing notes                                     */

/*****************************************************************************/

void Revitar::turnOff(int delta) {
    int j;

    for (j = 0; j < kNumFrequencies; j++)
        m_NumNoteOffs[j] = 0;

    for (j = 0; j < MAX_NUM_POLY; j++) {
        m_NoteTurningOff[j] = true;
        m_NoteOffFade[j] = NOTE_FADE;
        m_SlideFinished[j] = false;
        m_NoteCt[j] = 0;
        m_SlideNoteCt[j] = 0;
    }

    m_ForceOff = true;

    m_CurrentPan = 0.5;
    m_Down = 1;
    m_CurrentChordNote = NO_INFORMATION;
    m_FirstChordNote = NO_INFORMATION;
    m_MonoNote = NO_INFORMATION;
}

/*****************************************************************************/
/* computeRate :                                                             */

/*****************************************************************************/

float Revitar::computeRate
(
        int polyIdx, /* string index                      */
        long note, /* MIDI note                         */
        float tune /* tuning (0=-100,0.5=0,1=100 cents) */
        ) {
    int i;
    float rate;
    float val0;
    /* fill up with .0f                  */
    for (i = m_NumPoints[polyIdx] - 1; i < NUM_POINTS; i++) {
        m_ObjectPos[polyIdx][i] = 0.0f;
        m_ObjectVel[polyIdx][i] = 0.0f;
    }

    /* get Hz for next higher note       */
    float PitchUp = m_PitchUp[polyIdx] = (float) freqtab[(note + 1) & 0x7f] * m_SampleRateMult;
    /* get Hz for current note           */
    float Pitch = m_Pitch[polyIdx] = (float) freqtab[note & 0x7f] * m_SampleRateMult;
    /* get Hz for next lower note        */
    float PitchDown = m_PitchDown[polyIdx] = (float) freqtab[(note - 1) & 0x7f] * m_SampleRateMult;

    rate = 1.0f;
    m_NumPoints[polyIdx] = NUM_POINTS;

    while (rate > 0.8803f && m_NumPoints[polyIdx] > 6) {
        m_NumPoints[polyIdx] -= 2;
        float FreqRate = m_FreqRate[m_NumPoints[polyIdx]];
        if (tune > 0.5f) /* if tuning up                      */ {
            val0 = tune - 0.5f;
            val0 *= 2.0f;
            rate = val0 * FreqRate * PitchUp + (1.0f - val0) * FreqRate * Pitch;
        } else /* if tuning down                    */ {
            val0 = tune * 2.0f;
            rate = (1.0f - val0) * FreqRate * PitchDown + (val0) * FreqRate * Pitch;
        }
    }

    /* fill up with .0f                  */
    for (i = m_NumPoints[polyIdx] - 1; i < NUM_POINTS; i++) {
        m_ObjectPos[polyIdx][i] = 0.0f;
        m_ObjectVel[polyIdx][i] = 0.0f;
    }

    return rate;
}


/*****************************************************************************/
/* noteOn : processes note turn ons                                          */

/*****************************************************************************/

void Revitar::noteOn(long note, long velocity, long delta, int polyIdx, int hammer) {
    if (note > 96)
        return;

    int i = 0;

    while (delta > m_NoteQueue[polyIdx][i][0] && i < m_NoteCt[polyIdx])
        i++;

    m_NoteQueue[polyIdx][i][0] = (int) delta;
    m_NoteQueue[polyIdx][i][1] = (int) note;
    m_NoteQueue[polyIdx][i][2] = (int) velocity;
    m_NoteQueue[polyIdx][i][3] = 1;
    m_NoteCt[polyIdx] = i + 1;

    if (m_SlideFinished[polyIdx] == true && i == 0)
        m_SlideFinished[polyIdx] = false;

    //  fprintf(m_OutFile, "%d %d %d %d     %d     hammer %d\n", i, (int) note, (int) delta, m_MonoNote, m_Note[polyIdx], hammer);
    //  fflush(m_OutFile);

    if (prog.fMono > 0.f) {
        m_NoteQueue[polyIdx][i][3] = 0;

        if (m_MonoNote == NO_INFORMATION) {
            m_MonoNote = note;
            m_NoteQueue[polyIdx][i][3] = 1;
        }

        if (hammer)
            m_NoteQueue[polyIdx][i][3] = 2;
    }

    if (prog.fChordOnOff > 0.f) {
        if (m_FirstChordNote != NO_INFORMATION)
            m_NoteQueue[polyIdx][i][3] = 0;
        else
            m_FirstChordNotes[polyIdx] = note;

        if (hammer)
            m_NoteQueue[polyIdx][i][3] = 2;
    }

    m_NoteOn[polyIdx] = true;
    m_ShutDown = false;
}

/*****************************************************************************/
/* turnOffNote : turns off a note                                            */

/*****************************************************************************/

void Revitar::turnOffNote(int polyIdx) {
    if (prog.fMono > 0.f) {
        m_MonoNote = NO_INFORMATION;
        m_NoteTurningOff[polyIdx] = true; // note off by velocity 0
        m_NoteOffFade[polyIdx] = NOTE_FADE;
        m_NoteCt[polyIdx] = 0;

        for (int j = 0; j < 6; j++) {
            m_NoteTurningOff[j] = true; // note off by velocity 0
            m_NoteOffFade[j] = NOTE_FADE;
        }
    } else {
        if (prog.fChordOnOff == 0.f) {
            m_NoteTurningOff[polyIdx] = true; // note off by velocity 0
            m_NoteOffFade[polyIdx] = NOTE_FADE;
        }

        if (prog.fChordOnOff > 0.f &&
                (m_FirstChordNote == NO_INFORMATION || m_FirstChordNotes[polyIdx] == NO_INFORMATION)) {
            m_NoteTurningOff[polyIdx] = true; // note off by velocity 0
            m_NoteOffFade[polyIdx] = NOTE_FADE;

            if (prog.fSustain == 0.f)
                m_NoteCt[polyIdx] = 0;
        }
    }

    if (m_SlideFinished[polyIdx] == true && prog.fSustain == 0.f) {
        m_SlideFinished[polyIdx] = false;
        m_SlideNoteCt[polyIdx] = 0;

        for (int i = 0; i < m_NoteCt[polyIdx] - 1; i++) {
            m_NoteQueue[polyIdx][i][0] = m_NoteQueue[polyIdx][i + 1][0];
            m_NoteQueue[polyIdx][i][1] = m_NoteQueue[polyIdx][i + 1][1];
            m_NoteQueue[polyIdx][i][2] = m_NoteQueue[polyIdx][i + 1][2];
            m_NoteQueue[polyIdx][i][3] = m_NoteQueue[polyIdx][i + 1][3];
        }

        m_NoteCt[polyIdx]--;
    }

} // turnOffNote

/*****************************************************************************/
/* getOutputProperties : retrieves an output pin's properties                */

/*****************************************************************************/

bool Revitar::getOutputProperties(VstInt32 index, VstPinProperties* properties) {
    if (index < kNumOutputs) {
        sprintf(properties->label, "Vstx %1d", index + 1);
        properties->flags = kVstPinIsActive;
        if (index < 2)
            properties->flags |= kVstPinIsStereo; // test, make channel 1+2 stereo
        return true;
    }
    return false;
}

/*****************************************************************************/
/* getProgramNameIndexed : retrieves the name of one of the programs         */

/*****************************************************************************/

bool Revitar::getProgramNameIndexed(VstInt32 category, VstInt32 index, char* text) {
    if (index < kNumPrograms) {
        strcpy(text, programs[index].name);
        return true;
    }
    return false;
}

/*****************************************************************************/
/* copyProgram : copies the current program to another location              */

/*****************************************************************************/

bool Revitar::DECLARE_VST_DEPRECATED(copyProgram) (VstInt32 destination) {
    if (destination < kNumPrograms) {
        programs[destination] = prog;
        return true;
    }
    return false;
}

/*****************************************************************************/
/* getEffectName : returns the PlugIn's effect name                          */

/*****************************************************************************/

bool Revitar::getEffectName(char* name) {
    strcpy(name, "Revitar 2");
    return true;
}

/*****************************************************************************/
/* getVendorString : returns the vendor string                               */

/*****************************************************************************/

bool Revitar::getVendorString(char* text) {
    strcpy(text, "CutterMusic");
    return true;
}

/*****************************************************************************/
/* getProductString : returns the product string                             */

/*****************************************************************************/

bool Revitar::getProductString(char* text) {
    strcpy(text, "Guitar Synth");
    return true;
}

/*****************************************************************************/
/* canDo : returns whether the PlugIn can do a specific thing                */

/*****************************************************************************/

VstInt32 Revitar::canDo(char* text) {
    if (!strcmp(text, "receiveVstEvents"))
        return 1;
    if (!strcmp(text, "receiveVstMidiEvent"))
        return 1;

    return 0; // explicitly can't do; 0 => don't know
}

/*****************************************************************************/
/* setSampleRate : sets the sample rate                                      */

/*****************************************************************************/

void Revitar::setSampleRate(float sampleRate) {
    int i, polyIdx;

    for (polyIdx = 0; polyIdx < MAX_NUM_POLY; polyIdx++) {
        m_NoteOn[polyIdx] = false;
        m_ComputeCt[polyIdx] = 0;

        for (i = 0; i < NUM_POINTS; i++) {
            m_ObjectPos[polyIdx][i] = 0.0;
            m_ObjectVel[polyIdx][i] = 0.0;
        }
    }

    for (i = 0; i < kNumFrequencies; i++)
        m_NumNoteOffs[i] = 0;

    m_SampleRate = sampleRate;
    m_SampleRateMult = 44100.f / sampleRate;
    AudioEffectX::setSampleRate(sampleRate);
}

/*****************************************************************************/
/* setBlockSize : sets a new maximum block size                              */

/*****************************************************************************/

void Revitar::setBlockSize(VstInt32 blockSize) {
    AudioEffectX::setBlockSize(blockSize);
    // you may need to have to do something here...
}

/*****************************************************************************/
/* handleMIDINoteOn : handles an incoming Note On message                    */
/*****************************************************************************/
// MIDI channel is currently unused

void Revitar::handleMIDINoteOn(int channel, int note, int velocity) {
    int j;
    int polyIdx;
    int offsetBase, offset;
    bool noteAdded = false;

    switch (note) /* process special "notes"           */ {
        case 1:
            if (getParameter(kChordOnOff) == 0.f)
                setParameter(kChordOnOff, 1.f);
            else
                setParameter(kChordOnOff, 0.f);
            break;
        case 2:
            if (getParameter(kSustain) == 0.0f)
                setParameter(kSustain, 1.0f);
            else
                setParameter(kSustain, 0.0f);
            break;
        case 3:
            if (getParameter(kMono) == 0.0f)
                setParameter(kMono, 1.0f);
            else
                setParameter(kMono, 0.0f);
            break;
        case 4:
            curProgram--;
            if (curProgram < 0)
                curProgram = 0;
            setProgram(curProgram);
            break;
        case 5:
            curProgram++;
            if (curProgram >= kNumPrograms)
                curProgram = kNumPrograms - 1;
            setProgram(curProgram);
            break;
        case 6:
            setParameter(kChordSwitch, 0.0f);
            break;
        case 7:
            setParameter(kChordSwitch, 0.5f);
            break;
        case 8:
            setParameter(kChordSwitch, 1.0f);
            break;
        case 9:
            break;
        case 10:
            if (getParameter(kAbsRel) == 0.0)
                setParameter(kAbsRel, 1.0f);
            else
                setParameter(kAbsRel, 0.0f);
            break;
        case 11:
            setParameter(kStopSwitch, 1.0f);
            break;

        default:
            if (note > 96 || note < 12)
                return;
            break;
    }
    /* the rest is real note processing  */
    m_NumNoteOffs[note]++;

    if (prog.fChordOnOff == 0.f) {
        m_CurrentChordNote = NO_INFORMATION;
        polyIdx = 0;

        if (prog.fMono > 0.f)
            polyIdx = 0;
        else {
            int maxCt = -1;
            for (j = 0; j < m_NumPoly; j++) {
                if (m_ComputeCt[j] > maxCt && m_NoteCt[j] == 0) {
                    polyIdx = j;
                    maxCt = m_ComputeCt[j];
                }
            }

            for (j = 0; j < m_NumPoly; j++) {
                if (m_NoteOn[j] == false && m_NoteCt[j] == 0)
                    polyIdx = j;
            }

            for (j = 0; j < m_NumPoly; j++) {
                if (m_NoteOn[j] == false && m_Note[j] == note && m_NoteCt[j] == 0)
                    polyIdx = j;
            }

            for (j = 0; j < m_NumPoly; j++) {
                if (m_NoteOn[j] && m_Note[j] == note && m_NoteCt[j] == 0)
                    polyIdx = j;
            }
        }

        if (m_NoteOn[polyIdx] && m_MIDINote[polyIdx] != note)
            m_NumNoteOffs[m_MIDINote[polyIdx]] = 0;

        if (noteAdded == false) {
            if ((prog.fMono > 0.f) &&
                    m_MonoNote != NO_INFORMATION &&
                    prog.fKnob[kKnobSlideRate] < 0.2) {
                noteOn(note,
                        (long) (2.5f * (0.2f - prog.fKnob[kKnobSlideRate])*(float) velocity),
                        0, polyIdx, 1);
                m_MIDIVelocity[0] = (float) velocity / 127.0f;
            } else {
                noteOn(note, velocity, 0, polyIdx, 0);
            }
        }

        if (prog.fMono > 0.f)
            noteAdded = true;
    }

    if ((prog.fChordOnOff == 1.f) && noteAdded == false) {
        int noteIdx[6];

        if (prog.fChordSwitch == 1.f)
            m_Down = 0;
        else if (prog.fChordSwitch == 0.0)
            m_Down = 1;
        else // if(prog.fChordSwitch == 0.5)
            m_Down = 1 - m_Down;

        VstTimeInfo* myTime = getTimeInfo(kVstPpqPosValid | kVstTempoValid);
        if (myTime) {
            m_CurrentBPM = (float) myTime->tempo;
            if (m_CurrentBPM < 40.f)
                m_CurrentBPM = 40.f;
        }

        float SamplesPerBeat = 60.f / m_CurrentBPM;
        // SamplesPerBeat *= 44100.f;
        SamplesPerBeat *= m_SampleRate;
        offsetBase = (int) ((1.0f - prog.fKnob[kKnobChordRate]) *
                (1.0f - prog.fKnob[kKnobChordRate]) *
                (1.0f - prog.fKnob[kKnobChordRate]) *
                SamplesPerBeat);
        //  offsetBase *= 2;

        if (m_FirstChordNote == note)
            m_FirstChordNote = NO_INFORMATION;

        m_CurrentChordNote = note;

        for (j = 0; j < m_NumPoly; j++)
            m_NoteCt[j] = 0;

        if (prog.fAbsRel > 0.f) {
            int chordIdx = (int) (prog.fChordIdx);

            for (j = 0; j < NUM_CHORD_NOTES; j++) {
                noteIdx[j] = (int) floor(24.0 * prog.fChordNote[chordIdx][j] + 0.0001);
                if (noteIdx[j] != NO_INFORMATION)
                    noteIdx[j] += note;
            }

            for (j = 0; j < NUM_CHORD_NOTES; j++)
                if (noteIdx[j] != NO_INFORMATION)
                    noteIdx[j] += m_Tune[j] - m_Tune[0];
        } else {
            int chordIdx = note % NUM_CHORDS;
            for (j = 0; j < NUM_CHORD_NOTES; j++)
                noteIdx[j] = (int) floor(24.0 * prog.fChordAbsNote[chordIdx][j] + 0.0001);

            int octave = note / 24;
            octave -= 2;

            for (j = 0; j < NUM_CHORD_NOTES; j++) {
                if (noteIdx[j] != NO_INFORMATION) {
                    noteIdx[j] += m_Tune[j];
                    noteIdx[j] += 12 * octave;
                }
            }
        }

        if (!m_Down) {
            if (noteIdx[0] != NO_INFORMATION) {
                if (m_FirstChordNote != NO_INFORMATION && prog.fKnob[kKnobSlideRate] < 0.2) {
                    noteOn(noteIdx[0], (int) (2.5f * (0.2f - prog.fKnob[kKnobSlideRate]) * velocity), 0, 0, 1);
                    m_MIDIVelocity[0] = (float) velocity / 127.0f;
                } else
                    noteOn(noteIdx[0], velocity, 0, 0, 0);
            }

            offset = 0;

            for (j = 1; j < NUM_CHORD_NOTES; j++) {
                if (offset != 0 || noteIdx[j - 1] != NO_INFORMATION) {
                    offset += offsetBase + (int) ((float) offsetBase * 0.5f * m_Random[m_RandomCt]);
                    m_RandomCt++;
                    if (m_RandomCt >= NUM_SAMPLES)
                        m_RandomCt = 0;
                }

                if (noteIdx[j] != NO_INFORMATION) {
                    if (m_FirstChordNote != NO_INFORMATION && prog.fKnob[kKnobSlideRate] < 0.2f) {
                        noteOn(noteIdx[j], (int) (2.5f * (0.2f - prog.fKnob[kKnobSlideRate]) * velocity), offset, j, 1);
                        m_MIDIVelocity[j] = (float) velocity / 127.0f;
                    } else
                        noteOn(noteIdx[j], velocity, offset, j, 0);
                }
            }
        } else {
            if (noteIdx[5] != NO_INFORMATION) {
                if (m_FirstChordNote != NO_INFORMATION && prog.fKnob[kKnobSlideRate] < 0.2f) {
                    noteOn(noteIdx[5], (int) (2.5f * (0.2f - prog.fKnob[kKnobSlideRate]) * velocity), 0, 5, 1);
                    m_MIDIVelocity[5] = (float) velocity / 127.0f;
                } else
                    noteOn(noteIdx[5], velocity, 0, 5, 0);
            }

            offset = 0;

            for (j = 4; j >= 0; j--) {
                if (offset != 0 || noteIdx[j + 1] != NO_INFORMATION) {
                    offset += offsetBase + (int) ((float) offsetBase * 0.5 * m_Random[m_RandomCt]);
                    m_RandomCt++;
                    if (m_RandomCt >= NUM_SAMPLES)
                        m_RandomCt = 0;
                }

                if (noteIdx[j] != NO_INFORMATION) {
                    if (m_FirstChordNote != NO_INFORMATION && prog.fKnob[kKnobSlideRate] < 0.2f) {
                        noteOn(noteIdx[j], (int) (2.5f * (0.2f - prog.fKnob[kKnobSlideRate]) * velocity), offset, j, 1);
                        m_MIDIVelocity[j] = (float) velocity / 127.0f;
                    } else
                        noteOn(noteIdx[j], velocity, offset, j, 0);
                }
            }
        }

        if (m_FirstChordNote == NO_INFORMATION)
            m_FirstChordNote = note;

        noteAdded = true;

        for (j = 0; j < 6; j++)
            m_CurrentChordNotes[j] = noteIdx[j];
    }
}

/*****************************************************************************/
/* handleMIDINoteOff : handles an incoming Note Off message                  */
/*****************************************************************************/
// MIDI channel is currently unused

void Revitar::handleMIDINoteOff(int channel, int note, int velocity) {
    int j;

    switch (note) /* process special "notes"           */ {
        case 10: /* i'm NOT sure whether this is OK...*/
            if (getParameter(kAbsRel) == 0.0)
                setParameter(kAbsRel, 1.0f);
            else
                setParameter(kAbsRel, 0.0f);
            break;
        case 11:
            setParameter(kStopSwitch, 0.0f);
            break;

        default:
            if (note > 96 || note < 12)
                return;
            break;
    }
    /* the rest is real note processing  */
    m_NumNoteOffs[note]--;

    if (m_NumNoteOffs[note] < 0 || prog.fChordOnOff == 1.f)
        m_NumNoteOffs[note] = 0;

    // NB: this is from the original code. It evaluates to "just do it anyway."
    if (1 || m_NumNoteOffs[note] == 0) {
        if (prog.fMono == 0.f) {
            int tempFirstChordNote = m_FirstChordNote;

            for (j = 0; j < m_NumPoly; j++) {
                if (m_MIDINote[j] == note ||
                        (prog.fChordOnOff == 1.0 &&
                        (m_CurrentChordNote == note || tempFirstChordNote == note))) {
                    if (m_FirstChordNote == note)
                        m_FirstChordNote = NO_INFORMATION;

                    if (m_FirstChordNote != NO_INFORMATION &&
                            m_FirstChordNotes[j] != NO_INFORMATION) {
                        if (prog.fKnob[kKnobSlideRate] < 0.2f) {
                            if (m_CurrentChordNotes[j] != NO_INFORMATION)
                                noteOn(m_CurrentChordNotes[j], (int) (5.f * (0.2f - prog.fKnob[kKnobSlideRate])*100.f * m_MIDIVelocity[j]), 0, j, 1);
                            noteOn(m_FirstChordNotes[j], velocity, 5000, j, 0);
                        } else
                            noteOn(m_FirstChordNotes[j], velocity, 0, j, 0);
                    }

                    turnOffNote(j);
                }
            }
        } else {
            if (note == m_MonoNote)
                turnOffNote(0);
            else {
                if (m_MonoNote != NO_INFORMATION &&
                        m_MIDINote[0] == note &&
                        (m_NoteCt[0] == 0)) {
                    if (prog.fKnob[kKnobSlideRate] < 0.2f) {
                        noteOn(note, (int) (5.f * (0.2f - prog.fKnob[kKnobSlideRate]) * 100.f * m_MIDIVelocity[0]), 0, 0, 1);
                        noteOn(m_MonoNote, velocity, 5000, 0, 0);
                    } else
                        noteOn(m_MonoNote, velocity, 0, 0, 0);
                }
            }
        }
    }
}

/*****************************************************************************/
/* handleController : handles an incoming MIDI CC message                    */
/*****************************************************************************/
// MIDI channel is currently unused

void Revitar::handleController(int channel, int ctrl, int value) {
    float fMult; /* value multiplicator               */
    int nOld; /* previous value for this ctrl      */
    /* remember MIDI controller data     */
    if ((m_ControllerBits > 7) && /* handle 14-bit MIDI controllers    */
            (ctrl >= 0) && (ctrl < NUM_MIDI_CC)) {
        int nHi, nLo;
        nOld = (m_MIDICtrlVal[ctrl & 0x1f] << 7) |
                m_MIDICtrlVal[ctrl | 0x20];
        if ((ctrl >= 0) && (ctrl < 32)) {
            nHi = value;
            nLo = m_MIDICtrlVal[ctrl + 32];
        } else if ((ctrl >= 32) && (ctrl < NUM_MIDI_CC)) {
            nHi = m_MIDICtrlVal[ctrl - 32];
            nLo = value;
        }
        m_MIDICtrlVal[ctrl] = value;
        ctrl &= 0x1f;
        value = (nHi << 7) | nLo;
        fMult = (1.f / 16383.f);
    } else /* or 7-bit MIDI controllers         */ {
        nOld = m_MIDICtrlVal[ctrl];
        m_MIDICtrlVal[ctrl] = value;
        fMult = (1.f / 127.f);
    }

    if (value == nOld) /* if no change                      */
        return; /* do nothing                        */

    switch (ctrl) /* OK, which controller was it ?     */ {
        case 10: /* Pan                               */
            m_CurrentPan = (float) value * fMult;
            break;
        case 64: /* Sustain switch                    */
            setParameter(kSustain, (value) ? 1.0f : 0.f);
            break;
        case 123: /* All Notes Off                     */
            turnOff(0);
            break;
        case 126: /* Mono Mode On                      */
            turnOff(0);
            break;
        default: /* any other controller              */
            if (ctrl > 0 && ctrl < NUM_MIDI_CC) /* it might be one of the parm ones  */ {
                setParameter(m_MIDIControl[ctrl], (float) value * fMult);
                m_LastMIDICC = ctrl;
            }
            break;
    }
}

/*****************************************************************************/
/* handleProgramChange : handles an incoming program change message          */
/*****************************************************************************/
// MIDI channel is currently unused

void Revitar::handleProgramChange(int channel, int value) {
    if (value >= 0 && value < kNumPrograms)
        setProgram(value);
}

/*****************************************************************************/
/* handlePitchWheel : handles an incoming pitch wheel message                */
/*****************************************************************************/
// MIDI channel is currently unused

void Revitar::handlePitchWheel(int channel, int value) {
    double dBendFactor = (((float) value) / 8192.) - 1.;
    // little 1% fuse against uncalibrated wheels...
    if ((dBendFactor >= -0.01) && (dBendFactor <= 0.01))
        dBendFactor = 0.;
    double dCents = dBendFactor * m_BendRange;
    m_CurrentBendDest = (float) exp(dCents * 0.00057762265046662107);
}

/*****************************************************************************/
/* handleMIDIData : processes accumulated MIDI data                          */

/*****************************************************************************/

void Revitar::handleMIDIData() {
    int i, j;
    int status;
    int channel;
    int velocity;
    int note;

    for (i = 0; i < m_NumMIDIData; i++) {
        m_MIDIData[i][0]--;

        if (m_MIDIData[i][0] < 0) {
            status = m_MIDIData[i][1] & 0xf0;
            channel = m_MIDIData[i][1] & 0x0f;
            note = m_MIDIData[i][2];
            velocity = m_MIDIData[i][3];

            switch (status) {
                case 0x80: /* Not Off ?                         */
                    velocity = 0;
                    // fall thru to...
                case 0x90: /* Note On ?                         */
                    if (velocity)
                        handleMIDINoteOn(channel, note, velocity);
                    else
                        handleMIDINoteOff(channel, note, velocity);
                    break;
                case 0xB0: /* CC ?                              */
                    handleController(channel, note, velocity);
                    break;
                case 0xC0: /* Program Change ?                  */
                    handleProgramChange(channel, note);
                    break;
                case 0xE0: /* Pitch Wheel ?                     */
                    handlePitchWheel(channel, (velocity << 7) | note);
                    break;
            }
        }
    }

    for (i = 0, j = 0; i < m_NumMIDIData; i++) {
        if (i != j) {
            m_MIDIData[j][0] = m_MIDIData[i][0];
            m_MIDIData[j][1] = m_MIDIData[i][1];
            m_MIDIData[j][2] = m_MIDIData[i][2];
            m_MIDIData[j][3] = m_MIDIData[i][3];
        }

        if (m_MIDIData[i][0] >= 0)
            j++;
    }

    m_NumMIDIData = j;

} // handleMIDIData



#ifdef  TUNE_NOW

if (m_TestsDone < 20 && m_ZeroCtGood > 2000) {
    if (m_LastZero < 0.0 && m_ToneVals[2] > 0.0) {
        m_ZeroCt2++;
        //  fprintf(m_OutFile, "                               %f\n", 44100.0/((float) m_ZeroCt));
        m_ZeroCt = 0;

        if (m_ZeroCt2 == 100) {
            if (m_TestsDone > 5) {
                val1 = 44100.0 / ((float) m_ZeroTotal / 100.0);
                val1 /= m_Pitch[0];
                m_FreqRate[m_NumPoints[0]] /= val1;
                m_Rate[0] = m_FreqRate[m_NumPoints[0]] * m_Pitch[0];
                //  fprintf(m_OutFile, "rate     %f\n", m_FreqRate[m_NumPoints[0]]);
            }

            if (m_TestsDone == 19) {
                for (i = 20; i < 60; i++) {
                    fprintf(m_OutFile, "m_FreqRate[%d] = %1.10f;\n", i, m_FreqRate[i]);

                }
                fprintf(m_OutFile, "\n");
                fflush(m_OutFile);
            }

            fprintf(m_OutFile, "%d  %f\n", m_TestsDone, 44100.0 / ((float) m_ZeroTotal / 100.0));
            m_ZeroTotal = 0;
            m_ZeroCt2 = 0;

            m_TestsDone++;
        }
    }

    m_LastZero = m_ToneVals[2];
    m_ZeroCt++;
    m_ZeroTotal++;
}

m_ZeroCtGood++;
#endif

#ifdef  TUNE_NOW
m_ZeroCtGood = 0;
m_TestsDone = 0;
m_ZeroCt = 0;
m_ZeroCt2 = 0;
m_ZeroTotal = 0;
#endif


