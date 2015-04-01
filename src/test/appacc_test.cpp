/*
 * appacc_test.cpp
 *
 *  Created on: 2015��3��29��
 *      Author: ranger.shi
 */


#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>
#include "txdb.h"
#include "account.h"
#include <iostream>
#include  "boost/filesystem/operations.hpp"
#include  "boost/filesystem/path.hpp"
#include  "../vm/appuseraccout.h"

using namespace std;




// appacc_tests/key_test1

BOOST_AUTO_TEST_SUITE(appacc_tests)

BOOST_AUTO_TEST_CASE(key_test1)
 {
	auto StrTVector = [&](string tag)
	{
		return vector<unsigned char>(tag.begin(),tag.end());
	};

	srand((int) time(NULL));

	vector<unsigned char> AppuserId = StrTVector("test1");
	vector<unsigned char> fundtag = StrTVector("foundtag");
	vector<unsigned char> fundtag2 = StrTVector("foundtag2");

	CAppFundOperate opTe(AppuserId,fundtag, ADD_TAG_OP, 500, 800000);
	BOOST_CHECK(opTe.GetFundTagV() == fundtag);
	BOOST_CHECK(opTe.GetUint64Value()== 800000);
	BOOST_CHECK(opTe.getopeatortype()== ADD_TAG_OP);


	vector<CAppFundOperate> OpArry;
	uint64_t allmony = 0;
	int timeout = (rand() % 15000) + 51;
	int loop = 500;
	int maxtimeout = timeout + loop+1;
	for (int i = 0; i < loop; i++) {
		int64_t temp = ((rand() * rand()) % 15000000) + 20;
		allmony += temp;
		CAppFundOperate op(AppuserId,fundtag, ADD_TAG_OP, timeout + i, temp);
		OpArry.insert(OpArry.end(), op);
	}

	CAppUserAccout AccCount(AppuserId);
	BOOST_CHECK(AccCount.getaccUserId() == AppuserId);      //��ʼ����ID ������
	BOOST_CHECK(AccCount.Operate(OpArry));               //ִ�����еĲ�������
	BOOST_CHECK(AccCount.getllValues() == 0);            //��Ϊ������ȫ�ǼӶ����Ǯ�������ɽ�������0


	{
		CAppCFund tep;
		BOOST_CHECK(AccCount.GetAppCFund(tep, fundtag, timeout)); //��ȡ��Ӧ�Ķ�����
		BOOST_CHECK(tep.getvalue() == OpArry[0].GetUint64Value());                    //����Ľ����Ҫû������
		CAppCFund tep2;
		BOOST_CHECK(AccCount.GetAppCFund(tep2, fundtag, maxtimeout + 5) == false); //��ȡ��Ӧ�Ķ����� ��ʱʱ�䲻ͬ �����ȡ����

		AccCount.AutoMergeFreezeToFree(timeout - 1);               	   //�Զ��ϲ� ��ʱ�߶�û�е�  ����� 50 ��Ϊ�����ǩ�� time out de 51
		BOOST_CHECK(AccCount.GetAppCFund(tep, fundtag, timeout));			 //û�кϲ��������û�б䶯
		BOOST_CHECK(tep.getvalue() == OpArry[0].GetUint64Value());         	//û�кϲ��������û�б䶯
	}

	{
		vector<CAppFundOperate> OpArry2;
		CAppFundOperate subfreexeop(AppuserId,fundtag, SUB_TAG_OP, timeout, 8);
		OpArry2.insert(OpArry2.end(), subfreexeop);
		BOOST_CHECK(AccCount.Operate(OpArry2));               //ִ�����еĲ�������
	}

	{
		CAppCFund subtemptep;
		BOOST_CHECK(AccCount.GetAppCFund(subtemptep, fundtag, timeout));        //��ȡ��Ӧ�Ķ�����
		BOOST_CHECK(subtemptep.getvalue() == (OpArry[0].GetUint64Value() - 8));    //�����ȥ��8  ����Ƿ��
	}

	{
		vector<CAppFundOperate> OpArry2;
		CAppFundOperate revertfreexeop(AppuserId,fundtag, ADD_TAG_OP, timeout, 8);
		OpArry2.clear();
		OpArry2.insert(OpArry2.end(), revertfreexeop);
		BOOST_CHECK(AccCount.Operate(OpArry2));               //ִ�����еĲ�������
	}

	{
		CAppCFund reverttemptep;
		BOOST_CHECK(AccCount.GetAppCFund(reverttemptep, fundtag, timeout));			 //û�кϲ��������û�б䶯
		BOOST_CHECK(reverttemptep.getvalue() == OpArry[0].GetUint64Value());         	//û�кϲ��������û�б䶯
	}

	{         	//�ϲ���һ��
		CAppCFund tep;
		AccCount.AutoMergeFreezeToFree(timeout);                  				//�Զ��ϲ� ��0��
		BOOST_CHECK(AccCount.GetAppCFund(tep, fundtag, timeout) == false); 		//�����Ҳ�������
		BOOST_CHECK(AccCount.getllValues() == OpArry[0].GetUint64Value());;          				//�ϲ������ɽ�����û������
	}

	{          				//��ȥȫ��
		CAppFundOperate subfreeop(AppuserId,fundtag, SUB_FREE_OP, timeout, OpArry[0].GetUint64Value());
		vector<CAppFundOperate> OpArry2;
		OpArry2.insert(OpArry2.end(), subfreeop);
		BOOST_CHECK(AccCount.Operate(OpArry2));               				//ִ�����еĲ�������
		BOOST_CHECK(AccCount.getllValues() == 0);;                           //Ǯ������Ժ˶�
	}

	{
		vector<CAppFundOperate> OpArry2;
		CAppFundOperate addfreeop(AppuserId,fundtag, ADD_FREE_OP, timeout, OpArry[0].GetUint64Value());    //�ٴΰ����ݼӽ�ȥ
		OpArry2.clear();
		OpArry2.insert(OpArry2.end(), addfreeop);
		BOOST_CHECK(AccCount.Operate(OpArry2));               				//ִ�����еĲ�������
		BOOST_CHECK(AccCount.getllValues() == OpArry[0].GetUint64Value());                //���Ϻ� �ͻ�����
	}

	AccCount.AutoMergeFreezeToFree(maxtimeout);     				//ȫ���ϲ�
	BOOST_CHECK(AccCount.getllValues() == allmony);                //���ƽ��

}

BOOST_AUTO_TEST_SUITE_END()
