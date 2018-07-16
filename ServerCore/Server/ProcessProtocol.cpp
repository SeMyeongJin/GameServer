#include "stdafx.h"
#include "IOCPServer.h"


VOID IOCPServer::PROC_PT_SIGNUP(ConnectedSession * pConnectedSession, DWORD dwProtocol, BYTE * pPacket, DWORD dwPacketLength)
{
	S_PT_SIGNUP signUpData;
	READ_PT_SIGNUP(pPacket, signUpData);
	
	DWORD errorCode = 0;

	if (GDBManager->RegistUserQuery(signUpData.userID, signUpData.userPW, signUpData.user_name, &errorCode))
	{
		BYTE writeBuffer[MAX_BUFFER_LENGTH] = { 0, };
		S_PT_SIGNUP_SUCC succData;

		pConnectedSession->WritePacket(PT_SIGNUP_SUCC, writeBuffer, WRITE_PT_SIGNUP_SUCC(writeBuffer));
		return;
	}
	if (!GDBManager->RegistUserQuery(signUpData.userID, signUpData.userPW, signUpData.user_name, &errorCode))
	{
		BYTE writeBuffer[MAX_BUFFER_LENGTH] = { 0, };
		S_PT_SIGNUP_FAIL failData;
		
		pConnectedSession->WritePacket(PT_SIGNUP_FAIL, writeBuffer, WRITE_PT_SIGNUP_FAIL(writeBuffer, errorCode));
	}
}

VOID IOCPServer::PROC_PT_LOGIN(ConnectedSession * pConnectedSession, DWORD dwProtocol, BYTE * pPacket, DWORD dwPacketLength)
{
	S_PT_LOGIN loginData;
	READ_PT_LOGIN(pPacket, loginData);

	S_PT_LOGIN_SUCC succData;
	ZeroMemory(succData.user_name, sizeof(succData.user_name));

	if (GDBManager->LoginCheckQuery(loginData.userID, loginData.userPW))
	{
		if (GDBManager->LoadUserData(loginData.userID, loginData.userPW, succData.user_name, &succData.LifePoint))
		{
			BYTE writeBuffer[MAX_BUFFER_LENGTH] = { 0, };

			pConnectedSession->WritePacket(PT_LOGIN_SUCC, writeBuffer, WRITE_PT_LOGIN_SUCC(writeBuffer, succData.user_name, succData.LifePoint));
			return;
		}
	}
	if (!GDBManager->LoginCheckQuery(loginData.userID, loginData.userPW))
	{
		S_PT_LOGIN_FAIL failData;
		failData.errorCode = EC_LOGIN_ID_AND_PASS_ERROR;
		BYTE writeBuffer[MAX_BUFFER_LENGTH] = { 0, };

		pConnectedSession->WritePacket(PT_LOGIN_FAIL, writeBuffer, WRITE_PT_LOGIN_FAIL(writeBuffer, failData.errorCode));
	}
}

VOID IOCPServer::PROC_PT_CHAT(ConnectedSession * pConnectedSession, DWORD dwProtocol, BYTE * pPacket, DWORD dwPacketLength)
{
	BYTE writeBuffer[MAX_BUFFER_LENGTH] = { 0, };
	S_PT_CHAT chatData;
	READ_PT_CHAT(pPacket, chatData);

	mSessionManager.WriteAll(PT_CHAT, writeBuffer, WRITE_PT_CHAT(writeBuffer, chatData.user_name, chatData.message));
	if (wcslen(chatData.message) > 0)
		_tprintf(_T("%s :[chat message] %s\n"), chatData.user_name, chatData.message);
}