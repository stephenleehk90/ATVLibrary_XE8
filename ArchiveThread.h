//---------------------------------------------------------------------------
#ifndef ArchiveThreadH
#define ArchiveThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TThdArchive : public TThread
{
private:
protected:
    void __fastcall Execute();
public:
    TSession *snArchive;
    TQuery *qryArchive;
    TDatabase *dbArchive;
    bool fArchiveSucceed;
    
    __fastcall TThdArchive(bool CreateSuspended);
    bool InitDBConnect();
    void DBDisconnect();
    void __fastcall ArchiveFinish();
};
//---------------------------------------------------------------------------
extern TThdArchive *thdArchive;
//---------------------------------------------------------------------------
#endif
