#include "QtRedisHelper.h"
#include "QDateTime"
#include "QThread"

QtRedisHelper::QtRedisHelper(QObject *parent) : QObject(parent)
{
	m_pRedis = nullptr;
}

QtRedisHelper::~QtRedisHelper()
{
	if (m_pRedis != nullptr)
	{
		delete m_pRedis;
		m_pRedis = nullptr;
	}
}


bool QtRedisHelper::InitRedisClient(QString strHost, int nPort)
{
	if (m_pRedis == nullptr)
	{
		m_pRedis = new QtRedis(strHost, nPort);
		if (m_pRedis == nullptr)
		{
			return false;
		}
	}

	return true;
}

bool QtRedisHelper::InitConnect()
{
	bool bRet = connect(m_pRedis, SIGNAL(returnData(QtRedis::Reply)), this, SLOT(slotMessage(QtRedis::Reply)));
	if (!bRet)
	{
		return false;
	}

	if (!m_pRedis->openConnection())
	{
		qWarning() << "QtRedisHelper::InitConnect m_pRedis->openConnection()  Could not connect to server...";
		exit(0);

		return false;
	}

	return true;
}


//检测是否已经存在
bool QtRedisHelper::CheckKeyIsExistedFromRedis(QString strKey, bool &bIsExist)
{
	if (m_pRedis  == nullptr)
	{
		return false;
	}
	bool bRet = m_pRedis->bf_exists(strKey, g_strBloomFliter);

	bIsExist = bRet;
	return true;
}

//检测是否已经存在
bool QtRedisHelper::CheckKeyListIsExistedFromRedis(QStringList lstKey, QMap<QString, bool> &mapKey2IsExist)
{
	if (m_pRedis == nullptr)
	{
		return false;
	}
	bool bRet = m_pRedis->bf_mexists(lstKey, g_strBloomFliter, mapKey2IsExist);

	//bIsExist = bRet;

	return true;
}

bool QtRedisHelper::AppendKeyToRedis(QString strKey, bool &IsSuccess)
{
	if (m_pRedis == nullptr)
	{
		return false;
	}
	bool bRet = m_pRedis->bf_add(strKey, g_strBloomFliter);

	IsSuccess = bRet;
	return true;
}

bool QtRedisHelper::AppendKeyListToRedis(QStringList lstKey, bool &IsSuccess)
{
	if (m_pRedis == nullptr)
	{
		return false;
	}
	QMap<QString, bool> mapKey2IsSuccess;

	int nCount = 0;
	bool bRet = false;
	while (true)
	{
		bRet = m_pRedis->bf_madd(lstKey, g_strBloomFliter, mapKey2IsSuccess);
		if (bRet == true)
		{
			break;
		}

		if (nCount > 5)
		{
			qWarning() << "m_pRedis->bf_madd failed!";
			break;;
		}

		QThread::msleep(5);
		nCount = nCount +1 ;
	}
	
	IsSuccess = bRet;
	return true;
}


void QtRedisHelper::slotMessage(QtRedis::Reply reply)
{
	QTime time;
	qDebug() << time.currentTime();
	qDebug() << "Channel:" << reply.channel << "Pattern:" << reply.pattern;
	qDebug() << reply.value.toString();
}
