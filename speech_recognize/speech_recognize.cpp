#include "speech_recognize.h"
#include <QtGlobal>
#include <QString>
#include <string>
#include <QDebug>



#pragma comment(lib,"ole32.lib")   //CoInitialize CoCreateInstance需要调用ole32.dll
#pragma comment(lib,"sapi.lib")    //sapi.lib在SDK的lib目录,必需正确配置

#define WM_RECORD_INFO WM_USER+100

speech_recognize::speech_recognize(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), isConfig(true)
{
	connect(this, SIGNAL(REG()), this, SLOT(doSomeThing()));
	ui.setupUi(this);
	//初始化COM口
//	::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	HRESULT hr = ::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
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

	//m_pCmdGram初始化
	hr = m_pRecoCtxt->CreateGrammar(0, &m_pCmdGram);
	if(SUCCEEDED(hr))
		hr = m_pCmdGram->LoadDictation(NULL, SPLO_STATIC);
	Q_ASSERT(SUCCEEDED(hr));
	hr = m_pCmdGram->SetDictationState(SPRS_ACTIVE);
	Q_ASSERT(SUCCEEDED(hr));

	m_pVoice->Speak(L"欢迎使用简单语音识别软件", SPF_ASYNC, NULL);
	this->ui.lab_cmd->setText(tr("请说出你的命令"));
	this->ui.lab_config->setText(tr(""));
	this->ui.statusBar->addWidget(new QLabel(tr("正在聆听")));

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
		while( S_OK==event.GetFrom(m_pRecoCtxt) )//等待创建语言主接口结束
		{
			switch(event.eEventId)
			{
			case SPEI_FALSE_RECOGNITION: //错误识别
				break;

			case SPEI_HYPOTHESIS: //假识别
			case SPEI_RECOGNITION:  //正确识别
				{
					CComPtr <ISpRecoResult> cpResult;
					CSpDynamicString   dstrText;

					cpResult = event.RecoResult();
					cpResult->GetText(SP_GETWHOLEPHRASE,SP_GETWHOLEPHRASE,TRUE,&dstrText,NULL);//获取识别字
					std::string str(W2A(dstrText));
					this->ui.lab_cmd->setText(tr("你说的命令是：\t") + tr(str.data()));
					if (isConfig)
					{
						this->ui.lab_config->setText(tr("\t\t说\"确定\"执行命令，说\"取消\"取消命令"));
						isConfig = false;
					}
					else
					{
						isConfig = true;
						if (str == std::string("确定"))
							this->ui.lab_config->setText(tr("\t\t命令正在被执行......"));
						else
						{
							if (str == std::string("取消"))
								this->ui.lab_config->setText(tr("\t\t命令被取消......"));
							else
							{
								this->ui.lab_cmd->setText(tr("命令错误..."));
								this->ui.lab_config->setText(tr("\t\t说\"确定\"执行命令，说\"取消\"取消命令"));
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
