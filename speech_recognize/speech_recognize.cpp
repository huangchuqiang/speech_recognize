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

	this->setWindowTitle(tr("������ʶ�����"));
	this->setFixedWidth(this->width());
	this->setFixedHeight(this->height());

	m_SREngine.InitializeSapi((HWND)this->winId(), WM_RECORD_INFO);
	m_SREngine.LoadCmdFromFile("CmdCtrl.xml");
	m_SREngine.SetRuleState(NULL,NULL,TRUE);

	m_SREngine.m_pVoice->Speak(L"����һ��������ʶ�����", SPF_ASYNC, NULL);

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
	this->ui.lab_title->setText(tr("��ӭʹ�ü�����ʶ�����"));
	this->ui.lab_cmd->setText(tr("˵����ʼʶ�𡱽�������ģʽ"));
	this->ui.lab_config->setText(tr(""));
	this->ui.lab_caption->setText(tr(""));
	this->ui.lab_1->setText(tr(""));
	this->ui.lab_2->setText(tr(""));
	this->ui.lab_3->setText(tr(""));
	this->ui.lab_next->setText(tr(""));
	m_toolLabel.setText(tr("��ǰģʽ������ģʽ"));
}


void speech_recognize::recordEvent()
{
	USES_CONVERSION;
	CSpEvent event;

	HRESULT hr = S_OK;
	if(m_SREngine.m_pRecoCtxt)
	{
		while( S_OK==event.GetFrom(m_SREngine.m_pRecoCtxt) )//�ȴ������������ӿڽ���
		{
			switch(event.eEventId)
			{
			case SPEI_FALSE_RECOGNITION: //����ʶ��
				break;

			case SPEI_HYPOTHESIS: //��ʶ��
			case SPEI_RECOGNITION:  //��ȷʶ��
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
	m_toolLabel.setText(tr("��ǰģʽ������ģʽ"));
	this->ui.lab_cmd->setText(tr("��˵���������"));
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

	this->ui.lab_cmd->setText(tr("��ǰ�������ǣ�") + tr(dstrText.data()));
	this->ui.lab_config->setText(tr("\t��ȷ����ִ�������ȡ������ִ������"));
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
		m_SREngine.m_pVoice->Speak(L"������ִ��", SPF_ASYNC, NULL);
		this->ui.lab_cmd->setText(tr("��˵���������"));
		this->ui.lab_config->setText(tr(""));
		//TODO: ��������
		switch(m_cmdType)
		{
		case CMDClose:
			//�رճ���
			this->close();
			break;
		case CMDShutdown:
			//system("shutdown -s -f"); //ǿ�ƹػ�
			system("shutdown -s");	//�ػ�
			break;
		case CMDShowdesktop:
			//��ʾ����
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
			m_SREngine.m_pVoice->Speak(L"������ȡ��", SPF_ASYNC, NULL);
			this->ui.lab_cmd->setText(tr("��ǰ�������ǣ�"));
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

	this->ui.lab_cmd->setText(tr("��ǰ�������ǣ�") + tr(dstrText.data()));
	this->ui.lab_config->setText(tr("\t��ȷ����ִ�������ȡ������ִ������"));
	m_cmdType = pElements->pProperties->vValue.ulVal;
}


void speech_recognize::addListText(const QStringList &list, int &index)
{
	this->ui.lab_caption->setText(tr("�ã�1��2��3����ѡ�������Ŀ¼���ļ�,�������ϲ㡱�����ϲ�"));
	this->ui.lab_next->setText(tr("����һҳ������һҳ�����������������ǰ����"));
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



