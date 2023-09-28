//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ImportFrm.h"
#include "TemplateForm.h"
#include "utGlobDm.h"
#include "CommFunc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmInout *frmInout;
//---------------------------------------------------------------------------
__fastcall TfrmInout::TfrmInout(TComponent* Owner)
	: TForm(Owner)
{
	m_strNewTemplate = "";
}
//---------------------------------------------------------------------------
void __fastcall TfrmInout::btnImportClick(TObject *Sender)
{
	AnsiString strNodeID;

	if (frmTemplate->OpenDialog1->Execute())
	{
	  m_strNewTemplate = frmTemplate->OpenDialog1->FileName;

	  //更新界面
	  m_pHtmlFormTemp->CloseTemplate();

	  if (frmTemplate->m_nTemplateType==1)
		 m_pHtmlFormTemp->m_bInputTem = true;
	  else
		 m_pHtmlFormTemp->m_bInputTem = false;
	  m_pHtmlFormTemp->OpenTemplate((TScrollingWinControl *)sbPreview,m_strNewTemplate);

	  strNodeID = (AnsiString)((char *)frmTemplate->trvwTemplate->Selected->Data);
	  if (CheckHtmlValid(m_pHtmlFormTemp,strNodeID))
		  m_bModified = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmInout::btnExportClick(TObject *Sender)
{
	AnsiString strNewTemplate;

	if (frmTemplate->SaveDialog1->Execute())
	{
		strNewTemplate = frmTemplate->SaveDialog1->FileName;
		m_pHtmlFormTemp->SaveTemplate(strNewTemplate);
		Application->MessageBox(L"Export Successfully!",GLOBALMSGCAPTION,48);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmInout::FormShow(TObject *Sender)
{
	AnsiString strNodeID,strTemplateField;

	strNodeID = (AnsiString)((char *)frmTemplate->trvwTemplate->Selected->Data);

    switch (frmTemplate->m_nTemplateType)
    {
        case INPUTTEMPLATE:
                            strTemplateField = "InputTem";
                            break;
		case BROWSETEMPLATE:
							strTemplateField = "ViewTem";
							break;
		case SEARCHTEMPLATE:
							strTemplateField = "SearchTem";
							break;
		case ADVANCEDTEMPLATE:
							strTemplateField = "AdvanceTem";
							break;
		case LISTTEMPLATE:
							strTemplateField = "ListTem";
							break;
		default :
							strTemplateField = "InputTem";
	}

	m_pHtmlFormTemp = new ThtmlformTemp;
	m_pHtmlFormTemp->OnSubmit = NULL;
	m_pHtmlFormTemp->OnReset = NULL;
    if (strTemplateField=="InputTem")
        m_pHtmlFormTemp->m_bInputTem = true;
    else
        m_pHtmlFormTemp->m_bInputTem = false;
    m_pHtmlFormTemp->OpenTemplate((TScrollingWinControl *)sbPreview,
                              "Library",strSTORYLIBTREE,
                              "NodeID",strTemplateField,
                              SQLStr(strNodeID));

    m_bModified = false;    
}
//---------------------------------------------------------------------------

void __fastcall TfrmInout::FormClose(TObject *Sender, TCloseAction &Action)
{
	m_pHtmlFormTemp->CloseTemplate();
	delete m_pHtmlFormTemp;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInout::btnOkClick(TObject *Sender)
{
	AnsiString strNodeID,strTemplateField;
	TStringList * slTemplate = NULL;
	TBlobStream * bsTemplateStream = NULL;
	TLocateOptions LocateOption;

	strNodeID = (AnsiString)((char *)frmTemplate->trvwTemplate->Selected->Data);

	slTemplate = new TStringList;

	if (! m_bModified)
	{
		Application->MessageBox(L"No Template Available!",GLOBALMSGCAPTION,48);
		return;
	}

	try
	{
	  if (dmTemplate->tbCategory->Active)
		  dmTemplate->tbCategory->Close();

	  dmTemplate->tbCategory->Open();

	  //确定Locate操作的参数
	  LocateOption << loCaseInsensitive;

	  if (! dmTemplate->tbCategory->Locate("NodeID",SQLStr(strNodeID),LocateOption))
	  {
		  Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
		  return;
	  }

	  //打开界面模板
	  slTemplate->LoadFromFile(m_strNewTemplate);

	  if (slTemplate->Count > 0)
	  //执行Text字段的“写”操作
	  {
		switch (frmTemplate->m_nTemplateType)
		{
			case INPUTTEMPLATE:
								strTemplateField = "InputTem";
								break;
			case BROWSETEMPLATE:
								strTemplateField = "ViewTem";
								break;
			case SEARCHTEMPLATE:
								strTemplateField = "SearchTem";
								break;
			case ADVANCEDTEMPLATE:
								strTemplateField = "AdvanceTem";
								break;
			case LISTTEMPLATE:
								strTemplateField = "ListTem";
								break;
			default :
								strTemplateField = "InputTem";
		}

		dmTemplate->tbCategory->Edit();
		bsTemplateStream = new TBlobStream((TBlobField*)dmTemplate->tbCategory->FieldByName(strTemplateField),bmReadWrite);
		bsTemplateStream->Seek(0,soFromBeginning);
		bsTemplateStream->Truncate();
		bsTemplateStream->Write(AnsiString(slTemplate->Text).c_str(),slTemplate->Text.Length());
	  //	bsTemplateStream->WriteBuffer(L"RESR");
		dmTemplate->tbCategory->Post();
		delete bsTemplateStream;
		bsTemplateStream = NULL;
	  }

	  delete slTemplate;
	  slTemplate = NULL;
	}
	catch(...)
	{
      Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);

      if (bsTemplateStream)
          delete bsTemplateStream;

      if (slTemplate)
          delete slTemplate;

      return;
    }
}
//---------------------------------------------------------------------------
bool TfrmInout::CheckHtmlValid(ThtmlformTemp * pHtmlFormTemp,AnsiString strNodeID)
{
    TInputInfo * pInputInfo;
	TLocateOptions LocateOption;
	AnsiString strFieldName,strInvalidName,strWarning,strSQLClause;
	AnsiString strSourceFieldName;
	AnsiString strFlag;
	bool bIsValid;
	int i;
	int nPos,nIndex;

	pInputInfo = new TInputInfo;

	pHtmlFormTemp->GetControlName(pInputInfo);

	strSQLClause = (AnsiString)"select FieldName from "+
				   strLIBBASEFIELDS+
				   " union select FieldName from "+
				   strLIBEXTENDFIELDS+
				   " where CategoryID = '"+
				   SQLStr(strNodeID)+
				   "'";

	try
    {
        dmTemplate->qeGlobal->SQL->Clear();
        dmTemplate->qeGlobal->SQL->Add(strSQLClause);
        dmTemplate->qeGlobal->Open();

        //确定Locate操作的参数
        LocateOption << loCaseInsensitive;

        bIsValid = true;
        strInvalidName = "";
        for (i=0;i<pInputInfo->slInfoType->Count;i++)
        {
            strFieldName = pInputInfo->slInfoType->Strings[i];

            if (strFieldName.IsEmpty())
                continue;

            nIndex = pInputInfo->slInfoType->IndexOf(strFieldName);
            if (nIndex != i)
            {
				Application->MessageBox(L"The template is wrong.",GLOBALMSGCAPTION,48);
                return false;
            }

            nPos = strFieldName.AnsiPos("_");
            strFlag = (AnsiString)"";
            if (nPos > 0)
            {
                strSourceFieldName = strFieldName.SubString(0,nPos-1);
                strFlag = strFieldName.SubString(nPos+1,strFieldName.Length()-nPos+1);
            }
            else
                strSourceFieldName = strFieldName.Trim();
            nIndex = pInputInfo->slInfoType->IndexOf(strSourceFieldName + "_1");

            if ((strFlag != "1") && (strFlag != "2"))
            {
              if (nIndex >= 0)
              {
				  Application->MessageBox(L"The template is wrong.",GLOBALMSGCAPTION,48);
                  return false;
			  }

              nIndex = pInputInfo->slInfoType->IndexOf(strSourceFieldName + "_2");
              if (nIndex >= 0)
              {
				  Application->MessageBox(L"The template is wrong.",GLOBALMSGCAPTION,48);
				  return false;
			  }
			}

			if (! dmTemplate->qeGlobal->Locate("FieldName",SQLStr(strSourceFieldName),LocateOption))
			{
				bIsValid = false;

				strInvalidName = strInvalidName+strSourceFieldName+"\n";
			}
			else
			{
				if (frmTemplate->m_nTemplateType == INPUTTEMPLATE)
				{
				  if (strFieldName == "StoryID")
				  {
					  Application->MessageBox(L"The input template can not include field 'StoryID'.",GLOBALMSGCAPTION,48);
					  delete pInputInfo;
					  return false;
				  }
				  if (strFieldName == "WriteTime")
				  {
					  Application->MessageBox(L"The input template can not include field 'WriteTime'.",GLOBALMSGCAPTION,48);
					  delete pInputInfo;
					  return false;
				  }
				}
			}
		}

		if (! bIsValid)
		{
			//删除结尾的回车符
            strInvalidName.Delete(strInvalidName.Length(),1);

            strWarning = (AnsiString)"Below fields are not consistent with database fields :\n\n"+
                         strInvalidName;

			Application->MessageBox(UnicodeString(strWarning).c_str(),GLOBALMSGCAPTION,48);
            delete pInputInfo;
            return false;
        }
        delete pInputInfo;
		return true;
    }
    catch(...)
    {
        Application->MessageBox(DATABASEACCESSERROR,GLOBALMSGCAPTION,48);
        delete pInputInfo;
        return false;
    }
}
//---------------------------------------------------------------------------

