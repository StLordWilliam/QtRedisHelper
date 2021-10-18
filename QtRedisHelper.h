#pragma once

#include <QObject>
#include "QtRedis\qtredis.h"

const QString g_strBloomFliter = "spiderman";

class QtRedisHelper : public QObject
{
	Q_OBJECT

public:
	QtRedisHelper(QObject *parent = 0);
	~QtRedisHelper();

	bool InitRedisClient(QString strHost, int nPort);
	bool InitConnect();

	//����Ƿ��Ѿ�����
	bool CheckKeyIsExistedFromRedis(QString strKey, bool &bIsExist);
	bool AppendKeyToRedis(QString strKey, bool &IsSuccess);

	//����Ƿ��Ѿ�����
	bool CheckKeyListIsExistedFromRedis(QStringList lstKey, QMap<QString,bool> &mapKey2IsExist);
	bool AppendKeyListToRedis(QStringList lstKey, bool &IsSuccess);


signals:

public slots :
	void slotMessage(QtRedis::Reply);

private:
	QtRedis *m_pRedis;
};
