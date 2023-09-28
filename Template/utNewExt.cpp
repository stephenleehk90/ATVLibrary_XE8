//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CommFunc.h"
#include "utNewExt.h"
#include "TemplateForm.h"
#include "utGlobDm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmEditExtFields *fmEditExtFields;

extern int g_naFieldTypeSize[MAXFIELDTYPES];
//---------------------------------------------------------------------------
__fastcall TfmEditExtFields::TfmEditExtFields(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmEditExtFields::btnFunctionClick(TObject *Sender)
{
	AnsiString strFieldName,strFieldType,strFieldSize,strCategory;
    AnsiString strSQLClause,strNodeID;

    strFieldName = edFieldName->Text.Trim();
    if (strFieldName.IsEmpty())
	{
		Application->MessageBox(L"You should offer field name!",GLOBALMSGCAPTION,48);
        edFieldName->SetFocus();
        return;
    }

    strFieldType = cbbFieldType->Text.Trim();
    if (strFieldType.IsEmpty())
	{
		Application->MessageBox(L"You should choose a field type!",GLOBALMSGCAPTION,48);
		cbbFieldType->SetFocus();
		return;
	}

	strFieldSize = meFieldSize->Text.Trim();
	if ((cbbFieldType->ItemIndex == 0) && (strFieldSize.IsEmpty()))
	{
		Application->MessageBox(L"You should set the field size!",GLOBALMSGCAPTION,48);
        meFieldSize->SetFocus();
        return;
    }

    try
    {
        strNodeID = (AnsiString)(char *)frmTemplate->trvwTemplate->Selected->Data;

        if (m_nEditState == 1)
        //创建新字段
        {
               strSQLClause = "select FieldName from "+
                              (AnsiString)strLIBEXTENDFIELDS+
                              " where CategoryID = '"+
                              SQLStr(strNodeID)+
                              "' and FieldName = '"+
                              SQLStr(strFieldName)+
                              "'";

               dmTemplate->qeGlobal->SQL->Clear();
               dmTemplate->qeGlobal->SQL->Add(strSQLClause);
               dmTemplate->qeGlobal->Open();

               if (dmTemplate->qeGlobal->RecordCount > 0)
               {
					Application->MessageBox(L"Duplicate field name !",GLOBALMSGCAPTION,48);
                    edFieldName->SetFocus();
					return;
               }

               strSQLClause = "insert into "+
                              (AnsiString)strLIBEXTENDFIELDS+
                              " values('"+
                              SQLStr(strNodeID)+
                              "','"+
                              SQLStr(strFieldName)+
                              "','"+
                              SQLStr(strFieldType)+
                              "',"+
                              strFieldSize+
                              ")";

               dmTemplate->qeGlobal->SQL->Clear();
         	   dmTemplate->qeGlobal->SQL->Add(strSQLClause);
               dmTemplate->qeGlobal->ExecSQL();

               frmTemplate->lstvwFields->Items->Add();
               frmTemplate->lstvwFields->Items->Item[frmTemplate->lstvwFields->Items->Count-1]->Caption = strFieldName;

               frmTemplate->lstvwFields->Items->Item[frmTemplate->lstvwFields->Items->Count-1]->SubItems->Add(strFieldType);
               frmTemplate->lstvwFields->Items->Item[frmTemplate->lstvwFields->Items->Count-1]->SubItems->Add(strFieldSize);

//		tvCategoryChange(NULL,tvCategory->Selected);

    		   edFieldName->Clear();
               meFieldSize->Clear();
               cbbFieldType->ItemIndex = -1;

    	       if (! frmTemplate->mniRemoveField->Enabled)
                    frmTemplate->mniRemoveField->Enabled = true;

               if (! frmTemplate->mniModifyField->Enabled)
                    frmTemplate->mniModifyField->Enabled = true;

               edFieldName->SetFocus();
        }
        else
        {
            strNodeID = (AnsiString)(char *)frmTemplate->trvwTemplate->Selected->Data;

            if (m_nEditState == 2)
            //修改字段属性
            {
                strSQLClause = "Update "+
                               (AnsiString)strLIBEXTENDFIELDS+
                               " Set FieldType = '"+
                               SQLStr(strFieldType)+
                               "',FieldSize = "+
                               strFieldSize+
                               " where CategoryID = '"+
                               SQLStr(strNodeID)+
                               "' and FieldName = '"+
                               SQLStr(strFieldName)+
                               "'";

                dmTemplate->qeGlobal->SQL->Clear();
        		dmTemplate->qeGlobal->SQL->Add(strSQLClause);
	            dmTemplate->qeGlobal->ExecSQL();

                frmTemplate->lstvwFields->Selected->SubItems->Strings[0] =
                            cbbFieldType->Items->Strings[cbbFieldType->ItemIndex];
                frmTemplate->lstvwFields->Selected->SubItems->Strings[1] =
                            meFieldSize->Text;
            }

            ModalResult = mrOk;
        }
    }
    catch(...)
    {
    	Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfmEditExtFields::cbbFieldTypeChange(TObject *Sender)
{
    meFieldSize->Text = String(g_naFieldTypeSize[cbbFieldType->ItemIndex]);

	if (cbbFieldType->ItemIndex == 0)
    	meFieldSize->Enabled = true;
    else
	    meFieldSize->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfmEditExtFields::FormShow(TObject *Sender)
{
    edFieldName->Clear();
    meFieldSize->Clear();
    cbbFieldType->ItemIndex = -1;

    if (m_nEditState == 1)
    //创建新字段
    {
        fmEditExtFields->Caption = "New Extend Field";
        btnFunction->Caption    = "&New";
        edFieldName->Enabled    = true;
    }
    else
    {
        fmEditExtFields->Caption = "Modify Extend Field";
        btnFunction->Caption     = "&Modify";
        edFieldName->Text        = frmTemplate->lstvwFields->Selected->Caption;
        edFieldName->Enabled     = false;
        cbbFieldType->ItemIndex  = cbbFieldType->Items->IndexOf(frmTemplate->lstvwFields->Selected->SubItems->Strings[0]);
        meFieldSize->Text        = frmTemplate->lstvwFields->Selected->SubItems->Strings[1];
    }
}
//---------------------------------------------------------------------------
