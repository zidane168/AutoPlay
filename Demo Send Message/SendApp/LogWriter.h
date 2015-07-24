/******************************************************************************
* Copyright (c) 2009 VinaGame JSC. All Rights Reserved.
*
* This is unpublished proprietary source code of VinaGame JSC.
* The copyright notice above does not evidence any actual or intended
* publication of such source code.
*
* File Name: LogWriter.h
*
* Description: 
* 
* Author: DiepTH
*
* Creation:	mm/dd/yyyy
* 07/03/2009	DiepTH
* Change history: mm/dd/yyyy
* 
******************************************************************************/

#pragma once

#define LOG_TYPE_FATAL			0x0
#define LOG_TYPE_ERROR			0x1
#define LOG_TYPE_WARN			0x2				
#define LOG_TYPE_FYI			0x3
#define LOG_TYPE_DEBUG			0x4	

#define	DEF_LOG_FILESIZE		1024
#define	DEF_LIFETIME			30

#define FYI_COLOR		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define ERROR_COLOR		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
#define FATAL_COLOR		FOREGROUND_INTENSITY | FOREGROUND_RED

#define WARN_COLOR		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE
#define DEBUG_COLOR		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

typedef struct __tagLOG_CONFIG
{
	BOOL	bEnable;
	BOOL	bAutoClean;
	BYTE	byLogLevel;
	SHORT	nMaxSize;
	SHORT	nLifeTime;
	__tagLOG_CONFIG()
	{
		bEnable		= TRUE;
		bAutoClean	= FALSE;
		byLogLevel	= (BYTE)LOG_TYPE_FYI;
		nMaxSize	= DEF_LOG_FILESIZE;
		nLifeTime	= DEF_LIFETIME;
	}
} LOG_CONFIG;

class CLogWriter
{
private:
	CRITICAL_SECTION	m_csLockWriter;

	TCHAR				m_szLogDir[MAX_PATH];
	TCHAR				m_szTitle[MAX_PATH];
	HANDLE				m_hLogFile;
	DWORD				m_maxSize;
	BYTE				m_LogLevel;
	bool				m_bEnableConsole;
	HANDLE				m_hConsole;

	BOOL	Open();
	void	Close();
	void	GetFileName(LPTSTR filename);
	void	CheckOpenNewFile();
	BOOL	CreateDir(LPCTSTR strDir);
	void	WriteLogConsole(char type, const TCHAR* logLine);
	bool	OpenConsole(const TCHAR *szTitle);
	HWND	GetHWND();
	void	DisableClose();

public:
	CLogWriter(void);
	~CLogWriter(void);

	BOOL	Initialize(LPCTSTR lpszLogDir = NULL, BYTE LogLevel = LOG_TYPE_ERROR, LPCTSTR lpszTitle = NULL, bool bEnableConsole = FALSE);
	void	SetMaxLogSize(DWORD dwKb);
	BOOL	WriteLog(char type, LPCTSTR msg);
	BOOL	WriteLogFormat(char type, LPCTSTR msg, ...);
};
