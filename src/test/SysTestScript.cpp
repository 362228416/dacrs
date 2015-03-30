#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "init.h"
#include "miner.h"
#include "uint256.h"
#include "util.h"
#include <boost/test/unit_test.hpp>
#include "rpc/rpcclient.h"
#include "tx.h"
#include "wallet/wallet.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "vm/script.h"
#include "rpc/rpcserver.h"
#include "noui.h"
#include "ui_interface.h"
#include "SysTestBase.h"
#include <boost/algorithm/string/predicate.hpp>
#include "json/json_spirit_writer_template.h"
#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_reader.h"
#include "json/json_spirit_writer.h"
#include "json/json_spirit_value.h"
#include "json/json_spirit_stream_reader.h"
#include "tx.h"
using namespace std;
using namespace boost;
std::string TxHash("");

class CSysScriptTest:public SysTestBase
{
public:
	CSysScriptTest() {
		StartServer();
	}

	~CSysScriptTest(){
		StopServer();
	}
private:
	void StartServer() {

	}

	void StopServer() {
	}
public:
	uint64_t GetValue(Value val,string compare)
	{
		if (val.type() != obj_type)
		{
			return 0;
		}
		json_spirit::Value::Object obj=  val.get_obj();
		string ret;
		for(size_t i = 0; i < obj.size(); ++i)
		{
			const json_spirit::Pair& pair = obj[i];
			const std::string& str_name = pair.name_;
			const json_spirit::Value& val_val = pair.value_;

			if(str_name.compare(compare) == 0)
			{
				return val_val.get_int64();
			}

			if(compare == "value")
			{
				if(str_name =="FreedomFund")
					{
						json_spirit::Value::Array narray = val_val.get_array();
						if(narray.size() == 0)
							return false;
						json_spirit::Value::Object obj1 = narray[0].get_obj();
						for(size_t j = 0; j < obj1.size(); ++j)
						{
							const json_spirit::Pair& pair = obj1[j];
							const std::string& str_name = pair.name_;
							const json_spirit::Value& val_val = pair.value_;
							if(str_name == "value")
							{
								return val_val.get_int64();
							}
						}

					}
			}



		}
		return 0;
	}

	void CheckSdk()
	{
		string param ="01";
		Value resut =CreateContractTx("010000000100", "n4muwAThwzWvuLUh74nL3KYwujhihke1Kb", param,10);
		BOOST_CHECK(GetHashFromCreatedTx(resut,TxHash));
		BOOST_CHECK(GenerateOneBlock());
		uint256 hash(TxHash.c_str());
		param ="02";
		param += HexStr(hash);
		string temp;
		resut =CreateContractTx("010000000100", "n4muwAThwzWvuLUh74nL3KYwujhihke1Kb", param,10);
		BOOST_CHECK(GetHashFromCreatedTx(resut,temp));
		BOOST_CHECK(GenerateOneBlock());

		param ="03";
		resut =CreateContractTx("010000000100", "n4muwAThwzWvuLUh74nL3KYwujhihke1Kb", param,10);
		BOOST_CHECK(GetHashFromCreatedTx(resut,temp));
		BOOST_CHECK(GenerateOneBlock());

		param ="05";
		param += HexStr(hash);

		resut =CreateContractTx("010000000100", "n4muwAThwzWvuLUh74nL3KYwujhihke1Kb", param,10,10000000);
		BOOST_CHECK(GetHashFromCreatedTx(resut,temp));
		BOOST_CHECK(GenerateOneBlock());
	}

	string CreateRegScript(const char* strAddr, const char* sourceCode)
	{
		int nFee = 10000000;
		string strTxHash;
		string strFileName(sourceCode);
		Value valueRes = RegisterScriptTx(strAddr,strFileName , 100, nFee);
		BOOST_CHECK(GetHashFromCreatedTx(valueRes,strTxHash));
		BOOST_CHECK(GenerateOneBlock());
		return strTxHash;
	}
	string CreateContactTx(int param)
	{
		char buffer[3] = {0};
		sprintf(buffer,"%02x",param);
		string temp;
		Value resut =CreateContractTx("010000000100", "5yNhSL7746VV5qWHHDNLkSQ1RYeiheryk9uzQG6C5d", buffer,10);
		BOOST_CHECK(GetHashFromCreatedTx(resut,temp));
		BOOST_CHECK(GenerateOneBlock());
		return temp;
	}

	void CheckRollBack()
	{
		CreateContactTx(6);    //�����ű�����
		//cout<<6<<endl;
		CreateContactTx(7);;   //�޸Ľű�����
		//cout<<7<<endl;
		CreateContactTx(8);    //ɾ���ű�����
//		cout<<8<<endl;
//		DisConnectBlock(1);           //ɾ��1��block
//		mempool.mapTx.clear();
//		CreateContactTx(9);    //checkɾ���Ľű��Ƿ�ָ�
//	//	cout<<9<<endl;
//		DisConnectBlock(2);
//		mempool.mapTx.clear();
//		CreateContactTx(10);    //check�޸ĵĽű������Ƿ�ָ�
////		cout<<10<<endl;
//		DisConnectBlock(2);
//		mempool.mapTx.clear();
//		CreateContactTx(11);   //check�����Ľű������Ƿ�ָ�
	}
	bool CheckScriptid(Value val,string scriptid)
	{
		if (val.type() != obj_type)
		{
			return false;
		}
		const Value& value = val.get_obj();

		json_spirit::Value::Object obj= value.get_obj();
		for(size_t i = 0; i < obj.size(); ++i)
		{
			const json_spirit::Pair& pair = obj[i];
			const std::string& str_name = pair.name_;
			const json_spirit::Value& val_val = pair.value_;
			if(str_name =="PublicKey")
			{
				if(val_val.get_str() != "")
				{
					return false;
				}
			}
			else if(str_name =="KeyID")
			{
				CRegID regId(scriptid);
				CKeyID keyId = Hash160(regId.GetVec6());
				string key = HexStr(keyId.begin(), keyId.end()).c_str();
				if(val_val.get_str() != key)
				{
					return false;
				}
			}

		}
		return true;
	}
	bool CreateScriptAndCheck()
	{
		CreateRegScript("mvVp2PDRuG4JJh6UjkJFzXUC8K5JVbMFFA","unit_test.bin");

		CreateRegScript("mvVp2PDRuG4JJh6UjkJFzXUC8K5JVbMFFA","unit_test.bin");

		string strRegID = "010000000100";

		Value temp1 = GetAccountInfo(strRegID);
		if(!CheckScriptid(temp1,strRegID))
		{
			return false;
		}

		strRegID = "020000000100";
		temp1 = GetAccountInfo(strRegID);
		if(!CheckScriptid(temp1,strRegID))
		{
			return false;
		}
		return true;
	}

	void CheckScriptAccount()
	{
		// ���ű��˻������ĺϷ���
		BOOST_CHECK_EQUAL(CreateScriptAndCheck(),true);

		//// ���ű��˻���Ǯ
		string accountid = "010000000100";
		int param = 13;
		string temp = "";
		temp += tinyformat::format("%02x%s",param,accountid);
		Value resut =CreateContractTx("010000000100", "mv2eqSvyUA4JeJXBQpKvJEbYY89FqoRbX5", temp,10,1000000,10000);
		BOOST_CHECK(GetHashFromCreatedTx(resut,temp));

		BOOST_CHECK(SetAddrGenerteBlock("mjSwCwMsvtKczMfta1tvr78z2FTsZA1JKw"));
		Value temp1 = GetAccountInfo("010000000100");
		BOOST_CHECK_EQUAL(GetValue(temp1,"value"),10000);
		temp1 = GetAccountInfo("mv2eqSvyUA4JeJXBQpKvJEbYY89FqoRbX5");
		BOOST_CHECK_EQUAL(GetValue(temp1,"FreeValues"),999999998990000);

		/// �ű��˻�����ͨ�˻���Ǯ
		param = 14;
		temp = "";
		temp += tinyformat::format("%02x%s",param,accountid);
		resut =CreateContractTx("010000000100", "mv2eqSvyUA4JeJXBQpKvJEbYY89FqoRbX5", temp,10);
		BOOST_CHECK(GetHashFromCreatedTx(resut,temp));

		BOOST_CHECK(SetAddrGenerteBlock("msdDQ1SXNmknrLuTDivmJiavu5J9VyX9fV"));
		temp1 = GetAccountInfo("010000000100");
		BOOST_CHECK_EQUAL(GetValue(temp1,"value"),0);
		temp1 = GetAccountInfo("mv2eqSvyUA4JeJXBQpKvJEbYY89FqoRbX5");
		BOOST_CHECK_EQUAL(GetValue(temp1,"value"),10000);

		//���Բ��ܴ������ű���Ǯ����APP�ű��˻���
		accountid = "020000000100";
		param = 19;
		temp = "";
		temp += tinyformat::format("%02x%s",param,accountid);
		resut =CreateContractTx("010000000100", "mv2eqSvyUA4JeJXBQpKvJEbYY89FqoRbX5", temp,10);
		BOOST_CHECK(!GetHashFromCreatedTx(resut,temp));

	}

	void GetScriptDataSize() {
		const char *param[] = { "rpctest",
					"getscriptdbsize",
					"010000000100"};
//		CommandLineRPC(3, param);
		Value dummy;
		CommandLineRPC_GetValue(3, param,dummy);
	}

	bool CheckScriptDB(int nheigh,string srcipt,string hash,int flag)
	{
		int curtiph = chainActive.Height();

		string  hash2 = "hash";

		CRegID regid(srcipt);
		if (regid.IsEmpty() == true) {
			return false;
		}

		CScriptDBViewCache contractScriptTemp(*pScriptDBTip, true);
		if (!contractScriptTemp.HaveScript(regid)) {
			return false;
		}
		int dbsize;
		contractScriptTemp.GetScriptDataCount(regid, dbsize);
		if(curtiph <nheigh)
		{
			BOOST_CHECK(0==dbsize);
			return true;
		}
		BOOST_CHECK(1000==dbsize);


		vector<unsigned char> value;
		vector<unsigned char> vScriptKey;

		if (!contractScriptTemp.GetScriptData(curtiph,regid, 0, vScriptKey, value)) {
			return false;
		}
		uint256 hash1(value);
		string pvalue(value.begin(),value.end());
		if(flag)
		BOOST_CHECK(hash==hash1.GetHex()|| pvalue == hash2);
		else{
			BOOST_CHECK(hash==hash1.GetHex());
		}
		unsigned short key = 0;
		memcpy(&key,  &vScriptKey.at(0), sizeof(key));

		int count = dbsize - 1;
		while (count--) {
			if (!contractScriptTemp.GetScriptData(curtiph, regid, 1, vScriptKey, value)) {
				return false;
			}
			uint256 hash3(value);
			string pvalue(value.begin(), value.end());
			if (flag)
				BOOST_CHECK(hash == hash3.GetHex() || pvalue == hash2);
			else {
				BOOST_CHECK(hash == hash1.GetHex());
			}
		}
		return true;
	}

	void CreateTx(string pcontact,string addr)
	{
		string temp =addr;
		Value resut =CreateContractTx("010000000100", temp, pcontact,10);
		string strReturn;
		BOOST_CHECK(GetHashFromCreatedTx(resut,strReturn));
		BOOST_CHECK(GenerateOneBlock());
		return ;
	}
	bool GetScriptData(string srcipt,vector<unsigned char> key)
	{
		CRegID regid(srcipt);
			if (regid.IsEmpty() == true) {
				return false;
			}
			CScriptDBViewCache contractScriptTemp(*pScriptDBTip, true);
			if (!contractScriptTemp.HaveScript(regid)) {
				return false;
			}
			vector<unsigned char> value;
			int tipH = chainActive.Height();
			CScriptDBOperLog operLog;
			if (!contractScriptTemp.GetScriptData(tipH,regid,key, value)) {
				return false;
			}
			return true;
	}
	int GetScriptSize(string srcipt)
	{
			CRegID regid(srcipt);
			if (regid.IsEmpty() == true) {
				return 0;
			}

			if (!pScriptDBTip->HaveScript(regid)) {
				return 0;
			}
			int dbsize;
			pScriptDBTip->GetScriptDataCount(regid, dbsize);
			return dbsize;
	}
	string CreatWriteTx(string &hash)
	{
		string shash = CreateRegScript("mvVp2PDRuG4JJh6UjkJFzXUC8K5JVbMFFA","unit_test.bin");
		string scriptid;
		GetTxConfirmedRegID(shash, scriptid);
		BOOST_CHECK(scriptid !="");
		//// first tx
		string phash = CreateContactTx(15);
		int  height = chainActive.Height();

		BOOST_CHECK(CheckScriptDB(height,scriptid,phash,false));

		hash = phash;
		return scriptid;
	}
	void testdb()
	{
		string phash = "";
		string scriptid =  CreatWriteTx(phash);
		int height = chainActive.Height();
		int circle = 4;
		while(circle--)
		{
			BOOST_CHECK(GenerateOneBlock());
		}

		int count = 15;
		while(count > 1)
		{
			//// second tx
				uint256 hash(phash.c_str());
				int param =16;
				string temp = "";
				temp += tinyformat::format("%02x%s%02x",param,HexStr(hash),height);
			//	cout<<"cont:"<<temp<<endl;
				CreateTx(temp,"n4muwAThwzWvuLUh74nL3KYwujhihke1Kb");

				vector<unsigned char> key;
				const char *key1="2_error";
				key.insert(key.begin(),key1, key1 + strlen(key1) +1);
				BOOST_CHECK(!GetScriptData(scriptid,key));

				CheckScriptDB((height),scriptid,phash,false);
				count--;
		}

		while(true)
		{
			DisConnectBlock(1);
			CheckScriptDB(height,scriptid,phash,false);
			count = GetScriptSize(scriptid);
			if(count == 1000)
				break;
		}

	}

	void testdeletmodifydb()
	{
		string  writetxhash= "";
		string scriptid =  CreatWriteTx(writetxhash);
		int height = chainActive.Height();

		///// �޸�ɾ����
		int param =17;
		string temp = "";
		temp += tinyformat::format("%02x%02x",param,11);
		CreateTx(temp,"n4muwAThwzWvuLUh74nL3KYwujhihke1Kb");
		vector<unsigned char> key;
		const char *key1="3_error";
		key.insert(key.begin(),key1, key1 + strlen(key1) +1);
		BOOST_CHECK(!GetScriptData(scriptid,key));
		CheckScriptDB(height,scriptid,writetxhash,true);
		int modHeight = chainActive.Height();

	//	cout<<"end:"<<endl;
		//// ����
		int count = 15;
        while(count > 1)
        {
    		int param =18;
    		string temp = "";
    		temp += tinyformat::format("%02x",param);
    		CreateTx(temp,"n4muwAThwzWvuLUh74nL3KYwujhihke1Kb");
  //  		cout<<"cont:"<<endl;
   // 		cout<<chainActive.Height()<<endl;
        	CheckScriptDB(height,scriptid,writetxhash,true);
        	count--;
        }

      /// �ع�
		while(true)
		{
			DisConnectBlock(1);
			count = GetScriptSize(scriptid);
			if(chainActive.Height() > modHeight){
			CheckScriptDB(height,scriptid,writetxhash,true);
			}else{
				CheckScriptDB(height,scriptid,writetxhash,false);
			}
			if(count == 1000)
				break;
		}
	}
	void TestMinner()
	{
		for(int i=0;i < 100;i++)
		{
			string newaddr;
			srand(time(NULL));
			int r = (rand() % 2);

			BOOST_CHECK(GetNewAddr(newaddr,r));

			int nfee = GetRandomFee();

			Value value = CreateNormalTx(newaddr,nfee);

			string hash = "";
			BOOST_CHECK(GetHashFromCreatedTx(value,hash));
			BOOST_CHECK(GenerateOneBlock());

			Value value1 = registaccounttx(newaddr,nfee,r);
			BOOST_CHECK(GetHashFromCreatedTx(value1,hash));
			BOOST_CHECK(GenerateOneBlock());

			for(int j=0; j<100 ;++j)
			cout<<'\b';
			cout << "TestMinner progress: "<<  (int)(((i+1)/(float)100) * 100) << "%";
		}

		BOOST_CHECK(DisConnectBlock(chainActive.Height()-1));
		BOOST_CHECK(GenerateOneBlock());
		BOOST_CHECK(GenerateOneBlock());
	}
};


BOOST_FIXTURE_TEST_SUITE(sysScript_test,CSysScriptTest)

BOOST_FIXTURE_TEST_CASE(script_test,CSysScriptTest)
{
	//// pass
	ResetEnv();
	BOOST_CHECK(0==chainActive.Height());
	CreateRegScript("mvVp2PDRuG4JJh6UjkJFzXUC8K5JVbMFFA","unit_test.bin");
	CheckSdk();

	ResetEnv();
	BOOST_CHECK(0==chainActive.Height());
	CreateRegScript("mvVp2PDRuG4JJh6UjkJFzXUC8K5JVbMFFA","unit_test.bin");
	CheckRollBack();

	//// pass
	ResetEnv();
	BOOST_CHECK(0==chainActive.Height());
	CheckScriptAccount();

	ResetEnv();
	BOOST_CHECK(0==chainActive.Height());
	testdb();

	ResetEnv();
	BOOST_CHECK(0==chainActive.Height());
	testdeletmodifydb();
}

// ���Ը��ֵ�ַ�ڿ�
BOOST_FIXTURE_TEST_CASE(minier,CSysScriptTest)
{
	ResetEnv();
	TestMinner();
}
BOOST_AUTO_TEST_SUITE_END()

