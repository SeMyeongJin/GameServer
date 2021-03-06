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

		pConnectedSession->WritePacket(PT_SIGNUP_SUCC, writeBuffer, WRITE_PT_SIGNUP_SUCC(writeBuffer));

		Log::WriteLog(_T(" Signup Completely : ID(%s)"), signUpData.userID);
		return;
	}
	if (!GDBManager->RegistUserQuery(signUpData.userID, signUpData.userPW, signUpData.user_name, &errorCode))
	{
		BYTE writeBuffer[MAX_BUFFER_LENGTH] = { 0, };
		
		pConnectedSession->WritePacket(PT_SIGNUP_FAIL, writeBuffer, WRITE_PT_SIGNUP_FAIL(writeBuffer, errorCode));
	}
}

VOID IOCPServer::PROC_PT_LOGIN(ConnectedSession * pConnectedSession, DWORD dwProtocol, BYTE * pPacket, DWORD dwPacketLength)
{
	S_PT_LOGIN loginData;
	READ_PT_LOGIN(pPacket, loginData);

	S_PT_LOGIN_SUCC succData;
	ZeroMemory(succData.user_name, sizeof(succData.user_name));

	for (int i = 0; i < mSessionManager.mConnectedSessions.size(); i++)
	{
		if (pConnectedSession != mSessionManager.mConnectedSessions[i])
		{
			if (!_tcscmp(mSessionManager.mConnectedSessions[i]->GetUserID(), loginData.userID))
			{
				S_PT_LOGIN_FAIL failData;
				failData.errorCode = EC_LOGIN_ID_ALREADY_REGIST;
				BYTE writeBuffer[MAX_BUFFER_LENGTH] = { 0, };

				pConnectedSession->WritePacket(PT_LOGIN_FAIL, writeBuffer, WRITE_PT_LOGIN_FAIL(writeBuffer, failData.errorCode));
				return;
			}
		}
	}
	if (GDBManager->LoginCheckQuery(loginData.userID, loginData.userPW))
	{
		if (GDBManager->LoadUserData(loginData.userID, loginData.userPW, succData.user_name, &succData.rating))
		{
			BYTE writeBuffer[MAX_BUFFER_LENGTH] = { 0, };
			pConnectedSession->WritePacket(PT_LOGIN_SUCC, writeBuffer, WRITE_PT_LOGIN_SUCC(writeBuffer, succData.user_name, succData.rating));
			pConnectedSession->SetUserID(loginData.userID);
			Log::WriteLog(_T(" Login Completely : ID(%s)"), loginData.userID);
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