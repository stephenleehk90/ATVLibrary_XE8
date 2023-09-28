object frmQueryLog: TfrmQueryLog
  Left = 0
  Top = 0
  Caption = 'Log Query Setting'
  ClientHeight = 166
  ClientWidth = 367
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lblIndicator: TLabel
    Left = 12
    Top = 4
    Width = 209
    Height = 16
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'Please input the query qualification:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object grbDate: TGroupBox
    Left = 12
    Top = 30
    Width = 235
    Height = 92
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = '&Date Range'
    TabOrder = 0
    object lblFrom: TLabel
      Left = 20
      Top = 26
      Width = 28
      Height = 13
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = '&From:'
      FocusControl = dtpBegin
    end
    object lblTo: TLabel
      Left = 18
      Top = 59
      Width = 16
      Height = 13
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      Caption = '&To:'
      FocusControl = dtpEnd
    end
    object dtpBegin: TDateTimePicker
      Left = 59
      Top = 23
      Width = 151
      Height = 21
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      CalColors.BackColor = clGray
      Date = 35906.442377083330000000
      Time = 35906.442377083330000000
      ParseInput = True
      TabOrder = 0
    end
    object dtpEnd: TDateTimePicker
      Left = 59
      Top = 55
      Width = 151
      Height = 21
      Margins.Left = 2
      Margins.Top = 2
      Margins.Right = 2
      Margins.Bottom = 2
      CalColors.BackColor = clGray
      Date = 35906.442417129630000000
      Time = 35906.442417129630000000
      ParseInput = True
      TabOrder = 1
    end
  end
  object chkOpID: TCheckBox
    Left = 7
    Top = 126
    Width = 79
    Height = 14
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = '&Operator ID:'
    TabOrder = 1
    OnClick = chkOpIDClick
  end
  object edtOpID: TEdit
    Left = 107
    Top = 131
    Width = 140
    Height = 21
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    MaxLength = 30
    TabOrder = 2
  end
  object btnCancel: TButton
    Left = 271
    Top = 98
    Width = 61
    Height = 24
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object btnOk: TButton
    Left = 271
    Top = 30
    Width = 61
    Height = 24
    Margins.Left = 2
    Margins.Top = 2
    Margins.Right = 2
    Margins.Bottom = 2
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 4
    OnClick = btnOkClick
  end
end
