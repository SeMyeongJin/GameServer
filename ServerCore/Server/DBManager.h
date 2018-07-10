#pragma once
class DBManager
{
public:
	DBManager();
	~DBManager();

public:
	BOOL Begin(VOID);

	// 유저 등록 쿼리
	BOOL RegistUserQuery(WCHAR *id, WCHAR *pw, WCHAR *name);
	// 로그인 체크 쿼리
	BOOL LoginCheckQuery(WCHAR *id, WCHAR *pw);

private:
	const char * DB_HOST;
	const char * DB_NAME;
	const char * DB_SOCK;
	const char * DB_USER;
	const char * DB_PASS;

	enum {
		DB_PORT = 3306,
		DB_OPT = 0
	};

	MYSQL			*conn;			// DB 연결
	MYSQL_RES		*res;			// 쿼리 결과
	MYSQL_ROW		row;			// 결과값을 나타낼 문자열
};

extern DBManager *GDBManager;