/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Copyright (C) 2004 C. Lawrence Zitnick III <larryz@microsoft.com>
 * Copyright (C) 2010 Hermann Seib <him@hermannseib.com>
 * Copyright (C) 2014 Asseca <http://www.asseca.org/revitar.html>
 * Copyright (C) 2015 Markus Kitsinger (SwooshyCueb) <root@swooshalicio.us>
 */

#ifndef __REV_VSTCONTROLS__
#define __REV_VSTCONTROLS__

//#include <windows.h>
#include <math.h>
#include <stdio.h>

#include "vstgui.sf/vstgui/vstgui.h"
#include "vstgui.sf/vstgui/aeffguieditor.h"

#include "resource.h"

#ifndef minmax // Asseca 2.13
    #define minmax(v,a,b)  ( ((v) < (a)) ? (a) : ((v) > (b)) ? (b) : (v) )
#endif

/*****************************************************************************/
/* CDisplayScreen                                                            */
/*****************************************************************************/

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
    int kPickPos;
    int kPickupPos;

    // display screen variables
    char *m_BMDisplay;
    char *m_BMBackground;
    char *m_BMPick;
    char *m_BMPickUp;
    char *m_BMBody;
    char *m_BMChordNote;

    CDisplayScreen(const CRect &size, CControlListener *listener, long tag,
            CBitmap *background);
    virtual ~CDisplayScreen();
    
    inline void drawfunc();
    virtual void draw(CDrawContext *pContext);
    //void drawDisplay();
#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR < 6) || VSTGUI_VERSION_MAJOR < 3
    virtual void mouse(CDrawContext *pContext, CPoint &where, long buttons = -1);
#else
    virtual CMouseEventResult onMouseDown(CPoint &where, const long &buttons);
    virtual CMouseEventResult onMouseMoved(CPoint &where, const long &buttons);
    virtual CMouseEventResult onMouseUp(CPoint &where, const long &buttons);
    virtual bool onWheel(const CPoint &mouse, const float &distance, const long &buttons);
#endif

    void setChordOnValue(float value);
    void setBodyValue(float value);
    void updateBitmaps(CBitmap *body, CBitmap *pickup, CBitmap *pick, CBitmap *chordnote);
    CLASS_METHODS(CDisplayScreen, CControl);
};

/*****************************************************************************/
/* CVuMeterCircle : circular VU meter                                        */
/*****************************************************************************/

class CVuMeterCircle : public CVuMeter
{
public:
    CVuMeterCircle(const CRect &size, CBitmap *onBitmap, CBitmap *offBitmap,
            long nbLed, const long style) :
    CVuMeter(size, onBitmap, offBitmap, nbLed, style)
    {}
    virtual void draw(CDrawContext *pContext);
};

/*****************************************************************************/
/* CAnimKnobZ : CAnimKnob with some twists                                   */
/*****************************************************************************/

class CAnimKnobZ : public CAnimKnob {
public:
    CAnimKnobZ(struct CRect& size, CControlListener *listener, int tag,
            int subpixmaps, // number of subpixmaps 
            int heightOfOneImage, // pixel 
            CBitmap *handle, CPoint &offset);

#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR < 6) || VSTGUI_VERSION_MAJOR < 3
    virtual void mouse(CDrawContext *pContext, CPoint &where, long buttons = -1);
#else
    virtual CMouseEventResult onMouseDown(CPoint &mouse, const long &buttons);
    virtual CMouseEventResult onMouseUp(CPoint &mouse, const long &buttons);
    virtual CMouseEventResult onMouseMoved(CPoint &mouse, const long &buttons);
#endif
};

/*****************************************************************************/
/* CHorizontalSlider2                                                        */
/*****************************************************************************/

class CHorizontalSlider2 : public CHorizontalSlider {
public:

    CHorizontalSlider2(CRect &size, CControlListener *listener,
            long tag, long iMinPos, long iMaxPos,
            CBitmap *handle, CBitmap *bk, CPoint &offset, int style) :
        CHorizontalSlider(size, listener, tag, iMinPos, iMaxPos, handle, bk,
            offset, style) 
    {}

#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR < 6) || VSTGUI_VERSION_MAJOR < 3
    virtual void mouse(CDrawContext *pContext, CPoint &where, long buttons = -1);
#else
    virtual CMouseEventResult onMouseDown(CPoint &mouse, const long &buttons);
    virtual CMouseEventResult onMouseUp(CPoint &mouse, const long &buttons);
    virtual CMouseEventResult onMouseMoved(CPoint &mouse, const long &buttons);
#endif
};

/*****************************************************************************/
/* COptionMenuW : Modified COptionMenu to add onWheel support                */
/*****************************************************************************/

class COptionMenuW : public COptionMenu
{
public:
    COptionMenuW(const CRect &size, CControlListener *listener, long tag, CBitmap *background,
            CBitmap *bgWhenClick, const long style) :
        COptionMenu(size, listener, tag, background, bgWhenClick, style)
    {}

#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR > 5) || VSTGUI_VERSION_MAJOR > 3
    virtual bool onWheel(const CPoint &mouse, const float &distance, const long &buttons);
#endif
};

/*****************************************************************************/
/* COnOffButtonW : Modified COnOffButton to add onWheel support              */
/*****************************************************************************/

class COnOffButtonW : public COnOffButton
{
public:
    COnOffButtonW(const CRect &size, CControlListener *listener, long tag, CBitmap *background,
            long style = kPreListenerUpdate) :
        COnOffButton(size, listener, tag, background, style)
    {}
#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR > 5) || VSTGUI_VERSION_MAJOR > 3
    virtual bool onWheel(const CPoint &mouse, const float &distance, const long &buttons);
#endif
};

/*****************************************************************************/
/* CKickButton2 : Modified CKickButton to send valueChanged on onMouseDown   */
/*                and onMouseUp and to add onWheel support                   */
/*****************************************************************************/

class CKickButton2 : public CKickButton
{
public:
#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR > 5) || VSTGUI_VERSION_MAJOR > 3
    CKickButton2(const CRect &size, CControlListener *listener, long tag, CCoord heightOfOneImage,
            CBitmap *background, const CPoint &offset = CPoint(0, 0)) :
#else
    CKickButton2(const CRect &size, CControlListener *listener, long tag, CCoord heightOfOneImage,
            CBitmap *background, CPoint &offset) :
#endif
        CKickButton(size, listener, tag, heightOfOneImage, background, offset)
    {}
#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR > 5) || VSTGUI_VERSION_MAJOR > 3
    virtual CMouseEventResult onMouseDown(CPoint &mouse, const long &buttons);
    virtual CMouseEventResult onMouseUp(CPoint &mouse, const long &buttons);
    virtual CMouseEventResult onMouseMoved(CPoint &mouse, const long &buttons);
    virtual bool onWheel(const CPoint &mouse, const float &distance, const long &buttons);
#endif
};

/*****************************************************************************/
/* CVerticalSwitch20 : simulates the VST SDK 2.0 vertical switch behaviour   */
/*****************************************************************************/

class CVerticalSwitch20 : public CVerticalSwitch
{
public:
    CVerticalSwitch20(const CRect &size, CControlListener *listener, long tag,
            CBitmap *background, CPoint &offset) :
    CVerticalSwitch(size, listener, tag, background, offset)
    {}

    CVerticalSwitch20(const CRect &size, CControlListener *listener, long tag,
            long subPixmaps, // number of subPixmaps
            CCoord heightOfOneImage, // pixel
            long iMaxPositions,
            CBitmap *background, CPoint &offset) :
    CVerticalSwitch(size, listener, tag, subPixmaps, heightOfOneImage,
    iMaxPositions, background, offset) {
    }
    virtual void draw(CDrawContext*);
#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR > 5) || VSTGUI_VERSION_MAJOR > 3
    virtual bool onWheel(const CPoint &mouse, const float &distance, const long &buttons);
#endif
};

/*****************************************************************************/
/* C2DSwitch : pseudo 2-dimensional vertical switch                          */
/*****************************************************************************/

class C2DSwitch : public CVerticalSwitch20
{
public:
    int m_iMaxPositionsVert;
    int m_iMaxPositionsHorz;
    float m_VertVal;
    float m_HorzVal;
    int m_iSwitchVal;
    float coefVert;
    float coefHorz;
    C2DSwitch(const CRect &size, CControlListener *listener, long tag,
            long subPixmaps, // number of subPixmaps
            long heightOfOneImage, // height of one image in pixel
            long iMaxPositionsVert,
            long iMaxPositionsHorz,
            CBitmap *background, CPoint &offset) :
    CVerticalSwitch20(size, listener,
        tag, subPixmaps, heightOfOneImage,
        iMaxPositionsVert*iMaxPositionsHorz,
        background, offset)
    {
        m_iMaxPositionsVert = iMaxPositionsVert;
        m_iMaxPositionsHorz = iMaxPositionsHorz;
        iMaxPositions = iMaxPositionsHorz * iMaxPositionsVert;
    }
    void setHorz(float v);
    void setVert(float v);
#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR < 6) || VSTGUI_VERSION_MAJOR < 3
    virtual void mouse(CDrawContext *pContext, CPoint &where, long buttons = -1);
#else
    virtual CMouseEventResult onMouseDown(CPoint &mouse, const long &buttons);
    virtual CMouseEventResult onMouseUp(CPoint &mouse, const long &buttons);
    virtual CMouseEventResult onMouseMoved(CPoint &mouse, const long &buttons);
    virtual bool onWheel(const CPoint &mouse, const float &distance, const long &buttons);
#endif
};


#endif
