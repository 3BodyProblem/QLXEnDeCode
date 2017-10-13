
#ifndef _FAST_DEFINE_H
#define _FAST_DEFINE_H

#include "./fastdef.h"

#include <vector>
#include <string>
#include <iosfwd>
using namespace std;

//[1]Value
#define MAXTAGLEN 64
class L2VAL 
{
public:
	L2VAL();
	virtual ~L2VAL();

	//其他构造函数 
	L2VAL(__int64 iValue);
	L2VAL(long	lValue);
	L2VAL(const char * str);

	//Operator 重载 
	L2VAL & operator = ( const char * str );
	L2VAL & operator = ( const long lValue );
	L2VAL & operator = ( L2VAL & b );

    L2VAL& SetInt64(const __int64 llValue);
    L2VAL& SetString(const char * str);
    L2VAL& SetString(const char * str, size_t len);
	void ToInt32();

	bool operator == (const long & lValue);

    void clear();

public:
    FIELDTYPE	vt;
	union 
	{
		__int32 I4;
		__int64 I8;
		char String[MAXTAGLEN];
	};
};

std::ostream & operator<<(std::ostream &os, const L2VAL &val);


//[2]Field
class L2_FAST_MSG;
class L2_FAST_Field
{
public:
	L2_FAST_Field();
	virtual ~L2_FAST_Field();

public:
	__int32			id;				//Field indentified
	FIELDOP			op;				//Field encoding operator
	FIELDTYPE		type;			//Field data type
	FIELDPRESENCE	presence;		//Field presence:0 mandatory 1 optional
	
	__int32			slot;			//Field presence map slot position
	__int32			shift;			//decimalPlaces
	L2VAL			opVal;			//Field operator value //附带的常量或默认值
	L2VAL			currentvalue;	//Field current value	//当前值
	
	L2_FAST_MSG*	pSequence;		//id =0 表示有sequence, !=0 表示是一个Field
	__int32			typeEx;			//补充属性
									//{在dt_string 时，用来表示(0)ASCII or (1)UNICDOE}
									//{}

    __int32         precision;      // 精度

    /**
     * 重置动态opval
     */
    int reset_dynamic_opval();

private:
    /**
     * 是否是动态的opval
     * 动态opval是指在运行时会变化的opval
     */
    bool is_dynamic_opval();
};



//[3]Message
//含义包括Sequence 和template
#define		PMAPSIZE 16

class L2_FAST_MSG
{
public:
	L2_FAST_MSG();
	virtual ~L2_FAST_MSG();
public:
	//PMAP					pmap;		//presence map
	__int32					TID;		//template identifier
	
	__int32					slotcount;  		//slot count
										//field count
	vector<L2_FAST_Field*>	arrFields;  //field array

	bool					hasMap();

    int reset_dynamic_opval();
};

// FastDecoder解析代码所需要用到的事件回调接口
class Cb_FastDecoder
{
public:
	// 开始解码一个Fast数据段
	virtual void OnBegin(int nId) {}
	// 解码出一个数据项
	virtual int OnData(int ulTagName, L2VAL * pVal, int precision) = 0;
	// 一个Fast数据段解码完成
	virtual void OnEnd() {}
};

#endif