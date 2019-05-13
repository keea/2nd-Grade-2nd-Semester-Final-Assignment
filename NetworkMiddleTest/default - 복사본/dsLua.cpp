//
//  dsLua.mm
//  spEngine
//
//  Created by Ju Woosuk on 11. 2. 28..
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#include <sstream>
#include <iostream>
#include "dsLua.h"

// Below are the functions used for the yield funtions.

static int LuaYieldSeconds(lua_State * l)
{
	dsLuaScript * s;
	lua_pushlightuserdata(l,l);
	lua_gettable(l, LUA_GLOBALSINDEX);
	s = (dsLuaScript *)lua_touserdata(l,-1);
	
	float n = lua_tonumber(l,1);
	s->YieldSeconds(n);
	printf("waitSeconds %f\n",n);
	
	return (lua_yield(l, 0));
}

static int LuaYieldFrames(lua_State * l)
{
	dsLuaScript * s;
	lua_pushlightuserdata(l,l);
	lua_gettable(l, LUA_GLOBALSINDEX);
	s = (dsLuaScript *)lua_touserdata(l,-1);
	
	int f = lua_tonumber(l,1);
	s->YieldFrames(f);
	printf("waitFrames %d\n",f);
	
	return (lua_yield(l, 0));
}


static dsLuaManager g_LuaManager;

dsLuaManager *GetLuaManager() 
{
	return &g_LuaManager;
}


//---------------------------------------------------------------

dsLuaManager::dsLuaManager (void):
head_(NULL)
{
	mainState_ = luaL_newstate();
	//luaL_openlibs(state);
	
	//add the base library
	luaopen_base(mainState_);
	luaopen_math(mainState_);
	luaopen_string(mainState_);
	lua_settop(mainState_, 0); //스택을 초기화

/*	
	
	mainState_ = lua_open();
	
	//Add functions for waiting
	static const luaL_reg scriptLib[] = 
	{
		{"waitSeconds", LuaYieldSeconds },
		{"waitFrames", LuaYieldFrames },
		{NULL, NULL}
	};
	LuaOpenLibrary( "script", scriptLib);
	
	//add the base library
	luaopen_base(mainState_);
	luaopen_math(mainState_);
	luaopen_string(mainState_);
	lua_settop(mainState_, 0);
*/
	
}

dsLuaManager::~dsLuaManager(void)
{
/*	
	dsLuaScript * n = head_;
	while (n) {
		dsLuaScript * i = n->next_;
		delete (n);
		n=i;
	}
*/	
	lua_close(mainState_); mainState_ = 0;
}

bool dsLuaManager::LuaOpenLibrary(const char * name, const luaL_reg * libs)
{
	if(head_ != NULL)
		return 0;
	
	luaL_openlib(mainState_, name, libs, 0);
	return 1;	
}

void dsLuaManager::SetGlobalNumber(const char * name, double val)
{
	lua_pushnumber(mainState_, val);
	lua_setglobal(mainState_,name);
}

void dsLuaManager::SetGlobalInteger(const char * name, int val)
{
	lua_pushinteger(mainState_, val);
	lua_setglobal(mainState_,name);
}

void dsLuaManager::SetGlobalString(const char * name, const char * val)
{
	lua_pushstring(mainState_, val);
	lua_setglobal(mainState_,name);
}

dsLuaScript * dsLuaManager::CreateScript(void)
{
	lua_State * s = lua_newthread(mainState_);
	
	//Add reference to the new thread in the Lua Registry so
	//will not be garbage collected
	int r = luaL_ref(mainState_, LUA_REGISTRYINDEX);
	
	//Create a new mcLuaScript object to hold the new thread
	dsLuaScript * ns = new dsLuaScript(s,this,r);
	
	//Add an entry to map the new lua_State to the 
	//new mcLuaScript object 
	lua_pushlightuserdata(mainState_, s);
	lua_pushlightuserdata(mainState_, ns);
	lua_settable(mainState_, LUA_GLOBALSINDEX);
	
	//insert the new script into the list
	ns->next_ = head_;
	head_ = ns;
	return ns;
}

void dsLuaManager::DestroyScript(dsLuaScript * s)
{
	if(s == head_){
		head_ = s->next_;
		delete (s);
		return;
	}
	
	dsLuaScript * last = head_;
	for (dsLuaScript * l=last->next_; l != NULL; l=l->next_) {
		if (l == s) {
			last->next_ = l->next_;
			delete(l);
			return;
		}
		last = l;
	}
}

void dsLuaManager::Update(float elapsedSeconds)
{
	dsLuaScript * n = head_;
	while(n)
		n = n->Update(elapsedSeconds);
}

//---------------------------------------------------------------


dsLuaScript::dsLuaScript(lua_State * l, dsLuaManager * man, int registryRef):
next_(NULL)
,registryRef_(registryRef)
,state_(l)
,manager_(man)
,yieldMode_(YM_NONE)
,waitFrames_(0)
,waitTime_(0)
{
	
}

dsLuaScript::~dsLuaScript(void)
{
	luaL_unref(state_, LUA_REGISTRYINDEX, registryRef_);
}

void dsLuaScript::LoadFile(const char * name)
{
	printf("mcLuaScript::LoadFile() - %s\n",name);
	int err = luaL_loadfile(state_, name);
	if(err){
		printf("luaL_loadfile Error- %s\n",lua_tostring(state_,-1));
		lua_pop(state_,1);
	}
	
	lua_resume(state_, 0);
}

void dsLuaScript::LoadString(const char * buffer)
{
	int err = luaL_loadstring(state_, buffer);
	if(err){
		printf("%s",lua_tostring(state_,-1));
		lua_pop(state_,1);
	}
	
	lua_resume(state_, 0);
}

dsLuaScript * dsLuaScript::Update(float elapsedSeconds)
{
	if (yieldMode_ == YM_TIME) {
		waitTime_ -= elapsedSeconds;
		if(waitTime_ > 0)
			return next_;
	} 
	
	if( yieldMode_ == YM_FRAME){
		--waitFrames_;
		if (waitFrames_ > 0) {
			return next_;
		}
	}
	
	yieldMode_ = YM_NONE;
	lua_resume(state_, 0);
	return next_;
}

void dsLuaScript::YieldFrames(int num)
{
	yieldMode_ = YM_FRAME;
	waitFrames_ = num;
}

void dsLuaScript::YieldSeconds(float secs)
{
	yieldMode_ = YM_TIME;
	waitTime_ = secs;
}





//----------------------------------------------------------
// Lua_Rover
//----------------------------------------------------------

/*
lua_State* init_lua()
{
	lua_State* state = luaL_newstate();
	luaL_openlibs(state);
	return state;
}

void close_lua(lua_State* state)
{
	if(state)
		lua_close(state);
}
*/

void error(lua_State* lua_state,
		   const std::string additional_info = std::string())
{
	std::string err_msg = lua_tostring(lua_state, -1);
	lua_pop(lua_state, 1);
	throw dsLuaError(dsLuaError::LUA_GENERIC_ERROR, additional_info + err_msg);
}

void run_script(lua_State* lua_state, const char* filename)
{
	if(luaL_loadfile(lua_state, filename)
	   || lua_pcall(lua_state, 0, LUA_MULTRET, 0))
	{
		error(lua_state);
	}
}

// adapted from
// http://www.digitalpeer.com/id/simple
std::vector<std::string> get_field_names(const std::string& names)
{
	std::vector<std::string> fields;
	std::string delim(".");
	std::string::size_type last_pos = 0;
	std::string::size_type pos = 0;
	
	while(true)
	{
		pos = names.find_first_of(delim, last_pos);
		if( pos == std::string::npos )
		{
			fields.push_back(names.substr(last_pos));
			break;
		}
		else
		{
			fields.push_back(names.substr(last_pos, pos - last_pos));
			last_pos = pos + 1;
		}
	}
	return fields;
}

bool to_integer(const std::string& str, int& i)
{
	std::istringstream istr(str);
	if (!(istr >> i))
	{
		return false;
	}
	return true;
}

/**********************************************************
 
 class dsLuaError
 
 **********************************************************/

dsLuaError::dsLuaError(ErrorCode error_code, std::string msg)
: error_msg_(msg), error_code_(error_code)
{}

const char* dsLuaError::what() { return error_msg_.c_str(); }

/**********************************************************
 
 class dsLuaGlobal
 
 **********************************************************/
dsLuaGlobal::dsLuaGlobal(const char* filename) throw(dsLuaError)
{
	lua_state_ = GetLuaManager()->mainState_;
	run_script(lua_state_, filename);
}

dsLuaGlobal::dsLuaGlobal(const dsLuaGlobal& rhs)
: lua_state_(rhs.lua_state_)
{
	rhs.lua_state_ = 0;
}

dsLuaGlobal::~dsLuaGlobal()
{
	//close_lua(lua_state_);
}

dsLuaObject dsLuaGlobal::operator[](const char* name) throw(dsLuaError)
{
	return dsLuaObject(lua_state_, name);
}

dsLuaObject dsLuaGlobal::get(const char* name) throw(dsLuaError)
{
	dsLuaObject lo(lua_state_);
	const std::vector<std::string>& fields = get_field_names(name);
	if (!fields[0].empty())
	{
		lua_getglobal(lua_state_, "_G");
		lua_getfield(lua_state_, -1, fields[0].c_str());
		lo.base_stack_count_ = 2;
		lo.last_key_ = fields[0];
		if (lua_isnil(lua_state_, -1))
			throw dsLuaError(dsLuaError::LUA_GENERIC_ERROR,
						   fields[0] + " is Not Defined");
	}
	
	lo.stack_count_ = 0;
	for(int i = 1; i < fields.size(); i++)
	{
		int index;
		if (to_integer(fields[i], index))
		{
			lo[index];
		}
		else
		{
			lo[fields[i].c_str()];
		}
		lo.base_stack_count_ += lo.stack_count_;
		lo.stack_count_ = 0;
	}
	return lo;
}

/**********************************************************
 
 class dsLuaObject
 
 **********************************************************/
dsLuaObject::dsLuaObject(lua_State* state)
: lua_state_(state), base_stack_count_(0)
, stack_count_(0), global_name_(), last_key_()
{}

dsLuaObject::dsLuaObject(lua_State* state, const char* name) throw(dsLuaError)
: lua_state_(state), stack_count_(0)
, global_name_(name)
, last_key_(name)
{
	lua_getglobal(lua_state_, "_G");
	lua_getfield(lua_state_, -1, name);
	/*
	 if (lua_isnil(lua_state_, -1))
	 throw dsLuaError(dsLuaError::LUA_GENERIC_ERROR,
	 global_name_ + " is Not Defined");
	 */
	base_stack_count_ = 2;
}

dsLuaObject::dsLuaObject(const dsLuaObject& rhs)
: lua_state_(rhs.lua_state_)
, base_stack_count_(rhs.base_stack_count_ + rhs.stack_count_)
, stack_count_(0)
, global_name_(rhs.global_name_)
, last_key_(rhs.last_key_)
{
	rhs.base_stack_count_ = 0;
}

dsLuaObject::~dsLuaObject()
{
	lua_pop(lua_state_, base_stack_count_);
}

dsLuaObject& dsLuaObject::operator[](const char* name) throw(dsLuaError)
{
	if (!lua_istable(lua_state_, -1))
		throw dsLuaError(dsLuaError::NOT_A_TABLE,
					   last_key_ + " is Not A Table");
	
	lua_getfield(lua_state_, -1, name);
	++stack_count_;
	last_key_ = name;
	return *this;
}

dsLuaObject& dsLuaObject::operator[](int index) throw(dsLuaError)
{
	if (!lua_istable(lua_state_, -1))
		throw dsLuaError(dsLuaError::NOT_A_TABLE,
					   last_key_ + " is Not A Table");
	
	lua_pushinteger(lua_state_, index);
	lua_gettable(lua_state_, -2);
	
	std::stringstream s;
	s << index;
	last_key_ = s.str();
	++stack_count_;
	return *this;
}


template<>
unsigned char dsLuaObject::val<unsigned char>() throw(dsLuaError)
{
	if (!lua_isnumber(lua_state_, -1))
		throw dsLuaError(dsLuaError::NOT_A_NUMBER,
						 last_key_ + " is Not A Number");
	
	unsigned char result = (unsigned char)lua_tointeger(lua_state_, -1);
	pop_stack();
	return result;
}

template<>
short dsLuaObject::val<short>() throw(dsLuaError)
{
	if (!lua_isnumber(lua_state_, -1))
		throw dsLuaError(dsLuaError::NOT_A_NUMBER,
						 last_key_ + " is Not A Number");
	
	short result = (short)lua_tointeger(lua_state_, -1);
	pop_stack();
	return result;
}


template<>
int dsLuaObject::val<int>() throw(dsLuaError)
{
	if (!lua_isnumber(lua_state_, -1))
		throw dsLuaError(dsLuaError::NOT_A_NUMBER,
					   last_key_ + " is Not A Number");
	
	int result = lua_tointeger(lua_state_, -1);
	pop_stack();
	return result;
}

template<>
std::string dsLuaObject::val<std::string>() throw(dsLuaError)
{
	if (!lua_isstring(lua_state_, -1))
		throw dsLuaError(dsLuaError::NOT_A_STRING,
					   last_key_ + " is Not A String");
	
	std::string result = lua_tostring(lua_state_, -1);
	pop_stack();
	return result;
}

template<>
float dsLuaObject::val<float>() throw(dsLuaError)
{
	if (!lua_isnumber(lua_state_, -1))
		throw dsLuaError(dsLuaError::NOT_A_NUMBER,
						 last_key_ + " is Not A Number");
	
	float result = (float)lua_tonumber(lua_state_, -1);
	pop_stack();
	return result;
}


template<>
double dsLuaObject::val<double>() throw(dsLuaError)
{
	if (!lua_isnumber(lua_state_, -1))
		throw dsLuaError(dsLuaError::NOT_A_NUMBER,
					   last_key_ + " is Not A Number");
	
	double result = lua_tonumber(lua_state_, -1);
	pop_stack();
	return result;
}

dsLuaObject::operator unsigned char()
{
	return val<unsigned char>();
}

dsLuaObject::operator short()
{
	return val<short>();
}

dsLuaObject::operator int()
{
	return val<int>();
}

dsLuaObject::operator std::string()
{
	return val<std::string>();
}

dsLuaObject::operator float()
{
	return val<float>();
}

dsLuaObject::operator double()
{
	return val<double>();
}

void dsLuaObject::operator=(unsigned char val)
{
	lua_pushinteger(lua_state_, val);
	lua_setfield(lua_state_, -3, last_key_.c_str());
	pop_stack();
}

void dsLuaObject::operator=(short val)
{
	lua_pushinteger(lua_state_, val);
	lua_setfield(lua_state_, -3, last_key_.c_str());
	pop_stack();
}

void dsLuaObject::operator=(int val)
{
	lua_pushinteger(lua_state_, val);
	lua_setfield(lua_state_, -3, last_key_.c_str());
	pop_stack();
}

void dsLuaObject::operator=(float val)
{
	lua_pushnumber(lua_state_, val);
	lua_setfield(lua_state_, -3, last_key_.c_str());
	pop_stack();
}

void dsLuaObject::operator=(double val)
{
	lua_pushnumber(lua_state_, val);
	lua_setfield(lua_state_, -3, last_key_.c_str());
	pop_stack();
}

void dsLuaObject::operator=(std::string val)
{
	lua_pushstring(lua_state_, val.c_str());
	lua_setfield(lua_state_, -3, last_key_.c_str());
	pop_stack();
}

size_t dsLuaObject::length()
{
	size_t len = lua_objlen(lua_state_, -1);
	pop_stack();
	return len;
}

bool dsLuaObject::isnil()
{
	bool result = lua_isnil(lua_state_, -1);
	pop_stack();
	return result;
}

bool dsLuaObject::isnumber()
{
	bool result = lua_isnumber(lua_state_, -1);
	pop_stack();
	return result;
}

bool dsLuaObject::isstring()
{
	bool result = lua_isstring(lua_state_, -1);
	pop_stack();
	return result;
}

bool dsLuaObject::istable()
{
	bool result = lua_istable(lua_state_, -1);
	pop_stack();
	return result;
}

unsigned char dsLuaObject::or_val(unsigned char value)
{
	if (lua_isnil(lua_state_, -1))
	{
		pop_stack();
		return value;
	}
	else
		return val<unsigned char>();
}

short dsLuaObject::or_val(short value)
{
	if (lua_isnil(lua_state_, -1))
	{
		pop_stack();
		return value;
	}
	else
		return val<short>();
}

int dsLuaObject::or_val(int value)
{
	if (lua_isnil(lua_state_, -1))
	{
		pop_stack();
		return value;
	}
	else
		return val<int>();
}

float dsLuaObject::or_val(float value)
{
	if (lua_isnil(lua_state_, -1))
	{
		pop_stack();
		return value;
	}
	else
		return val<float>();
}

double dsLuaObject::or_val(double value)
{
	if (lua_isnil(lua_state_, -1))
	{
		pop_stack();
		return value;
	}
	else
		return val<double>();
}

std::string dsLuaObject::or_val(const std::string& value)
{
	if (lua_isnil(lua_state_, -1))
	{
		pop_stack();
		return value;
	}
	else
		return val<std::string>();
}

void dsLuaObject::pop_stack()
{
	lua_pop(lua_state_, stack_count_);
	last_key_ = global_name_;
	stack_count_ = 0;
}
