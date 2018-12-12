//
//  dsLua.h

#ifndef _DSLUA_H_
#define _DSLUA_H_


extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

#include "dsObject.h"
#include <assert.h>



class dsLuaScript;



//----------------------------------------------------------
//class dsLuaManager
//----------------------------------------------------------

class dsLuaManager{
public:
	dsLuaManager (void);
	~dsLuaManager(void);
	
	bool LuaOpenLibrary(const char * name, const luaL_reg * libs);
	
	void SetGlobalNumber(const char * name, double val);
	void SetGlobalInteger(const char * name, int val);
	void SetGlobalString(const char * name, const char * val);
	
	dsLuaScript * CreateScript(void);
	void DestroyScript(dsLuaScript *);
	
	//call this function at every frame with the number of seconds since the last call
	void Update(float elapsedSeconds);
	
private:
	lua_State * mainState_;
	dsLuaScript * head_;
	
	
	friend class dsLuaScript;
	friend class dsLuaGlobal;
	friend class dsLuaObject;	
};

dsLuaManager *GetLuaManager();


//----------------------------------------------------------
//class dsLuaScript
//----------------------------------------------------------

class dsLuaScript{
public:
	void LoadFile(const char * name);
	void LoadString(const char * buffer);
	
	//used internally by library- not for end user use
	void YieldFrames(int num);
	void YieldSeconds(float secs);
	
protected:
	friend class dsLuaManager;
	
	dsLuaScript(lua_State *, dsLuaManager * man, int registryRef);
	~dsLuaScript(void);
	dsLuaScript * Update(float elapsedSeconds);
	dsLuaScript * next_;
	int registryRef_;
	
private:
	typedef enum {
		YM_NONE,
		YM_FRAME,
		YM_TIME
	} YIELDMODE;
	
	lua_State * state_;
	dsLuaManager * manager_;
	YIELDMODE yieldMode_;
	int waitFrames_;
	float waitTime_;
};


//----------------------------------------------------------
//class dsLuaError
//----------------------------------------------------------

class dsLuaError
{
public:
	enum ErrorCode 
	{
		LUA_GENERIC_ERROR,
		NOT_A_TABLE,
		NOT_A_NUMBER,
		NOT_A_STRING,
		NOT_A_FUNCTION
	};
	
	dsLuaError(ErrorCode error_code,
			 std::string error_msg = std::string() );
	
	const char* what();
	ErrorCode code();
	
private:
	std::string error_msg_;
	ErrorCode error_code_;
};


//----------------------------------------------------------
//class dsLuaObject
//----------------------------------------------------------

class dsLuaObject
{
public:
	friend class dsLuaGlobal;
	dsLuaObject(const dsLuaObject&);
	~dsLuaObject();
	
	dsLuaObject& operator[](const char* name) throw(dsLuaError);
	dsLuaObject& operator[](int index) throw(dsLuaError);
	
	template<typename T> T val() throw(dsLuaError);
	
	//getters
	operator unsigned char();
	operator short();
	operator int();
	operator std::string();
	operator float();
	operator double();
	
	//setters
	void operator=(unsigned char val);
	void operator=(short val);
	void operator=(int val);
	void operator=(float val);
	void operator=(double val);
	void operator=(std::string val);
	
	// query
	size_t length();
	bool isnil();
	bool isnumber();
	bool isstring();
	bool istable();
	
	// getting default value
	unsigned char or_val(unsigned char);
	short or_val(short);
	int or_val(int);
	float or_val(float);
	double or_val(double);
	std::string or_val(const std::string&);
	
	// for LUA_ROVER_WITH_TABLE macro below
	operator bool() { return false; } 
	
private:
	dsLuaObject();
	dsLuaObject(lua_State*);
	dsLuaObject(lua_State*, const char* name) throw(dsLuaError);
	dsLuaObject operator=(const dsLuaObject&);
	lua_State* lua_state_;
	mutable size_t base_stack_count_;
	size_t stack_count_;
	void pop_stack();
	std::string global_name_;
	std::string last_key_;
};


//----------------------------------------------------------
//class dsLuaGlobal
//----------------------------------------------------------

class dsLuaGlobal
{
public:
	dsLuaGlobal(const char* filename) throw(dsLuaError);
	dsLuaGlobal(const dsLuaGlobal&);
	
	~dsLuaGlobal();
	
	dsLuaObject operator[](const char* name) throw(dsLuaError);
	dsLuaObject get(const char* name) throw(dsLuaError);
	
	// for LUA_ROVER_WITH_FILE macro below
	operator bool() { return false; } 
	
private:
	dsLuaGlobal operator=(const dsLuaGlobal&);
	mutable lua_State* lua_state_;
};




#if !defined(LUA_ROVER_GLOBAL_ID)
#define LUA_ROVER_GLOBAL_ID dsLG
#endif

#if !defined(LUA_ROVER_LOCAL_ID)
#define LUA_ROVER_LOCAL_ID dsL
#endif




#define LUA_ROVER_WITH_FILE(name) \
if( dsLuaGlobal LUA_ROVER_GLOBAL_ID = dsLuaGlobal(name) ) assert( 0 ); else

#define LUA_ROVER_WITH_TABLE(name) \
if( dsLuaObject LUA_ROVER_LOCAL_ID = LUA_ROVER_GLOBAL_ID.get(name) ) assert( 0 ); else


#define WITH_T  LUA_ROVER_WITH_TABLE

#endif

