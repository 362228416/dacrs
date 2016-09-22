/*
 * VmScript.h
 *
 *  Created on: Sep 2, 2014
 *      Author: ranger.shi
 */

#ifndef VMSCRIPT_H_
#define VMSCRIPT_H_

#include "serialize.h"
using namespace std;

/**
 * @brief Load script binary code class
 */
class CVmScript {

public:
	vector<unsigned char> Rom;      		//!< Binary code
	vector<unsigned char> ScriptExplain;	// !<explain the binary code action
	int  scriptType = 0;                    //!<�ű������� 0:8051,1:lua

public:
	/**
	 * @brief
	 * @return
	 */
	bool IsValid()
	{
		///Binary code'size less 64k
		if((Rom.size() > 64*1024) || (Rom.size() <= 0))
			return false;
		if(Rom[0] != 0x02){
			if(!memcmp(&Rom[0],"mylib = require",strlen("mylib = require"))){
				scriptType = 1;//lua�ű�
				return true;//lua�ű���ֱ�ӷ���
			}else{
				return false;
			}
		}else{
			scriptType = 0;//8051�ű�
		}

		//!<ָ���汾��SDK���ϣ���ȥУ�� �˻�ƽ�⿪�ص�ȡֵ
		if(memcmp(&Rom[0x0004],"\x00\x02\x02",3) >= 0){
           if(!((Rom[0x0014] == 0x00) || (Rom[0x0014] == 0x01))){
        	   return false;
           }
		}

		return true;
	}

	bool IsCheckAccount(void){
		if(scriptType){
			return false;//lua�ű���ֱ�ӷ���(�ص��˻�ƽ��)
		}

		//!<ָ���汾��SDK���ϣ���ȥ��ȡ �˻�ƽ�⿪�ص�ȡֵ
		if(memcmp(&Rom[0x0004],"\x00\x02\x02",3) >= 0)
		{
	        if(Rom[0x0014] == 0x01){
	        	return true;
	        }
		}
        return false;
	}
	CVmScript();
    int getScriptType(){
    	return scriptType;
    }
	 IMPLEMENT_SERIALIZE
	(
		READWRITE(Rom);
		READWRITE(ScriptExplain);
	)
	virtual ~CVmScript();
};

#endif /* VMSCRIPT_H_ */
