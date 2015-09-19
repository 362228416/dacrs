/*
 * CBlackHalo_tests.h
 *
 *  Created on: 2015-04-24
 *      Author: frank.shi
 */

#ifndef CANONY_TESTS_H
#define CANONY_TESTS_H

#include "CycleTestBase.h"
#include "../test/systestbase.h"
#include "../rpc/rpcclient.h"
#include "tx.h"

using namespace std;
using namespace boost;
using namespace json_spirit;


#define	TX_REGISTER   0x01   //ע���ٲ��˻�
#define TX_MODIFYREGISTER  0x02 // �޸��ٲ���ע����Ϣ
#define	TX_UNREGISTER  0x03 //ע���ٲ��˻�
#define	TX_SEND  0x04 //�ҵ�
#define	TX_CANCEL  0x05 //ȡ���ҵ�
#define	TX_ACCEPT  0x06 //�ӵ�
#define	TX_BUYERCONFIRM  0x07 //���ȷ���ջ�
#define	TX_ARBITRATION  0x08 //�����ٲ�
#define	TX_FINALRESULT  0x09 //�þ����



#define	SEND_TYPE_BUY   0x00   //!<�ҵ� ��
#define	SEND_TYPE_SELL  0x01  //!<�ҵ� ��


typedef struct {
	unsigned char type;            //!<��������
	uint64_t arbiterMoneyX;             //!<�ٲ÷���X
	uint64_t overtimeMoneyYmax;  //!<��ʱδ�о�������⳥����Y
	uint64_t configMoneyZ;              //!<������þ�����Z
	unsigned int  overtimeheightT;  //!<�о�����ʱ��T
	unsigned char  ucLen;           //!<��ע˵������
	char  comment[128];             //!<��ע˵��
	IMPLEMENT_SERIALIZE
	(
			READWRITE(type);
			READWRITE(arbiterMoneyX);
			READWRITE(overtimeMoneyYmax);
			READWRITE(configMoneyZ);
			READWRITE(overtimeheightT);
			READWRITE(ucLen);
			for(int i = 0; i < 128; i++)
			{
				READWRITE(comment[i]);
			}
	)

}TX_REGISTER_CONTRACT;  //!<ע���ٲ��˻�

typedef struct {
	unsigned char type;            //!<��������
	unsigned char sendType;         //!<�ҵ�����:0 ��  1��
	char arbitationID[6];        //!<�ٲ���ID������6�ֽڵ��˻�ID��
	uint64_t moneyM;                   //!<���׽��
	unsigned int height;           //!<ÿ�����׻��ڵĳ�ʱ�߶�

	unsigned char ucLen;            //!<��ע˵������
	char  comment[128];             //!<��ע˵��
	IMPLEMENT_SERIALIZE
	(
			READWRITE(type);
			READWRITE(sendType);
			for(int i = 0; i < 6; i++)
			{
				READWRITE(arbitationID[i]);
			}
			READWRITE(moneyM);
			READWRITE(height);
			READWRITE(ucLen);
			for(int i = 0; i < 128; i++)
			{
				READWRITE(comment[i]);
			}
	)
}TX_SNED_CONTRACT;                  //!<�ҵ�

typedef struct {
	unsigned char type;            //!<��������
	unsigned char txhash[32];       //!<�ҵ��Ľ���hash
	IMPLEMENT_SERIALIZE
	(
			READWRITE(type);
			for(int i = 0; i < 32; i++)
			{
				READWRITE(txhash[i]);
			}
	)
} TX_CONTRACT;


typedef struct {
	unsigned char type;            //!<��������
	unsigned char sendhash[32];       //!<�ҵ��Ľ���hash
	unsigned char accepthash[32];    //!<�ӵ��Ľ���hash
	char 	winner[6];      	//!<Ӯ��ID������6�ֽڵ��˻�ID��
	uint64_t winnerMoney;            //!<���ջ�õĽ��
//	char  loser[6];       //!<���ID������6�ֽڵ��˻�ID��
//	uint64_t loserMoney;            //!<���ջ�õĽ��
	IMPLEMENT_SERIALIZE
	(
		READWRITE(type);
		for(int i = 0; i < 32; i++)
		{
			READWRITE(sendhash[i]);
		}
		for(int i = 0; i < 32; i++)
		{
			READWRITE(accepthash[i]);
		}
		for(int i = 0; i < 6; i++)
		{
			READWRITE(winner[i]);
		}
		READWRITE(winnerMoney);
//		for(int i = 0; i < 6; i++)
//		{
//			READWRITE(loser[i]);
//		}
//		READWRITE(loserMoney);
	)
}TX_FINALRESULT_CONTRACT;        //!<���ղþ�


class CGuaranteeTest: public CycleTestBase {
	int nNum;
	int nStep;
	string strTxHash;
	string strAppRegId;//ע��Ӧ�ú��Id
public:
	CGuaranteeTest();
	~CGuaranteeTest(){};
	virtual TEST_STATE Run() ;
	bool RegistScript();

	bool Register(void);
	bool UnRegister(void);
	bool SendStartTrade(void);
	bool SendCancelTrade(void);
	bool AcceptTrade(void);
	bool BuyerConfirm(void);
    bool Arbitration(void);
    bool RunFinalResult(void);

};

#endif /* CANONY_TESTS_H */
