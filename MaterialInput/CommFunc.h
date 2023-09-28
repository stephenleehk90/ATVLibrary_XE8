//---------------------------------------------------------------------------
#ifndef CommFuncH
#define CommFuncH

//---------------------------------------------------------------------------
#define MAXROWS (300)

//---------------------------------------------------------------------------
void DecodeString(AnsiString str);
void EncodeString(AnsiString str);

AnsiString SQLStr(AnsiString strText);
AnsiString OrigStr(AnsiString strText);

void DisableMenu(TMainMenu *mnu1);
void EnableMenu(TMainMenu *mnu1);

//---------------------------------------------------------------------------
#endif
