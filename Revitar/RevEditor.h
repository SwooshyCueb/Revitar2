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

#ifndef WITH_REGISTRATION
#define WITH_REGISTRATION 0
#endif

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

/*****************************************************************************/
/* CAnimKnobZ : CAnimKnob with some twists                                   */
/*****************************************************************************/

class CAnimKnobZ : public CAnimKnob {
public:
    CAnimKnobZ(struct CRect& size, CControlListener *listener, int tag,
            int subpixmaps, // number of subpixmaps 
            int heightOfOneImage, // pixel 
            CBitmap *handle, CPoint &offset);

#if VSTGUI_VERSION_MAJOR <= 3 && VSTGUI_VERSION_MINOR < 5
    virtual void mouse(CDrawContext *pContext, CPoint &where, long buttons = -1);
#else
#if DEF_ONWHEEL
    virtual bool onWheel(const CPoint &where, const float &distance, const long &buttons);
#endif
    virtual CMouseEventResult onMouseDown(CPoint &where, const long &buttons);
    //We can just use the CControl methods as they are
    /*virtual CMouseEventResult onMouseUp (CPoint &where, const long &buttons);
    virtual CMouseEventResult onMouseMoved (CPoint &where, const long &buttons);
    virtual void setWheelInc (float val);
    virtual float getWheelInc () const;*/
protected:
    float preVal;
#endif
};

class CDisplayScreen : public CControl {
public:

    CPoint offset;

    CBitmap *pHandle;
    CBitmap *pDisplay;

    float m_fPickupPos;
    float m_fPickPos;
    int m_iBodyType;
    int m_iChordNotes[NUM_CHORD_NOTES];
    bool m_BodyChanged;
    bool m_PickupPressed;
    bool m_PickPressed;
    int m_ChordPressed;
    int m_FirstNote;
    float m_ChordOn;
    float *m_pChordDisplayUpdate;
    float (*m_ChordDisplay)[NUM_POINTS][CHORD_DISPLAY_HEIGHT];
    float (*m_ChordDisplayCurrent)[NUM_POINTS][CHORD_DISPLAY_HEIGHT];
    int m_BPP;
    int kPickPosition;
    int kPickUp;

    // display screen variables
    char *m_BMDisplay;
    char *m_BMBackground;
    char *m_BMPick;
    char *m_BMPickUp;
    char *m_BMBody;
    char *m_BMChordNote;

    CDisplayScreen(const CRect &size, CControlListener *listener, long tag,
            CBitmap *background)
    : CControl(size, listener, tag, background) {
        pHandle = 0;
        m_fPickupPos = 0.1f;
        m_fPickPos = 0.25;
        m_iBodyType = 0;
        m_PickPressed = false;
        m_PickupPressed = false;
        m_ChordPressed = NO_INFORMATION;
        m_ChordOn = 0.0;

        int i;
        for (i = 0; i < 6; i++) {
            m_iChordNotes[i] = -1;
        }
        m_iChordNotes[0] = 0;
        m_iChordNotes[1] = 3;
        m_iChordNotes[5] = 16;

        m_BodyChanged = true;
    }
#if 0

    CDisplayScreen(const CDisplayScreen& c) : CControl(c)
    , offset(c.offset)
    , pHandle(c.pHandle)
    , pDisplay(c.pDisplay)
    , m_fPickupPos(c.m_fPickupPos)
    , m_fPickPos(c.m_fPickPos)
    , m_iBodyType(c.m_iBodyType)
    , m_BodyChanged(c.m_BodyChanged)
    , m_PickupPressed(c.m_PickupPressed)
    , m_PickPressed(c.m_PickPressed)
    , m_ChordPressed(c.m_ChordPressed)
    , m_FirstNote(c.m_FirstNote)
    , m_ChordOn(c.m_ChordOn)
    , m_pChordDisplayUpdate(c.m_pChordDisplayUpdate)
    , m_ChordDisplay(c.m_ChordDisplay)
    , m_ChordDisplayCurrent(c.m_ChordDisplayCurrent)
    , m_BPP(c.m_BPP)
    , kPickPosition(c.kPickPosition)
    , kPickUp(c.kPickUp) {
    }
#endif

    virtual ~CDisplayScreen();
    virtual void draw(CDrawContext *pContext);
#if VSTGUI_VERSION_MAJOR <= 3 && VSTGUI_VERSION_MINOR < 5
    virtual void mouse(CDrawContext *pContext, CPoint &where, long buttons = -1);
#else
    virtual CMouseEventResult onMouseDown(CPoint &where, const long &buttons);
    virtual CMouseEventResult onMouseMoved(CPoint &where, const long &buttons);
    virtual CMouseEventResult onMouseUp(CPoint &where, const long &buttons);
#endif

    void setChordOnValue(float value);
    void setBodyValue(float value);
    void drawDisplay();
    void updateBitmaps(CBitmap *background, CBitmap *pickup, CBitmap *pick, CBitmap *chordnote);

#if VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR >= 5
    CLASS_METHODS(CDisplayScreen, CControl);
#endif

};

class CHorizontalSlider2 : public CHorizontalSlider {
public:

    CHorizontalSlider2(CRect &size, CControlListener *listener,
            long tag, long iMinPos, long iMaxPos,
            CBitmap *handle, CBitmap *bk, CPoint &offset, int style) :
    CHorizontalSlider(size, listener, tag, iMinPos, iMaxPos, handle, bk,
    offset, style) {
    }

#if VSTGUI_VERSION_MAJOR <= 3 && VSTGUI_VERSION_MINOR < 5
    virtual void mouse(CDrawContext *pContext, CPoint &where, long buttons = -1);
#else
    virtual CMouseEventResult onMouseDown(CPoint &where, const long &buttons);
#endif

};


/*****************************************************************************/
/* CVerticalSwitch20 : simulates the VST SDK 2.0 vertical switch behaviour   */
/*****************************************************************************/

class CVerticalSwitch20 : public CVerticalSwitch {
public:

    CVerticalSwitch20(const CRect &size, CControlListener *listener, long tag,
            CBitmap *background, CPoint &offset)
    : CVerticalSwitch(size, listener, tag, background, offset) {
    }

    CVerticalSwitch20(const CRect &size, CControlListener *listener, long tag,
            long subPixmaps, // number of subPixmaps
            CCoord heightOfOneImage, // pixel
            long iMaxPositions,
            CBitmap *background, CPoint &offset)
    : CVerticalSwitch(size, listener, tag, subPixmaps, heightOfOneImage,
    iMaxPositions, background, offset) {
    }
    virtual void draw(CDrawContext*);

};

/*****************************************************************************/
/* C2DSwitch : pseudo 2-dimensional vertical switch                          */
/*****************************************************************************/

class C2DSwitch : public CVerticalSwitch20 {
public:

    int m_iMaxPositionsVert;
    int m_iMaxPositionsHorz;
    float m_VertVal;
    float m_HorzVal;
    int m_iSwitchVal;

    C2DSwitch(const CRect &size, CControlListener *listener, long tag,
            long subPixmaps, // number of subPixmaps
            long heightOfOneImage, // height of one image in pixel
            long iMaxPositionsVert,
            long iMaxPositionsHorz,
            CBitmap *background, CPoint &offset)
    : CVerticalSwitch20(size, listener,
    tag, subPixmaps, heightOfOneImage,
    iMaxPositionsVert*iMaxPositionsHorz,
    background, offset) {
        m_iMaxPositionsVert = iMaxPositionsVert;
        m_iMaxPositionsHorz = iMaxPositionsHorz;
    }


    void setHorz(float v);
    void setVert(float v);
#if VSTGUI_VERSION_MAJOR <= 3 && VSTGUI_VERSION_MINOR < 5
    virtual void mouse(CDrawContext *pContext, CPoint &where, long buttons = -1);
#else
    virtual CMouseEventResult onMouseDown(CPoint &where, const long &buttons);
    virtual CMouseEventResult onMouseMoved(CPoint &where, const long &buttons);
protected:
    float coefVert;
    float coefHorz;
#endif

};

/*****************************************************************************/
/* CVuMeterCircle : circular VU meter                                        */
/*****************************************************************************/

class CVuMeterCircle : public CVuMeter {
public:

    CVuMeterCircle(const CRect &size, CBitmap *onBitmap, CBitmap *offBitmap,
            long nbLed, const long style)
    : CVuMeter(size, onBitmap, offBitmap, nbLed, style) {
    }


    virtual void draw(CDrawContext *pContext);

};

//-----------------------------------------------------------------------------

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
#if VSTGUI_VERSION_MAJOR <= 3 && VSTGUI_VERSION_MINOR < 5
    virtual void valueChanged(CDrawContext* context, CControl* control);
#else
    virtual void valueChanged(CControl* control);
#endif

    void updateChordMenu();


private:
    // Controls
    CAnimKnobZ *knob[NUM_KNOBS];
    COnOffButton *chordOnOff;
    COnOffButton *mono;
    COnOffButton *sustain;
    COnOffButton *picknoise;
    COnOffButton *absRel;
    CTextEdit *textReg;
    CSplashScreen *welcome;
    CSplashScreen *about;
    CVuMeterCircle *meter;
    COptionMenu *chords;
    CVerticalSwitch *chordSwitch;
    C2DSwitch *bodySwitch;
    C2DSwitch *pickSwitch;
    CParamDisplay *paramDisplay;
    CParamDisplay *paramDisplay2;
    CParamDisplay *MIDIDisplay;
    CDisplayScreen *displayScreen0;
    CVerticalSwitch *guitarTop;
    CVerticalSwitch *guitarBottom;
    CHorizontalSlider2 *palmSlider;
    CKickButton *stopSwitch;

    // Bitmaps
    CBitmap *hBackground;
    CBitmap *hKnob;
    CBitmap *hTuneKnob;
    CBitmap *hSlideKnob;
    CBitmap *hRoundButton;
    CBitmap *hSquareButton;
    CBitmap *hRegButton;
    CBitmap *hVuMeter;
    CBitmap *hWelcome;
    CBitmap *hChordSwitch;
    CBitmap *hAbout;
    CBitmap *hDisplayBack;
    CBitmap *hChordVert;
    CBitmap *hBodySelector;
    CBitmap *hPickSwitch;
    CBitmap *hDisplayBackground;
    CBitmap *hPickUp;
    CBitmap *hPickPosition;
    CBitmap *hChordNote;
    CBitmap *hBodies;
    CBitmap *hGuitarBottom;
    CBitmap *hGuitarTop;
    CBitmap *hPalmSliderBackground;
    CBitmap *hPalmSliderHandle;
    CBitmap *hStopSwitch;
    CBitmap *hAbsRel;


};

#endif
