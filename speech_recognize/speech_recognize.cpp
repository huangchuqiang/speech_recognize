#include "speech_recognize.h"
#include <QtGlobal>
#include <QString>
#include <string>
#include <QDebug>



#pragma comment(lib,"ole32.lib")   //CoInitialize CoCreateInstance��Ҫ����ole32.dll
#pragma comment(lib,"sapi.lib")    //sapi.lib��SDK��libĿ¼,������ȷ����

#define WM_RECORD_INFO WM_USER+100

speech_recognize::speech_recognize(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), isConfig(true)
{
	connect(this, SIGNAL(REG()), this, SLOT(doSomeThing()));
	ui.setupUi(this);
	//��ʼ��COM��
//	::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	HRESULT hr = ::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
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
//	HRESULT hr = m_pRecognizer.CoCreateInstance(CLSID_SpSharedRecognizer);
	hr = m_pRecognizer->CreateRecoContext(&m_pRecoCtxt);
	Q_ASSERT(SUCCEEDED(hr));

	hr = m_pRecoCtxt->GetVoice(&m_pVoice);
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

	m_pVoice->Speak(L"��ӭʹ�ü�����ʶ�����", SPF_ASYNC, NULL);
	this->ui.lab_cmd->setText(tr("��˵���������"));
	this->ui.lab_config->setText(tr(""));
	this->ui.statusBar->addWidget(new QLabel(tr("��������")));

}

speech_recognize::~speech_recognize()
{
	::CoUninitialize();
}

void speech_recognize::doSomeThing()
{
	USES_CONVERSION;
	CSpEvent event;

	HRESULT hr = S_OK;
	if(m_pRecoCtxt)
	{
		while( S_OK==event.GetFrom(m_pRecoCtxt) )//�ȴ������������ӿڽ���
		{
			switch(event.eEventId)
			{
			case SPEI_FALSE_RECOGNITION: //����ʶ��
				break;

			case SPEI_HYPOTHESIS: //��ʶ��
			case SPEI_RECOGNITION:  //��ȷʶ��
				{
					CComPtr <ISpRecoResult> cpResult;
					CSpDynamicString   dstrText;

					cpResult = event.RecoResult();
					cpResult->GetText(SP_GETWHOLEPHRASE,SP_GETWHOLEPHRASE,TRUE,&dstrText,NULL);//��ȡʶ����
					std::string str(W2A(dstrText));
					this->ui.lab_cmd->setText(tr("��˵�������ǣ�\t") + tr(str.data()));
					if (isConfig)
					{
						this->ui.lab_config->setText(tr("\t\t˵\"ȷ��\"ִ�����˵\"ȡ��\"ȡ������"));
						isConfig = false;
					}
					else
					{
						isConfig = true;
						if (str == std::string("ȷ��"))
							this->ui.lab_config->setText(tr("\t\t�������ڱ�ִ��......"));
						else
						{
							if (str == std::string("ȡ��"))
								this->ui.lab_config->setText(tr("\t\t���ȡ��......"));
							else
							{
								this->ui.lab_cmd->setText(tr("�������..."));
								this->ui.lab_config->setText(tr("\t\t˵\"ȷ��\"ִ�����˵\"ȡ��\"ȡ������"));
								isConfig = false;
							}
						}

						
					}
					
					
					cpResult.Release();

				}
				break;

			default : 
				break;
			}
		}
	}
}

bool speech_recognize::winEvent(MSG *message, long *result)
{
	if(message->message == WM_RECORD_INFO)
	{
		qDebug()<<"OK";
		emit REG();
		return true;
	}
	else
		return QMainWindow::winEvent(message, result);

}
