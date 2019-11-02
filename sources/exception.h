#pragma once

// Universal macros
#define EXCEPT(message) (throw CException(message, __FUNCTION__, __FILE__, __LINE__))

class CException
{
public:
	CException(
        const char *desc, 
        const char *funcName, 
        const char *filePath, 
        int lineNumber
    );
    ~CException() {};

    int GetLineNumber() const;
	const char *GetFormattedMessage();
	const char *GetDescription() const;
	const char *GetFunctionName() const;
	const char *GetFileName() const;

	char m_szMessageBuffer[256];

private:
	int m_iLine;
	const char *m_szDescription;
	const char *m_szFuncName;
	const char *m_szFilePath;
};

