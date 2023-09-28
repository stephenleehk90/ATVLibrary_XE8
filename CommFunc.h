//---------------------------------------------------------------------------
#ifndef CommFuncH
#define CommFuncH

//---------------------------------------------------------------------------
#define MAXROWS (3000)

//---------------------------------------------------------------------------
void DecodeString(AnsiString str);
void EncodeString(AnsiString str);

AnsiString SQLStr(AnsiString strText);
AnsiString OrigStr(AnsiString strText);

void DisableMenu(TMainMenu *mnu1);
void EnableMenu(TMainMenu *mnu1);

AnsiString IniName(void);
bool SetIniInt (AnsiString Section, AnsiString Key, int value);
int GetIniInt (AnsiString Section, AnsiString Key, int value);
void SetIniFormPos(AnsiString Section, TForm* form);
void GetIniFormPos(AnsiString Section, TForm* form);
//---------------------------------------------------------------------------
#endif
