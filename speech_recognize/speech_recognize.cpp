#include "speech_recognize.h"
#include <QtGlobal>
#include <QPushButton>
#include <QDebug>



#pragma comment(lib,"ole32.lib")   //CoInitialize CoCreateInstance��Ҫ����ole32.dll
#pragma comment(lib,"sapi.lib")    //sapi.lib��SDK��libĿ¼,������ȷ����

#define WM_RECORD_INFO WM_USER+100

speech_recognize::speech_recognize(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), label(this)
{
	ui.setupUi(this);
	isRecognize = true;
	//��ʼ��COM��
//	::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	HRESULT hr = ::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	Q_ASSERT(SUCCEEDED(hr));
	label.setText(QString::fromUtf16(L"����������"));
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
//	HRESULT hr = m_pRecognizer.CoCreateInstance(CLSID_SpSharedRecognizer);
	hr = m_pRecognizer->CreateRecoContext(&m_pRecoCtxt);
	Q_ASSERT(SUCCEEDED(hr));
	hr = m_pRecoCtxt->SetNotifyWindowMessage((HWND)this->winId(), WM_RECORD_INFO, 0, 0);
//	hr = m_pRecoCtxt->SetNotifyCallbackFunction(doSomeThing, 0, 0);
	Q_ASSERT(SUCCEEDED(hr));
	const ULONGLONG ullInterest = SPFEI(SPEI_SOUND_START)|SPFEI(SPEI_SOUND_END)|SPFEI(SPEI_RECOGNITION);
	hr = m_pRecoCtxt->SetInterest(ullInterest,ullInterest);
	Q_ASSERT(SUCCEEDED(hr));

	hr = m_pRecognizer->SetRecoState(SPRST_ACTIVE);
	Q_ASSERT(SUCCEEDED(hr));

	//m_pCmdGram��ʼ��
	hr = m_pRecoCtxt->CreateGrammar(0, &m_pCmdGram);
	if(SUCCEEDED(hr))
		hr = m_pCmdGram->LoadDictation(NULL, SPLO_STATIC);
	Q_ASSERT(SUCCEEDED(hr));
	hr = m_pCmdGram->SetDictationState(SPRS_ACTIVE);
	Q_ASSERT(SUCCEEDED(hr));

}

speech_recognize::~speech_recognize()
{
	::CoUninitialize();
}

void speech_recognize::doSomeThing()
{
	static int i =1;
	USES_CONVERSION;
	CSpEvent event;
	HRESULT hr = S_OK;
	CComPtr<ISpRecoResult> cpResult;
	label.setText(QString::fromUtf16(L"����ʶ��"));
	label.setText(label.text() + QString(i));
	i = i + 1;

	while (SUCCEEDED(hr) &&
		SUCCEEDED(hr = event.GetFrom(m_pRecoCtxt)) &&
		hr == S_OK)
	{
		hr = m_pRecoCtxt->WaitForNotifyEvent(INFINITE);
	}
	

	cpResult = event.RecoResult();
	m_pCmdGram->SetDictationState(SPRS_INACTIVE);
	CSpDynamicString dstrText;

	if (SUCCEEDED(cpResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, 
		TRUE, &dstrText, NULL)))
	{
		label.setText(QString::fromUtf16((const ushort*)(W2A(dstrText))));
	}

	m_pCmdGram->SetDictationState( SPRS_ACTIVE );
}

bool speech_recognize::winEvent(MSG *message, long *result)
{
	if(message->message == WM_RECORD_INFO && isRecognize)
	{
		qDebug()<<"OK";
		isRecognize = false;
		emit REG();
		isRecognize = true;
		return true;
	}
	else
		return QMainWindow::winEvent(message, result);

}
