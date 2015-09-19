/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Copyright (C) 2004 C. Lawrence Zitnick III <larryz@microsoft.com>
 * Copyright (C) 2010 Hermann Seib <him@hermannseib.com>
 * Copyright (C) 2014 Asseca <http://www.asseca.org/revitar.html>
 * Copyright (C) 2015 Markus Kitsinger (SwooshyCueb) <root@swooshalicio.us>
 */


//#include <windows.h>
#include <math.h>
#include <stdio.h>

#include "resource.h"
#include "RevEditor.h"
#include "Revitar.h"
#include "controls.h"
#define BOOST_THREAD_USE_LIB

#define KNOB_X                  44
#define KNOB_Y                  44
#define ROUND_BUTTON_SIZE       22
#define SQUARE_BUTTON_SIZE      24
#define KNOB_POSITIONS          61

int lastIndex;
float noise;

//-----------------------------------------------------------------------------
// RevEditor class implementation
//-----------------------------------------------------------------------------

RevEditor::RevEditor(AudioEffect *effect)
//        RevEditor::RevEditor (void *effect = 0)
: AEffGUIEditor(effect) {
    int i;

    for (i = 0; i < NUM_KNOBS; i++) {
        knob[i] = 0;
    }

    mono = 0;
    sustain = 0;
    picknoise = 0;
    about = 0;
    meter = 0;
    chords = 0;
    chordSwitch = 0;
    guitarBottom = 0;
    guitarTop = 0;
    palmSlider = 0;
    stopSwitch = 0;
    absRel = 0;
    MIDIDisplay = 0;
    chordOnOff = 0;
    bodySwitch = 0;
    pickSwitch = 0;
    paramDisplay = 0;
    paramDisplay2 = 0;
    displayScreen0 = 0;

    hKnob = 0;
    hKnobTune = 0;
    hKnobSlide = 0;
    hVuMeter = 0;
    hChordDirection = 0;
    hAbout = 0;
    hDisplayBack = 0;
    hChordVert = 0;
    hBtnSquare = 0;
    hBtnRound = 0;
    hBodySelector = 0;
    hCtlPickType = 0;
    hDisplayBackground = 0;
    hPickPos = 0;
    hPickupPos = 0;
    hChordNoteOn = 0;
    hBodies = 0;
    hGuitarBottom = 0;
    hGuitarTop = 0;
    hPalmSliderBackground = 0;
    hPalmSliderHandle = 0;
    hBtnAbsRel = 0;
    hBtnStop = 0;

    // load the background bitmap
    // we don't need to load all bitmaps, this could be done when open is called
    hBackground = new CBitmap(BACKGROUND_BETA_8_3);

    // init the size of the plugin
    rect.left = 0;
    rect.top = 0;
    rect.right = 696; //(short)hBackground->getWidth ();
    rect.bottom = 432; //(short)hBackground->getHeight ();
}

//-----------------------------------------------------------------------------

RevEditor::~RevEditor() {
    // free background bitmap
    if (hBackground)
        hBackground->forget();
    hBackground = 0;

    // free some bitmaps
    if (hKnob)
        hKnob->forget();
    hKnob = 0;

    if (hKnobTune)
        hKnobTune->forget();
    hKnobTune = 0;

    if (hBtnSquare)
        hBtnSquare->forget();
    hBtnSquare = 0;

    if (hBtnRound)
        hBtnRound->forget();
    hBtnRound = 0;

    if (hVuMeter)
        hVuMeter->forget();
    hVuMeter = 0;

    if (hChordDirection)
        hChordDirection->forget();
    hChordDirection = 0;

    if (hAbout)
        hAbout->forget();
    hAbout = 0;

    if (hDisplayBack)
        hDisplayBack->forget();
    hDisplayBack = 0;

    if (hChordVert)
        hChordVert->forget();
    hChordVert = 0;

    if (hBodySelector)
        hBodySelector->forget();
    hBodySelector = 0;

    if (hCtlPickType)
        hCtlPickType->forget();
    hCtlPickType = 0;

    if (hDisplayBackground)
        hDisplayBackground->forget();
    hDisplayBackground = 0;

    if (hPickPos)
        hPickPos->forget();
    hPickPos = 0;

    if (hPickupPos)
        hPickupPos->forget();
    hPickupPos = 0;

    if (hChordNoteOn)
        hChordNoteOn->forget();
    hChordNoteOn = 0;

    if (hBodies)
        hBodies->forget();
    hBodies = 0;

    if (hGuitarBottom)
        hGuitarBottom->forget();
    hGuitarBottom = 0;

    if (hGuitarTop)
        hGuitarTop->forget();
    hGuitarTop = 0;

    if (hPalmSliderBackground)
        hPalmSliderBackground->forget();
    hPalmSliderBackground = 0;

    if (hPalmSliderHandle)
        hPalmSliderHandle->forget();
    hPalmSliderHandle = 0;

    if (hBtnStop)
        hBtnStop->forget();
    hBtnStop = 0;

    if (hBtnAbsRel)
        hBtnAbsRel->forget();
    hBtnAbsRel = 0;

    if (hKnobSlide)
        hKnobSlide->forget();
    hKnobSlide = 0;

}

void printMIDI(float value, char *string) {
    int i = (int) value;

    sprintf(string, " ", (int) value);

    if (i != NO_INFORMATION) {
        sprintf(string, "%d", (int) value);
    }

}

void convertParams(float value, char *string, void *userdata) {
    RevEditor *editor = (RevEditor *) userdata;

    switch (lastIndex) {
        case kGain: sprintf(string, " Gain ");
            break;
        case kBodyGain: sprintf(string, " Body Gain ");
            break;
        case kPickVolume: sprintf(string, " Pick Volume ");
            break;
        case kTuning: sprintf(string, " Tune ");
            break;
        case kBridgeDamping: sprintf(string, " Bridge Damping ");
            break;
        case kStringDamping: sprintf(string, " String Damping ");
            break;
        case kVibratoAmplit: sprintf(string, " Vibrato Amplitude ");
            break;
        case kVibratoRate: sprintf(string, " Vibrato Rate ");
            break;
        case kSympathetic: sprintf(string, " Sympathetic ");
            break;
        case kSlap: sprintf(string, " Slap ");
            break;
        case kPickSpeed: sprintf(string, " Pick Speed ");
            break;
        case kChordRate: sprintf(string, " Chord Rate ");
            break;
        case kStringType: sprintf(string, " String Type ");
            break;
        case kPalmDamp: sprintf(string, " Palm Damping ");
            break;
        case kSlideRate: sprintf(string, (value > 0.2) ? " Slide Rate ": " Hammer / Pull ");
            break;
        case kMono: sprintf(string, (value < 0.5f) ? " Polyphonic ": " Monophonic ");
            break;
        case kAbsRel: sprintf(string, (value < 0.5f) ? " Absolute ": " Relative ");
            break;
        case kSustain: sprintf(string, " Sustain ");
            break;
        case kPickNoise: sprintf(string, " Pluck Noise ");
            break;
        case kChord: sprintf(string, " Chord Type ");
            break;
        case kChordSwitch: sprintf(string, " Strum Direction ");
            break;
        case kBodySwitch: sprintf(string, " Body Type ");
            break;
        case kChordOnOff: sprintf(string, " Chord Play ");
            break;
        case kPickSwitch: sprintf(string, " Pick Width ");
            break;
        case kPickupPos: sprintf(string, " Pick Up Position ");
            break;
        case kPickPos: sprintf(string, " Pick Position ");
            break;
        case kChordNote0: sprintf(string, " Chord Note 0 ");
            break;
        case kChordNote1: sprintf(string, " Chord Note 1 ");
            break;
        case kChordNote2: sprintf(string, " Chord Note 2 ");
            break;
        case kChordNote3: sprintf(string, " Chord Note 3 ");
            break;
        case kChordNote4: sprintf(string, " Chord Note 4 ");
            break;
        case kChordNote5: sprintf(string, " Chord Note 5 ");
            break;
        case kPalmPos: sprintf(string, " Palm Position ");
            break;
        default: strcpy(string, "  ");
            break;
    }

} // convertParams

void convertParams2(float value, char *string, void *userdata) {
    int note;
    RevEditor *editor = (RevEditor *) userdata;
    Revitar *effect = (Revitar *) editor->getEffect();

    switch (lastIndex) {
        case kGain: sprintf(string, " %1.2f ", value);
            break;
        case kBodyGain: sprintf(string, " %1.2f ", value);
            break;
        case kPickVolume: sprintf(string, " %1.2f ", value);
            break;
        case kTuning: sprintf(string, " %d cents ", (int) (200.0 * value - 100.0));
            break;
        case kBridgeDamping: sprintf(string, " %1.2f ", value);
            break;
        case kStringDamping: sprintf(string, " %1.2f ", value);
            break;
        case kVibratoAmplit: sprintf(string, " %1.2f ", value);
            break;
        case kVibratoRate: sprintf(string, " %1.2f ", value);
            break;
        case kSympathetic: sprintf(string, " %1.2f ", value);
            break;
        case kSlap: sprintf(string, " %1.2f ", value);
            break;
        case kPickSpeed: sprintf(string, " %1.2f ", value);
            break;
        case kChordRate: sprintf(string, " %1.2f ", value);
            break;
        case kStringType: sprintf(string, " %1.2f ", value);
            break;
        case kPalmDamp: sprintf(string, " %1.2f ", value);
            break;
        case kSlideRate: sprintf(string, " %1.2f ", (value > 0.2f) ? (value - 0.2f) / 0.8f: 5.0f * (0.2f - value));
            break;
        case kPalmPos: sprintf(string, " %1.2f ", value);
            break;
        case kPickPos: sprintf(string, " %1.2f ", value);
            break;
        case kPickupPos: sprintf(string, " %1.2f ", value);
            break;
        case kPickSwitch: sprintf(string, " and Stiffness ");
            break;
        case kAbsRel: sprintf(string, " Position ");
            break;
        case kChordNote0:
        case kChordNote1:
        case kChordNote2:
        case kChordNote3:
        case kChordNote4:
        case kChordNote5:
            if (value < -0.01f)
                sprintf(string, " Off ", value);
            else {
                note = (int) (value * 24.0f) + effect->GetTune(lastIndex - kChordNote0);
                static char NoteName[12][3] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

                sprintf(string, " %s ", NoteName[note % 12]);
            }
            break;
        case kChordSwitch:
            if (value >= -0.1f && value < 0.1f)
                sprintf(string, " Up ");
            else if (value >= 0.4f && value < 0.6f)
                sprintf(string, " Down and Up ");
            else if (value >= 0.9f && value < 1.1f)
                sprintf(string, " Down ");
            break;
        case kChordOnOff:
        case kSustain:
            sprintf(string, (value > 0.f) ? " On " : " Off ");
            break;
        case kPickNoise:
            sprintf(string, (value > 0.5f) ? " Full " : " Reduced ");
            break;

        default:
            strcpy(string, "  ");
            break;
    }

} // convertParams

//-----------------------------------------------------------------------------

bool RevEditor::open(void *ptr) {
    // !!! always call this !!!
    AEffGUIEditor::open(ptr);

    // load some bitmaps
    if (!hKnob)
        hKnob = new CBitmap(KNOB_SHADOW_1);
    if (!hKnobTune)
        hKnobTune = new CBitmap(TUNE_KNOB_0);
    if (!hBtnSquare)
        hBtnSquare = new CBitmap(IDB_BITMAP39);
    if (!hBtnRound)
        hBtnRound = new CBitmap(IDB_BITMAP38);
    if (!hAbout)
        hAbout = new CBitmap(ABOUT_0);
    if (!hVuMeter)
        hVuMeter = new CBitmap(VU_METER_0);
    if (!hChordDirection)
        hChordDirection = new CBitmap(CHORD_DIRECTION_1);
    if (!hBodySelector)
        hBodySelector = new CBitmap(BODY_SELECTOR0);
    if (!hCtlPickType)
        hCtlPickType = new CBitmap(PICK_TYPE_3);
    if (!hDisplayBack)
        hDisplayBack = new CBitmap(IDB_BITMAP25);
    if (!hChordVert)
        hChordVert = new CBitmap(IDB_BITMAP28);
    if (!hDisplayBackground)
        hDisplayBackground = new CBitmap(STRING_BACKGROUND_0);
    if (!hPickupPos)
        hPickupPos = new CBitmap(PICK_UP_1);
    if (!hPickPos)
        hPickPos = new CBitmap(PICK_1);
    if (!hChordNoteOn)
        hChordNoteOn = new CBitmap(CHORD_ON_0);
    if (!hBodies)
        hBodies = new CBitmap(BODIES_2);
    if (!hGuitarBottom)
        hGuitarBottom = new CBitmap(GUITAR_BOTTOM_1);
    if (!hGuitarTop)
        hGuitarTop = new CBitmap(GUITAR_TOP_1);
    if (!hPalmSliderBackground)
        hPalmSliderBackground = new CBitmap(PALM_SLIDER_BACKGROUND_0);
    if (!hPalmSliderHandle)
        hPalmSliderHandle = new CBitmap(PALM_SLIDER_HANDLE_1);
    if (!hBtnStop)
        hBtnStop = new CBitmap(STOP_SWITCH_0);
    if (!hBtnAbsRel)
        hBtnAbsRel = new CBitmap(ROCKER_SWITCH_4);
    if (!hKnobSlide)
        hKnobSlide = new CBitmap(KNOB_SLIDE_0);

    //--init background frame-----------------------------------------------
    CRect size(0, 0, 696, 432);
    CFrame* lFrame = new CFrame(size, ptr, this);
    lFrame->setBackground(hBackground);


    //--init the meter ------------------------------------------------

    size(415, 171,
            415 + 62, 171 + 62);
    meter = new CVuMeterCircle(size, hVuMeter, hVuMeter, 40, kVertical);
    meter->setValue(0.0f); //effect->getVu());
    lFrame->addView(meter);

    CPoint point(0, 0);

    //--init the chord switch------------------------------------------------

    size(603, 170,
            603 + 17, 170 + 62);
    chordSwitch = new CVerticalSwitch20(size, this, kChordSwitch, 3, 62, 3, hChordDirection, point);
    chordSwitch->setValue(effect->getParameter(kChordSwitch));
    lFrame->addView(chordSwitch);

    //--init the body selector------------------------------------------------

    size(26, 291,
            26 + 100, 291 + 56);
    bodySwitch = new C2DSwitch(size, this, kBodySwitch, 6, 56, 2, 3, hBodySelector, point);
    bodySwitch->setValue(effect->getParameter(kBodySwitch));
    lFrame->addView(bodySwitch);

    //--init the pick switch------------------------------------------------

    size(182, 288,
            182 + 60, 288 + 60);
    pickSwitch = new C2DSwitch(size, this, kPickSwitch, 100, 60, 10, 10, hCtlPickType, point);
    pickSwitch->setHorz(effect->getParameter(kPickStiffness));
    pickSwitch->setVert(effect->getParameter(kPickWidth));
    lFrame->addView(pickSwitch);

    //--init the stop switch------------------------------------------------
    size(550, 262,
            550 + 44, 262 + 50);
    stopSwitch = new CKickButton2(size, this, kStopSwitch, 50, hBtnStop, point);
    stopSwitch->setValue(0.0f);
    lFrame->addView(stopSwitch);

    //--init display screen 0------------------------------------------------
    size(6, 71,
            6 + DISPLAY_WIDTH, 71 + DISPLAY_HEIGHT);
    displayScreen0 = new CDisplayScreen(size, this, kDS0, hDisplayBackground);
    displayScreen0->pDisplay = new CBitmap(BODIES_2); // why can't we use hBodies here?
    displayScreen0->updateBitmaps(hBodies, hPickupPos, hPickPos, hChordNoteOn);
    displayScreen0->m_pChordDisplayUpdate = &(((Revitar*) effect)->m_ChordDisplayUpdate);
    displayScreen0->m_ChordDisplay = (((Revitar*) effect)->m_ChordDisplay);
    displayScreen0->setChordOnValue(effect->getParameter(kChordOnOff));
    displayScreen0->setBodyValue(effect->getParameter(kBodySwitch));
    displayScreen0->m_fPickupPos = (effect->getParameter(kPickupPos));
    displayScreen0->m_fPickPos = (effect->getParameter(kPickPos));
    displayScreen0->m_iChordNotes[0] = (int) floor(24.f * effect->getParameter(kChordNote0) + 0.0001f);
    displayScreen0->m_iChordNotes[1] = (int) floor(24.f * effect->getParameter(kChordNote1) + 0.0001f);
    displayScreen0->m_iChordNotes[2] = (int) floor(24.f * effect->getParameter(kChordNote2) + 0.0001f);
    displayScreen0->m_iChordNotes[3] = (int) floor(24.f * effect->getParameter(kChordNote3) + 0.0001f);
    displayScreen0->m_iChordNotes[4] = (int) floor(24.f * effect->getParameter(kChordNote4) + 0.0001f);
    displayScreen0->m_iChordNotes[5] = (int) floor(24.f * effect->getParameter(kChordNote5) + 0.0001f);
    displayScreen0->kPickPos = kPickPos;
    displayScreen0->kPickupPos = kPickupPos;

    lFrame->addView(displayScreen0);

    //size(6, 7, 290, 72);
    size(6, 7, 6 + 285, 7 + 65); //285 x 65
    guitarTop = new CVerticalSwitch20(size, this, kGuitarTop, 6, 65, 6, hGuitarTop, point);
    guitarTop->setValue(effect->getParameter(kGuitarTop));
    guitarTop->setMouseEnabled(false);
    guitarTop->setValue(effect->getParameter(kBodySwitch));
    lFrame->addView(guitarTop);

    //size(6, 149, 285, 225);
    size(6, 149, 6 + 280, 149 + 76); //280 x 76
    guitarBottom = new CVerticalSwitch20(size, this, kGuitarBottom, 6, 76, 6, hGuitarBottom, point);
    guitarBottom->setValue(effect->getParameter(kGuitarBottom));
    guitarBottom->setMouseEnabled(false);
    guitarBottom->setValue(effect->getParameter(kBodySwitch));
    lFrame->addView(guitarBottom);

    //--init the palm slider------------------------------------------------
    size(350, 275,
            350 + 190, 275 + 18);
    CPoint offsetPalm(0, 0);
    palmSlider = new CHorizontalSlider2(size, this,
            kPalmPos, 352, 431, hPalmSliderHandle,
            hPalmSliderBackground, offsetPalm, kLeft);
    // hPalmSliderHandle -> hSliderPalmPos
    // hPalmSliderBackground -> NULL
    // offsetPalm -> point
    palmSlider->setValue(effect->getParameter(kPalmPos));
    lFrame->addView(palmSlider);

    //--init the buttons------------------------------------------------

    // Mono
    size(343, 196,
            343 + ROUND_BUTTON_SIZE, 196 + ROUND_BUTTON_SIZE);
    mono = new COnOffButtonW(size, this, kMono, hBtnRound);
    mono->setValue(effect->getParameter(kMono));
    lFrame->addView(mono);

    // Chord ON/Off
    size(499, 173,
            499 + SQUARE_BUTTON_SIZE, 173 + SQUARE_BUTTON_SIZE);
    chordOnOff = new COnOffButtonW(size, this, kChordOnOff, hBtnSquare);
    chordOnOff->setValue(effect->getParameter(kChordOnOff));
    lFrame->addView(chordOnOff);

    // Sustain
    size(303, 196,
            303 + ROUND_BUTTON_SIZE, 196 + ROUND_BUTTON_SIZE);
    sustain = new COnOffButtonW(size, this, kSustain, hBtnRound);
    sustain->setValue(effect->getParameter(kSustain));
    lFrame->addView(sustain);

    // Pluck Noise
    size(250, 253,
            250 + ROUND_BUTTON_SIZE, 253 + ROUND_BUTTON_SIZE);
    picknoise = new COnOffButtonW(size, this, kPickNoise, hBtnRound);
    picknoise->setValue(effect->getParameter(kPickNoise));
    lFrame->addView(picknoise);

    // Absolute/Relative Position
    size(576, 188,
            576 + 21, 188 + 46);
    absRel = new COnOffButtonW(size, this, kAbsRel, hBtnAbsRel);
    absRel->setValue(effect->getParameter(kAbsRel));
    lFrame->addView(absRel);

    //--init the knobs------------------------------------------------
    CPoint offset(0, 0);

    // Gain
    size(372, 175,
            372 + KNOB_X, 175 + KNOB_Y);
    knob[0] = new CAnimKnobZ(size, this, kGain, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[0]->setValue(effect->getParameter(kGain));
    lFrame->addView(knob[0]);

    // Body Gain
    size(27, 355,
            27 + KNOB_X, 355 + KNOB_Y);
    knob[1] = new CAnimKnobZ(size, this, kBodyGain, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[1]->setValue(effect->getParameter(kBodyGain));
    lFrame->addView(knob[1]);

    // Pick Volume
    size(220, 355,
            220 + KNOB_X, 355 + KNOB_Y);
    knob[2] = new CAnimKnobZ(size, this, kPickVolume, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[2]->setValue(effect->getParameter(kPickVolume));
    lFrame->addView(knob[2]);

    // Tune
    size(601, 64,
            601 + KNOB_X, 64 + KNOB_Y);
    knob[3] = new CAnimKnobZ(size, this, kTuning, KNOB_POSITIONS, KNOB_Y, hKnobTune, point);
    knob[3]->setValue(effect->getParameter(kTuning));
    lFrame->addView(knob[3]);

    // Bridge
    size(446, 355,
            446 + KNOB_X, 355 + KNOB_Y);
    knob[4] = new CAnimKnobZ(size, this, kBridgeDamping, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[4]->setValue(effect->getParameter(kBridgeDamping));
    lFrame->addView(knob[4]);

    // String
    size(495, 355,
            495 + KNOB_X, 355 + KNOB_Y);
    knob[5] = new CAnimKnobZ(size, this, kStringDamping, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[5]->setValue(effect->getParameter(kStringDamping));
    lFrame->addView(knob[5]);

    // Amp
    size(625, 355,
            625 + KNOB_X, 355 + KNOB_Y);
    knob[6] = new CAnimKnobZ(size, this, kVibratoAmplit, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[6]->setValue(effect->getParameter(kVibratoAmplit));
    lFrame->addView(knob[6]);

    // Rate
    size(625, 288,
            625 + KNOB_X, 288 + KNOB_Y);
    knob[7] = new CAnimKnobZ(size, this, kVibratoRate, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[7]->setValue(effect->getParameter(kVibratoRate));
    lFrame->addView(knob[7]);

    // Sympathetic
    size(81, 355,
            81 + KNOB_X, 355 + KNOB_Y);
    knob[8] = new CAnimKnobZ(size, this, kSympathetic, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[8]->setValue(effect->getParameter(kSympathetic));
    lFrame->addView(knob[8]);

    // Slap
    size(341, 355,
            341 + KNOB_X, 355 + KNOB_Y);
    knob[9] = new CAnimKnobZ(size, this, kSlap, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[9]->setValue(effect->getParameter(kSlap));
    lFrame->addView(knob[9]);

    // Speed 
    size(161, 355,
            161 + KNOB_X, 355 + KNOB_Y);
    knob[10] = new CAnimKnobZ(size, this, kPickSpeed, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[10]->setValue(effect->getParameter(kPickSpeed));
    lFrame->addView(knob[10]);

    // Chord Rate
    size(625, 175,
            625 + KNOB_X, 175 + KNOB_Y);
    knob[11] = new CAnimKnobZ(size, this, kChordRate, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[11]->setValue(effect->getParameter(kChordRate));
    lFrame->addView(knob[11]);

    // Type
    size(293, 355,
            293 + KNOB_X, 355 + KNOB_Y);
    knob[12] = new CAnimKnobZ(size, this, kStringType, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[12]->setValue(effect->getParameter(kStringType));
    lFrame->addView(knob[12]);

    // Palm
    size(398, 355,
            398 + KNOB_X, 355 + KNOB_Y);
    knob[13] = new CAnimKnobZ(size, this, kPalmDamp, KNOB_POSITIONS, KNOB_Y, hKnob, point);
    knob[13]->setValue(effect->getParameter(kPalmDamp));
    lFrame->addView(knob[13]);

    // Slide Rate
    size(552, 355,
            552 + KNOB_X, 355 + KNOB_Y);
    knob[14] = new CAnimKnobZ(size, this, kSlideRate, KNOB_POSITIONS, KNOB_Y, hKnobSlide, point);
    knob[14]->setValue(effect->getParameter(kSlideRate));
    lFrame->addView(knob[14]);

    size(506, 206,
            506 + 65, 206 + 15);
    chords = new COptionMenuW(size, this, kChord, 0, 0, 0);
    updateChordMenu();
    chords->setCurrent((int) effect->getParameter(kChord));
    chords->setNbItemsPerColumn(12);
    lFrame->addView(chords);

    CRect sizeAbout(470, 13, 625, 51);
    size(131, 114,
            131 + 434, 114 + 204);
    about = new CSplashScreen(sizeAbout, this, kAbout, hAbout, size, point);
    lFrame->addView(about);

    size(364, 27,
            364 + 90, 27 + 15);
    //CColor background = { 186, 109, 56, 0};
    //CColor background = { 39, 40, 40, 0};
    CColor background = {34, 35, 35, 0};
    CColor textColor = {235, 220, 180, 0};
    CColor textColor2 = {20, 10, 0, 0};
    paramDisplay = new CParamDisplay(size, 0, kShadowText);
    paramDisplay->setStringConvert(convertParams, this);
    paramDisplay->setBackColor(background);
    paramDisplay->setFrameColor(background);
    paramDisplay->setFontColor(textColor);
    paramDisplay->setShadowColor(textColor2);
    paramDisplay->setFont(kNormalFontSmall);
    lFrame->addView(paramDisplay);

    size(364, 23 + 20,
            364 + 90, 23 + 35);
    paramDisplay2 = new CParamDisplay(size, 0, kShadowText);
    paramDisplay2->setBackColor(background);
    paramDisplay2->setFrameColor(background);
    paramDisplay2->setFontColor(textColor);
    paramDisplay2->setShadowColor(textColor2);
    paramDisplay2->setFont(kNormalFontSmall);
    paramDisplay2->setStringConvert(convertParams2, this);
    lFrame->addView(paramDisplay2);

    size(663, 26,
            680, 37);
    CColor textColor3 = {185, 170, 140, 0};
    //CColor background2 = { 159, 77, 36, 0};
    CColor background2 = {39, 40, 40, 0};
    MIDIDisplay = new CParamDisplay(size, 0, kShadowText);
    MIDIDisplay->setStringConvert(printMIDI);
    MIDIDisplay->setBackColor(background2);
    MIDIDisplay->setFrameColor(background2);
    MIDIDisplay->setFontColor(textColor3);
    MIDIDisplay->setShadowColor(textColor2);
    MIDIDisplay->setFont(kNormalFontSmall);
    lFrame->addView(MIDIDisplay);

    lastIndex = -1;

    frame = lFrame;

    return true;
}


//-----------------------------------------------------------------------------

void RevEditor::close() {
    hasVstWheel = hasFocus = false;
    delete frame;
    frame = 0;
}


//-----------------------------------------------------------------------------

void RevEditor::setParameter(VstInt32 index, float value) {
    if (!frame)
        return;

    lastIndex = index;

    noise = 0.0;

    // called from ADelayEdit
    switch (index) {
        case kGain:
        case kBodyGain:
        case kPickVolume:
        case kTuning:
        case kBridgeDamping:
        case kStringDamping:
        case kVibratoAmplit:
        case kVibratoRate:
        case kSympathetic:
        case kSlap:
        case kPickSpeed:
        case kChordRate:
        case kStringType:
        case kPalmDamp:
        case kSlideRate:
            if (knob[index])
                knob[index]->setValue(effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            break;

        case kMono:
            if (mono)
                mono->setValue(effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            break;

        case kSustain:
            if (sustain)
                sustain->setValue(effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            break;

        case kPickNoise:
            if (picknoise)
                picknoise->setValue(effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            break;

        case kPalmPos:
            if (palmSlider)
                palmSlider->setValue(effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            break;

        case kStopSwitch:
            if (stopSwitch)
                stopSwitch->setValue(effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            break;

        case kAbsRel:
            if (absRel)
                absRel->setValue(effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            updateChordMenu();
            break;

        case kChordSwitch:
            if (chordSwitch)
                chordSwitch->setValue(effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            break;

        case kBodySwitch:
            if (bodySwitch)
                bodySwitch->setValue(value);
            if (displayScreen0)
                displayScreen0->setBodyValue(value);
            if (guitarBottom)
                guitarBottom->setValue(value);
            if (guitarTop)
                guitarTop->setValue(value);
            if (paramDisplay)
                paramDisplay->setValue(value);
            if (paramDisplay2)
                paramDisplay2->setValue(value);
            break;

        case kChordNote0:
        case kChordNote1:
        case kChordNote2:
        case kChordNote3:
        case kChordNote4:
        case kChordNote5:
            if (displayScreen0)
                displayScreen0->m_iChordNotes[index - kChordNote0] = (int) floor(24.f * (effect->getParameter(index) + 0.0001));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            displayScreen0->m_BodyChanged = true;
            displayScreen0->setDirty(true);
            break;

        case kPickPos:
            if (displayScreen0)
                displayScreen0->m_fPickPos = (effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            displayScreen0->m_BodyChanged = true;
            displayScreen0->setDirty(true);
            break;

        case kPickupPos:
            if (displayScreen0)
                displayScreen0->m_fPickupPos = (effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            displayScreen0->m_BodyChanged = true;
            displayScreen0->setDirty(true);
            break;

        case kPickStiffness:
            if (pickSwitch)
                pickSwitch->setHorz(effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            break;

        case kPickWidth:
            if (pickSwitch)
                pickSwitch->setVert(effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            break;

        case kChordOnOff:
            if (chordOnOff)
                chordOnOff->setValue(effect->getParameter(index));
            if (displayScreen0)
                displayScreen0->setChordOnValue(effect->getParameter(index));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            break;

        case kChord:
            if (chords)
                chords->setCurrent((int) (effect->getParameter(index)));
            if (paramDisplay)
                paramDisplay->setValue(effect->getParameter(index));
            if (paramDisplay2)
                paramDisplay2->setValue(effect->getParameter(index));
            if (displayScreen0) {
                displayScreen0->m_iChordNotes[0] = (int) floor(24.f * (effect->getParameter(kChordNote0) + 0.0001));
                displayScreen0->m_iChordNotes[1] = (int) floor(24.f * (effect->getParameter(kChordNote1) + 0.0001));
                displayScreen0->m_iChordNotes[2] = (int) floor(24.f * (effect->getParameter(kChordNote2) + 0.0001));
                displayScreen0->m_iChordNotes[3] = (int) floor(24.f * (effect->getParameter(kChordNote3) + 0.0001));
                displayScreen0->m_iChordNotes[4] = (int) floor(24.f * (effect->getParameter(kChordNote4) + 0.0001));
                displayScreen0->m_iChordNotes[5] = (int) floor(24.f * (effect->getParameter(kChordNote5) + 0.0001));
                displayScreen0->m_BodyChanged = true;
                displayScreen0->setDirty(true);
            }
            break;

        case kMeter:
            if (meter)
                meter->setValue(effect->DECLARE_VST_DEPRECATED(getVu)());
            break;

    }

    paramDisplay->setDirty(true);
    paramDisplay2->setDirty(true);

#if 0  // not in VST SDK 2.4
    postUpdate();
#endif
}

void RevEditor::idle() {
#if WINDOWS
    HWND gtFcs = GetFocus();
    if (wFcs == NULL) {
        wFcs = gtFcs;
    }
    if (gtFcs == NULL) {
        hasFocus = false;
    }
    if (!hasFocus && gtFcs != NULL && pFcs == NULL) {
        hasFocus = true;
        if (!hasVstWheel) {
            SetFocus((HWND)getFrame()->getSystemWindow());
        }
    }
    pFcs = gtFcs;
#elif MAC
    hasFocus = true;
#endif
    float val = effect->DECLARE_VST_DEPRECATED(getVu)();
    meter->setValue(val);
    //  if(MIDIDisplay->getValue() != ((Revitar*) effect)->m_LastMIDICC)
    //  {
    MIDIDisplay->setValue((float) ((Revitar*) effect)->m_LastMIDICC);
    //  }


    if (((Revitar*) effect)->m_ChordDisplayUpdate > 200.0f) {
        displayScreen0->setDirty(true);
    }

    AEffGUIEditor::idle();

}

void RevEditor::removeMIDICC(int tag) {
    if (tag == kGain ||
            tag == kBodyGain ||
            tag == kPickVolume ||
            tag == kTuning ||
            tag == kBridgeDamping ||
            tag == kStringDamping ||
            tag == kVibratoAmplit ||
            tag == kVibratoRate ||
            tag == kSympathetic ||
            tag == kSlap ||
            tag == kPickSpeed ||
            tag == kChordRate ||
            tag == kStringType ||
            tag == kPalmDamp ||
            tag == kSlideRate ||
            tag == kBodySwitch ||
            tag == kPickupPos ||
            tag == kPalmPos ||
            tag == kPickPos) {
        for (int i = 0; i < NUM_MIDI_CC; i++) {
            if (((Revitar*) effect)->m_MIDIControl[i] == tag) {
                ((Revitar*) effect)->m_MIDIControl[i] = NO_INFORMATION;
                ((Revitar*) effect)->saveMIDIControls();
            }
        }

    }

    if (tag == kPickWidth) {

        for (int i = 0; i < NUM_MIDI_CC; i++) {
            if (((Revitar*) effect)->m_MIDIControl[i] == kPickWidth ||
                    ((Revitar*) effect)->m_MIDIControl[i] == kPickStiffness) {
                ((Revitar*) effect)->m_MIDIControl[i] = NO_INFORMATION;
                ((Revitar*) effect)->saveMIDIControls();
            }
        }
    }

}

void RevEditor::updateMIDICC(int tag) {
    if (((Revitar*) effect)->m_LastMIDICC != NO_INFORMATION) {
        if (tag == kGain ||
                tag == kBodyGain ||
                tag == kPickVolume ||
                tag == kTuning ||
                tag == kBridgeDamping ||
                tag == kStringDamping ||
                tag == kVibratoAmplit ||
                tag == kVibratoRate ||
                tag == kSympathetic ||
                tag == kSlap ||
                tag == kPickSpeed ||
                tag == kChordRate ||
                tag == kStringType ||
                tag == kPalmDamp ||
                tag == kSlideRate ||
                tag == kBodySwitch ||
                tag == kPickupPos ||
                tag == kPalmPos ||
                tag == kPickPos) {
            ((Revitar*) effect)->m_MIDIControl[((Revitar*) effect)->m_LastMIDICC] = tag;
            ((Revitar*) effect)->saveMIDIControls();
        }

        if (tag == kPickWidth) {
            if (((Revitar*) effect)->m_LastMIDICC < 16) {
                ((Revitar*) effect)->m_MIDIControl[((Revitar*) effect)->m_LastMIDICC] = kPickWidth;
                ((Revitar*) effect)->m_MIDIControl[((Revitar*) effect)->m_LastMIDICC + 1] = kPickStiffness;
                ((Revitar*) effect)->saveMIDIControls();
            } else {
                ((Revitar*) effect)->m_MIDIControl[((Revitar*) effect)->m_LastMIDICC] = kPickWidth;
                ((Revitar*) effect)->m_MIDIControl[((Revitar*) effect)->m_LastMIDICC - 1] = kPickStiffness;
                ((Revitar*) effect)->saveMIDIControls();
            }

        }
    }
}



//-----------------------------------------------------------------------------
#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR < 6) || VSTGUI_VERSION_MAJOR < 3
void RevEditor::valueChanged(CDrawContext* context, CControl* control)
#else

void RevEditor::valueChanged(CControl* control)
#endif
{

    CRect size(403, 22, 503, 40);
    CRect size4(404, 23, 504, 41);
    CRect size2(403, 42, 503, 60);
    CRect size3(404, 43, 504, 61);
    lastIndex = -1;

    long tag = control->getTag();
    switch (tag) {
        case kGain:
        case kBodyGain:
        case kPickVolume:
        case kTuning:
        case kBridgeDamping:
        case kStringDamping:
        case kVibratoAmplit:
        case kVibratoRate:
        case kSympathetic:
        case kSlap:
        case kPickSpeed:
        case kChordRate:
        case kStringType:
        case kPalmDamp:
        case kSlideRate:
        case kChordNote0:
        case kChordNote1:
        case kChordNote2:
        case kChordNote3:
        case kChordNote4:
        case kChordNote5:
        case kMono:
        case kSustain:
        case kPickNoise:
        case kChord:
        case kChordOnOff:
        case kChordSwitch:
        case kPickSwitch:
        case kBodySwitch:
        case kPickPos:
        case kPickupPos:
        case kPickWidth:
        case kPickStiffness:
        case kPalmPos:
        case kAbsRel:
        case kStopSwitch:

            if (tag == kChord)
#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR < 6) || VSTGUI_VERSION_MAJOR < 3
                effect->setParameter(kChord, (float) chords->getCurrent(0));
#else
                effect->setParameter(kChord, (float) chords->getCurrentIndex(true));
#endif
else
                effect->setParameterAutomated(tag, control->getValue());

            if (tag == kChordOnOff) {
                effect->setParameterAutomated(kMono, control->getValue());
                effect->setParameterAutomated(tag, control->getValue());
            }

            if (tag == kAbsRel) {
                updateChordMenu();
            }

            if (tag == kPickWidth || tag == kPickStiffness || tag == kPickSwitch) {
                ((Revitar*) effect)->setDisplayPickParameters(pickSwitch->m_VertVal,
                        pickSwitch->m_HorzVal);
                lastIndex = kPickSwitch;
                if (paramDisplay)
                    paramDisplay->setValue(pickSwitch->m_VertVal);
                if (paramDisplay2)
                    paramDisplay2->setValue(pickSwitch->m_VertVal);
            }

            control->setDirty();

            break;

        case kDS0:
            if (displayScreen0->m_PickupPressed == true) {
                effect->setParameterAutomated(kPickupPos, displayScreen0->m_fPickupPos);
            }

            if (displayScreen0->m_PickPressed == true) {
                effect->setParameterAutomated(kPickPos, displayScreen0->m_fPickPos);
            }

            if ((displayScreen0->m_ChordPressed >= 0) &&
                    (displayScreen0->m_ChordPressed <= 5)) {
                int num = displayScreen0->m_ChordPressed;
                ((Revitar*) effect)->setDisplayChordNote(num,
                        displayScreen0->m_iChordNotes[num]);
            }
            control->setDirty();
            break;

        case kMeter:
            control->setDirty();
            break;
    }

}
//#endif

#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR > 5) || VSTGUI_VERSION_MAJOR > 3
bool RevEditor::onWheel(float distance)
{
    long btns;
    CPoint pnt;
    bool res = false; // distance is positive or negative to indicate wheel direction
#if WINDOWS
    if (getFrame() == NULL) {
        goto Q;
    }
    if (!getFrame()->getCurrentMouseLocation(pnt)) {
        goto Q;
    }
    btns = getFrame()->getCurrentMouseButtons();
    if (!hasVstWheel && hasFocus && wFcs != NULL) {
        if (wFcs != NULL) {
            SetFocus(wFcs);
        }
    } else {
        res = getFrame()->onWheel(pnt, distance, btns);
    }
    hasVstWheel = true;
#endif
Q:
    return res;
}
#endif

void RevEditor::updateChordMenu() {
    // this should be changed to allow editing of the chord names,
    // since changes to the chords are already saved with the programs.
    static const char *ChordNames[] = {
        "Major",
        "Minor",
        "Power F",
        "Power G",
        "7th",
        "Major 7th",
        "Minor 7th",
        "6th",
        "Minor 6th",
        "9th",
        "Major 9th",
        "Minor 9th",
        "11th",
        "6/9",
        "7+9",
        "7-9",
        "Aug",
        "Dim",
        "Slash A/E",
        "Slash A/F",
        "Slash A/G",
        "Slash C/C",
        "Slash C/D",
        "Slash C/G",
    };
    static const char *AbsChordNames[] = {
        "C",
        "D Minor",
        "D",
        "E Minor",
        "E",
        "F",
        "G Minor",
        "G",
        "A Minor",
        "A",
        "B Minor",
        "B",
        "Pwr C",
        "Pwr D Minor",
        "Pwr D",
        "Pwr E Minor",
        "Pwr E",
        "Pwr F",
        "Pwr G Minor",
        "Pwr G",
        "Pwr A Minor",
        "Pwr A",
        "Pwr B Minor",
        "Pwr B",
    };

    bool bResAbs = (effect->getParameter(kAbsRel) >= 0.5f);
    char **texts = (bResAbs) ? ChordNames : AbsChordNames;
    if (chords) {
        chords->removeAllEntry();

        for (int i = 0; i < 24; i++)
            chords->addEntry(texts[i], i);
        if (bResAbs) {
            CColor background = {0, 0, 0, 0};
            chords->setBackColor(background);
        } else {
            CColor background = {20, 20, 20, 0};
            chords->setBackColor(background);
        }
        chords->setCurrent((int) effect->getParameter(kChord));
        chords->setDirty(true);
    }

    if (displayScreen0) {
        displayScreen0->m_iChordNotes[0] = (int) floor(24.f * (effect->getParameter(kChordNote0) + 0.0001f));
        displayScreen0->m_iChordNotes[1] = (int) floor(24.f * (effect->getParameter(kChordNote1) + 0.0001f));
        displayScreen0->m_iChordNotes[2] = (int) floor(24.f * (effect->getParameter(kChordNote2) + 0.0001f));
        displayScreen0->m_iChordNotes[3] = (int) floor(24.f * (effect->getParameter(kChordNote3) + 0.0001f));
        displayScreen0->m_iChordNotes[4] = (int) floor(24.f * (effect->getParameter(kChordNote4) + 0.0001f));
        displayScreen0->m_iChordNotes[5] = (int) floor(24.f * (effect->getParameter(kChordNote5) + 0.0001f));
        displayScreen0->m_BodyChanged = true;
        displayScreen0->setDirty(true);
    }

} // updateChordMenu
