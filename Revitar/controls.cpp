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
#include "controls.h"

/*===========================================================================*/
/* CDisplayScreen class members                                              */
/*===========================================================================*/

CDisplayScreen::CDisplayScreen(const CRect &size, CControlListener *listener, long tag, CBitmap *background)
    :   CControl(size, listener, tag, background)
{
    pHandle = 0;
    m_fPickupPos = .1f;
    m_fPickPos = .25f;
    m_iBodyType = 0;
    m_PickPressed = false;
    m_PickupPressed = false;
    m_ChordPressed = NO_INFORMATION;
    m_FirstNote = NO_INFORMATION;
    m_ChordOn = 0.f;
    for (int i = 0; i < 6; i++) {
        m_iChordNotes[i] = -1;
    }
    m_iChordNotes[0] = 0;
    m_iChordNotes[1] = 3;
    m_iChordNotes[5] = 16;
    m_BodyChanged = true;
}

CDisplayScreen::~CDisplayScreen()
{
    if (pHandle) {
        pHandle->forget();
    }
}

/*****************************************************************************/
/* drawfunc                                                                  */
/*****************************************************************************/

inline void CDisplayScreen::drawfunc()
{
    int rowDel, colDel, row, col, k;
    if (m_BodyChanged) {
        m_BodyChanged = false;
        //draw GuitarMidBody
        memcpy(m_BMBackground,
                m_BMBody + m_iBodyType * DISPLAY_WIDTH * DISPLAY_HEIGHT * m_BPP,
                DISPLAY_WIDTH * DISPLAY_HEIGHT * m_BPP * sizeof (char));

        colDel = 135 + (int) (m_fPickupPos * 110.0f);
        rowDel = 1;

        for (row = 0; row < PICKUP_HEIGHT; row++) {
            //draw Pickup
            int offsetTmp0 = row * PICKUP_WIDTH * m_BPP;
            int offsetTmp1 = (row + rowDel) * DISPLAY_WIDTH * m_BPP;

            for (col = 0; col < PICKUP_WIDTH; col++) {
                int offset1 = offsetTmp0 + col * m_BPP;

                if (m_BMPickUp[offset1 + 2] != -1) {
                    int offset = offsetTmp1 + (col + colDel) * m_BPP;
                    m_BMBackground[offset + 0] = m_BMPickUp[offset1 + 0];
                    m_BMBackground[offset + 1] = m_BMPickUp[offset1 + 1];
                    if (m_BPP != 2)
                        m_BMBackground[offset + 2] = m_BMPickUp[offset1 + 2];
                }
            }
        }

        if (m_ChordOn > 0.f) {
            //draw ChordNote-LEDs
            int i;

            for (i = 0; i < 6; i++) {
                int note = m_iChordNotes[i];

                if (note >= 0) {
                    rowDel = 11 + i * 10;
                    colDel = DISPLAY_WIDTH - 13 - 16 * note;

                    for (row = 0; row < CHORDNOTE_HEIGHT; row++) {
                        if (row == 0 || row == CHORDNOTE_HEIGHT - 1) {
                            for (col = 1; col < CHORDNOTE_WIDTH - 1; col++) {
                                int offset1 = (row) * CHORDNOTE_WIDTH * m_BPP + col * m_BPP;
                                int offset = (row + rowDel) * DISPLAY_WIDTH * m_BPP + (col + colDel) * m_BPP;
                                m_BMBackground[offset + 0] = m_BMChordNote[offset1 + 0];
                                m_BMBackground[offset + 1] = m_BMChordNote[offset1 + 1];
                                if (m_BPP != 2)
                                    m_BMBackground[offset + 2] = m_BMChordNote[offset1 + 2];
                            }
                        } else {
                            for (col = 0; col < CHORDNOTE_WIDTH; col++) {
                                int offset1 = (row) * CHORDNOTE_WIDTH * m_BPP + col * m_BPP;
                                int offset = (row + rowDel) * DISPLAY_WIDTH * m_BPP + (col + colDel) * m_BPP;
                                m_BMBackground[offset + 0] = m_BMChordNote[offset1 + 0];
                                m_BMBackground[offset + 1] = m_BMChordNote[offset1 + 1];
                                if (m_BPP != 2)
                                    m_BMBackground[offset + 2] = m_BMChordNote[offset1 + 2];
                            }
                        }
                    }
                }
            }
        }

        for (k = 0; k < MAX_NUM_POLY; k++) {
            //draw static strings for right-most LEDs
            for (col = 442; col < 453; col++) {
                int ofsetTmp0 = (k * CHORD_DISPLAY_HEIGHT + 9) * DISPLAY_WIDTH + (col + 126);
                int offset = (CHORD_DISPLAY_HEIGHT / 2) * DISPLAY_WIDTH + ofsetTmp0;
                offset *= m_BPP;

                m_BMBackground[offset + 0] = (BYTE) 255;
                m_BMBackground[offset + 1] = (BYTE) 255;
                if (m_BPP != 2)
                    m_BMBackground[offset + 2] = (BYTE) 255;
            }
        }

    }

    if (*m_pChordDisplayUpdate >= 200.0f) {
        if (m_BPP != 2) {
            memcpy(m_ChordDisplayCurrent, m_ChordDisplay,
                    MAX_NUM_POLY * NUM_POINTS * CHORD_DISPLAY_HEIGHT * sizeof (float));

            float val0 = (*m_pChordDisplayUpdate) / MAX_NUM_POLY;

            for (k = 0; k < MAX_NUM_POLY; k++)
                for (col = 0; col < NUM_POINTS; col++)
                    for (row = 0; row < CHORD_DISPLAY_HEIGHT; row++)
                        m_ChordDisplayCurrent[k][col][row] /= val0;
        }
        *m_pChordDisplayUpdate = 0.0;
        memset(m_ChordDisplay, 0, MAX_NUM_POLY * NUM_POINTS * CHORD_DISPLAY_HEIGHT * sizeof (float));
    }

    memcpy(m_BMDisplay, m_BMBackground, DISPLAY_WIDTH * DISPLAY_HEIGHT * m_BPP * sizeof (char));

    for (k = 0; k < MAX_NUM_POLY; k++) {
        //draw static/vibrating strings
        for (col = 0; col < 442; col++) {
            int idx = (int) ((NUM_POINTS - 1) * col / 451);
            int ofsetTmp0 = (k * CHORD_DISPLAY_HEIGHT + 9) * DISPLAY_WIDTH * m_BPP + (col + 126) * m_BPP;

            for (row = 0; row < CHORD_DISPLAY_HEIGHT; row++) {
                float del = m_ChordDisplayCurrent[k][idx][row];
                if (del > 0.01f) {
                    int offset = (row) * DISPLAY_WIDTH * m_BPP + ofsetTmp0;
                    if (m_BPP == 2)
                    {
                        m_BMDisplay[offset + 0] = (BYTE) 255;
                        m_BMDisplay[offset + 1] = (BYTE) 255;
                    } else {
                        char val = (char) (250.0f * del);
                        del = 1.0f - del;

                        m_BMDisplay[offset + 0] = (BYTE) (val + (float) (unsigned char) m_BMDisplay[offset + 0] * del);
                        m_BMDisplay[offset + 1] = (BYTE) (val + (float) (unsigned char) m_BMDisplay[offset + 1] * del);
                        m_BMDisplay[offset + 2] = (BYTE) (val + (float) (unsigned char) m_BMDisplay[offset + 2] * del);
                    }
                }
            }
        }
    }

    colDel = 135 + (int) (m_fPickPos * 107.0f);
    rowDel = 4;

    for (row = 0; row < PICK_HEIGHT; row++) {
        //draw Pick
        for (col = 0; col < PICK_WIDTH; col++) {
            int offset1 = row * PICK_WIDTH * m_BPP + col * m_BPP;

            if ((m_BMPick[offset1 + 0] != -1) && (m_BPP ==2)) {
                int offset = (row + rowDel) * DISPLAY_WIDTH * m_BPP + (col + colDel) * m_BPP;
                m_BMDisplay[offset + 0] = m_BMPick[offset1 + 0];
                m_BMDisplay[offset + 1] = m_BMPick[offset1 + 1];
            } else if ((m_BMPick[offset1 + 2] != -1) && (m_BPP !=2)) {
                int offset = (row + rowDel) * DISPLAY_WIDTH * m_BPP + (col + colDel) * m_BPP;
                m_BMDisplay[offset + 0] = m_BMPick[offset1 + 0];
                m_BMDisplay[offset + 1] = m_BMPick[offset1 + 1];
                m_BMDisplay[offset + 2] = m_BMPick[offset1 + 2];
            }
        }
    }

    SetBitmapBits((HBITMAP) pDisplay->getHandle(), DISPLAY_WIDTH * DISPLAY_HEIGHT*m_BPP, m_BMDisplay);
}

/*****************************************************************************/
/* draw                                                                      */
/*****************************************************************************/

void CDisplayScreen::draw(CDrawContext *pContext) {
    if (!pDisplay) {
        setDirty(false);
        return;
    }

    drawfunc();

    pDisplay->draw(pContext, size, offset);
    setDirty(false);
}

/*****************************************************************************/
/* drawDisplay                                                               */
/*****************************************************************************/
#if 0

void CDisplayScreen::drawDisplay() {
    drawfunc();
}

#endif
/*****************************************************************************/
/* updateBitmaps                                                             */
/*****************************************************************************/

void CDisplayScreen::updateBitmaps(CBitmap *body, CBitmap *pickup, CBitmap *pick, CBitmap *chordnote) {
    m_BMDisplay = new char [DISPLAY_WIDTH * DISPLAY_HEIGHT * 4];
    m_BMBackground = new char [DISPLAY_WIDTH * DISPLAY_HEIGHT * 4];
    m_BMPick = new char [PICK_WIDTH * PICK_HEIGHT * 4];
    m_BMPickUp = new char [PICKUP_WIDTH * PICKUP_HEIGHT * 4];
    m_BMBody = new char [DISPLAY_WIDTH * NUM_BODIES * DISPLAY_HEIGHT * 4];
    m_BMChordNote = new char [CHORDNOTE_WIDTH * 2 * CHORDNOTE_HEIGHT * 4];
    m_ChordDisplayCurrent = new float [MAX_NUM_POLY][NUM_POINTS][CHORD_DISPLAY_HEIGHT];

    GetBitmapBits((HBITMAP) body->getHandle(), DISPLAY_WIDTH * NUM_BODIES * DISPLAY_HEIGHT * 4, m_BMBody);
    GetBitmapBits((HBITMAP) pick->getHandle(), PICK_WIDTH * PICK_HEIGHT * 4, m_BMPick);
    GetBitmapBits((HBITMAP) pickup->getHandle(), PICKUP_WIDTH * PICKUP_HEIGHT * 4, m_BMPickUp);
    int bytesRead = GetBitmapBits((HBITMAP) chordnote->getHandle(), CHORDNOTE_WIDTH * 2 * CHORDNOTE_HEIGHT * 4, m_BMChordNote);

    m_BPP = bytesRead / (CHORDNOTE_WIDTH * CHORDNOTE_HEIGHT);

    memset(m_ChordDisplayCurrent, 0, MAX_NUM_POLY * NUM_POINTS * CHORD_DISPLAY_HEIGHT * sizeof (float));

    for (int polyIdx = 0; polyIdx < MAX_NUM_POLY; polyIdx++)
        for (int i = 0; i < NUM_POINTS; i++)
            m_ChordDisplayCurrent[polyIdx][i][CHORD_DISPLAY_HEIGHT / 2] = 1.0f;
}

#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR < 6) || VSTGUI_VERSION_MAJOR < 3
/*****************************************************************************/
/* mouse                                                                     */
/*****************************************************************************/

void CDisplayScreen::mouse(CDrawContext *pContext, CPoint &where, long button) {

    if (!bMouseEnabled)
        return;

    if (button == -1) button = pContext->getMouseButtons();
    if (!(button & kLButton) && !(button & kRButton && button & kControl)) {
        return;
    }

    // begin of edit parameter

    beginEdit();

    do {
        int pickh = (int) (m_fPickPos * 107.f) + 150;

        if (m_ChordPressed == NO_INFORMATION) {
            if (m_fPickPos || m_PickupPressed || (where.h > 150 && where.h < 150 + 107)) {
                if (m_PickPressed ||
                        (!m_PickupPressed && where.h > pickh - 12 && where.h < pickh + 12 && where.v < 100)) {
                    if (button & kLButton && button & kControl) {
                        ((RevEditor*) listener)->updateMIDICC(kPickPos);
                        m_PickPressed = false;
                        return;
                    }

                    if (button & kRButton && button & kControl) {
                        ((RevEditor*) listener)->removeMIDICC(kPickPos);
                        m_PickPressed = false;
                        return;
                    }

                    m_fPickPos = (float) (where.h - 150) / 107.f;

                    if (m_fPickPos < 0.0)
                        m_fPickPos = 0.0;
                    if (m_fPickPos > 1.0)
                        m_fPickPos = 1.0;

                    m_PickPressed = true;
                    //  m_BodyChanged = true;
                    setDirty(true);
                } else {
                    int pickuph = (int) (m_fPickupPos * 110.f) + 145;

                    if (m_PickupPressed || (where.h > pickuph - 8 && where.h < pickuph + 8)) {
                        if (button & kLButton && button & kControl) {
                            ((RevEditor*) listener)->updateMIDICC(kPickupPos);
                            m_PickupPressed = false;
                            return;
                        }

                        if (button & kRButton && button & kControl) {
                            ((RevEditor*) listener)->removeMIDICC(kPickPos);
                            m_PickPressed = false;
                            return;
                        }

                        m_fPickupPos = (float) (where.h - 145) / 110.f;

                        if (m_fPickupPos < 0.0)
                            m_fPickupPos = 0.0;
                        if (m_fPickupPos > 1.0)
                            m_fPickupPos = 1.0;

                        m_PickupPressed = true;
                        m_BodyChanged = true;
                        setDirty(true);
                    }

                }
            }
        }

        if (m_ChordOn == 1.0 && !m_PickPressed && !m_PickupPressed) {
            if (m_ChordPressed != NO_INFORMATION) {
                int note = 19 - (where.h - 266) / 16;

                if (note != m_FirstNote) {
                    m_iChordNotes[m_ChordPressed] = note;
                    if (m_iChordNotes[m_ChordPressed] < 0)
                        m_iChordNotes[m_ChordPressed] = 0;
                    if (m_iChordNotes[m_ChordPressed] > 19)
                        m_iChordNotes[m_ChordPressed] = 19;

                    m_FirstNote = NO_INFORMATION;

                    m_BodyChanged = true;
                    setDirty(true);
                }
            } else {
                if (where.h >= 266 && where.h <= 586 && where.v >= 80 && where.v <= 140) {
                    m_ChordPressed = (where.v - 80) / 10;
                    if (m_ChordPressed < 0)
                        m_ChordPressed = 0;
                    if (m_ChordPressed > 5)
                        m_ChordPressed = 5;

                    m_FirstNote = 19 - (where.h - 266) / 16;
                    if (m_iChordNotes[m_ChordPressed] == m_FirstNote) {
                        m_iChordNotes[m_ChordPressed] = NO_INFORMATION;
                    } else {
                        m_iChordNotes[m_ChordPressed] = m_FirstNote;
                        if (m_iChordNotes[m_ChordPressed] < 0)
                            m_iChordNotes[m_ChordPressed] = 0;
                        if (m_iChordNotes[m_ChordPressed] > 19)
                            m_iChordNotes[m_ChordPressed] = 19;
                    }

                    m_BodyChanged = true;
                    setDirty(true);
                }
            }
        }

        if (isDirty() && listener)
            listener->valueChanged(pContext, this);

        if (m_BodyChanged == true) {
            setDirty(true);
        }

        pContext->getMouseLocation(where);

        doIdleStuff();

    } while (pContext->getMouseButtons() == button);

    m_PickPressed = false;
    m_PickupPressed = false;
    m_ChordPressed = NO_INFORMATION;
    m_FirstNote = NO_INFORMATION;


    // end of edit parameter

    endEdit();

}

#else
/*****************************************************************************/
/* onMouseDown                                                               */
/*****************************************************************************/

CMouseEventResult CDisplayScreen::onMouseDown(CPoint &mouse, const long &buttons)
{
    if (!bMouseEnabled) {
        return kMouseEventHandled;
    }
    //getFrame()->setFocusView(this);
    if (mouse.h > 137 && mouse.h < 269) { //left: 150-12=138 | 145-8=137
                                          //right: 150+107+12=269 | 145+110+8=263
        int pickh = 150 + int(m_fPickPos * 107.f);
        int pickuph = 145 + int(m_fPickupPos * 110.f);
        if (mouse.h > pickh - 12 && mouse.h < pickh + 12 && mouse.v < 100) {
            if (buttons & kRButton && buttons & kShift) {
                ((RevEditor *)listener)->updateMIDICC(kPickPos);
                return kMouseEventHandled;
            }
            if (buttons & kRButton && buttons & kControl) {
                ((RevEditor *)listener)->removeMIDICC(kPickPos);
                return kMouseEventHandled;
            }
            m_PickPressed = true;
        } else if (mouse.h > pickuph - 8 && mouse.h < pickuph + 8) {
            if (buttons & kRButton && buttons & kShift) {
                ((RevEditor *)listener)->updateMIDICC(kPickupPos);
                return kMouseEventHandled;
            }
            if (buttons & kRButton && buttons & kControl) {
                ((RevEditor *)listener)->removeMIDICC(kPickupPos);
                return kMouseEventHandled;
            }
            m_PickupPressed = true;
        }
    }
    if (!(buttons & kLButton)) {
        return kMouseEventHandled;
    }
    beginEdit();
    return onMouseMoved(mouse, buttons);
}

/*****************************************************************************/
/* onMouseUp                                                                 */
/*****************************************************************************/

CMouseEventResult CDisplayScreen::onMouseUp(CPoint &mouse, const long &buttons)
{
    endEdit();
    m_PickPressed   = false;
    m_PickupPressed = false;
    m_ChordPressed  = NO_INFORMATION;
    m_FirstNote     = NO_INFORMATION;
    return kMouseEventHandled;
}

/*****************************************************************************/
/* onMouseMoved                                                              */
/*****************************************************************************/

CMouseEventResult CDisplayScreen::onMouseMoved(CPoint &mouse, const long &buttons)
{
    if (!(buttons & kLButton)) {
        return kMouseEventHandled;
    }
    if (m_PickPressed) {
        m_fPickPos = minmax((float)(mouse.h - 150) / 107.f, 0.f, 1.f);
        //  m_BodyChanged = true;
        setDirty(true);
    } else if (m_PickupPressed) {
        m_fPickupPos = minmax((float)(mouse.h - 145) / 110.f, 0.f, 1.f);
        m_BodyChanged = true;
        setDirty(true);
    } else if (m_ChordOn == 1.0) {
        if (m_ChordPressed != NO_INFORMATION) {
            int note = 19 - (mouse.h - 266) / 16;
            if (note != m_FirstNote) {
                m_iChordNotes[m_ChordPressed] = minmax(note, 0, 19);
                m_FirstNote = NO_INFORMATION;
                m_BodyChanged = true;
                setDirty(true);
            }
        } else if (mouse.h >= 266 && mouse.h <= 586 && mouse.v >= 80 && mouse.v <= 140) {
            m_ChordPressed = minmax((mouse.v - 80) / 10, 0, 5);
            m_FirstNote = 19 - (mouse.h - 266) / 16;
            if (m_iChordNotes[m_ChordPressed] == m_FirstNote) {
                m_iChordNotes[m_ChordPressed] = NO_INFORMATION;
            } else {
                m_iChordNotes[m_ChordPressed] = minmax(m_FirstNote, 0, 19);
            }
            m_BodyChanged = true;
            setDirty(true);
        }
    }
    if (isDirty() && listener) {
        listener->valueChanged(this);
    }
    return kMouseEventHandled;
}

/*****************************************************************************/
/* onWheel                                                                   */
/*****************************************************************************/

bool CDisplayScreen::onWheel(const CPoint &mouse, const float &distance, const long &buttons)
{
    if (!bMouseEnabled) {
        return false;
    }
    int iOld;
    float fOld;
    if (mouse.h > 137 && mouse.h < 269) {
        int guitarMidY = 7 + 65;
        if (mouse.v > guitarMidY + 3 && mouse.v < guitarMidY + 3 + 23) { //=PickPos, 18px x 23px
            m_PickPressed = true;
            fOld = m_fPickPos;
            if (buttons & kShift) {
                m_fPickPos += 0.1f * distance * wheelInc;
            } else {
                m_fPickPos += distance * wheelInc;
            }
            m_fPickPos = minmax(m_fPickPos, 0.f, 1.f);
            if (fOld != m_fPickPos) {
                setDirty(true);
            }
        } else if (mouse.v > guitarMidY + 3 + 24 && mouse.v < guitarMidY + 76) { //=PickupPos, 10px x 76px
            m_PickupPressed = true;
            fOld = m_fPickupPos;
            if (buttons & kShift) {
                m_fPickupPos += 0.1f * distance * wheelInc;
            } else {
                m_fPickupPos += distance * wheelInc;
            }
            m_fPickupPos = minmax(m_fPickupPos, 0.f, 1.f);
            if (fOld != m_fPickupPos) {
                m_BodyChanged = true;
                setDirty(true);
            }
        }
    } else if (m_ChordOn == 1.0) { //=ChordLEDs
        if (mouse.h >= 270 && mouse.h <= 586 && mouse.v >= 80 && mouse.v <= 180) {
            m_ChordPressed = minmax(round((float)(mouse.v - 85) / 10.f), 0, 5);
            iOld = m_iChordNotes[m_ChordPressed];
            m_iChordNotes[m_ChordPressed] -= distance;
            m_iChordNotes[m_ChordPressed] = minmax(m_iChordNotes[m_ChordPressed], 0, 19);
            if (iOld != m_iChordNotes[m_ChordPressed]) {
                m_BodyChanged = true;
                setDirty(true);
            }
        }
    }
    if (isDirty() && listener) {
        beginEdit();
        listener->valueChanged(this);
        endEdit();
    }
    m_PickPressed   = false;
    m_PickupPressed = false;
    m_ChordPressed  = NO_INFORMATION;
    return true;
}

#endif
/*****************************************************************************/
/* setChordOnValue                                                           */
/*****************************************************************************/

void CDisplayScreen::setChordOnValue(float value) {

    m_ChordOn = value;
    m_BodyChanged = true;
    setDirty(true);

}

/*****************************************************************************/
/* setBodyValue                                                              */
/*****************************************************************************/

void CDisplayScreen::setBodyValue(float value) {

    m_iBodyType = minmax(round((float)(NUM_BODIES - 1) * value), 0, NUM_BODIES - 1);
    m_BodyChanged = true;
    setDirty(true);

}

/*===========================================================================*/
/* CVuMeterCircle class members                                              */
/*===========================================================================*/

/*****************************************************************************/
/* draw                                                                      */
/*****************************************************************************/

void CVuMeterCircle::draw(CDrawContext *_pContext) {

    if (!onBitmap)
        return;

    CPoint pointOn;
    CPoint pointOff;
    CDrawContext *pContext = _pContext;

    bounceValue();

    float newValue = oldValue - decreaseValue;
    if (newValue < getMin())
        newValue = getMin();
    if (value < newValue)
        value = newValue;
    oldValue = value;

#if 0
    if (bUseOffscreen) {
        if (!pOScreen) {
            pOScreen = new COffscreenContext(getFrame(), (long) size.width(), (long) size.height(), kBlackCColor);
            rectOn(0, 0, size.width(), size.height());
            rectOff(0, 0, size.width(), size.height());
        }
        pContext = pOScreen;
    }
#endif

    if (style & kHorizontal) {
        long tmp = (long) (((long) (nbLed * value + 0.5f) / (float) nbLed) * onBitmap->getWidth());
        pointOff(tmp, 0);
        tmp += size.left;

        rectOff.left = tmp;
        rectOn.right = tmp;
    }

    pointOff(0, 62 * (int) ((float) nbLed * value));
    if (offBitmap) {
        if (bTransparencyEnabled)
            offBitmap->drawTransparent(pContext, size, pointOff);
        else
            offBitmap->draw(pContext, size, pointOff);
    }

#if 0
    if (pOScreen)
        pOScreen->copyFrom(_pContext, size);
#endif

    setDirty(false);

}

/*===========================================================================*/
/* CAnimKnobZ class members                                                  */
/*===========================================================================*/

CAnimKnobZ::CAnimKnobZ(struct CRect& size, CControlListener *listener, int tag,
        int subpixmaps, // number of subpixmaps 
        int heightOfOneImage, // pixel 
        CBitmap *handle, CPoint &offset)
: CAnimKnob(size, listener, tag, subpixmaps, heightOfOneImage, handle, offset) {
    zoomFactor = 10.0f;
}

#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR < 6) || VSTGUI_VERSION_MAJOR < 3
/*****************************************************************************/
/* mouse : called when the mouse is moved over the knob                      */
/*****************************************************************************/

void CAnimKnobZ::mouse(CDrawContext *pContext, CPoint &where, long button) {
    if (!bMouseEnabled)
        return;

    if (button == -1) button = pContext->getMouseButtons();

    if ((button & (kLButton | kControl)) == (kLButton | kControl)) {
        ((RevEditor*) listener)->updateMIDICC(getTag());
        return;
    } else if ((button & (kRButton | kControl)) == (kRButton | kControl)) {
        ((RevEditor*) listener)->removeMIDICC(getTag());
        return;
    }

    // set the default value
    if (button & kRButton) {
        // begin of edit parameter
        beginEdit();

        value = getDefaultValue();

        if (isDirty() && listener) 
            listener->valueChanged(pContext, this);

        // end of edit parameter
        endEdit();
        return;
    }

    if (!(button & kLButton))
        return;

    // for compatibility reasons, use linear mode for knobs!
    AEffEditor *pEditor = (AEffEditor *) getEditor();
    if (pEditor) {
        long oldMode = getFrame()->getKnobMode();
        pEditor->setKnobMode(kLinearMode);
        CAnimKnob::mouse(pContext, where, button);
        pEditor->setKnobMode(oldMode);
    }
}

#else
/*****************************************************************************/
/* onMouseDown                                                               */
/*****************************************************************************/

CMouseEventResult CAnimKnobZ::onMouseDown(CPoint &mouse, const long &buttons)
{
    if (!bMouseEnabled) {
        return kMouseEventHandled;
    }
    //getFrame()->setFocusView(this);
    if ((buttons & (kRButton | kShift)) == (kRButton | kShift)) {
        ((RevEditor *)listener)->updateMIDICC(getTag());
        return kMouseEventHandled;
    } else if ((buttons & (kRButton | kControl)) == (kRButton | kControl)) {
        ((RevEditor *)listener)->removeMIDICC(getTag());
        return kMouseEventHandled;
    }
    return CAnimKnob::onMouseDown(mouse, buttons);
}

/*****************************************************************************/
/* onMouseUp                                                                 */
/*****************************************************************************/

CMouseEventResult CAnimKnobZ::onMouseUp(CPoint &mouse, const long &buttons)
{
    return CAnimKnob::onMouseUp(mouse, buttons);
}

/*****************************************************************************/
/* onMouseMoved                                                              */
/*****************************************************************************/

CMouseEventResult CAnimKnobZ::onMouseMoved(CPoint &mouse, const long &buttons)
{
    return CAnimKnob::onMouseMoved(mouse, buttons);
}

#endif
/*===========================================================================*/
/* CHorizontalSlider2 class members                                          */
/*===========================================================================*/

#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR < 6) || VSTGUI_VERSION_MAJOR < 3
/*****************************************************************************/
/* mouse : called when the mouse is moved over the slider                    */
/*****************************************************************************/

void CHorizontalSlider2::mouse(CDrawContext *pContext, CPoint &where, long button) {
    if (!bMouseEnabled)
        return;

    if (button == -1) button = pContext->getMouseButtons();

    if (button & kLButton && button & kControl) {
        ((RevEditor*) listener)->updateMIDICC(getTag());
        return;
    }

    if (button & kRButton && button & kControl) {
        ((RevEditor*) listener)->removeMIDICC(getTag());
        return;
    }

    CHorizontalSlider::mouse(pContext, where, button);
} // mouse

#else
/*****************************************************************************/
/* onMouseDown : called when a mouse down event occurs                       */
/*****************************************************************************/

CMouseEventResult CHorizontalSlider2::onMouseDown(CPoint& where, const long& buttons) {
    if (!bMouseEnabled) {
        return kMouseEventHandled;
    }
    //getFrame()->setFocusView(this);
    if ((buttons & (kRButton | kShift)) == (kRButton | kShift)) {
        ((RevEditor *)listener)->updateMIDICC(getTag());
        return kMouseEventHandled;
    } else if ((buttons & (kRButton | kControl)) == (kRButton | kControl)) {
        ((RevEditor *)listener)->removeMIDICC(getTag());
        return kMouseEventHandled;
    }
    return CHorizontalSlider::onMouseDown(where, buttons);
}

/*****************************************************************************/
/* onMouseUp                                                                 */
/*****************************************************************************/

CMouseEventResult CHorizontalSlider2::onMouseUp(CPoint &mouse, const long &buttons)
{
    return CHorizontalSlider::onMouseUp(mouse, buttons);
}

/*****************************************************************************/
/* onMouseMoved                                                              */
/*****************************************************************************/

CMouseEventResult CHorizontalSlider2::onMouseMoved(CPoint &mouse, const long &buttons)
{
    return CHorizontalSlider::onMouseMoved(mouse, buttons);
}

#endif
/*===========================================================================*/
/* COptionMenuW class members                                                */
/*===========================================================================*/

#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR > 5) || VSTGUI_VERSION_MAJOR > 3
/*****************************************************************************/
/* onWheel                                                                   */
/*****************************************************************************/

bool COptionMenuW::onWheel(const CPoint &mouse, const float &distance, const long &buttons)
{
    if (!bMouseEnabled) {
        return false;
    }
    int iOld = getCurrentIndex(0);
    int i = minmax(iOld - distance, 0, getNbEntries() - 1);
    if (i != iOld) {
        setCurrent(i);
        setDirty(true);
    }
    //beginEdit();
    //if (isDirty() && listener) listener->valueChanged(this);
    //endEdit();
    return true;
}

#endif
/*===========================================================================*/
/* COnOffButtonW class members                                               */
/*===========================================================================*/

#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR > 5) || VSTGUI_VERSION_MAJOR > 3
/*****************************************************************************/
/* onWheel                                                                   */
/*****************************************************************************/

bool COnOffButtonW::onWheel(const CPoint &mouse, const float &distance, const long &buttons)
{
    if (!bMouseEnabled) {
        return false;
    }
    if (distance > 0) {
        value = 1.f;
    }   else {
        value = 0.f;
    }
    beginEdit();
    if (isDirty() && listener) {
        listener->valueChanged(this);
    }
    endEdit();
    return true;
}

#endif
/*===========================================================================*/
/* CKickButton2 class members                                                */
/*===========================================================================*/

#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR > 5) || VSTGUI_VERSION_MAJOR > 3

/*****************************************************************************/
/* onMouseDown                                                               */
/*****************************************************************************/

CMouseEventResult CKickButton2::onMouseDown(CPoint &mouse, const long &buttons)
{
    if (!bMouseEnabled) {
        return kMouseEventHandled;
    }
    //getFrame()->setFocusView(this);
    if ((buttons & (kRButton | kShift)) == (kRButton | kShift)) {
        ((RevEditor *)listener)->updateMIDICC(getTag());
        return kMouseEventHandled;
    } else if ((buttons & (kRButton | kControl)) == (kRButton | kControl)) {
        ((RevEditor *)listener)->removeMIDICC(getTag());
        return kMouseEventHandled;
    }
    if (!(buttons & kLButton)) {
        return kMouseEventHandled;
    }
    beginEdit();
    value = 1.f;
    if (isDirty() && listener) {
        listener->valueChanged(this);
    }
    return onMouseMoved(mouse, buttons);
}

/*****************************************************************************/
/* onMouseUp                                                                 */
/*****************************************************************************/

CMouseEventResult CKickButton2::onMouseUp(CPoint &mouse, const long &buttons)
{
    value = 0.f;
    if (isDirty() && listener) {
        listener->valueChanged(this);
    }
    endEdit();
    return kMouseEventHandled;
}

/*****************************************************************************/
/* onMouseMoved                                                              */
/*****************************************************************************/

CMouseEventResult CKickButton2::onMouseMoved(CPoint &mouse, const long &buttons)
{
    return kMouseEventHandled;
}

/*****************************************************************************/
/* onWheel                                                                   */
/*****************************************************************************/

bool CKickButton2::onWheel(const CPoint &mouse, const float &distance, const long &buttons)
{
    if (!bMouseEnabled) {
        return false;
    }
    if (distance > 0) {
        value = 1.f;
    }   else {
        value = 0.f;
    }
    beginEdit();
    if (isDirty() && listener) {
        listener->valueChanged(this);
    }
    endEdit();
    return true;
}

#endif
/*===========================================================================*/
/* CVerticalSwitch20 class members                                           */
/*===========================================================================*/

/*****************************************************************************/
/* draw : draw the vertical switch                                           */
/*****************************************************************************/
// subclassing this method is only necessary since there's obviously a
// difference in VST SDK 2 and 2.4 regarding the value range <sigh>
// and we need to keep the values from Revitar 2.0 intact

void CVerticalSwitch20::draw(CDrawContext *pContext) {

    int iOffset = minmax(round(value * (float)(iMaxPositions - 1)), 0, iMaxPositions - 1);

    if (pBackground) {
        // source position in bitmap
        CPoint where(0, heightOfOneImage * iOffset);

        if (bTransparencyEnabled)
            pBackground->drawTransparent(pContext, size, where);
        else
            pBackground->draw(pContext, size, where);
    }
    setDirty(false);
}

#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR > 5) || VSTGUI_VERSION_MAJOR > 3
/*****************************************************************************/
/* onWheel                                                                   */
/*****************************************************************************/

bool CVerticalSwitch20::onWheel(const CPoint &mouse, const float &distance, const long &buttons)   //v2.13
{
    if (!bMouseEnabled) {
        return false;
    }
    int iVal = round(value * (float)(iMaxPositions - 1));
    if (distance > 0) {
        iVal = minmax(iVal - 1, 0, iMaxPositions - 1);
    } else {
        iVal = minmax(iVal + 1, 0, iMaxPositions - 1);
    }

    value = minmax((float)(iVal) / (float)(iMaxPositions - 1), 0.f, 1.f);

    beginEdit();
    if (isDirty() && listener) {
        listener->valueChanged(this);
    }
    endEdit();
    return true;
}

#endif
/*===========================================================================*/
/* C2DSwitch class members                                                   */
/*===========================================================================*/

#if (VSTGUI_VERSION_MAJOR == 3 && VSTGUI_VERSION_MINOR < 6) || VSTGUI_VERSION_MAJOR < 3
/*****************************************************************************/
/* mouse : mouse is moved over the control                                   */
/*****************************************************************************/

void C2DSwitch::mouse(CDrawContext *pContext, CPoint &where, long button) {
    if (!bMouseEnabled)
        return;

    if (button == -1) button = pContext->getMouseButtons();
    if ((button & (kLButton | kControl)) == (kLButton | kControl)) {
        ((RevEditor*) listener)->updateMIDICC(getTag());
        return;
    } else if ((button & (kRButton | kControl)) == (kRButton | kControl)) {
        ((RevEditor*) listener)->removeMIDICC(getTag());
        return;
    }

    if (!(button & kLButton))
        return;

    // set the default value
    /*
        if (button == (kControl|kLButton))
        {
            value = getDefaultValue ();
            if (isDirty () && listener)
                listener->valueChanged (pContext, this);
            return;
        }
     */

    float coefVert = (float) (size.bottom - size.top);
    float coefHorz = (float) (size.right - size.left);

    // begin of edit parameter

    beginEdit();
    do {
        //  if(where.v >= size.top && where.v < size.bottom &&
        //      where.h >= size.left && where.h < size.right)
        {

            m_HorzVal = minmax((float)(where.h - size.left) / coefHorz, 0.f, 1.f);  //.99f);
            m_VertVal = minmax((float)(where.v - size.top)  / coefVert, 0.f, 1.f);  //.99f);

            int iHorzVal = round(m_HorzVal * (float)(m_iMaxPositionsHorz - 1));
            int iVertVal = round(m_VertVal * (float)(m_iMaxPositionsVert - 1));

            m_iSwitchVal = iVertVal * m_iMaxPositionsHorz + iHorzVal;// + 1;

            value = minmax((float)(m_iSwitchVal) / (float)(iMaxPositions - 1), 0.f, 1.f);

            setDirty(true);
        }


        if (isDirty() && listener)     
            listener->valueChanged(pContext, this);

        getMouseLocation(pContext, where);
        doIdleStuff();
    } while (pContext->getMouseButtons() == button);

    // end of edit parameter
    endEdit();
}

#else
/*****************************************************************************/
/* onMouseDown                                                               */
/*****************************************************************************/

CMouseEventResult C2DSwitch::onMouseDown(CPoint& where, const long& buttons)
{
    if (!bMouseEnabled) {
        return kMouseEventHandled;
    }
    //getFrame()->setFocusView(this);
    if ((buttons & (kRButton | kShift)) == (kRButton | kShift)) {
        ((RevEditor *)listener)->updateMIDICC(getTag());
        return kMouseEventHandled;
    } else if ((buttons & (kRButton | kControl)) == (kRButton | kControl)) {
        ((RevEditor *)listener)->removeMIDICC(getTag());
        return kMouseEventHandled;
    }
    if (!(buttons & kLButton)) {
        return kMouseEventHandled;
    }
    beginEdit();
    coefHorz = (float)(size.right - size.left);
    coefVert = (float)(size.bottom - size.top);
    return onMouseMoved(where, buttons);
}

/*****************************************************************************/
/* onMouseUp                                                                 */
/*****************************************************************************/

CMouseEventResult C2DSwitch::onMouseUp(CPoint &mouse, const long &buttons)
{
    endEdit();
    return kMouseEventHandled;
}

/*****************************************************************************/
/* onMouseMoved                                                              */
/*****************************************************************************/

CMouseEventResult C2DSwitch::onMouseMoved(CPoint &mouse, const long &buttons)
{
    if (!(buttons & kLButton)) {
        return kMouseEventHandled;
    }
    m_HorzVal = minmax((float)(mouse.h - size.left) / coefHorz, 0.f, 1.f);  //.99f);
    m_VertVal = minmax((float)(mouse.v - size.top)  / coefVert, 0.f, 1.f);  //.99f);
    int iHorzVal = round(m_HorzVal * (float)(m_iMaxPositionsHorz - 1));
    int iVertVal = round(m_VertVal * (float)(m_iMaxPositionsVert - 1));
    m_iSwitchVal = iVertVal * (m_iMaxPositionsHorz) + iHorzVal; // + 1;
    value = minmax((float)(m_iSwitchVal) / (float)(iMaxPositions - 1), 0.f, 1.f);
    if (isDirty() && listener) {
        listener->valueChanged(this);
    }
    //if (isDirty()) invalid();
    return kMouseEventHandled;
}

/*****************************************************************************/
/* onWheel                                                                   */
/*****************************************************************************/

bool C2DSwitch::onWheel(const CPoint &mouse, const float &distance, const long &buttons)   //v2.13
{
    if (!bMouseEnabled) {
        return false;
    }
    if (iMaxPositions < 7) {
        if (distance > 0) {
            m_iSwitchVal = minmax(m_iSwitchVal + 1, 0, iMaxPositions - 1);
        } else {
            m_iSwitchVal = minmax(m_iSwitchVal - 1, 0, iMaxPositions - 1);
        }
    } else {
        int iHorzVal = round(m_HorzVal * (float)(m_iMaxPositionsHorz - 1));
        int iVertVal = round(m_VertVal * (float)(m_iMaxPositionsVert - 1));
        if (buttons & kControl) {
            if (distance > 0) {
                iHorzVal = minmax(iHorzVal + 1, 0, m_iMaxPositionsHorz - 1);
            } else {
                iHorzVal = minmax(iHorzVal - 1, 0, m_iMaxPositionsHorz - 1);
            }
            m_HorzVal = (float)(iHorzVal) / (float)(m_iMaxPositionsHorz - 1);
        } else {
            if (distance > 0) {
                iVertVal = minmax(iVertVal - 1, 0, m_iMaxPositionsVert - 1);
            } else {
                iVertVal = minmax(iVertVal + 1, 0, m_iMaxPositionsVert - 1);
            }
            m_VertVal = (float)(iVertVal) / (float)(m_iMaxPositionsVert - 1);
        }
        m_iSwitchVal = iVertVal * (m_iMaxPositionsHorz) + iHorzVal;
    }
    value = minmax((float)(m_iSwitchVal) / (float)(iMaxPositions - 1), 0.f, 1.f);
    beginEdit();
    if (isDirty() && listener) {
        listener->valueChanged(this);
    }
    endEdit();
    return true;
}

#endif
/*****************************************************************************/
/* setHorz                                                                   */
/*****************************************************************************/

void C2DSwitch::setHorz(float v) {
    m_HorzVal = v;

    int iVertVal = round(m_VertVal * (float) (m_iMaxPositionsVert -1));
    int iHorzVal = round(m_HorzVal * (float) (m_iMaxPositionsHorz -1));
    // -1's and int->round added in Asseca's 2.13

    m_iSwitchVal = iVertVal * m_iMaxPositionsHorz + iHorzVal;// + 1; --Asseca 2.13

    value = minmax((float)(m_iSwitchVal) / (float)(iMaxPositions - 1), 0.0f, 1.0f);

    setDirty(true);
}

/*****************************************************************************/
/* setVert                                                                   */
/*****************************************************************************/

void C2DSwitch::setVert(float v) {
    m_VertVal = v;

    int iVertVal = round(m_VertVal * (float) (m_iMaxPositionsVert -1));
    int iHorzVal = round(m_HorzVal * (float) (m_iMaxPositionsHorz -1));
    // -1's and int->round added in Asseca's 2.13

    m_iSwitchVal = iVertVal * m_iMaxPositionsHorz + iHorzVal;// + 1; --Asseca 2.13

    value = minmax((float)(m_iSwitchVal) / (float)(iMaxPositions - 1), 0.0f, 1.0f);

    setDirty(true);
}
