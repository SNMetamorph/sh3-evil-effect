#include <cstdio>
#include <cstring>
#include "exception.h"

CException::CException(
    const char *desc, 
    const char *funcName, 
    const char *fileName, 
    int lineNumber
)
{
	m_szFuncName = funcName;
	m_szFilePath = fileName;
	m_szDescription = desc;
	m_iLine = lineNumber;
    m_szMessageBuffer[0] = '\0';
}

const char *CException::GetFormattedMessage()
{
	snprintf(
		m_szMessageBuffer, 
		sizeof(m_szMessageBuffer), 
		"%s() [%s:%d]: %s", 
		m_szFuncName, 
		GetFileName(), 
		m_iLine, 
		m_szDescription
	);
	return m_szMessageBuffer;
}

const char *CException::GetDescription() const
{
	return m_szDescription;
}

const char *CException::GetFunctionName() const
{
	return m_szFuncName;
}

const char *CException::GetFileName() const
{
	return strrchr(m_szFilePath, '\\') + 1;
}

int CException::GetLineNumber() const
{
	return m_iLine;
}
