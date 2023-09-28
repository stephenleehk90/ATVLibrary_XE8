//---------------------------------------------------------------------------
#ifndef htmlformH
#define htmlformH

#include <dbtables.hpp>
//---------------------------------------------------------------------------
enum HtmlReadState { stHTMLHEAD = 0,stHTMLBEGIN,stHTMLEND,stFORMBEGIN,stFORMEND,stHTMLTAIL };
enum ALIGNMODE { alLEFT = 0,alCENTER,alRIGHT };
enum CONTROLTYPE { ctrlLABEL = 0,ctrlRADIOBUTTON,ctrlCHECKBOX,ctrlCOMBOBOXITEM,ctrlMEMO };
#define IMAGEWIDTH (150)
#define IMAGEHEIGHT (IMAGEWIDTH*3/4)
struct ControlInfo
{
	TControl * pControl;
	AnsiString strClassName;
    AnsiString strControlName;
    POINT ptLeftTop;
	struct ControlInfo * pNextControl;
};

class TInputInfo
{
public :
    //�ؼ������б�
    TStringList * slInfoType;

    //�ؼ�����ֵ�б�
    TList       * lInfo;

    TInputInfo();
   ~TInputInfo();
};

//typedef void __fastcall (__closure * FOnSubmit)(System::TObject* Sender);
//typedef void __fastcall (__closure * FOnReset)(System::TObject* Sender);

class THtmlForm
{
public :
    THtmlForm();
   ~THtmlForm(void);

    void __fastcall (__closure * FOnSubmit)(System::TObject* Sender);
    void __fastcall (__closure * FOnReset)(System::TObject* Sender);

    __property Classes::TNotifyEvent OnSubmit = {read=FOnSubmit, write=FOnSubmit};
    __property Classes::TNotifyEvent OnReset  = {read=FOnReset,  write=FOnReset};

//   FOnSummit OnSubmit;
//   FOnReset  OnReset;

   bool OpenTemplate(TScrollingWinControl* AParent,AnsiString Template);

   bool OpenTemplate(TScrollingWinControl * AParent,
                     AnsiString strDBName,AnsiString strTableName,
                     AnsiString strNodeField,AnsiString strTemplateField,
                     AnsiString strNodeID);

   //Templateģ��ר��
   //bool OpenTemplate(AnsiString Template);
   bool ExtOpenTemplate(AnsiString Template);
   void ExtAnalyseName(AnsiString& strCaption);   
   void ClearTemplate();

   //һ������ʾ���пؼ�
   void ShowControls();

   bool SaveTemplate(AnsiString TemplatePath);

   void CloseTemplate();

   //�������пؼ���ֵ
   void GetControlText(TInputInfo * NewValues);
   //��ȡ�ؼ���
   void GetControlName(TInputInfo * NewValues);
   //�������пؼ��ĳ�ʼֵ
   void SetControlText(TInputInfo * NewValues);
   //���õ�һ�ؼ��ĳ�ʼֵ
   bool SetSingleControlText(AnsiString FieldName,char * pNewValue);
   //�������ֵ
   void ClearValues(TInputInfo * NewValues);
   //������пؼ���ֵ
   void ClearControlText(TInputInfo* pInfo);
   void ClearControlText();
   AnsiString GetTimeCodeName();
   AnsiString GetImageName();
   bool SetImage(TStream* pStream,AnsiString strFieldName);
   bool SetTimeCode(AnsiString strTimeCode,AnsiString strFieldName);
   bool GetImage(TStream* pStream,AnsiString strFieldName);
   bool CheckValues();


   //ParentResize(void);

   TInputInfo * MyVars;
   AnsiString strKeywords;
   AnsiString strKeyField;
   ControlInfo * m_pControlInfoHeader;
   bool m_bInputTem;
    __property TInputInfo * Vars = {read=MyVars};

protected:
    //������ָ��
    TScrollingWinControl * m_pParentWindow;

private :

    //----------˽�г�Ա����----------

    //�ؼ���Ϣ����ͷָ��

    //�ؼ���Ϣ����βָ��
    ControlInfo * m_pCurrentPointer;

    //��ǰ��������
    TFontStyles m_FontStyles;

    //��ǰ����
    TFont * m_pGlobalFont;

    //HTMLģ��
    TStringList * m_slTemplate;

    //��ʾ�ؼ��ĳ�ʼλ��
    int m_nInitX,m_nInitY;

    //��ǰ�ؼ����ڵ��к���
    int m_nRow,m_nColumn;

    //��ǰ�ؼ������Ͻ�
    int m_nCurrentWidth,m_nCurrentHeight;

    //��ǰ�������пؼ������߶�
    int m_nLineMaxHeight;

    //�������е������
    int m_nLineMaxWidth;

    //��ǰ�еĶ��뷽ʽ
    int m_nAlignMode;

    //��ʶ�Ƿ�����һ��
    bool m_bNewLinePerControl;

    bool m_bTimeLength;

    bool m_bTimeCode;

    bool m_bEdit;

    //��ʶ�Ƿ�DateTimePicker
    bool m_bDatetime;


    //��ǰ�ؼ�����
    int m_nType;

    //ģ�����ݱ�
    TTable * tbTemplate;

    //----------˽�г�Ա����----------

    //���ص�ǰ�����ƽ����Ⱥ͸߶�
    SIZE getTextExtent();

    //��HTML�ļ������о䷨������������Ӧ�ؼ�
    void Analyse(AnsiString&);

    //��HTML�ļ������о䷨��������ֻ��ȡ�ؼ���
    void AnalyseName(AnsiString&);

    //���ַ����ײ�����ģʽƥ��
    bool MatchString(AnsiString&,AnsiString);

    //�����ַ�����ɾ�����õ����ݣ�
    void FilterString(AnsiString&);

    //��ȡ�ı���Ϣ
    AnsiString PickupCaption(AnsiString& strCaption);

    //��ȡ�ؼ�����
    AnsiString PickupName(AnsiString strCaption);

    //��SQL�����й���
    AnsiString SQLStr(AnsiString strText);

    //��ɫƥ��
    TColor MapColor(AnsiString strColor);

    //��ʼ����ǰ����
    void InitGlobalFont(void);
};
#endif
