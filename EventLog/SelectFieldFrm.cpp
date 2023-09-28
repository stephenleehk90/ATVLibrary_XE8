//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CommFunc.h"
#include "SelectFieldFrm.h"
#include "LogFrm.h"
#include "LogData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSelectFields *frmSelectFields;
//---------------------------------------------------------------------------
__fastcall TfrmSelectFields::TfrmSelectFields(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSelectFields::FormShow(TObject *Sender)
{
    AnsiString strSQLClause,strFieldName,strSourceNodeID;
    AnsiString strFieldType,strAssociateType;
    TLocateOptions LocateOption;
    int i;

    strFieldName    = frmLogs->lstvwFields->Selected->Caption;
    strSourceNodeID = frmLogs->edtSourceTb->Text.Trim();

    lstvwFields->Items->Clear();
        
    try
    {
        //确定Locate操作的参数
        LocateOption << loCaseInsensitive;

        //确定目标字段的类型
        if (dmdLog->qeBaseFields->Locate("FieldName",SQLStr(strFieldName),LocateOption))
        {
			strFieldType = OrigStr(dmdLog->qeBaseFields->FieldValues["FieldType"]);
		}
		else
		{
			if (dmdLog->qeExtFields->Locate("FieldName",SQLStr(strFieldName),LocateOption))
			{
				strFieldType = OrigStr(dmdLog->qeExtFields->FieldValues["FieldType"]);
            }
        }

		if (! frmLogs->chkOld->Checked)
        //使用新库
        {
            dmdLog->qeBaseFields->First();
            while (! dmdLog->qeBaseFields->Eof)
            {
				strAssociateType = OrigStr(dmdLog->qeBaseFields->FieldValues["FieldType"]);
				if (strAssociateType == strFieldType)
                {
                    lstvwFields->Items->Add();
					lstvwFields->Items->Item[lstvwFields->Items->Count-1]->Caption =
						OrigStr(dmdLog->qeBaseFields->FieldValues["FieldName"]);
				}

				dmdLog->qeBaseFields->Next();
            }

            //检索扩展字段
            strSQLClause = (AnsiString)"select * from "+
                           (AnsiString)strLIBEXTENDFIELDS+
                           (AnsiString)" where CategoryID = '"+
                           SQLStr(strSourceNodeID)+
                           (AnsiString)"'";

            dmdLog->qeGlobal->SQL->Clear();
            dmdLog->qeGlobal->SQL->Add(strSQLClause);
            dmdLog->qeGlobal->Open();

            while (! dmdLog->qeGlobal->Eof)
			{
				strAssociateType = OrigStr(dmdLog->qeGlobal->FieldValues["FieldType"]);
                if (strAssociateType == strFieldType)
                {
                    lstvwFields->Items->Add();
                    lstvwFields->Items->Item[lstvwFields->Items->Count-1]->Caption =
						OrigStr(dmdLog->qeGlobal->FieldValues["FieldName"]);
				}

                dmdLog->qeGlobal->Next();
            }
        }
        else
        //使用老库
        {
            if (1)//(strFieldType == typeVARCHAR) || (strFieldType == typeTEXT))
            {
				if (dmdLog->tbOldData->Active)
                {
                    for (i=0;i<dmdLog->tbOldData->FieldCount;i++)
                    {
                        lstvwFields->Items->Add();
                        lstvwFields->Items->Item[lstvwFields->Items->Count-1]->Caption =
							dmdLog->tbOldData->Fields->Fields[i]->FieldName;
                    }
                }
			}
        }
    }
	catch(...)
    {
        Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectFields::btnOkClick(TObject *Sender)
{
    if ((lstvwFields->Items->Count > 0)&&(! lstvwFields->Selected))
    {
		Application->MessageBox(L"Please specify a associated field !",GLOBALMSGCAPTION,48);
        ModalResult = mrNone;
        return;
	}
}
//---------------------------------------------------------------------------

