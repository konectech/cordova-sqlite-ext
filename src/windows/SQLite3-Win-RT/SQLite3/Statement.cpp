#include "Winerror.h"

#include "Statement.h"
#include "Database.h"
#include <string>
#include "base64.h"

namespace SQLite3
{
  Statement::Statement(Database^ database, Platform::String^ sql)
  {
    int ret = sqlite3_prepare16(database->sqlite, sql->Data(), -1, &statement, 0);

    if (ret != SQLITE_OK)
    {
      sqlite3_finalize(statement);

      HRESULT hresult = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, ret);
      throw ref new Platform::COMException(hresult);
    }
  }

  Statement::~Statement()
  {
    sqlite3_finalize(statement);
  }

  int Statement::Step()
  {
    return sqlite3_step(statement);
  }

  int Statement::ColumnCount()
  {
    return sqlite3_column_count(statement);
  }

  int Statement::ColumnType(int index)
  {
    return sqlite3_column_type(statement, index);
  }

  Platform::String^ Statement::ColumnName(int index)
  {
    return ref new Platform::String(static_cast<const wchar_t*>(sqlite3_column_name16(statement, index)));
  }

  Platform::String^ Statement::ColumnText(int index)
  {
    return ref new Platform::String(static_cast<const wchar_t*>(sqlite3_column_text16(statement, index)));
  }

  int Statement::ColumnInt(int index)
  {
    return sqlite3_column_int(statement, index);
  }

  long long Statement::ColumnInt64(int index)
  {
    return sqlite3_column_int64(statement, index);
  }

  double Statement::ColumnDouble(int index)
  {
    return sqlite3_column_double(statement, index);
  }

  Platform::String^ Statement::ColumnBlob(int index)
  {
	  //return sqlite3_column_blob(statement, index);
	  
	  uint8* blob = (uint8*)sqlite3_column_blob(statement, index);
	  const int blobSize = sqlite3_column_bytes(statement, index);

	  std::string encoded = base64_encode(blob, blobSize);

	  
	  std::wstring str(encoded.begin(), encoded.end());

	  //std::wstring str = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes("some string");

	  return ref new Platform::String(str.c_str());
	  //Platform::ArrayReference<uint8> blobArray(blob, blobSize);
	  //using Windows::Security::Cryptography::CryptographicBuffer;
	  //auto base64Text = CryptographicBuffer::EncodeToBase64String(CryptographicBuffer::CreateFromByteArray(blobArray));
	  //outstream << L'"' << base64Text->Data() << L'"';	  
  }
  

  int Statement::BindText(int index, Platform::String^ val)
  {
    return sqlite3_bind_text16(statement, index, val->Data(), -1, SQLITE_TRANSIENT);
  }

  int Statement::BindInt(int index, int val)
  {
    return sqlite3_bind_int(statement, index, val);
  }

  int Statement::BindInt64(int index, long long val)
  {
    return sqlite3_bind_int64(statement, index, val);
  }

  int Statement::BindDouble(int index, double val)
  {
    return sqlite3_bind_double(statement, index, val);
  }

  int Statement::BindNull(int index)
  {
    return sqlite3_bind_null(statement, index);
  }
}
