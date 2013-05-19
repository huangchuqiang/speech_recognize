#include "speech_recognize.h"
#include <QtGlobal>
#include <QFile>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QPainter>
#include <QDateTime>

#define WM_RECORD_INFO WM_USER+100

speech_recognize::speech_recognize(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	connect(this, SIGNAL(REG()), this, SLOT(recordEvent()));
	ui.setupUi(this);

	this->setWindowTitle(tr("简单语音识别软件"));
	this->setFixedWidth(this->width());
	this->setFixedHeight(this->height());

	m_SREngine.InitializeSapi((HWND)this->winId(), WM_RECORD_INFO);
	m_SREngine.LoadCmdFromFile("CmdCtrl.xml");
	m_SREngine.SetRuleState(NULL,NULL,TRUE);

	m_SREngine.m_pVoice->Speak(L"这是一个简单语音识别软件", SPF_ASYNC, NULL);

	initUI();
	this->ui.statusBar->addWidget(&m_toolLabel);
	cmdBegin();
}

speech_recognize::~speech_recognize()
{
}

void speech_recognize::initUI()
{
	QPalette pe;
	pe.setColor(QPalette::WindowText,Qt::blue);
	this->ui.lab_cmd->setPalette(pe);
	this->ui.lab_config->setPalette(pe);
	pe.setColor(QPalette::WindowText,Qt::red);
	m_toolLabel.setPalette(pe);
	this->ui.lab_title->setText(tr("欢迎使用简单语音识别软件"));
	this->ui.lab_cmd->setText(tr("说“开始识别”进入聆听模式"));
	this->ui.lab_config->setText(tr(""));
	this->ui.lab_caption->setText(tr(""));
	this->ui.lab_1->setText(tr(""));
	this->ui.lab_2->setText(tr(""));
	this->ui.lab_3->setText(tr(""));
	this->ui.lab_next->setText(tr(""));
	m_toolLabel.setText(tr("当前模式：休眠模式"));
}


void speech_recognize::recordEvent()
{
	USES_CONVERSION;
	CSpEvent event;

	HRESULT hr = S_OK;
	if(m_SREngine.m_pRecoCtxt)
	{
		while( S_OK==event.GetFrom(m_SREngine.m_pRecoCtxt) )//等待创建语言主接口结束
		{
			switch(event.eEventId)
			{
			case SPEI_FALSE_RECOGNITION: //错误识别
				break;

			case SPEI_HYPOTHESIS: //假识别
			case SPEI_RECOGNITION:  //正确识别
				{
					CSpDynamicString dstrText;
					if (SUCCEEDED(event.RecoResult()->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, 
						TRUE, &dstrText, NULL)))
					{
						m_SREngine.m_pVoice->Speak(dstrText, SPF_ASYNC, NULL);
						executeCommand(event.RecoResult(), W2A(dstrText));
					}
					
				}
				break;

			default : 
				break;
			}
		}
	}
}
void speech_recognize::executeCommand( ISpPhrase *pPhrase, std::string dstrText)
{
	SPPHRASE *pElements;
	// Get the phrase elements, one of which is the rule id we specified in
	// the grammar.  Switch on it to figure out which command was recognized.
	if (SUCCEEDED(pPhrase->GetPhrase(&pElements)))
	{        
		switch ( pElements->Rule.ulId )
		{
		case CMD_Begin:
			cmdBegin();
			break;
		case CMD_End:
			cmdEnd();
			break;
		case CMD_Single:
		case CMD_Group:
			command(pElements, dstrText);
			break;
		case CMD_Options:
			cmdOptios(pElements, dstrText);
			break;
		case CMD_Config:
			cmdConfig(pElements);
			break;
		}
		// Free the pElements memorySPRS_ACTIVElocated for us
		::CoTaskMemFree(pElements);
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

void speech_recognize::cmdBegin()
{
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Begin, SPRS_INACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_End, SPRS_ACTIVE);

	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Single, SPRS_ACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Group, SPRS_ACTIVE);

	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Options, SPRS_INACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Config, SPRS_INACTIVE);
	m_toolLabel.setText(tr("当前模式：聆听模式"));
	this->ui.lab_cmd->setText(tr("请说出你的命令"));
	this->ui.lab_config->setText(tr(""));
}

void speech_recognize::cmdEnd()
{
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Begin, SPRS_ACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_End, SPRS_INACTIVE);

	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Group, SPRS_INACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Single, SPRS_INACTIVE);

	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Options, SPRS_INACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Config, SPRS_INACTIVE);
	initUI();
}

void speech_recognize::command(SPPHRASE *pElements, std::string dstrText)
{
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Group, SPRS_INACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Single, SPRS_INACTIVE);

	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Options, SPRS_ACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Config, SPRS_ACTIVE);

	this->ui.lab_cmd->setText(tr("当前的命令是：") + tr(dstrText.data()));
	this->ui.lab_config->setText(tr("\t“确定”执行命令，“取消”不执行命令"));
	m_cmdType = pElements->pProperties->vValue.ulVal;
	m_cmdLine = QString(dstrText.data());
}

void speech_recognize::cmdConfig(SPPHRASE *pElements)
{
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Single, SPRS_ACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Group, SPRS_ACTIVE);

	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Config, SPRS_INACTIVE);

	switch( pElements->pProperties->vValue.ulVal )
	{
	case CMDYes:
		m_SREngine.m_pVoice->Speak(L"命令已执行", SPF_ASYNC, NULL);
		this->ui.lab_cmd->setText(tr("请说出你的命令"));
		this->ui.lab_config->setText(tr(""));
		//TODO: 命令的入口
		switch(m_cmdType)
		{
		case CMDClose:
			//关闭程序
			this->close();
			break;
		case CMDShutdown:
			//system("shutdown -s -f"); //强制关机
			system("shutdown -s");	//关机
			break;
		case CMDShowdesktop:
			//显示桌面
			system("%USERPROFILE%\\AppData\\Roaming\\Microsoft\\\"Internet Explorer\"\\\"Quick Launch\"\\\"shows desktop.lnk\"");
			m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Options, SPRS_INACTIVE);
			break;
		case CMDOpenDesktop:
			m_cmdDir = QDir::home();
			m_cmdDir.cd("desktop");
			m_cmdLine = m_cmdDir.path();
			m_index = 2;
			addListText(m_cmdDir.entryList(), m_index);
			break;
		case CMDOne:
			chooseNumber(this->ui.lab_1, 1);
			break;
		case CMDTwo:
			chooseNumber(this->ui.lab_2, 2);
			break;
		case CMDThree:
			chooseNumber(this->ui.lab_3, 3);
			break;
		case CMDPgup:
			m_index -= 6;
			m_index = m_index > 2? m_index: 2;
			addListText(m_cmdDir.entryList(), m_index);
			break;
		case CMDPgdn:
			//m_index += 3;
			addListText(m_cmdDir.entryList(), m_index);
			break;
		case CMDBack:
			if(m_cmdDir.cdUp())
			{
				m_index = 2;
				addListText(m_cmdDir.entryList(), m_index);
			}
			else
			{
				this->ui.lab_config->setText(tr(""));
				this->ui.lab_caption->setText(tr(""));
				this->ui.lab_1->setText(tr(""));
				this->ui.lab_2->setText(tr(""));
				this->ui.lab_3->setText(tr(""));
				this->ui.lab_next->setText(tr(""));
			}

			break;
		}
		break;
	case CMDNo:
		{
			m_SREngine.m_pVoice->Speak(L"命令已取消", SPF_ASYNC, NULL);
			this->ui.lab_cmd->setText(tr("当前的命令是："));
			this->ui.lab_config->setText(tr(""));
		}
		break;
	}
}

void speech_recognize::cmdOptios(SPPHRASE *pElements, std::string dstrText)
{
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Begin, SPRS_INACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_End, SPRS_INACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Single, SPRS_INACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Group, SPRS_INACTIVE);
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Config, SPRS_ACTIVE);

	this->ui.lab_cmd->setText(tr("当前的命令是：") + tr(dstrText.data()));
	this->ui.lab_config->setText(tr("\t“确定”执行命令，“取消”不执行命令"));
	m_cmdType = pElements->pProperties->vValue.ulVal;
}


void speech_recognize::addListText(const QStringList &list, int &index)
{
	this->ui.lab_caption->setText(tr("用（1，2，3）来选择打开以下目录或文件,“返回上层”返回上层"));
	this->ui.lab_next->setText(tr("“下一页”打开下一页，“结束命令”结束当前命令"));
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Options, SPRS_ACTIVE);


	int count = list.count();
	if (index < count)
	{
		while(list[index].indexOf(".") == 0)
		{
			++index;
		}
		this->ui.lab_1->setText(tr("1\t") + QString::fromUtf16((const ushort*)dealString(list[index++]).data()));

		if (index < count)
		{
			this->ui.lab_2->setText(tr("2\t") + QString::fromUtf16((const ushort*)dealString(list[index++]).data()));

			if (index < count)
			{
				this->ui.lab_3->setText(tr("3\t") + QString::fromUtf16((const ushort*)dealString(list[index++]).data()));
			}
		}
	}
}
void speech_recognize::chooseNumber(QLabel* label, int number)
{
	m_SREngine.m_pCmdGram->SetRuleIdState(CMD_Options, SPRS_ACTIVE);
	if (label->text() != "")
	{
		int index = m_index + number -4;
		QFileInfo fileType = m_cmdDir.entryInfoList().at(index);
		if (fileType.isDir())
		{
			m_cmdDir.cd(m_cmdDir.entryList().at(index));
			m_index = 2;
			addListText(m_cmdDir.entryList(), m_index);
		}
		else
		{
			if (fileType.isFile())
			{
				QFile file(m_cmdDir.filePath(m_cmdDir.entryList().at(index)));
				QDesktopServices::openUrl ( QUrl::fromLocalFile(file.fileName()) );
			}
		}

	}
}

QString speech_recognize::dealString(const QString &str)
{
	int index = str.lastIndexOf(".");
	if (index == -1)
		return str;
	return QString(str.data(), index);
}

void speech_recognize::paintEvent( QPaintEvent *event)
{
	QPainter painter(this);
	QImage image("plant.jpg");
	painter.setPen(Qt::red);
	painter.drawImage(QRect(0, 0, this->width(), this->height()), image);
}



