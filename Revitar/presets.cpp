/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Revitar originally by C. Lawrence Zitnick III
 * This fork maintained by SwooshyCueb
 */

#include <stdio.h>
#include <windows.h>

#include "resource.h"
#include "RevEditor.h"
#include "Revitar.h"
#include "vstsdk2.4.2\AEffEditor.h"


/*****************************************************************************/
/* SwapBytes : swaps bytes between little- and big-endian order              */
/*****************************************************************************/

void inline SwapBytes(VstInt32 &l)
{
unsigned char *b = (unsigned char *)&l;
VstInt32 intermediate = ((VstInt32)b[0] << 24) |
                        ((VstInt32)b[1] << 16) |
                        ((VstInt32)b[2] << 8) |
                        (VstInt32)b[3];
l = intermediate;
}

void inline SwapBytes(float &f)
{
unsigned char *b = (unsigned char *)&f;
VstInt32 intermediate = ((VstInt32)b[0] << 24) |
                        ((VstInt32)b[1] << 16) |
                        ((VstInt32)b[2] << 8) |
                        (VstInt32)b[3];
f = *((float *)&intermediate);
}


/*===========================================================================*/
/* CResource class                                                           */
/*===========================================================================*/

extern void *hInstance;                 /* instance handle for the DLL       */
class CResource
  {
  public:
    CResource ( )
      { Init(); }
	CResource (LPCTSTR sName, LPCTSTR sType)
	  { Init(); LoadResource(sName, sType); }
    CResource (UINT nResource, LPCTSTR sType)
      { Init(); LoadResource(nResource, sType); }
    CResource (CResource const & org)   /* copy constructor                  */
      { DoCopy(org); }
                                        /* destructor                        */
	virtual ~CResource() { FreeResource(); }
                                        /* assignment operator               */
    CResource &operator=(CResource const &org) 
      { return DoCopy(org); }

    void Init ( );

	int LoadResource(LPCTSTR sName, LPCTSTR sType);
	int LoadResource(UINT nResource, LPCTSTR sType)
      { return LoadResource(MAKEINTRESOURCE(nResource), sType); }
	void FreeResource();

	void * GetData() { return lpResource; }
	int GetSize() { return (int)dwResourceSize; }

// implementation
  private:
    CResource &DoCopy (CResource const &org);
	LPSTR  lpiName;
	LPSTR  lpiType;
	HGLOBAL hResource;
	DWORD   dwResourceSize;
	LPVOID  lpResource;
  };

/*****************************************************************************/
/* Init : (re-)initialize configuration                                      */
/*****************************************************************************/

void CResource::Init()
{
hResource = 0;
dwResourceSize = 0;
lpResource = NULL;
lpiName = NULL;
lpiType = NULL;
}

/*****************************************************************************/
/* DoCopy : copies another CResource object                                   */
/*****************************************************************************/

CResource &CResource::DoCopy (CResource const &org)
{
FreeResource();
if (org.hResource)
  LoadResource(org.lpiName, org.lpiType);
return *this;
}

/*****************************************************************************/
/* LoadResource : loads in a resource                                        */
/*****************************************************************************/

BOOL CResource::LoadResource
    (
	LPCTSTR lpName, 
	LPCTSTR lpType
	)
{
FreeResource();

if (HIWORD(lpName))
  {
  lpiName = new char[lstrlen(lpName)+1];
  if (lpiName)
	lstrcpy(lpiName, lpName);
  }
else
  lpiName = (char *)lpName;

if (HIWORD(lpType))
  {
  lpiType = new char[lstrlen(lpType)+1];
  if (lpiType)
	lstrcpy(lpiType, lpType);
  }
else
  lpiType = (char *)lpType;

HRSRC hFind = ::FindResource((HINSTANCE)hInstance, lpName, lpType);
if (hFind == NULL)
  return 0;

dwResourceSize = ::SizeofResource((HINSTANCE)hInstance, hFind);
hResource = ::LoadResource((HINSTANCE)hInstance, hFind);
if (hResource)                          /* if resource is available          */
  lpResource = ::LockResource(hResource);

return !!lpResource;
}

/*****************************************************************************/
/* LoadResource : loads in a resource                                        */
/*****************************************************************************/

void CResource::FreeResource ()
{
if (lpResource)
  UnlockResource(hResource);
if (hResource)
  ::FreeResource(hResource);
if (HIWORD(lpiName))
  delete[] lpiName;
if (HIWORD(lpiType))
  delete[] lpiType;
Init();
}


/*****************************************************************************/
/* LoadBank : loads a bank of presets                                        */
/*****************************************************************************/

bool Revitar::LoadBank(void *data, size_t lSize)
{
if (lSize < sizeof(struct fxBank))
  return false;

fxBank *fxb = (fxBank *)data;
                                        /* look whether byte swapping needed */
static const VstInt32 lcMagic = cMagic;
bool NeedsBSwap = !!memcmp(&fxb->chunkMagic, &lcMagic, sizeof(lcMagic));
if (NeedsBSwap)
  {
  SwapBytes(fxb->chunkMagic);
  if (fxb->chunkMagic != cMagic)
    {
    SwapBytes(fxb->chunkMagic);
    return false;
    }
  }
if (NeedsBSwap)                         /* swap header contents if needed    */
  {
  SwapBytes(fxb->byteSize);
  SwapBytes(fxb->fxMagic);
  SwapBytes(fxb->version);
  SwapBytes(fxb->fxID);
  SwapBytes(fxb->fxVersion);
  SwapBytes(fxb->numPrograms);
  SwapBytes(fxb->currentProgram);
  }

if ((fxb->chunkMagic != cMagic) ||      /* if erroneous data in there        */
    (fxb->version > 2) ||
    (fxb->fxMagic != bankMagic) ||
    (fxb->fxID != cEffect.uniqueID))
  return false;                         /* return error                      */

if (fxb->version < 2)                   /* autocorrect invalid program       */
  fxb->currentProgram = 0;

int i, j;
                                        /* validate & swap bytes             */
fxProgram *program = (fxProgram *)((char *)data +
                                     (long)((fxBank *)0)->content.programs);
for (i = 0; i < fxb->numPrograms; i++)
  {
  if (NeedsBSwap)                       /* swap program header data if needed*/
    {
    SwapBytes(program->chunkMagic);
    SwapBytes(program->byteSize);
    SwapBytes(program->fxMagic);
    SwapBytes(program->version);
    SwapBytes(program->fxID);
    SwapBytes(program->fxVersion);
    SwapBytes(program->numParams);
    }                                   /* if erroneous data in there        */
  if ((program->chunkMagic != cMagic) ||
      (program->version > 1) ||
      (program->fxMagic != fMagic) ||
      (program->fxID != cEffect.uniqueID))
    return false;
  if (!program->byteSize)               /* autocorrect byte size             */
    program->byteSize = (int)(&((struct fxProgram *)0)->content.params) -
                        (int)(&((struct fxProgram *)0)->fxMagic) +
                        program->numParams * sizeof(float);
  
  if (NeedsBSwap)
    for (j = 0; j < program->numParams; j++)
      SwapBytes(program->content.params[j]);
    program = (fxProgram *)((char *)program +
                            2 * sizeof(VstInt32) +
                            program->byteSize);
  }
                                        /* now load the programs             */
program = (fxProgram *)((char *)data +
                        (long)((fxBank *)0)->content.programs);
int numPrograms = min((int)fxb->numPrograms, cEffect.numPrograms);
for (i = 0; i < numPrograms; i++)
  {
  int numParams = min((int)program->numParams, cEffect.numParams);
                                        /* copy as many data as possible     */
  memcpy(programs[i].name, program->prgName, sizeof(program->prgName));
  programs[i].name[sizeof(program->prgName)] = '\0';
  
  programs[i].fKnob[kKnobGain] = program->content.params[kKnobGain];
  programs[i].fKnob[kKnobBodyGain] = program->content.params[kKnobBodyGain];
  programs[i].fKnob[kKnobPickVolume] = program->content.params[kKnobPickVolume];
  programs[i].fKnob[kKnobTuning] = program->content.params[kKnobTuning];
  programs[i].fKnob[kKnobBridgeDamping] = program->content.params[kKnobBridgeDamping];
  programs[i].fKnob[kKnobStringDamping] = program->content.params[kKnobStringDamping];
  programs[i].fKnob[kKnobVibratoAmplit] = program->content.params[kKnobVibratoAmplit];
  programs[i].fKnob[kKnobVibratoRate] = program->content.params[kKnobVibratoRate];
  programs[i].fKnob[kKnobSympathetic] = program->content.params[kKnobSympathetic];
  programs[i].fKnob[kKnobSlap] = program->content.params[kKnobSlap];
  programs[i].fKnob[kKnobPickSpeed] = program->content.params[kKnobPickSpeed];
  programs[i].fKnob[kKnobChordRate] = program->content.params[kKnobChordRate];
  programs[i].fKnob[kKnobStringType] = program->content.params[kKnobStringType];
  programs[i].fKnob[kKnobPalmDamp] = program->content.params[kKnobPalmDamp];
  programs[i].fKnob[kKnobSlideRate] = program->content.params[kKnobSlideRate];

  programs[i].fChordOnOff = program->content.params[kChordOnOff];
  programs[i].fSustain = program->content.params[kSustain];
  programs[i].fPickNoise = program->content.params[kPickNoise];
  programs[i].fMono = program->content.params[kMono];
  programs[i].fAbsRel = program->content.params[kAbsRel];
  programs[i].fChordIdx = program->content.params[kChord];
  programs[i].fChordAbsIdx = program->content.params[kChordAbs];
  programs[i].fChordSwitch = program->content.params[kChordSwitch];
  programs[i].fBodySwitch = program->content.params[kBodySwitch];
  programs[i].fPickPosition = program->content.params[kPickPosition];
  programs[i].fPickUp = program->content.params[kPickUp];
  programs[i].fPickWidth = program->content.params[kPickWidth];
  programs[i].fPickStiffness = program->content.params[kPickStiffness];
  programs[i].fPalmSlider = program->content.params[kPalmSlider];
  programs[i].fStopSwitch = program->content.params[kStopSwitch];
  
  int curparm = kChordNotes;
  if (numParams >= kChordAbsNotes)
    {
    for (int j = 0; j < NUM_CHORDS; j++)
      for (int k = 0; k < NUM_CHORD_NOTES; k++)
        programs[i].fChordNote[j][k] = program->content.params[curparm++];
    }
  if (numParams >= kNumParams)
    {
    for (int j = 0; j < NUM_CHORDS; j++)
      for (int k = 0; k < NUM_CHORD_NOTES; k++)
        programs[i].fChordAbsNote[j][k] = program->content.params[curparm++];
    }
  // ignore all other parameters. They're calculated at runtime. Or useless.

  program = (fxProgram *)((char *)program +
                          2 * sizeof(VstInt32) +
                          program->byteSize);
  }

setProgram(fxb->currentProgram);
return true;
}

bool Revitar::LoadBank(char *name)
{
FILE *fp = fopen(name, "rb");
if (!fp)
  return false;
bool bRC = true;
unsigned char *pData = NULL;
try
  {
  fseek(fp, 0, SEEK_END);
  size_t fs = ftell(fp);
  pData = new unsigned char[fs];
  if (!pData)
    throw (int)1;
  fseek(fp, 0, SEEK_SET);
  if (fread(pData, 1, fs, fp) != fs)
    throw (int)2;
  bRC = LoadBank(pData, fs);
  }
catch(...)
  {
  bRC = false;
  }

if (pData)
  delete[] pData;
fclose(fp);
return bRC;
}

/*****************************************************************************/
/* SaveBank : saves a bank of presets                                        */
/*****************************************************************************/

bool Revitar::SaveBank(char *name)
{
return false;  // not implemented yet
}

/*****************************************************************************/
/* setPresets : initializes the presets                                      */
/*****************************************************************************/

void Revitar::setPresets()
{
unsigned char *pBank = NULL;

try
  {
  // use preset resource
  CResource res;
  if (!res.LoadResource(IDR_PRESETBANK, "FXB"))
    return;
  pBank = new unsigned char[res.GetSize()];
  if (!pBank)
    return;
  memcpy(pBank, res.GetData(), res.GetSize());
  LoadBank(pBank, res.GetSize());
  }
catch(...)
  {
  }
if (pBank)
  delete pBank;
} //setPresets


/*===========================================================================*/
/* RevitarProgram class members                                              */
/*===========================================================================*/

/*****************************************************************************/
/* RevitarProgram : constructor                                              */
/*****************************************************************************/

RevitarProgram::RevitarProgram ()
{	
	strcpy(name, "Standard");
	fKnob[0] = 0.500000f;
	fKnob[1] = 0.500000f;
	fKnob[2] = 0.196778f;
	fKnob[3] = 0.500000f;
	fKnob[4] = 0.404050f;
	fKnob[5] = 0.600000f;
	fKnob[6] = 0.100000f;
	fKnob[7] = 0.500000f;
	fKnob[8] = 0.45000f;
	fKnob[9] = 0.160000f;
	fKnob[10] = 0.75000f;
	fKnob[11] = 0.760000f;
	fKnob[12] = 0.50000f;
	fKnob[13] = 0.000000f;
	fKnob[14] = 0.500000f;
	fPickPosition = 0.156778f;
	fPickUp = 0.40f;
	fPickStiffness = 0.5f;
	fPickWidth = 0.5f;
	fMono = 0.000000f;
	fSustain = 1.000000f;
        fPickNoise = 1.000000f;
	fChordOnOff = 0.000000f;
	fChordIdx = 0.000000f;
	fChordAbsIdx = 0.000000f;
	fChordSwitch = 0.500000f;
	fBodySwitch = 0.400000f;
	fPalmSlider	= 0.3f;
	fAbsRel	= 1.0f;
	fStopSwitch = 0.0f;

	setChord(fChordNote);
	setAbsChord(fChordAbsNote);

}

/*****************************************************************************/
/* setChord :                                                                */
/*****************************************************************************/

void RevitarProgram::setChord(float chordNote[NUM_CHORDS][NUM_CHORD_NOTES])
{
	int i = 0;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 1.f;
	chordNote[i][4] = 0.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 0.f;
	chordNote[i][4] = 0.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 0.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = -1.f;
	chordNote[i][3] = 1.f;
	chordNote[i][4] = 3.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 1.f;
	chordNote[i][3] = 1.f;
	chordNote[i][4] = 0.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = -1.f;
	chordNote[i][3] = 0.f;
	chordNote[i][4] = 3.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 1.f;
	chordNote[i][4] = 2.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 0.f;
	chordNote[i][4] = 2.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 0.f;
	chordNote[i][3] = 1.f;
	chordNote[i][4] = 3.f;
	chordNote[i][5] = 2.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 1.f;
	chordNote[i][3] = 1.f;
	chordNote[i][4] = 0.f;
	chordNote[i][5] = 2.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 0.f;
	chordNote[i][4] = 3.f;
	chordNote[i][5] = 2.f;
	i++;

	chordNote[i][0] = -1.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 2.f;
	chordNote[i][4] = 3.f;
	chordNote[i][5] = 2.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = -1.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 1.f;
	chordNote[i][4] = 2.f;
	chordNote[i][5] = 2.f;
	i++;

	chordNote[i][0] = -1.f;
	chordNote[i][1] = 0.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 1.f;
	chordNote[i][4] = 3.f;
	chordNote[i][5] = 3.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = -1.f;
	chordNote[i][2] = 0.f;
	chordNote[i][3] = 1.f;
	chordNote[i][4] = 0.f;
	chordNote[i][5] = 1.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 3.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 1.f;
	chordNote[i][4] = 1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 3.f;
	chordNote[i][1] = 4.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 3.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 0.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 2.f;
	chordNote[i][4] = 2.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = 1.f;
	chordNote[i][1] = 0.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 2.f;
	chordNote[i][4] = 2.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = 3.f;
	chordNote[i][1] = 0.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 2.f;
	chordNote[i][4] = 2.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = -1.f;
	chordNote[i][1] = 3.f;
	chordNote[i][2] = 5.f;
	chordNote[i][3] = 5.f;
	chordNote[i][4] = 5.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = -1.f;
	chordNote[i][1] = 5.f;
	chordNote[i][2] = 5.f;
	chordNote[i][3] = 5.f;
	chordNote[i][4] = 5.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 3.f;
	chordNote[i][1] = -1.f;
	chordNote[i][2] = 5.f;
	chordNote[i][3] = 5.f;
	chordNote[i][4] = 5.f;
	chordNote[i][5] = -1.f;
	i++;

	int j;                              /* convert to 1/24th  (0=0, 1=24)    */
	for(i=0;i<NUM_CHORDS;i++)
	{
		for(j=0;j<NUM_CHORD_NOTES;j++)
		{
			chordNote[i][j] /= 24.f;
		}
	}
}

/*****************************************************************************/
/* setAbsChord :                                                             */
/*****************************************************************************/

void RevitarProgram::setAbsChord(float chordNote[NUM_CHORDS][NUM_CHORD_NOTES])
{
	int i = 0;

	chordNote[i][0] = -1.f;
	chordNote[i][1] = 3.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 0.f;
	chordNote[i][4] = 1.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = -1.f;
	chordNote[i][1] = -1.f;
	chordNote[i][2] = 0.f;
	chordNote[i][3] = 2.f;
	chordNote[i][4] = 3.f;
	chordNote[i][5] = 1.f;
	i++;

	chordNote[i][0] = -1.f;
	chordNote[i][1] = -1.f;
	chordNote[i][2] = 0.f;
	chordNote[i][3] = 2.f;
	chordNote[i][4] = 3.f;
	chordNote[i][5] = 2.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 0.f;
	chordNote[i][4] = 0.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 1.f;
	chordNote[i][4] = 0.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = 1.f;
	chordNote[i][1] = 3.f;
	chordNote[i][2] = 3.f;
	chordNote[i][3] = 2.f;
	chordNote[i][4] = 1.f;
	chordNote[i][5] = 1.f;
	i++;

	chordNote[i][0] = 3.f;
	chordNote[i][1] = 5.f;
	chordNote[i][2] = 5.f;
	chordNote[i][3] = 3.f;
	chordNote[i][4] = 3.f;
	chordNote[i][5] = 3.f;
	i++;

	chordNote[i][0] = 3.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 0.f;
	chordNote[i][3] = 0.f;
	chordNote[i][4] = 0.f;
	chordNote[i][5] = 3.f;
	i++;

	chordNote[i][0] = -1.f;
	chordNote[i][1] = 0.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 2.f;
	chordNote[i][4] = 1.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = -1.f;
	chordNote[i][1] = 0.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = 2.f;
	chordNote[i][4] = 2.f;
	chordNote[i][5] = 0.f;
	i++;

	chordNote[i][0] = -1.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 4.f;
	chordNote[i][3] = 4.f;
	chordNote[i][4] = 3.f;
	chordNote[i][5] = 2.f;
	i++;

	chordNote[i][0] = -1.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 4.f;
	chordNote[i][3] = 4.f;
	chordNote[i][4] = 4.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 8.f;
	chordNote[i][1] = 10.f;
	chordNote[i][2] = 10.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 9.f;
	chordNote[i][1] = 11.f;
	chordNote[i][2] = 11.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 10.f;
	chordNote[i][1] = 12.f;
	chordNote[i][2] = 12.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 11.f;
	chordNote[i][1] = 13.f;
	chordNote[i][2] = 13.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 0.f;
	chordNote[i][1] = 2.f;
	chordNote[i][2] = 2.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 1.f;
	chordNote[i][1] = 3.f;
	chordNote[i][2] = 3.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 2.f;
	chordNote[i][1] = 4.f;
	chordNote[i][2] = 4.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 3.f;
	chordNote[i][1] = 5.f;
	chordNote[i][2] = 5.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 4.f;
	chordNote[i][1] = 6.f;
	chordNote[i][2] = 6.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 5.f;
	chordNote[i][1] = 7.f;
	chordNote[i][2] = 7.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 6.f;
	chordNote[i][1] = 8.f;
	chordNote[i][2] = 8.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	chordNote[i][0] = 7.f;
	chordNote[i][1] = 9.f;
	chordNote[i][2] = 9.f;
	chordNote[i][3] = -1.f;
	chordNote[i][4] = -1.f;
	chordNote[i][5] = -1.f;
	i++;

	int j;

	for(i=0;i<NUM_CHORDS;i++)
	{
		for(j=0;j<NUM_CHORD_NOTES;j++)
		{
			chordNote[i][j] /= 24.f;
		}
	}


}