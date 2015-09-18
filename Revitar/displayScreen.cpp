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
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <winreg.h>

#include "RevEditor.h"
#include "Revitar.h"

void CDisplayScreen::drawDisplay() {
    int rowDel, colDel, row, col, k;


    if (m_BPP == 3 || m_BPP == 4) {
        if (m_BodyChanged) {
            m_BodyChanged = false;

            memcpy(m_BMBackground,
                    m_BMBody + m_iBodyType * DISPLAY_WIDTH * DISPLAY_HEIGHT * m_BPP,
                    DISPLAY_WIDTH * DISPLAY_HEIGHT * m_BPP * sizeof (char));

            colDel = 135 + (int) (m_fPickUp * 110.0f);
            rowDel = 1;

            for (row = 0; row < PICKUP_HEIGHT; row++) {
                int offsetTmp0 = row * PICKUP_WIDTH * m_BPP;
                int offsetTmp1 = (row + rowDel) * DISPLAY_WIDTH * m_BPP;

                for (col = 0; col < PICKUP_WIDTH; col++) {
                    int offset1 = offsetTmp0 + col * m_BPP;

                    if (m_BMPickUp[offset1 + 2] != -1) {
                        int offset = offsetTmp1 + (col + colDel) * m_BPP;
                        m_BMBackground[offset + 0] = m_BMPickUp[offset1 + 0];
                        m_BMBackground[offset + 1] = m_BMPickUp[offset1 + 1];
                        m_BMBackground[offset + 2] = m_BMPickUp[offset1 + 2];
                    }
                }
            }

            if (m_ChordOn > 0.f) {
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
                                    m_BMBackground[offset + 2] = m_BMChordNote[offset1 + 2];
                                }
                            } else {
                                for (col = 0; col < CHORDNOTE_WIDTH; col++) {
                                    int offset1 = (row) * CHORDNOTE_WIDTH * m_BPP + col*m_BPP;
                                    int offset = (row + rowDel) * DISPLAY_WIDTH * m_BPP + (col + colDel) * m_BPP;
                                    m_BMBackground[offset + 0] = m_BMChordNote[offset1 + 0];
                                    m_BMBackground[offset + 1] = m_BMChordNote[offset1 + 1];
                                    m_BMBackground[offset + 2] = m_BMChordNote[offset1 + 2];
                                }
                            }
                        }
                    }
                }
            }

            for (k = 0; k < MAX_NUM_POLY; k++) {
                for (col = 442; col < 453; col++) {
                    int ofsetTmp0 = (k * CHORD_DISPLAY_HEIGHT + 9) * DISPLAY_WIDTH + (col + 126);
                    int offset = (CHORD_DISPLAY_HEIGHT / 2) * DISPLAY_WIDTH + ofsetTmp0;
                    offset *= m_BPP;

                    m_BMBackground[offset + 0] = (BYTE) 255;
                    m_BMBackground[offset + 1] = (BYTE) 255;
                    m_BMBackground[offset + 2] = (BYTE) 255;
                }
            }

        }

        if (*m_pChordDisplayUpdate >= 200.0f) {
            memcpy(m_ChordDisplayCurrent, m_ChordDisplay,
                    MAX_NUM_POLY * NUM_POINTS * CHORD_DISPLAY_HEIGHT * sizeof (float));

            float val0 = (*m_pChordDisplayUpdate) / MAX_NUM_POLY;

            for (k = 0; k < MAX_NUM_POLY; k++)
                for (col = 0; col < NUM_POINTS; col++)
                    for (row = 0; row < CHORD_DISPLAY_HEIGHT; row++)
                        m_ChordDisplayCurrent[k][col][row] /= val0;

            *m_pChordDisplayUpdate = 0.0;
            memset(m_ChordDisplay, 0, MAX_NUM_POLY * NUM_POINTS * CHORD_DISPLAY_HEIGHT * sizeof (float));
        }

        memcpy(m_BMDisplay, m_BMBackground, DISPLAY_WIDTH * DISPLAY_HEIGHT * m_BPP * sizeof (char));

        for (k = 0; k < MAX_NUM_POLY; k++) {
            for (col = 0; col < 442; col++) {
                int idx = (int) ((NUM_POINTS - 1) * col / 451);
                int ofsetTmp0 = (k * CHORD_DISPLAY_HEIGHT + 9) * DISPLAY_WIDTH * m_BPP + (col + 126) * m_BPP;

                for (row = 0; row < CHORD_DISPLAY_HEIGHT; row++) {
                    float del = m_ChordDisplayCurrent[k][idx][row];
                    if (del > 0.01f) {
                        int offset = (row) * DISPLAY_WIDTH * m_BPP + ofsetTmp0;
                        char val = (char) (250.0f * del);
                        del = 1.0f - del;

                        m_BMDisplay[offset + 0] = (BYTE) (val + (float) (unsigned char) m_BMDisplay[offset + 0] * del);
                        m_BMDisplay[offset + 1] = (BYTE) (val + (float) (unsigned char) m_BMDisplay[offset + 1] * del);
                        m_BMDisplay[offset + 2] = (BYTE) (val + (float) (unsigned char) m_BMDisplay[offset + 2] * del);
                    }
                }
            }
        }

        colDel = 135 + (int) (m_fPickPosition * 107.0f);
        rowDel = 4;

        for (row = 0; row < PICK_HEIGHT; row++) {
            for (col = 0; col < PICK_WIDTH; col++) {
                int offset1 = row * PICK_WIDTH * m_BPP + col * m_BPP;

                if (m_BMPick[offset1 + 2] != -1) {
                    int offset = (row + rowDel) * DISPLAY_WIDTH * m_BPP + (col + colDel) * m_BPP;
                    m_BMDisplay[offset + 0] = m_BMPick[offset1 + 0];
                    m_BMDisplay[offset + 1] = m_BMPick[offset1 + 1];
                    m_BMDisplay[offset + 2] = m_BMPick[offset1 + 2];
                }
            }
        }
    } else if (m_BPP == 2) {
        if (m_BodyChanged) {
            m_BodyChanged = false;

            memcpy(m_BMBackground, m_BMBody + m_iBodyType * DISPLAY_WIDTH * DISPLAY_HEIGHT * m_BPP,
                    DISPLAY_WIDTH * DISPLAY_HEIGHT * m_BPP * sizeof (char));

            colDel = 135 + (int) (m_fPickUp * 110.0f);
            rowDel = 1;

            for (row = 0; row < PICKUP_HEIGHT; row++) {
                int offsetTmp0 = row * PICKUP_WIDTH * m_BPP;
                int offsetTmp1 = (row + rowDel) * DISPLAY_WIDTH * m_BPP;

                for (col = 0; col < PICKUP_WIDTH; col++) {
                    int offset1 = offsetTmp0 + col * m_BPP;
                    int offset = offsetTmp1 + (col + colDel) * m_BPP;

                    if (m_BMPickUp[offset1 + 0] != -1) {
                        m_BMBackground[offset + 0] = m_BMPickUp[offset1 + 0];
                        m_BMBackground[offset + 1] = m_BMPickUp[offset1 + 1];
                    }
                }
            }

            if (m_ChordOn == 1.0f) {
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
                                }
                            } else {
                                for (col = 0; col < CHORDNOTE_WIDTH; col++) {
                                    int offset1 = (row) * CHORDNOTE_WIDTH * m_BPP + col * m_BPP;
                                    int offset = (row + rowDel) * DISPLAY_WIDTH * m_BPP + (col + colDel) * m_BPP;
                                    m_BMBackground[offset + 0] = m_BMChordNote[offset1 + 0];
                                    m_BMBackground[offset + 1] = m_BMChordNote[offset1 + 1];
                                }
                            }
                        }

                    }
                }
            }

            for (k = 0; k < MAX_NUM_POLY; k++) {
                for (col = 442; col < 453; col++) {
                    int ofsetTmp0 = (k * CHORD_DISPLAY_HEIGHT + 9) * DISPLAY_WIDTH + (col + 126);
                    int offset = (CHORD_DISPLAY_HEIGHT / 2) * DISPLAY_WIDTH + ofsetTmp0;
                    offset *= m_BPP;

                    m_BMBackground[offset + 0] = (BYTE) 255;
                    m_BMBackground[offset + 1] = (BYTE) 255;
                }
            }
        }

        if (*m_pChordDisplayUpdate >= 200.0f) {
            *m_pChordDisplayUpdate = 0.0f;
            memset(m_ChordDisplay, 0, MAX_NUM_POLY * NUM_POINTS * CHORD_DISPLAY_HEIGHT * sizeof (float));
        }

        memcpy(m_BMDisplay, m_BMBackground, DISPLAY_WIDTH * DISPLAY_HEIGHT * m_BPP * sizeof (char));

        for (k = 0; k < MAX_NUM_POLY; k++) {
            for (col = 0; col < 442; col++) {
                int idx = (int) ((NUM_POINTS - 1) * col / 451);
                int ofsetTmp0 = (k * CHORD_DISPLAY_HEIGHT + 9) * DISPLAY_WIDTH * m_BPP + (col + 126) * m_BPP;

                for (row = 0; row < CHORD_DISPLAY_HEIGHT; row++) {
                    float del = m_ChordDisplayCurrent[k][idx][row];
                    if (del > 0.01f) {
                        int offset = (row) * DISPLAY_WIDTH * m_BPP + ofsetTmp0;

                        m_BMDisplay[offset + 0] = (BYTE) 255;
                        m_BMDisplay[offset + 1] = (BYTE) 255;
                    }
                }
            }
        }

        colDel = 135 + (int) (m_fPickPosition * 107.0f);
        rowDel = 4;

        for (row = 0; row < PICK_HEIGHT; row++) {
            for (col = 0; col < PICK_WIDTH; col++) {
                int offset1 = row * PICK_WIDTH * m_BPP + col * m_BPP;
                int offset = (row + rowDel) * DISPLAY_WIDTH * m_BPP + (col + colDel) * m_BPP;

                if (m_BMPick[offset1 + 0] != -1) {
                    m_BMDisplay[offset + 0] = m_BMPick[offset1 + 0];
                    m_BMDisplay[offset + 1] = m_BMPick[offset1 + 1];
                }
            }
        }
    }

    SetBitmapBits((HBITMAP) pDisplay->getHandle(), DISPLAY_WIDTH * DISPLAY_HEIGHT*m_BPP, m_BMDisplay);
} // drawDisplay

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
            m_ChordDisplayCurrent[polyIdx][i][CHORD_DISPLAY_HEIGHT / 2] = 1.0;
}

#if VSTGUI_VERSION_MAJOR <= 3 && VSTGUI_VERSION_MINOR < 5

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
        int pickh = (int) (m_fPickPosition * 107.f) + 150;

        if (m_ChordPressed == NO_INFORMATION) {
            if (m_fPickPosition || m_PickUpPressed || (where.h > 150 && where.h < 150 + 107)) {
                if (m_PickPressed ||
                        (!m_PickUpPressed && where.h > pickh - 12 && where.h < pickh + 12 && where.v < 100)) {
                    if (button & kLButton && button & kControl) {
                        ((RevEditor*) listener)->updateMIDICC(kPickPosition);
                        m_PickPressed = false;
                        return;
                    }

                    if (button & kRButton && button & kControl) {
                        ((RevEditor*) listener)->removeMIDICC(kPickPosition);
                        m_PickPressed = false;
                        return;
                    }

                    m_fPickPosition = (float) (where.h - 150) / 107.f;

                    if (m_fPickPosition < 0.0)
                        m_fPickPosition = 0.0;
                    if (m_fPickPosition > 1.0)
                        m_fPickPosition = 1.0;

                    m_PickPressed = true;
                    //	m_BodyChanged = true;
                    setDirty(true);
                } else {
                    int pickuph = (int) (m_fPickUp * 110.f) + 145;

                    if (m_PickUpPressed || (where.h > pickuph - 8 && where.h < pickuph + 8)) {
                        if (button & kLButton && button & kControl) {
                            ((RevEditor*) listener)->updateMIDICC(kPickUp);
                            m_PickUpPressed = false;
                            return;
                        }

                        if (button & kRButton && button & kControl) {
                            ((RevEditor*) listener)->removeMIDICC(kPickUp);
                            m_PickPressed = false;
                            return;
                        }

                        m_fPickUp = (float) (where.h - 145) / 110.f;

                        if (m_fPickUp < 0.0)
                            m_fPickUp = 0.0;
                        if (m_fPickUp > 1.0)
                            m_fPickUp = 1.0;

                        m_PickUpPressed = true;
                        m_BodyChanged = true;
                        setDirty(true);
                    }

                }
            }
        }

        if (m_ChordOn == 1.0 && !m_PickPressed && !m_PickUpPressed) {
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
    m_PickUpPressed = false;
    m_ChordPressed = NO_INFORMATION;
    m_FirstNote = NO_INFORMATION;


    // end of edit parameter

    endEdit();

} // mouse
#else
//I have no idea what I'm doing here.
/*****************************************************************************/

/* onMouseDown : called when a mouse down event occurs
/*****************************************************************************/
CMouseEventResult CDisplayScreen::onMouseDown(CPoint &where, const long &buttons) {
    if (!bMouseEnabled)
        return kMouseEventNotHandled;
    //may not even need to perform that check?

    if (!(buttons & kLButton) && !(buttons & kRButton && buttons & kControl)) {
        return kMouseEventNotHandled;
    }

    // begin of edit parameter

    beginEdit();
    return onMouseMoved(where, buttons);



    // end of edit parameter

}

CMouseEventResult CDisplayScreen::onMouseMoved(CPoint& where, const long& buttons) {

    int pickh = (int) (m_fPickPosition * 107.f) + 150;

    if (m_ChordPressed == NO_INFORMATION) {
        if (m_fPickPosition || m_PickUpPressed || (where.h > 150 && where.h < 150 + 107)) {
            if (m_PickPressed ||
                    (!m_PickUpPressed && where.h > pickh - 12 && where.h < pickh + 12 && where.v < 100)) {
                if (buttons & kLButton && buttons & kControl) {
                    ((RevEditor*) listener)->updateMIDICC(kPickPosition);
                    m_PickPressed = false;
                    return;
                }

                if (buttons & kRButton && buttons & kControl) {
                    ((RevEditor*) listener)->removeMIDICC(kPickPosition);
                    m_PickPressed = false;
                    return;
                }

                m_fPickPosition = (float) (where.h - 150) / 107.f;

                if (m_fPickPosition < 0.0)
                    m_fPickPosition = 0.0;
                if (m_fPickPosition > 1.0)
                    m_fPickPosition = 1.0;

                m_PickPressed = true;
                //	m_BodyChanged = true;
                setDirty(true);
            } else {
                int pickuph = (int) (m_fPickUp * 110.f) + 145;

                if (m_PickUpPressed || (where.h > pickuph - 8 && where.h < pickuph + 8)) {
                    if (buttons & kLButton && buttons & kControl) {
                        ((RevEditor*) listener)->updateMIDICC(kPickUp);
                        m_PickUpPressed = false;
                        return;
                    }

                    if (buttons & kRButton && buttons & kControl) {
                        ((RevEditor*) listener)->removeMIDICC(kPickUp);
                        m_PickPressed = false;
                        return;
                    }

                    m_fPickUp = (float) (where.h - 145) / 110.f;

                    if (m_fPickUp < 0.0)
                        m_fPickUp = 0.0;
                    if (m_fPickUp > 1.0)
                        m_fPickUp = 1.0;

                    m_PickUpPressed = true;
                    m_BodyChanged = true;
                    setDirty(true);
                }

            }
        }
    }

    if (m_ChordOn == 1.0 && !m_PickPressed && !m_PickUpPressed) {
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
        listener->valueChanged(this);

    if (m_BodyChanged == true) {
        setDirty(true);
    }

    //pContext->getMouseLocation(where);
    getFrame()->getCurrentMouseLocation(where);



    //doIdleStuff();



}

CMouseEventResult CDisplayScreen::onMouseUp(CPoint& where, const long& buttons) {

    m_PickPressed = false;
    m_PickUpPressed = false;
    m_ChordPressed = NO_INFORMATION;
    m_FirstNote = NO_INFORMATION;
    endEdit();
    return kMouseEventHandled;
}
#endif

void CDisplayScreen::setChordOnValue(float value) {

    m_ChordOn = value;

    m_BodyChanged = true;

    setDirty(true);

} // setChordOnValue

void CDisplayScreen::setBodyValue(float value) {

    m_iBodyType = (int) ((float) (NUM_BODIES - 1) * value);

    if (m_iBodyType < 0)
        m_iBodyType = 0;
    if (m_iBodyType > NUM_BODIES - 1)
        m_iBodyType = NUM_BODIES - 1;

    m_BodyChanged = true;

    setDirty(true);

} // setChordOnValue

void CDisplayScreen::draw(CDrawContext *pContext) {

    if (pDisplay) {
        drawDisplay();
        pDisplay->draw(pContext, size, offset);
    }

    setDirty(false);

}

CDisplayScreen::~CDisplayScreen() {
    if (pHandle)
        pHandle->forget();
}

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

} // draw

