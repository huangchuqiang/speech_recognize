#include "SREngine.h"



KSREngine::KSREngine()
{
	::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
}

KSREngine::~KSREngine()
{
	::CoUninitialize();
}

void KSREngine::InitializeSapi(HWND hWnd, UINT Msg)  //��ʼ������ʶ�����
{
	//��ʼ��COM��
	//	::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	HRESULT hr;// = ::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	Q_ASSERT(SUCCEEDED(hr));

	//��ʼ������ʶ�����
	//m_pRecognizer��ʼ��
	hr = m_pRecognizer.CoCreateInstance(CLSID_SpInprocRecognizer);//����
	Q_ASSERT(SUCCEEDED(hr));
	//m_pAudio��ʼ��
	hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &m_pAudio);//����Ĭ�ϵ���Ƶ�������
	Q_ASSERT(SUCCEEDED(hr));
	hr = m_pRecognizer->SetInput(m_pAudio,TRUE);//����ʶ����������Դ
	Q_ASSERT(SUCCEEDED(hr));
	//m_pRecoCtxt��ʼ��
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

	if(!m_pCmdGram) //û�й����ʱ��
	{
		hr = m_pRecoCtxt->CreateGrammar(0, &m_pCmdGram);//������������
		if(FAILED(hr))
		{
			Q_ASSERT(!"Error create grammar");
			return false;
		}

		hr = m_pCmdGram->LoadCmdFromFile(XMLFileName.utf16(), SPLO_DYNAMIC);//�����﷨����
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
		hr = m_pCmdGram->SetRuleState(pszRuleName,NULL,SPRS_ACTIVE);//������������
	}
	else
	{
		hr = m_pCmdGram->SetRuleState(pszRuleName,NULL,SPRS_INACTIVE);
	}
}