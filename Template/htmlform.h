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
    //控件名字列表
    TStringList * slInfoType;

    //控件输入值列表
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

   //Template模块专用
   //bool OpenTemplate(AnsiString Template);
   bool ExtOpenTemplate(AnsiString Template);
   void ExtAnalyseName(AnsiString& strCaption);   
   void ClearTemplate();

   //一次性显示所有控件
   void ShowControls();

   bool SaveTemplate(AnsiString TemplatePath);

   void CloseTemplate();

   //检索所有控件的值
   void GetControlText(TInputInfo * NewValues);
   //提取控件名
   void GetControlName(TInputInfo * NewValues);
   //设置所有控件的初始值
   void SetControlText(TInputInfo * NewValues);
   //设置单一控件的初始值
   bool SetSingleControlText(int nIndex,char * pNewValue);
   //清除链表值
   void ClearValues(TInputInfo * NewValues);
   //清除所有控件的值
   void ClearControlText(TInputInfo* pInfo);
   void ClearControlText();
   AnsiString GetTimeCodeName();
   AnsiString GetImageName();
   bool SetImage(TStream* pStream,AnsiString strFieldName);
   bool SetTimeCode(AnsiString strTimeCode,AnsiString strFieldName);
   bool GetImage(TStream* pStream,AnsiString strFieldName);


//   ParentResize(void);

   TInputInfo * MyVars;
   ControlInfo * m_pControlInfoHeader;
   bool m_bInputTem;
    __property TInputInfo * Vars = {read=MyVars};

protected:
    //父窗口指针
    TScrollingWinControl * m_pParentWindow;

private :

    //----------私有成员变量----------

    //控件信息链表头指针

    //控件信息链表尾指针
    ControlInfo * m_pCurrentPointer;

    //当前字体类型
    TFontStyles m_FontStyles;

    //当前字体
    TFont * m_pGlobalFont;

    //HTML模板
    TStringList * m_slTemplate;

    //显示控件的初始位置
    int m_nInitX,m_nInitY;

    //当前控件所在的行和列
    int m_nRow,m_nColumn;

    //当前控件的左上角
    int m_nCurrentWidth,m_nCurrentHeight;

    //当前行上所有控件的最大高度
    int m_nLineMaxHeight;

    //所有行中的最大宽度
    int m_nLineMaxWidth;

    //当前行的对齐方式
    int m_nAlignMode;

    //标识是否另起一行
    bool m_bNewLinePerControl;

    bool m_bTimeLength;

    bool m_bTimeCode;

    bool m_bEdit;

    //标识是否DateTimePicker
    bool m_bDatetime;


    //当前控件类型
    int m_nType;

    //模板数据表
    TTable * tbTemplate;

    //----------私有成员函数----------

    //返回当前字体的平均宽度和高度
    SIZE getTextExtent();

    //对HTML文件逐句进行句法分析并创建相应控件
    void Analyse(AnsiString&);

    //对HTML文件逐句进行句法分析，但只提取控件名
    void AnalyseName(AnsiString&);

    //对字符串首部进行模式匹配
    bool MatchString(AnsiString&,AnsiString);

    //过滤字符串（删除无用的数据）
    void FilterString(AnsiString&);

    //提取文本信息
    AnsiString PickupCaption(AnsiString& strCaption);

    //提取控件名字
    AnsiString PickupName(AnsiString strCaption);

    //对SQL语句进行过滤
    AnsiString SQLStr(AnsiString strText);

    //颜色匹配
    TColor MapColor(AnsiString strColor);

    //初始化当前字体
    void InitGlobalFont(void);
};
#endif
