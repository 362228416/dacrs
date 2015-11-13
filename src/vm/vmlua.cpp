// ComDirver.cpp: implementation of the CComDirver class.
//
//////////////////////////////////////////////////////////////////////

#include "vmlua.h"
#include "lua/lua.hpp"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "hash.h"
#include "key.h"
#include "main.h"
#include <openssl/des.h>
#include <vector>
#include "vmrunevn.h"
#include "tx.h"
//#include "Typedef.h"



#if 0
typedef struct NumArray{
	int size;          // �����С
	double values[1];  //���黺����
}NumArray;

/*
 * ��ȡuserdatum*/
static NumArray *checkarray(lua_State *L){
	//�����ջ��ָ��λ�õĶ����Ƿ�Ϊ���и������ֵ�metatable��userdatum
    void *ud = luaL_checkudata(L,1,"LuaBook.array");
    luaL_argcheck(L,ud != NULL,1,"'array' expected");
    return (NumArray *)ud;
}
/*
 * ��ȡ��������ָ��*/
static double *getelem(lua_State *L){
    NumArray *a = checkarray(L);
    int index = luaL_checkint(L,2);
    luaL_argcheck(L,1 <= index && index <= a->size,2,"index out of range");
    /*return element address*/
    return &a->values[index - 1];
}
/*
 * ����������*/
int newarray(lua_State *L){
   int n = luaL_checkint(L,1);   //���֤ʵ��luaL_checknumber�ı���
   size_t nbytes = sizeof(NumArray) + (n -1) * sizeof(double);

	/*һ��userdatum �ṩһ����Lua��û��Ԥ���������raw�ڴ�����
          ����ָ���Ĵ�С����һ���ڴ棬����Ӧ��userdatum�ŵ�ջ��,�������ڴ��ĵ�ַ*/
   NumArray *a = (NumArray *)lua_newuserdata(L,nbytes);
   luaL_getmetatable(L,"LuaBook.array");   //��ȡregistry�е�tname��Ӧ��metatable
   lua_setmetatable(L,-2);   //�����ջ����������Ϊ����λ�õĶ����metatable  �����µ�userdatum
   a->size = n;
   return 1; /*new userdatnum is already on the statck*/
}
/*
 * �洢Ԫ��,array.set(array,index,value)*/
int setarray(lua_State *L){
#if 0
	NumArray *a = (NumArray *)lua_touserdata(L,1);
	int index = luaL_checkint(L,2);
    double value = luaL_checknumber(L,3);

    luaL_argcheck(L,a != NULL,1,"'array' expected");
    luaL_argcheck(L,1 <= index && index <= a->size,2,"index out of range");
    a->values[index -1] = value;
#else
    double newvalue = luaL_checknumber(L,3);
    *getelem(L) = newvalue;
#endif
	return 0;
}
/*
 * ��ȡһ������Ԫ��*/
int getarray(lua_State *L){
#if 0
	NumArray *a = (NumArray *)lua_touserdata(L,1);
    int index = luaL_checkint(L,2);

    luaL_argcheck(L,a != NULL,1,"'array' expected");
    luaL_argcheck(L,1 <= index && index <= a->size,2,"index out of range");
    lua_pushnumber(L,a->values[index - 1]);
#else
    lua_pushnumber(L,*getelem(L));
#endif
    return 1;
}
/*
 * ��ȡ����Ĵ�С*/
int getsize(lua_State *L){
#if 0
	NumArray *a = (NumArray *)lua_touserdata(L,1);
	luaL_argcheck(L,a != NULL,1,"'array' expected");
#else
	NumArray *a = checkarray(L);
#endif
	lua_pushnumber(L,a->size);
	return 1;
}
static const struct luaL_Reg arraylib[] = {
		{"new",newarray},
		{"set",setarray},
		{"get",getarray},
		{"size",getsize},
		{NULL,NULL}
};
static int luaopen_array(lua_State *L){
	/*��������userdata��Ҫ�õ���metatable*/
	luaL_newmetatable(L,"LuaBook.array");
	luaL_openlib(L,"array",arraylib,0);

	/*now the statck has the metatable at index 1 and
	 * 'array' at index 2*/
    lua_pushstring(L,"__index");
    lua_pushstring(L,"get");
    lua_gettable(L,2); /*get array.get*/
    lua_settable(L,1); /*metatable.__index - array.get*/

    lua_pushstring(L,"__newindex");
    lua_pushstring(L,"set");
    lua_gettable(L,2); /*get array.get*/
    lua_settable(L,1); /*metatable.__newindex - array.get*/
	return 0;
}

#endif




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVmlua::CVmlua(const vector<unsigned char> & vRom, const vector<unsigned char> &InputData){
	unsigned long len = 0;
	/*vRom �������script,InputData �������contract*/
    memset(m_ExRam,0,sizeof(m_ExRam));
    memset(m_ExeFile,0,sizeof(m_ExeFile));

    len = vRom.size();
    if(len >= sizeof(m_ExeFile)){
    	len = sizeof(m_ExeFile) -1;
    }
	memcpy(m_ExeFile, &vRom[0], len);
	int count = InputData.size();
	memcpy(m_ExRam, &count, 2);
	memcpy(&m_ExRam[2], &InputData[0],count);

}

CVmlua::~CVmlua() {

}
LUAMOD_API int luaopen_mylib(lua_State *L);
int64_t CVmlua::run(uint64_t maxstep,CVmRunEvn *pVmScriptRun) {
	 long long step = 0;
	 int16_t count = 0;

	if((maxstep == 0) || (NULL == pVmScriptRun)){
		return -1;
	}

	//1.����Lua���л���
   lua_State *lua_state = luaL_newstate();
   if(NULL == lua_state){
	   LogPrint("vm", "luaL_newstate error\n");
	   return -1;
   }
#if 1
   //2.���ô�ע���Lua��׼��
   static const luaL_Reg lualibs[] =
   {
	   {"base",luaopen_base},
	   {LUA_LOADLIBNAME, luaopen_package},
//	   {LUA_COLIBNAME, luaopen_coroutine},
	   {LUA_TABLIBNAME, luaopen_table},
	   {LUA_IOLIBNAME, luaopen_io},
//	   {LUA_OSLIBNAME, luaopen_os},
	   {LUA_STRLIBNAME, luaopen_string},
	   {LUA_MATHLIBNAME, luaopen_math},
//	   {LUA_UTF8LIBNAME, luaopen_utf8},
	   {LUA_DBLIBNAME, luaopen_debug},
	   {NULL,NULL}
   };
   //3.ע��Lua��׼�Ⲣ���ջ
   const luaL_Reg *lib = lualibs;
   for(;lib->func != NULL;lib++)
   {
	   lib->func(lua_state);
	   lua_settop(lua_state,0);
   }
#else
   //����Ҫ�Ŀ�
   luaL_openlibs(lua_state);
#endif
   //ע���Զ���ģ��
   luaL_requiref(lua_state,"mylib",luaopen_mylib,1);

//   //ע�ắ��ΪLua��ȫ�ֱ���
//   lua_register(lua_state,"GetCurTxAccount",ExGetCurTxAccountFunc);
#if 1
   //4.��lua�ű����ݺ�Լ����
//   luaopen_array(lua_state);
//   lua_pop(lua_state,1);
	lua_newtable(lua_state);    //�½�һ����,ѹ��ջ��
	lua_pushnumber(lua_state,-1);
	lua_rawseti(lua_state,-2,0);
	memcpy(&count,m_ExRam,  2);
    for(int16_t n = 0;n < count;n++)
    {
        lua_pushinteger(lua_state,m_ExRam[2 + n]);// valueֵ����
        lua_rawseti(lua_state,-2,n+1);  //set table at key 'n + 1'
    }
    lua_setglobal(lua_state,"contact");
#endif
   //5.���ؽű�        //<��ȷ�ϣ��������ű�����Ƿ�256��byte????? m_ExeFile
    step = maxstep;
//   if(luaL_loadbuffer(lua_state,(char *)m_ExeFile,strlen((char *)m_ExeFile),"line") || lua_pcall(lua_state,0,0,0))
    if(luaL_loadbuffer(lua_state,(char *)m_ExeFile,strlen((char *)m_ExeFile),"line") || lua_pcallk(lua_state,0,0,0,0,NULL,&step))
   {
	   LogPrint("vm", "luaL_loadbuffer fail:%s\n", lua_tostring(lua_state,-1));
	   step = -1;
   }else{
//	   step = 111;
#if 0
	//6.ִ�нű�  ,����lua�ĺ���
	   int ret = -1;
       lua_getglobal(lua_state,"main"); //the function name
       lua_pushnumber(lua_state,11);     //push 1st argument
       lua_pushnumber(lua_state,12);     //push 2nd argument
       ret = lua_pcall(lua_state,2,1,0); //Ҫ����һ������ֵ
       LogPrint("vm", "run ret:%d\n", ret);
	   if(ret != LUA_OK)
       {
    	   LogPrint("vm", "lua_pcall err1:%s\n", lua_tostring(lua_state,-1));
    	   step = -1;
       }else{
    	    //!<У�鷵��ֵ
           if(!lua_isnumber(lua_state,-1))
           {
        	   LogPrint("vm", "lua_pcall must return a number\n");
        	   step = -1;
           }else{
        	   ret = lua_tonumber(lua_state,-1);
        	   lua_pop(lua_state,1);             //���ջ��ķ���ֵ
        	   LogPrint("vm", "run state rslt:%d\n", ret);
			   step = 111;
           }
       }
#endif
   }

    //5.�ر�Lua�����
	lua_close(lua_state);
    LogPrint("vm", "run step=%d\n",step);
	return step;
}



