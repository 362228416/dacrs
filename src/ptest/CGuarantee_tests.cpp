/*
 * CAnony_tests.cpp
 *
 *  Created on: 2015-04-24
 *      Author: frank
 */

#include "CGuarantee_tests.h"
#include "CycleTestManger.h"
#include <boost/assign/list_of.hpp>
#include "json/json_spirit_utils.h"
#include "json/json_spirit_value.h"
#include "json/json_spirit_reader.h"
using namespace std;
using namespace boost;
using namespace boost::assign;
using namespace json_spirit;


/*
 * ���Ե�ַ�����
 * */
#if 0
{
    "Address" : "dcmCbKbAfKrofNz35MSFupxrx4Uwn3vgjL",
    "KeyID" : "000f047f75144705b0c7f4eb30d205cd66f4599a",
    "RegID" : "1826-1437",
    "PublicKey" : "02bbb24c80a808cb6eb13de90c1dca99196bfce02bcf32812b7c4357a368877c68",
    "MinerPKey" : "02d546cc51b22621b093f08c679102b9b8ca3f1a07ea1d751de3f67c10670e635b",
    "Balance" : 129999825142,
    "CoinDays" : 0,
    "UpdateHeight" : 45433,
    "CurCoinDays" : 1457,
    "postion" : "inblock"
}
{
    "Address" : "dcmWdcfxEjRXUHk8LpygtgDHTpixoo3kbd",
    "KeyID" : "001e1384420cad01b0cd364ef064852b1bf3fd96",
    "RegID" : "1826-1285",
    "PublicKey" : "03d765b0f2bae7f6f61350b17bce5e57445cc286cada56d9c61987db5cbd533c43",
    "MinerPKey" : "025cae56b5faf1042f2d6610cde892f0cb1178282fb7b345b78611ccee4feab128",
    "Balance" : 128999717658,
    "CoinDays" : 0,
    "UpdateHeight" : 46859,
    "CurCoinDays" : 169,
    "postion" : "inblock"
}
{
    "Address" : "dcnGLkGud6c5bZJSUghzxvCqV45SJEwRcH",
    "KeyID" : "00429013e06bbcdc0529dd5b1117ddf4630544ad",
    "RegID" : "1826-1081",
    "PublicKey" : "02b509a4240ae08118ff2336981301cb2baf6207faf86aa1731a9ce8443e72f7f0",
    "MinerPKey" : "0394b395e1ef08f9c6e71eb2ecd70fe511f7ec0c0fe5a96c139fd4589b8f8a671c",
    "Balance" : 126999676405,
    "CoinDays" : 0,
    "UpdateHeight" : 45130,
    "CurCoinDays" : 1691,
    "postion" : "inblock"
}
#endif



#define ADDR_SEND_A        "dcmCbKbAfKrofNz35MSFupxrx4Uwn3vgjL"  //�ҵ���A  220700009d05 ���
// 1826-1437

#define ADDR_ACCEPT_B      "dcmWdcfxEjRXUHk8LpygtgDHTpixoo3kbd"  //�ӵ���B  220700000505 ����
// 1826-1285

#define ADDR_ARBITRATION_C "dcnGLkGud6c5bZJSUghzxvCqV45SJEwRcH"  //�ٲ���C   220700003904
// 1826-1081

//const static char DeveloperAccount[6]="\x00\x00\x00\x00\x14\x00";//!<�����ߵ�ַID
#define ADDR_DeveloperAccount   "dk2NNjraSvquD9b4SQbysVRQeFikA55HLi"   //RegID = "0-20"

#define ID_strAppRegId  "47244-1"    //�ű�Ӧ��ID ����
//#define HASH_sendhash     "7de1faafc2c9f14be5294f5f2b1082eaf92c7d66da5d42be1016e0988143318d"  //�ҵ�����hash ����
static const unsigned char HASH_sendhash[] ={
		0x2e,0xd9,0x28,0xc5,0x84,0xde,0x82,0x88,
		0xa2,0x05,0x4a,0xb4,0xcc,0xd6,0x89,0x5f,
		0xc8,0xfd,0x57,0xfe,0x71,0xba,0xbf,0x44,
		0x84,0x03,0x09,0x37,0x9e,0x50,0x1f,0x71

};
static const unsigned char HASH_accepthash[] ={
		0x4f,0xe5,0xcd,0xc0,0xc4,0x23,0x1e,0x5f,
		0xa5,0x27,0x22,0x88,0x94,0xfe,0x45,0x47,
		0x29,0xf0,0xbd,0x5a,0x54,0x98,0xe2,0x72,
		0x10,0x81,0xc6,0xd9,0x17,0x94,0x5c,0x1a
};


//!<�ٲ���C��������Ϣ
#define ARBITER_arbiterMoneyX      (2 * 1000000)      //!<�ٲ÷���X
#define ARBITER_overtimeMoneyYmax  (1 * 100000000) //!<��ʱδ�о�������⳥����Y
#define ARBITER_configMoneyZ       (1 * 1000000)       //!<������þ�����Z
#define ARBITER_overtimeheightT    (1 * 1440)  //!<�о�����ʱ��T

//!<�ҵ��ߵ�������Ϣ
#define SEND_moneyM     (2 * 100000000)    //!<���׽��
#define SEND_height     (2 * 1440)       //!<ÿ�����׻��ڵĳ�ʱ�߶�

//#define  ARBITER_winnerMoney  (1 * 100000000) //!<�þ���Ӯ�ҷ�����
#define  ARBITER_winnerMoney  (198000000) //!<�þ���Ӯ�ҷ�����

CGuaranteeTest::CGuaranteeTest():nNum(0), nStep(0), strTxHash(""), strAppRegId(ID_strAppRegId) {

}

TEST_STATE CGuaranteeTest::Run(){


	cout<<"CGuaranteeTest run start"<<endl;
#if 0
    // ע��ipo�ű�
	if(!RegistScript()){
		cout<<"CGuaranteeTest RegistScript err"<<endl;
		return end_state;
	}

	/// �ȴ�ipo�ű���ȷ�ϵ�block��
	while(true)
	{
		if(WaitComfirmed(strTxHash, strAppRegId)) {
					break;
				}
	}
#endif

#if 0
	/// ��ÿ����ַתһ���Ľ��
	int64_t money = COIN;
//	size_t t_num = sizeof(arrayData) / sizeof(arrayData[0]);
//	BOOST_CHECK(t_num <= max_user);         //��ֹԽ��
//	for(int i=0;i <t_num;i++)
	{
		string des =strprintf("%s", ADDR_ARBITRATION_C);//userarray[i].address);
		basetest.CreateNormalTx(des,money);

//		des =strprintf("%s", ADDR_ACCEPT_B);//userarray[i].address);
//		basetest.CreateNormalTx(des,money);
//
//		des =strprintf("%s", ADDR_SEND_A);//userarray[i].address);
//		basetest.CreateNormalTx(des,money);
	}

	 cout<<"end mempool"<<endl;
	while(true)
	{
		if(basetest.IsMemoryPoolEmpty())
			break;
		MilliSleep(100);
	}
#endif

//	Recharge();
//	Withdraw();
//	WithdrawSomemoney();

//	Register(TX_REGISTER);
//	Register(TX_MODIFYREGISTER);
//	UnRegister();
//	SendStartTrade();
//	SendCancelTrade();
//	AcceptTrade();
//	BuyerConfirm();
//	Arbitration();
	RunFinalResult();
	cout<<"CGuaranteeTest run end"<<endl;
	return end_state;
}


bool CGuaranteeTest::RegistScript(){

	const char* pKey[] = { "cNcJkU44oG3etbWoEvY46i5qWPeE8jVb7K44keXxEQxsXUZ85MKU",
			"cNcJkU44oG3etbWoEvY46i5qWPeE8jVb7K44keXxEQxsXUZ85MKU"};
	int nCount = sizeof(pKey) / sizeof(char*);
	basetest.ImportWalletKey(pKey, nCount);

	string strFileName("guarantee.bin");
	int nFee = basetest.GetRandomFee();
	int nCurHight;
	basetest.GetBlockHeight(nCurHight);
	string regAddr="dk2NNjraSvquD9b4SQbysVRQeFikA55HLi";

	//reg anony app
	Value regscript = basetest.RegisterAppTx(regAddr, strFileName, nCurHight, nFee + 1 *COIN);// + 20 *COIN
	if(basetest.GetHashFromCreatedTx(regscript, strTxHash)){

		return true;
	}
	return false;
}


bool CGuaranteeTest::Recharge()
{
   cout<<"Recharge start"<<endl;

   APPACC senddata;
	memset(&senddata,0,sizeof(senddata));
	senddata.systype = 0xff;
	senddata.type = 0x02;

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 20 * COIN;
//	uint64_t nTempSend = (10000 + 5) * COIN;

    cout<<"Recharge data:"<<sendcontract<<endl;
	Value  retValue= basetest.CreateContractTx(strAppRegId,ADDR_ACCEPT_B,sendcontract,0,0,nTempSend);
//   ADDR_DeveloperAccount  //RegID = "0-20"
//   ADDR_SEND_A   //	Id = "1826-1437";
//   ADDR_ACCEPT_B   //	Id = "1826-1285";
//	ADDR_ARBITRATION_C  Id = "1826-1081"
	if (basetest.GetHashFromCreatedTx(retValue, strTxHash)) {
		nStep++;
	}else{
		cout<<"Recharge err end"<<endl;
		return false;
	}

#if 1
	/// �ȴ����ױ�ȷ�ϵ�block��
	while(true)
	{
		if(WaitComfirmed(strTxHash, strAppRegId)) {
					break;
				}
	}
#endif
    cout<<"Recharge success end"<<endl;
	return false;
}

bool CGuaranteeTest::Withdraw()
{
   cout<<"Withdraw start"<<endl;

   APPACC senddata;
	memset(&senddata,0,sizeof(senddata));
	senddata.systype = 0xff;
	senddata.type = 0x01;
	senddata.typeaddr = TX_REGID;

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;
    cout<<"Withdraw data:"<<sendcontract<<endl;
	Value  retValue= basetest.CreateContractTx(strAppRegId,ADDR_DeveloperAccount,sendcontract,0,0,nTempSend);
           //ADDR_DeveloperAccount  ADDR_ARBITRATION_C
	if (basetest.GetHashFromCreatedTx(retValue, strTxHash)) {
		nStep++;

	}else{
	    cout<<"Withdraw err end"<<endl;
	    return false;
    }

#if 1
/// �ȴ����ױ�ȷ�ϵ�block��
while(true)
{
	if(WaitComfirmed(strTxHash, strAppRegId)) {
				break;
			}
}
#endif

	 cout<<"Withdraw success end"<<endl;
	return false;
}

bool CGuaranteeTest::WithdrawSomemoney()
{
   cout<<"WithdrawwSomemoney start"<<endl;

   APPACC_money senddata;
	memset(&senddata,0,sizeof(senddata));
	senddata.systype = 0xff;
	senddata.type = 0x03;
	senddata.typeaddr = TX_REGID;
	senddata.money = 10000 * COIN;
//	senddata.money = COIN;

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;
    cout<<"WithdrawwSomemoney data:"<<sendcontract<<endl;
	Value  retValue= basetest.CreateContractTx(strAppRegId,ADDR_DeveloperAccount,sendcontract,0,0,nTempSend);
           //ADDR_DeveloperAccount  ADDR_ARBITRATION_C
	if (basetest.GetHashFromCreatedTx(retValue, strTxHash)) {
		nStep++;

	}else{
	    cout<<"WithdrawwSomemoney err end"<<endl;
	    return false;
    }

#if 1
	/// �ȴ����ױ�ȷ�ϵ�block��
	while(true)
	{
		if(WaitComfirmed(strTxHash, strAppRegId)) {
				break;
			}
	}
#endif
	 cout<<"WithdrawwSomemoney success end"<<endl;
	return false;
}

bool CGuaranteeTest::Register(unsigned char type)
{
	TX_REGISTER_CONTRACT senddata;
	memset(&senddata,0,sizeof(senddata));
    if(type == TX_REGISTER)
    {
    	senddata.type = TX_REGISTER;
    	 cout<<"TX_REGISTER start"<<endl;
    }else if(type == TX_MODIFYREGISTER)
    {
    	senddata.type = TX_MODIFYREGISTER;
    	 cout<<"TX_MODIFYREGISTER start"<<endl;
    }
    else{
    	cout<<"Register input err"<<endl;
    	return false;
    }
	senddata.arbiterMoneyX = ARBITER_arbiterMoneyX;
	senddata.overtimeMoneyYmax = ARBITER_overtimeMoneyYmax;
	senddata.configMoneyZ = ARBITER_configMoneyZ;//0x8234567812345678
	senddata.overtimeheightT = ARBITER_overtimeheightT;
    strcpy(senddata.comment,"��ϵ�绰:13418656754");

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;
    cout<<"Register data:"<<sendcontract<<endl;
    cout<<"Register strAppRegId:"<<strAppRegId.c_str()<<endl;
	Value  retValue= basetest.CreateContractTx(strAppRegId,ADDR_ARBITRATION_C,sendcontract,0,0,nTempSend);

	if (basetest.GetHashFromCreatedTx(retValue, strTxHash)) {
		nStep++;

	}else
	{
	    cout<<"Register fail"<<endl;
	    return false;
	}
#if 1
	/// �ȴ����ױ�ȷ�ϵ�block��
	while(true)
	{
		if(WaitComfirmed(strTxHash, strAppRegId)) {
					break;
				}
	}
#endif
	 cout<<"Register success"<<endl;
	return true;
}

bool CGuaranteeTest::UnRegister()
{
   cout<<"UnRegister start"<<endl;

   unsigned char senddata = TX_UNREGISTER;

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;
	cout<<"UnRegister data:"<<sendcontract.c_str()<<endl;
	Value  retValue= basetest.CreateContractTx(strAppRegId,ADDR_ARBITRATION_C,sendcontract,0,0,nTempSend);

	if (basetest.GetHashFromCreatedTx(retValue, strTxHash)) {
		nStep++;

	}
	else{
	    cout<<"UnRegister fail"<<endl;
	    return false;
	}
#if 1
	/// �ȴ����ױ�ȷ�ϵ�block��
	while(true)
	{
		if(WaitComfirmed(strTxHash, strAppRegId)) {
					break;
				}
	}
#endif

	cout<<"UnRegister success end"<<endl;
	return false;
}

bool CGuaranteeTest::SendStartTrade()
{
	cout<<"SendStartTrade start"<<endl;

	TX_SNED_CONTRACT senddata;
	memset(&senddata,0,sizeof(senddata));
	senddata.type = TX_SEND;
//	senddata.sendType = SEND_TYPE_BUY;   //���޸� �ҵ�����
	senddata.sendType = SEND_TYPE_SELL;   //���޸� �ҵ�����

//	string arbitationID = "47046-1";
	unsigned int height = 1826;   //����   �ٲ���ID arbiterAddr_C
	unsigned short index = 1081;

	memcpy(&senddata.arbitationID[0],&height,4);
    memcpy(&senddata.arbitationID[4],&index,2);
	senddata.moneyM = SEND_moneyM;
	senddata.height = SEND_height;
	strcpy(senddata.goods,"С��3�ֻ�");
	strcpy(senddata.comment,"1������.��ϵ�绰:13418656754");

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;
	cout<<"SendStartTrade data:"<<sendcontract.c_str()<<endl;
	Value sendret;
    if(senddata.sendType == SEND_TYPE_BUY)
    {
    	sendret= basetest.CreateContractTx(strAppRegId,ADDR_SEND_A,sendcontract,0,0,nTempSend); // ����д
    }else{
    	sendret= basetest.CreateContractTx(strAppRegId,ADDR_ACCEPT_B,sendcontract,0,0,nTempSend); // ����д
    }

	if (basetest.GetHashFromCreatedTx(sendret, strTxHash)) {
		nStep++;

	}
	else{
		cout<<"SendStartTrade err end"<<endl;
		return false;
	}
#if 1
		/// �ȴ����ױ�ȷ�ϵ�block��
		while(true)
		{
			if(WaitComfirmed(strTxHash, strAppRegId)) {
						break;
					}
		}
#endif
		cout<<"SendStartTrade success end"<<endl;
	return true;
}

bool CGuaranteeTest::SendCancelTrade()
{
   cout<<"SendCancelTrade start"<<endl;

	TX_CONTRACT senddata;
	memset(&senddata,0,sizeof(senddata));
	senddata.type = TX_CANCEL;
//	memcpy(senddata.txhash, uint256S(HASH_sendhash).begin(), sizeof(senddata.txhash)); //�����HASH
	memcpy(senddata.txhash,HASH_sendhash, sizeof(senddata.txhash)); //�����HASH

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;
	cout<<"SendCancelTrade data:"<<sendcontract.c_str()<<endl;
//	Value  sendret= basetest.CreateContractTx(strAppRegId,ADDR_SEND_A,sendcontract,0,0,nTempSend); // ȡ������
    Value  sendret= basetest.CreateContractTx(strAppRegId,ADDR_ACCEPT_B,sendcontract,0,0,nTempSend); //ȡ��������
	if (basetest.GetHashFromCreatedTx(sendret, strTxHash)) {
		nStep++;
	}
	else{
		cout<<"SendCancelTrade err end"<<endl;
		return false;
	}
#if 1
		/// �ȴ����ױ�ȷ�ϵ�block��
		while(true)
		{
			if(WaitComfirmed(strTxHash, strAppRegId)) {
						break;
					}
		}
#endif
	cout<<"SendCancelTrade success end"<<endl;
	return true;
}

bool CGuaranteeTest::AcceptTrade()
{
   cout<<"AcceptTrade start"<<endl;

	TX_CONTRACT senddata;
	memset(&senddata,0,sizeof(senddata));
	senddata.type = TX_ACCEPT;
	memcpy(senddata.txhash,HASH_sendhash, sizeof(senddata.txhash)); //�����HASH

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;
	cout<<"AcceptTrade data:"<<sendcontract.c_str()<<endl;
//	Value  sendret= basetest.CreateContractTx(strAppRegId,ADDR_ACCEPT_B,sendcontract,0,0,nTempSend);//���ҽӵ�
	Value  sendret= basetest.CreateContractTx(strAppRegId,ADDR_SEND_A,sendcontract,0,0,nTempSend);//��ҽӵ�

	if (basetest.GetHashFromCreatedTx(sendret, strTxHash)) {
		nStep++;
	}
	else{
		cout<<"AcceptTrade err end"<<endl;
		return false;
	}
#if 1
		/// �ȴ����ױ�ȷ�ϵ�block��
		while(true)
		{
			if(WaitComfirmed(strTxHash, strAppRegId)) {
						break;
					}
		}
#endif
	cout<<"AcceptTrade success end"<<endl;
	return true;
}
bool CGuaranteeTest::BuyerConfirm()
{
   cout<<"BuyerConfirm start"<<endl;

	TX_CONTRACT senddata;
	memset(&senddata,0,sizeof(senddata));
	senddata.type = TX_BUYERCONFIRM;
	memcpy(senddata.txhash,HASH_sendhash, sizeof(senddata.txhash)); //�����HASH

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;
	cout<<"BuyerConfirm data:"<<sendcontract.c_str()<<endl;
	Value  sendret= basetest.CreateContractTx(strAppRegId,ADDR_SEND_A,sendcontract,0,0,nTempSend); //����д

	if (basetest.GetHashFromCreatedTx(sendret, strTxHash)) {
		nStep++;
	}
	else{
		cout<<"BuyerConfirm err end"<<endl;
		return false;
	}
#if 1
		/// �ȴ����ױ�ȷ�ϵ�block��
		while(true)
		{
			if(WaitComfirmed(strTxHash, strAppRegId)) {
						break;
					}
		}
#endif
	cout<<"BuyerConfirm success end"<<endl;
	return true;
}


bool CGuaranteeTest::Arbitration()
{
   cout<<"Arbitration start"<<endl;

   TX_Arbitration senddata;
	memset(&senddata,0,sizeof(senddata));
	senddata.type = TX_ARBITRATION;
	memcpy(senddata.txhash,HASH_sendhash, sizeof(senddata.txhash)); //�����HASH
//	string arbitationID = "47046-1";
	unsigned int height = 1826;   //����   �ٲ���ID arbiterAddr_C
	unsigned short index = 1081;

	memcpy(&senddata.arbitationID[0],&height,4);
	memcpy(&senddata.arbitationID[4],&index,2);


	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;
	cout<<"Arbitration data:"<<sendcontract.c_str()<<endl;
	Value  sendret= basetest.CreateContractTx(strAppRegId,ADDR_ACCEPT_B,sendcontract,0,0,nTempSend);// ����д ADDR_ARBITRATION_C

	if (basetest.GetHashFromCreatedTx(sendret, strTxHash)) {
		nStep++;

	}
	else{
		cout<<"Arbitration err end"<<endl;
		return false;
	}
#if 1
		/// �ȴ����ױ�ȷ�ϵ�block��
		while(true)
		{
			if(WaitComfirmed(strTxHash, strAppRegId)) {
						break;
					}
		}
#endif
	cout<<"Arbitration success end"<<endl;
	return true;
}
bool CGuaranteeTest::RunFinalResult()
{
   cout<<"RunFinalResult start"<<endl;

	TX_FINALRESULT_CONTRACT senddata;
	memset(&senddata,0,sizeof(senddata));
	senddata.type = TX_FINALRESULT;
	memcpy(senddata.arbitHash, HASH_sendhash, sizeof(senddata.arbitHash)); //�����HASH
//	memcpy(senddata.accepthash, HASH_accepthash, sizeof(senddata.accepthash)); //�����HASH

//	string buyId = "1826-1437";
	unsigned int height = 1826;   //����   Ӯ�� ID ��� Ӯ
	unsigned short index = 1285;  //1437

    memcpy(&senddata.winner[0],&height,4);
    memcpy(&senddata.winner[4],&index,2);
	senddata.winnerMoney = ARBITER_winnerMoney;

//	string sellerId = "1826-1285";
	height = 1826;  //����   ���ID
	index = 1437;  //1285
    memcpy(&senddata.loser[0],&height,4);
    memcpy(&senddata.loser[4],&index,2);
	senddata.loserMoney = SEND_moneyM - ARBITER_winnerMoney;  //  ���׽��M - Ӯ�ҷ����Ǯ  ����д

	CDataStream scriptData(SER_DISK, CLIENT_VERSION);
	scriptData << senddata;
	string sendcontract = HexStr(scriptData);
	uint64_t nTempSend = 0;
	cout<<"RunFinalResult data:"<<sendcontract.c_str()<<endl;
	Value  sendret= basetest.CreateContractTx(strAppRegId,ADDR_ARBITRATION_C,sendcontract,0,0,nTempSend);//ADDR_ARBITRATION_C

	if (basetest.GetHashFromCreatedTx(sendret, strTxHash)) {
		nStep++;

	}
	else{
		cout<<"RunFinalResult err end"<<endl;
		return false;
	}
#if 1
		/// �ȴ����ױ�ȷ�ϵ�block��
		while(true)
		{
			if(WaitComfirmed(strTxHash, strAppRegId)) {
						break;
					}
		}
#endif
	cout<<"RunFinalResult success end"<<endl;
	return true;
}

BOOST_FIXTURE_TEST_SUITE(CGuaranteeTxTest,CGuaranteeTest)

BOOST_FIXTURE_TEST_CASE(Test,CGuaranteeTest)
{
	Run();
}
BOOST_AUTO_TEST_SUITE_END()

