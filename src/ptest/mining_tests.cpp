#include "rpcserver.h"
#include "rpcclient.h"
#include "util.h"
#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost;
using namespace json_spirit;

map<string, string> mapDesAddress[] = {
        boost::assign::map_list_of
        ("000000000900",	"mvVp2PDRuG4JJh6UjkJFzXUC8K5JVbMFFA")
        ("000000000500",	"mv2eqSvyUA4JeJXBQpKvJEbYY89FqoRbX5")
        ("000000000300",	"mhVJJSAdPNDPvFWCmQN446GUBPzFm8aN4y")
        ("000000000800",	"n4muwAThwzWvuLUh74nL3KYwujhihke1Kb")
        ("000000000700",	"mfu6nTXP9LR9mRSPmnVwXUSDVQiRCBDJi7")
        ("000000000400",	"moZJZgsGFC4qvwRdjzS7Bj3fHrtpUfEVEE")
        ("000000000100",	"mjSwCwMsvtKczMfta1tvr78z2FTsZA1JKw")
        ("000000000a00",	"msdDQ1SXNmknrLuTDivmJiavu5J9VyX9fV")
        ("000000000600",	"mrjpqG4WsyjrCh8ssVs9Rp6JDini8suA7v")
        ("000000000200",	"mw5wbV73gXbreYy8pX4FSb7DNYVKU3LENc"),

        boost::assign::map_list_of
        ("020000000d00",	"mm8f5877wY4u2WhhX2JtGWPTPKwLbGJi37")
        ("020000001200",	"n31DG5wjP1GcKyVMupGBvjvweNkM75MPuR")
        ("020000002700",	"mkeict2uyvmb4Gjx3qXh6vReoTw1A2gkLZ")
        ("020000002800",	"mtnCTLWKqnTf6BVqaXQQAYuPDyV78d8sTz")
        ("020000001c00",	"mqwRHqjQZcJBqJP46E256Z2VuqqRAkZkKH")
        ("020000000f00",	"moDJqwMZW1pKGrQp4NYcaYS4j5tdvqBevv")
        ("020000002c00",	"mvV1fW4NMv9MGoRwHw583TDi8gnqqjQovZ")
        ("020000002a00",	"mzGkgfAkWtkQ4AP2Ut4yiAzCNx6EDzWjW8")
        ("020000000a00",	"n3wo9Ts6AUmHdGM1PixpRnLRFrG5G8a5QA")
        ("020000000b00",	"mxrmM6qNswgZHmp1u2HTu2soncQLkud7tF"),

        boost::assign::map_list_of
        ("020000002b00",	"n3eyjajBMwXiK56ohkzvA2Xu53W9E6jj8K")
        ("020000002100",	"n3YqGMig2xqbMvdHJ9q4Jfk5exbXK2tvLo")
        ("020000001400",	"muDs5TAdk6n8rSyLdq6HTzBkJT2XxPF1wP")
        ("020000001000",	"mxZqVtfao3A6dbwymKtn6oE4GacXoJNsac")
        ("020000000900",	"mspw67fn4KGwUrG9oo9mvLJQAPrTYwxQ6w")
        ("020000002400",	"mrRS8A9D1FXWXMFAMpZxmdfchyD1E1cAu6")
        ("020000001700",	"mvNmNnB98GDSeYqg2jH2gSU557XEivs3N5")
        ("020000000100",	"mjEGztB67nfscqSg5ryUGtzyTGwwEASZeQ")
        ("020000001b00",	"mmCvt8WZzF27VGBMWWVkED3vsDRdpnigGV")
        ("020000001a00",	"mxx4MohV2ZfifQiZnmU4yVUVf2QUVM2grx"),

		boost::assign::map_list_of
        ("020000000e00",	"mjuZWVqVQ2cmFoB8pJRj7XWVCPkeoiWJAq")
        ("020000002200",	"n2tTaaF8xoWWYvaxSDkfQP5GeEcCCsjq1t")
        ("020000000c00",	"mrKYnvKuTtxntndbJwMpR5fCJt571GsF9b")
        ("020000002500",	"mw1XUknDsVtb68BUJNj25rKAikYG8qELHJ")
        ("020000000200",	"mgE3hASaGCRPxJdZruAsydr2ygQz2UBWZM")
        ("020000002600",	"n1vNXyu2GNypJGdZYxzCBCeQFVt1Fd42Qn")
        ("020000001500",	"n4Cti65cSeufvfxStKUozHNGX3fQSHsDe5")
        ("020000000300",	"mnZUhyb83ZTQWc9TXXFfhjJEu65q4cFj4S")
        ("020000000700",	"muGiULSeqi2FQ2ypzU7aP8Uu1SWC5kRBki")
        ("020000001100",	"mrMFs4kk8sqZ7iE8DquqPLL8udyGNDUZ8T"),

        boost::assign::map_list_of
        ("020000001900",	"mogX7FTZ9Yuu6gYscKaEf2oxroeRuNDi76")
        ("020000001d00",	"mgs1mDsaXuj16aJ5YMHqLx7xsQ88snsZmB")
        ("020000000600",	"mzUKrawp7a7LNB7D7kKzKEpgAStsAAHz18")
        ("020000001e00",	"mgTkNe74wwdSdR3QjGoU5PKvSzFQsHxNTG")
        ("020000001600",	"miNou7awKXUPN9wbzVP32zTXcWvPsZBpYg")
        ("020000002300",	"mnnd1QQx2dM5yfp1j8Vp7Dcq7BhiS6bNEQ")
        ("020000000500",	"mfiaCx5k4XpYBS4iHviWgBiQg4Riv2zT4R")
        ("020000001f00",	"muS2Nxtva88d45uN6up7WeHszi3oWAcadK")
        ("020000000400",	"mw8yB7Pp7GYiDHhLQT2GNsLc439rfJ3Fai")
        ("020000001300",	"miRVDrwxtJJh4XnZFnYR6YbdqpAuirVDzZ")};


string sendValues[2][10] = {
		{"3B9ACA00","77359400","B2D05E00","EE6B2800","12A05F200","165A0BC00","1A13B8600","1DCD65000","218711A00","2540BE400"},
		{"1000000000","2000000000","3000000000","4000000000","5000000000","6000000000","7000000000","8000000000","9000000000","10000000000"}
	};


class CMiningTest {
public:
	//��ʼ�����л���������Block��Ϣ
	CMiningTest() {

	}
	~CMiningTest() {};

};


class CSendItem{
private:
	string m_strRegId;
	string m_strAddress;
	string m_strSendValue;
public:
	CSendItem(const string &strRegId, const string &strDesAddr, const string & strSendValue)
	{
		m_strRegId = strRegId;
		m_strAddress = strDesAddr;
		m_strSendValue = strSendValue;
	}
	void GetContranctData(vector<unsigned char> &vContranct ) {
		CDataStream ds(SER_DISK, CLIENT_VERSION);
		ds << ParseHex(m_strRegId) << ParseHex(m_strSendValue);
		vContranct.insert(vContranct.end(), ds.begin(), ds.end());
	}
	//nFlag 0:ȡ����sendValues1��ֵ  1:ȡ����sendValues��ֵ
	//nIndex ȡֵ��Χ1~5����ʾ1~5���ͻ���
	static const CSendItem &GetRandomSendItem(int nFlag, int nIndex) {
		int randAddr = std::rand()%10;
		int randSendValue = std::rand()%10;
		map<string, string>::iterator iterAddr = mapDesAddress[nIndex-1].begin();
		do {
			iterAddr++;
		}while(randAddr-- && iterAddr != mapDesAddress[nIndex-1].end());
		return CSendItem(iterAddr->first, iterAddr->second, sendValues[nFlag][randSendValue]);
	}
	string GetRegID() {
		return m_strRegId;
	}

	string GetAddress() {
		return m_strAddress;
	}

	string GetSendValue() {
		return m_strSendValue;
	}


};
/**
 *������ͨ����
 * @param param
 * param[0]:Դ��ַ
 * param[1]:Ŀ�ĵ�ַ
 * param[2]:ת�˽��
 * param[3]:������
 * param[4]:��Ч�ڸ߶�
 */
void CreateNormalTx(vector<string> &param) {
	if(5 != param.size())
		return;
	param.insert(param.begin(), "createnormaltx");
	param.insert(param.begin(), "rpctest");
//	char *argv[7] = { "rpctest", "createnormaltx", param.at(0), param.at(1), param.at(2), param.at(3), param.at(4)};
	char *argv[param.size()];
	int i=0;
	for(auto & item : param) {
		argv[i] = const_cast<char *>(param[i].c_str());
		++i;
	}
	CommandLineRPC(param.size(), argv);
}

/**
 * ������Լ����
 * @param param
 * param[0]:�ű�ע��ID
 * param[1]:�˻���ַ�б�,json�������ʽ
 * param[2]:��Լ����
 * param[3]:������
 * param[4]:��Ч�ڸ߶�
 */
void CreateContractTx(vector<string> &param) {
	if(5 != param.size())
		return;
	param.insert(param.begin(), "createcontracttx");
	param.insert(param.begin(), "rpctest");
	//char *argv[7] = {"rpctest", "createcontracttx", param.at(0), param.at(1), param.at(2), param.at(3), param.at(4)};
	char *argv[param.size()];
	int i=0;
	for(auto & item : param) {
		argv[i] = const_cast<char *>(param[i].c_str());
		++i;
	}
	CommandLineRPC(param.size(), argv);
}


/**
 * ����ע��ű�����
 * @param param
 * param[0]:ע��ű����˻���ַ
 * param[1]:ע��ű���ʶλ��0-��ʶ�ű����ݵ��ļ�·����1-��ע��ű�ID
 * param[2]:�ļ�·����ע��ű�ID
 * param[3]:������
 * param[4]:��Ч�ڸ߶�
 * param[5]:�ű����� �������ע��ű�,��ѡ��
 * param[6]:�ű���Ȩʱ�� ����ѡ��
 * param[7]:��Ȩ�ű�ÿ�δ��˻��пۼ�������� ����ѡ��
 * param[8]:��Ȩ�ű��ܹ���Ǯ������� ����ѡ��
 * param[9]:��Ȩ�ű�ÿ���Ǯ������� ����ѡ��
 * param[10]:�û��Զ�������
 *
 */
void CreateRegScriptTx(vector<string> &param) {
	if(5 > param.size())
		return;
	param.insert(param.begin(), "registerscripttx");
	param.insert(param.begin(), "rpctest");

	char *argv[param.size()];
	int i=0;
	for(auto & item : param) {
		argv[i] = const_cast<char *>(param[i].c_str());
		++i;
	}
	CommandLineRPC(param.size(), argv);
}

time_t string2time(const char * str,const char * formatStr)
{
  struct tm tm1;
  int year,mon,mday,hour,min,sec;
  if( -1 == sscanf(str,formatStr,&year,&mon,&mday,&hour,&min,&sec)) return -1;
  tm1.tm_year=year-1900;
  tm1.tm_mon=mon-1;
  tm1.tm_mday=mday;
  tm1.tm_hour=hour;
  tm1.tm_min=min;
  tm1.tm_sec=sec;
  return mktime(&tm1);
}
BOOST_AUTO_TEST_SUITE(auto_mining_test)
BOOST_AUTO_TEST_CASE(reg_script) {
	//ע��ű�����
	vector<string> param;
	param.push_back("mvVp2PDRuG4JJh6UjkJFzXUC8K5JVbMFFA");
	param.push_back("0");
	param.push_back("D:\\cppwork\\vmsdk\\testUint\\Debug\\Exe\\test.bin");
	param.push_back("100000000");
	param.push_back("0");
	param.push_back("test");
	int64_t curTime = GetTime();
	char charTime[20] = {0};
	sprintf(charTime, "%ld", curTime);
	param.push_back(charTime);
	param.push_back("1000000000000");
	param.push_back("100000000000000");
	param.push_back("100000000000000");
	param.push_back("userdefine");
	CreateRegScriptTx(param);
}
BOOST_AUTO_TEST_CASE(test1)
{

//	time_t t1 = string2time("2014-12-01 17:30:00","%d-%d-%d %d:%d:%d");
	int64_t runTime = GetTime()+10*10*60;   //����ʮ����
	vector<string> param;
	while(GetTime()<runTime) {
		//�����ͻ���1->�ͻ���2����ͨ����
		CSendItem sendItem = CSendItem::GetRandomSendItem(1,1);
		CSendItem recItem = CSendItem::GetRandomSendItem(1,2);
		param.clear();
		param.push_back(sendItem.GetAddress());      	//Դ��ַ
		param.push_back(recItem.GetAddress());    	    //Ŀ�ĵ�ַ
		param.push_back(recItem.GetSendValue());	    //ת�˽��
		param.push_back("100000000");					//������
		param.push_back("0");                           //��Ч�߶�

		CreateNormalTx(param);                          //������ͨ����
		Sleep(1);

		//�����ͻ���1->�ͻ���2�ĺ�Լ����
		CSendItem sendItem1 = CSendItem::GetRandomSendItem(1,1);
		CSendItem recItem1 = CSendItem::GetRandomSendItem(1,1);
		CSendItem recItem2 = CSendItem::GetRandomSendItem(1,1);
		CSendItem recItem3 = CSendItem::GetRandomSendItem(1,1);
		param.clear();
		param.push_back("000000000100");                     //�ű�ID
		param.push_back("[\""+sendItem1.GetAddress()+"\"]"); //���׷����ַ
		vector<unsigned char> vContranct;
		vContranct.clear();
		vector<unsigned char> vTemp;
		vTemp.clear();
		recItem1.GetContranctData(vTemp);
		vContranct.insert(vContranct.end(), vTemp.begin(), vTemp.end());
		vTemp.clear();
		recItem2.GetContranctData(vTemp);
		vContranct.insert(vContranct.end(), vTemp.begin(), vTemp.end());
		recItem3.GetContranctData(vTemp);
		vContranct.insert(vContranct.end(), vTemp.begin(), vTemp.end());
		param.push_back(HexStr(vContranct));			//��Լ����
		param.push_back("100000000");					//������
		param.push_back("0");                           //��Ч�߶�
		CreateContractTx(param);                        //������Լ����
		Sleep(1);

	}
}
BOOST_AUTO_TEST_CASE(test2)
{

}
BOOST_AUTO_TEST_CASE(test3)
{

}

BOOST_AUTO_TEST_CASE(test4)
{

}
BOOST_AUTO_TEST_CASE(test5)
{

}
BOOST_AUTO_TEST_CASE(regscript){

}
BOOST_AUTO_TEST_SUITE_END()
