/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Copyright (C) 2004 C. Lawrence Zitnick III <larryz@microsoft.com>
 * Copyright (C) 2010 Hermann Seib <him@hermannseib.com>
 * Copyright (C) 2014 Asseca <http://www.asseca.org/revitar.html>
 * Copyright (C) 2015 Markus Kitsinger (SwooshyCueb) <root@swooshalicio.us>
 */

#ifndef __REVEDITOR_H__
#define __REVEDITOR_H__

#include "vstgui.sf/vstgui/vstgui.h"
#include "vstgui.sf/vstgui/aeffguieditor.h"

#ifndef NO_INFORMATION
#define NO_INFORMATION					-1
#endif

// has to be a multiple of 4, because of loop unrolling in Revitar::process()
#define		NUM_POINTS					120

#ifndef		NUM_KNOBS
#define		NUM_KNOBS					15
#endif

#define		SLIDE_LENGTH				6250
#define		MAX_NUM_NOTES				300
#define		NUM_CHORD_NOTES				6
#define		NUM_FRETS					20.f

#define		NUM_SAMPLES					5000
#define		MAX_NUM_POLY				6
#define		NOTE_FADE					60000

#define		NUM_RESON					20
#define		NUM_BODIES					6
#define		BODY_FADE					300

#define		MAX_MIDI_DATA				1000
#define		MAX_NOTE_QUEUE				1000
#define		NUM_MIDI_CC					64

#define		NO_INFORMATION				-1

#define		MIN_LENGTH					20.0

#define		CHORD_DISPLAY_HEIGHT		10
#define		NUM_CHORDS					24

#define     DEFAULT_BEND_CENTS          200

#define		PICKUP_WIDTH				10
#define		PICKUP_HEIGHT				76
#define		DISPLAY_WIDTH				581
#define		DISPLAY_HEIGHT				79
#define		CHORDNOTE_WIDTH				8
#define		CHORDNOTE_HEIGHT			8
#define		PICK_WIDTH					18
#define		PICK_HEIGHT					23

#include "controls.h"

class RevEditor : public AEffGUIEditor, public CControlListener {
public:
    RevEditor(AudioEffect *effect = 0);
    //        RevEditor (void *effect = 0);
    virtual ~RevEditor();
    void updateMIDICC(int tag);
    void removeMIDICC(int tag);

protected:
    virtual bool open(void *ptr);
    virtual void close();
    virtual void idle();

    virtual void setParameter(VstInt32 index, float value);
#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR < 6) || VSTGUI_VERSION_MAJOR < 3
    virtual void valueChanged(CDrawContext* context, CControl* control);
#else
    virtual void valueChanged(CControl* control);
    virtual bool onWheel(float distance);
#endif
    void updateChordMenu();
private:
    // Controls
    CAnimKnobZ *knob[NUM_KNOBS];
    COnOffButtonW *chordOnOff;
    COnOffButtonW *mono;
    COnOffButtonW *sustain;
    COnOffButtonW *picknoise;
    COnOffButtonW *absRel;
    CSplashScreen *about;
    CVuMeterCircle *meter;
    COptionMenuW *chords;
    CVerticalSwitch20 *chordSwitch;
    C2DSwitch *bodySwitch;
    C2DSwitch *pickSwitch;
    CParamDisplay *paramDisplay;
    CParamDisplay *paramDisplay2;
    CParamDisplay *MIDIDisplay;
    CDisplayScreen *displayScreen0;
    CVerticalSwitch20 *guitarTop;
    CVerticalSwitch20 *guitarMid;
    CVerticalSwitch20 *guitarBottom;
    CHorizontalSlider2 *sliderPickupPos;
    CHorizontalSlider2 *sliderPickPos;
    CHorizontalSlider2 *sliderPalmPos;
    CKickButton2 *stopSwitch;

    // Bitmaps
    CBitmap *hBackground;
    CBitmap *hKnob;
    CBitmap *hKnobTune;
    CBitmap *hKnobSlide;
    CBitmap *hBtnRound;
    CBitmap *hBtnSquare;
    CBitmap *hVuMeter;
    CBitmap *hChordDirection;
    CBitmap *hAbout;
    CBitmap *hBodySelector;
    CBitmap *hCtlPickType;
    CBitmap *hPickupPos;
    CBitmap *hPickPos;
    CBitmap *hChordNoteOn;
    CBitmap *hGuitarTop;
    CBitmap *hGuitarMid;
    CBitmap *hGuitarBottom;
    CBitmap *hSliderPalmPos;
    CBitmap *hBtnStop;
    CBitmap *hBtnAbsRel;

    // Misc
    bool hasVstWheel;
    bool hasFocus;
#if WINDOWS
    HWND pFcs;
    HWND wFcs;
#endif

};

#endif
