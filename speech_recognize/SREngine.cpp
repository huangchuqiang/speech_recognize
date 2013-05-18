#include "SREngine.h"



KSREngine::KSREngine()
{
	::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
}

KSREngine::~KSREngine()
{
	::CoUninitialize();
}

void KSREngine::InitializeSapi(HWND hWnd, UINT Msg)  //初始化语音识别程序
{
	//初始化COM口
	//	::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	HRESULT hr;// = ::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	Q_ASSERT(SUCCEEDED(hr));

	//初始化语音识别对象
	//m_pRecognizer初始化
	hr = m_pRecognizer.CoCreateInstance(CLSID_SpInprocRecognizer);//独享
	Q_ASSERT(SUCCEEDED(hr));
	//m_pAudio初始化
	hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &m_pAudio);//建立默认的音频输入对象
	Q_ASSERT(SUCCEEDED(hr));
	hr = m_pRecognizer->SetInput(m_pAudio,TRUE);//设置识别引擎输入源
	Q_ASSERT(SUCCEEDED(hr));
	//m_pRecoCtxt初始化
	hr = m_pRecognizer->CreateRecoContext(&m_pRecoCtxt);
	Q_ASSERT(SUCCEEDED(hr));

	hr = m_pRecoCtxt->GetVoice(&m_pVoice);
	Q_ASSERT(SUCCEEDED(hr));
	hr = m_pRecoCtxt->SetNotifyWindowMessage(hWnd, Msg, 0, 0);
	Q_ASSERT(SUCCEEDED(hr));
	const ULONGLONG ullInterest = SPFEI(SPEI_SOUND_START)|SPFEI(SPEI_SOUND_END)|SPFEI(SPEI_RECOGNITION);
	hr = m_pRecoCtxt->SetInterest(ullInterest,ullInterest);
	Q_ASSERT(SUCCEEDED(hr));
	hr = m_pRecognizer->SetRecoState(SPRST_ACTIVE);
	Q_ASSERT(SUCCEEDED(hr));
}

bool  KSREngine::LoadCmdFromFile(QString  XMLFileName)
{
	HRESULT   hr = S_OK;

	if(!m_pCmdGram) //没有规则的时候
	{
		hr = m_pRecoCtxt->CreateGrammar(0, &m_pCmdGram);//创建语音对象
		if(FAILED(hr))
		{
			Q_ASSERT(!"Error create grammar");
			return false;
		}

		hr = m_pCmdGram->LoadCmdFromFile(XMLFileName.utf16(), SPLO_DYNAMIC);//加载语法规则
		if(FAILED(hr))
		{
			Q_ASSERT(!"Error LoadCmdFromFile");
			return false;
		}
	}
	return true;
}

void KSREngine::SetRuleState(const  WCHAR* pszRuleName, const WCHAR*pszValue, BOOL fActivate)
{
	HRESULT   hr = S_OK;

	if(fActivate)
	{
		hr = m_pCmdGram->SetRuleState(pszRuleName,NULL,SPRS_ACTIVE);//激活语音规则
	}
	else
	{
		hr = m_pCmdGram->SetRuleState(pszRuleName,NULL,SPRS_INACTIVE);
	}
}