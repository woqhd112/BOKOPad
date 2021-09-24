#pragma once

#include "ComplexString.h"
#include "ComplexMap.h"
#include "sqlite3.h"

/*
* Query문 기초작성 방법
* 단일 쿼리문은 쿼리문 뒤에 ;를 붙히지 않는다.
* 다중 쿼리문은 쿼리문 마다 뒤에 ;를 붙혀서 구분한다.
* 쿼리문은 칼럼명 및 값을 제외하고 대문자로 작성한다.
* 문자열은 \" 이나 ' 로 입력한다.

DDL 명령어 (CREATE, ALTER DROP)
CREATE TABLE table_name (column_name column_type(size), ...);
ALTER TABLE table_name ...;
-> (...에는 ADD, ALTER, DROP 입력가능)
-> ADD column_name column_type(size)
-> ALTER column_name column_type(size)
-> DROP column_name
DROP TABLE table_name;

DML 명령어 (SELECT, INSERT, UPDATE, DELETE)
SELECT ...(1) FROM table_name WHERE ...(2);
-> ...(1)에는 검색하고 싶은 칼럼명 입력 (* 는 모든 칼럼 검색)
-> ...(2)에는 특정 튜플만 검색하고 싶은 칼럼명=값 입력
INSERT INTO table_name (column_name, ...) VALUES(column_value);
-> INTO 뒤의 괄호는 생략 가능하며 생략안하면 입력하고 싶은 칼럼을 특정해서 입력한다.
-> 생략안하면 VALUES 뒤의 값 입력 때도 같은 포맷으로 입력 해야한다.
-> 입력 안한 VALUE는 NULL값으로 들어가며 notnull인 VALUE는 생략 불가능하다.
UPDATE table_name SET ...;
-> ...에는 특정 칼럼의 값에대한 조건을 입력한다.
DELETE FROM table_name WHERE ...;
-> ...에는 특정 칼럼의 값에대한 조건을 입력한다.

*/

//#define SQLITE_OK           0   /* Successful result */
/* beginning-of-error-codes */
//#define SQLITE_ERROR        1   /* SQL error or missing database */
//#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
//#define SQLITE_PERM         3   /* Access permission denied */
//#define SQLITE_ABORT        4   /* Callback routine requested an abort */
//#define SQLITE_BUSY         5   /* The database file is locked */
//#define SQLITE_LOCKED       6   /* A table in the database is locked */
//#define SQLITE_NOMEM        7   /* A malloc() failed */
//#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
//#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
//#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
//#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
//#define SQLITE_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
//#define SQLITE_FULL        13   /* Insertion failed because database is full */
//#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
//#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
//#define SQLITE_EMPTY       16   /* Database is empty */
//#define SQLITE_SCHEMA      17   /* The database schema changed */
//#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
//#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
//#define SQLITE_MISMATCH    20   /* Data type mismatch */
//#define SQLITE_MISUSE      21   /* Library used incorrectly */
//#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
//#define SQLITE_AUTH        23   /* Authorization denied */
//#define SQLITE_FORMAT      24   /* Auxiliary database format error */
//#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
//#define SQLITE_NOTADB      26   /* File opened that is not a database file */
//#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
//#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */

namespace ComplexLibrary
{
	class ComplexDatabase
	{
	public:

		ComplexString GetErrorCode(int code)
		{
			ComplexString error = "";
			switch (code)
			{
			case SQLITE_OK:
				error = "success.";
				break;
			case SQLITE_ERROR:
				error = "error!, not found error code.";
				break;
			case SQLITE_INTERNAL:
				error = "find bug in sqlite.";
				break;
			case SQLITE_PERM:
				error = "access deny.";
				break;
			case SQLITE_ABORT:
				error = "sqlite3_exec() callback_func not return 0.";
				break;
			case SQLITE_BUSY:
				error = "db file is locked.";
				break;
			case SQLITE_LOCKED:
				error = "db table is locked.";
				break;
			case SQLITE_NOMEM:
				error = "sqlite full memory";
				break;
			case SQLITE_READONLY:
				error = "have not write grant.";
				break;
			case SQLITE_IOERR:
				error = "os detected I/O error.";
				break;
			case SQLITE_CORRUPT:
				error = "db file is damaged.";
				break;
			case SQLITE_FULL:
				error = "disk is full.";
				break;
			case SQLITE_CANTOPEN:
				error = "can not open db file.";
				break;
			case SQLITE_SCHEMA:
				error = "en route processing 'sqlite3_step()', schema changed.";
				break;
			case SQLITE_TOOBIG:
				error = "size of String and BLOB is exceed.";
				break;
			case SQLITE_CONSTRAINT:
				error = "query is violated a constraint.";
				break;
			case SQLITE_MISMATCH:
				error = "data type is mismatched.";
				break;
			case SQLITE_RANGE:
				error = "row index of parameter of sqlite3_bind() and sqlite3_coloumn() out of range.";
				break;
			case SQLITE_NOTADB:
				error = "file is not db file.";
				break;
			case SQLITE_ROW:
				error = "result of sqlite3_step() indicates that it can be returned.";
				break;
			case SQLITE_DONE:
				error = "sqlite3_setp() is processed.";
				break;
			default:
				break;
			}

			return error;
		}

		ComplexDatabase()
			: m_bDBConnection(false)
			, m_bPrepareStatement(false)
			, m_bResultSet(false)
			, m_bUseAutoCommit(true)
			, m_nCommitStartCount(0)
		{

		}

		~ComplexDatabase()
		{
			DisConnectDatabase();
		}

		/*
		* @ brief db를 연결한다.
		* @ param 1 : 에러 텍스트
		* @ return 연결 성공 여부
		*/
		bool ConnectDatabase(ComplexString fileName, ComplexString* error = NULL)
		{
			int isOpen = sqlite3_open(fileName.GetBuffer(), &m_dbHandler);
			if (error != NULL)
				*error = GetErrorCode(isOpen);
			if (isOpen != SQLITE_OK)
			{
				sqlite3_close(m_dbHandler);

				return false;
			}

			m_bDBConnection = true;

			return true;
		}

		/*
		* @ brief db 연결여부를 확인한다.
		* @ return 연결 성공 여부
		*/
		bool IsConnect() const
		{
			return m_bDBConnection;
		}

		/*
		* @ brief 연결된 db를 해제한다.
		* @ param 1 : 에러 텍스트
		* @ return 해제 성공 여부
		*/
		bool DisConnectDatabase(ComplexString* error = NULL)
		{
			if (m_bDBConnection)
			{
				int errorcode = sqlite3_close(m_dbHandler);
				m_bDBConnection = false;
				if (error != NULL)
					*error = GetErrorCode(errorcode);
				return true;
			}

			return false;
		}

		/*
		* @ brief select절 실행문
		* prepare query가 아니어도 실행된다.
		* @ param 1 : select 절 쿼리문 작성 (쿼리문은 전부 대문자로)
		*         2 : preparestatement의 ? 값을 채울 외부함수 파라미터 (모두 작성하여 전송)
		*         3 : resultset의 결과값을 받을 외부함수 파라미터 (모두 작성하여 전송) (여러 튜플이면 반복적으로 호출됨)
		*		  4 : 에러 텍스트
		* @ return 쿼리문 성공 여부
		*/
		bool PrepareStatement_Execute(ComplexString query, void(*SetPrepareStatement)(), void(*ResultSet)(), ComplexString* error = NULL)
		{
			if (!m_bDBConnection)
				return false;

			if (query.Find("SELECT") == -1)
				return false;

			if (!m_bUseAutoCommit)
			{
				if (m_nCommitStartCount == 0)
					sqlite3_exec(m_dbHandler, "BEGIN IMMEDIATE TRANSACTION;", NULL, NULL, NULL);
			}

			int result = sqlite3_prepare_v2(m_dbHandler, query.GetBuffer(), -1, &m_queryHandler, 0);
			if (result == SQLITE_OK)
			{
				m_bPrepareStatement = true;
				if (SetPrepareStatement != NULL)
					SetPrepareStatement();
				m_bResultSet = true;

				while (sqlite3_step(m_queryHandler) == SQLITE_ROW)
				{
					if (ResultSet != NULL)
						ResultSet();
				}

				sqlite3_finalize(m_queryHandler);
				m_queryHandler = NULL;
				m_bPrepareStatement = false;
				m_bResultSet = false;
			}
			if (error != NULL)
				*error = GetErrorCode(result);

			return true;
		}

		/*
		* @ brief not preparestatement 쿼리문 실행
		* @ param 1 : 모든 쿼리문 작성 (쿼리문은 전부 대문자로)
		*         2 : sqlite3_callback 함수를 작성하여 전송 (일정 포맷팅이 존재함)
		*		  3 : 에러 텍스트
		* @ 콜백함수 포맷팅
		* static int 함수명(void* notUse, int arg_count, char** arg_value, char** azColName)
		* 만약 리턴이 여러개의 튜플일 경우 해당 콜백함수가 여러번 호출됨
		* @ return 쿼리문 성공 여부
		*/
		bool ExecuteQuery(ComplexString query, sqlite3_callback query_callback, ComplexString* error = NULL)
		{
			if (!m_bDBConnection)
				return false;

			int result = sqlite3_exec(m_dbHandler, query, query_callback, 0, 0);
			if (error != NULL)
				*error = GetErrorCode(result);

			return true;
		}

		/*
		* @ brief insert, update, delete절 실행문
		* prepare query가 아니어도 실행된다.
		* @ param 1 : insert, update, delete 절 쿼리문 작성 (쿼리문은 전부 대문자로)
		*         2 : preparestatement의 ? 값을 채울 외부함수 파라미터 (모두 작성하여 전송)
		*		  3 : 에러 텍스트
		* @ return 쿼리문 성공 여부
		*/
		bool PrepareStatement_Execute(ComplexString query, void(*SetPrepareStatement)(), ComplexString* error = NULL)
		{
			if (!m_bDBConnection)
				return false;

			if (!m_bUseAutoCommit)
			{
				if (m_nCommitStartCount == 0)
					sqlite3_exec(m_dbHandler, "BEGIN IMMEDIATE TRANSACTION;", NULL, NULL, NULL);
			}

			if (query.Find("INSERT") == -1 && query.Find("UPDATE") == -1 && query.Find("DELETE") == -1)
				return false;

			int result = sqlite3_prepare_v2(m_dbHandler, query.GetBuffer(), -1, &m_queryHandler, 0);
			if (result == SQLITE_OK)
			{
				m_bPrepareStatement = true;
				if (SetPrepareStatement != NULL)
					SetPrepareStatement();
				sqlite3_step(m_queryHandler);

				sqlite3_finalize(m_queryHandler);
				m_queryHandler = NULL;
				m_bPrepareStatement = false;
			}
			if (error != NULL)
				*error = GetErrorCode(result);
			return true;
		}

		/*
		* @ brief preparestatement의 ? 값을 채우기 위한 함수 (꼭 SetPrepareStatement의 외부함수에 작성하자)
		* @ param 1 : ? 위치의 인덱스번호 1부터 시작
		*         2 : ? 의 값 ('이나 \" 는 붙이지말고 전송)
		*		  3 : 에러 텍스트
		*/
		void SetString(int prepare_index, ComplexString value, ComplexString* error = NULL)
		{
			if (!m_bPrepareStatement)
				return;

			int result = sqlite3_bind_text(m_queryHandler, prepare_index, value.GetBuffer(), -1, SQLITE_TRANSIENT);	// SQLITE_TRANSIENT 는 통째로 복사하여 전달. / SQLITE_STATIC 은 주소값만 복사해서 전달
			if (error != NULL)
				*error = GetErrorCode(result);
		}

		/*
		* @ brief preparestatement의 ? 값을 채우기 위한 함수 (꼭 SetPrepareStatement의 외부함수에 작성하자)
		* @ param 1 : ? 위치의 인덱스번호 1부터 시작
		*         2 : ? 의 값
		*		  3 : 에러 텍스트
		*/
		void SetInt(int prepare_index, int value, ComplexString* error = NULL)
		{
			if (!m_bPrepareStatement)
				return;

			int result = sqlite3_bind_int(m_queryHandler, prepare_index, value);
			if (error != NULL)
				*error = GetErrorCode(result);
		}

		/*
		* @ brief preparestatement의 ? 값을 채우기 위한 함수 (꼭 SetPrepareStatement의 외부함수에 작성하자)
		* @ param 1 : ? 위치의 인덱스번호 1부터 시작
		*         2 : ? 의 값
		* @		  3 : 에러 텍스트
		*/
		void SetDouble(int prepare_index, double value, ComplexString* error = NULL)
		{
			if (!m_bPrepareStatement)
				return;

			int result = sqlite3_bind_double(m_queryHandler, prepare_index, value);
			if (error != NULL)
				*error = GetErrorCode(result);
		}

		/*
		* @ brief resultset의 결과값을 리턴한다.
		* @ param 1 : 칼럼의 인덱스번호 0부터 시작
		* @ return 결과값
		*/
		ComplexString GetString(int column_index)
		{
			if (!m_bResultSet)
				return "";

			return ComplexString((const char*)sqlite3_column_text(m_queryHandler, column_index));
		}

		/*
		* @ brief resultset의 결과값을 리턴한다.
		* @ param 1 : 칼럼의 인덱스번호 0부터 시작
		* @ return 결과값
		*/
		int GetInt(int column_index)
		{
			if (!m_bResultSet)
				return -1;

			return sqlite3_column_int(m_queryHandler, column_index);
		}

		/*
		* @ brief resultset의 결과값을 리턴한다.
		* @ param 1 : 칼럼의 인덱스번호 0부터 시작
		* @ return 결과값
		*/
		double GetDouble(int column_index)
		{
			if (!m_bResultSet)
				return -1;

			return sqlite3_column_double(m_queryHandler, column_index);
		}

		/*
		* @ brief 오토 커밋을 설정한다.
		* @ param 1 : true = 오토커밋 적용, false = 오토커밋 미적용
		*/
		void SetAutoCommit(bool useAutoCommit)
		{
			m_bUseAutoCommit = useAutoCommit;
		}

		/*
		* @ brief 오토커밋 미적용일 때 수행. 지금까지 적용한 쿼리문을 커밋한다.
		* @ param 1 : 에러 텍스트
		*/
		void Commit(ComplexString* error = NULL)
		{
			if (!m_bUseAutoCommit)
			{
				int result = sqlite3_exec(m_dbHandler, "COMMIT TRANSACTION;", NULL, NULL, NULL);
				if (error != NULL)
					*error = GetErrorCode(result);
				m_nCommitStartCount = 0;
			}
		}

		/*
		* @ brief 오토커밋 미적용일 때 수행. 지금까지 적용한 쿼리문을 롤백한다. (세이브 포인트로)
		* @ param 1 : 롤백할 세이브 포인트 이름. (값을 입력하지 않으면 초기로 돌아간다.)
		* @ param 2 : 에러 텍스트
		*/
		void Rollback(ComplexString savepoint_name = "", ComplexString* error = NULL)
		{
			if (!m_bUseAutoCommit)
			{
				int result = -1;
				if (savepoint_name.IsEmpty())
				{
					result = sqlite3_exec(m_dbHandler, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
				}
				else
				{
					ComplexString query;
					query.Format("ROLLBACK TO %s;", savepoint_name.GetBuffer());
					result = sqlite3_exec(m_dbHandler, query, NULL, NULL, NULL);
				}
				if (error != NULL)
					*error = GetErrorCode(result);
				m_nCommitStartCount = 0;
			}
		}

		/*
		* @ brief 오토커밋 미적용일 때 수행. 세이브 포인트를 설정한다.
		* @ param 1 : 설정할 세이브 포인트 이름.
		* @ param 2 : 에러 텍스트
		*/
		void SetSavePoint(ComplexString savepoint_name, ComplexString* error = NULL)
		{
			if (!m_bUseAutoCommit)
			{
				ComplexString query;
				query.Format("SAVEPOINT %s;", savepoint_name.GetBuffer());
				int result = sqlite3_exec(m_dbHandler, query.GetBuffer(), NULL, NULL, NULL);
				if (error != NULL)
					*error = GetErrorCode(result);
			}
		}

		/*
		* @ brief 오토커밋 미적용일 때 수행. 설정한 세이브 포인트를 해제한다.
		* @ param 1 : 해제할 세이브 포인트 이름.
		* @ param 2 : 에러 텍스트
		*/
		void ReleaseSavePoint(ComplexString savepoint_name, ComplexString* error = NULL)
		{
			if (!m_bUseAutoCommit)
			{
				ComplexString query;
				query.Format("RELEASE %s;", savepoint_name.GetBuffer());
				int result = sqlite3_exec(m_dbHandler, query.GetBuffer(), NULL, NULL, NULL);
				if (error != NULL)
					*error = GetErrorCode(result);
			}
		}

	private:

		sqlite3*		m_dbHandler;			// 데이터베이스 핸들
		sqlite3_stmt*	m_queryHandler;			// 쿼리문 핸들

		bool			m_bDBConnection;		// db 연결 여부
		bool			m_bPrepareStatement;	// preparestatement 사용 여부
		bool			m_bResultSet;			// resultset 사용 여부

		bool			m_bUseAutoCommit;		// autocommit 사용 여부
		int				m_nCommitStartCount;	// autocommit 사용 안할 때 "BEGIN"을 작성할 카운트 번호 (0번일때만 작성함, ROLLBACK이나 COMMIT하면 0으로 초기화)

	};
}