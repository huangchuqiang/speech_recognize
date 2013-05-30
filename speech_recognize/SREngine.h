#ifndef SRENGINE_H
#define SRENGINE_H

#include <sapi.h>
#include <sphelper.h>
#include <spuihelp.h>
#include <QString>


class KSREngine  
{
public:
	KSREngine();
	~KSREngine();


	void  SetRuleState(const  WCHAR* pszRuleName, 
			const WCHAR*pszValue, BOOL fActivate);

	bool  LoadCmdFromFile(QString XMLFileName);
	void  InitializeSapi(HWND hWnd, UINT Msg);

public:
	CComPtr<ISpRecognizer> m_pRecognizer;
	CComPtr<ISpRecoContext> m_pRecoCtxt;
	CComPtr<ISpRecoGrammar> m_pCmdGram;
	CComPtr<ISpAudio> m_pAudio;
	CComPtr<ISpVoice> m_pVoice;
};

#endif // SRENGINE_H
