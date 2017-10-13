
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

	//�������캯�� 
	L2VAL(__int64 iValue);
	L2VAL(long	lValue);
	L2VAL(const char * str);

	//Operator ���� 
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
	L2VAL			opVal;			//Field operator value //�����ĳ�����Ĭ��ֵ
	L2VAL			currentvalue;	//Field current value	//��ǰֵ
	
	L2_FAST_MSG*	pSequence;		//id =0 ��ʾ��sequence, !=0 ��ʾ��һ��Field
	__int32			typeEx;			//��������
									//{��dt_string ʱ��������ʾ(0)ASCII or (1)UNICDOE}
									//{}

    __int32         precision;      // ����

    /**
     * ���ö�̬opval
     */
    int reset_dynamic_opval();

private:
    /**
     * �Ƿ��Ƕ�̬��opval
     * ��̬opval��ָ������ʱ��仯��opval
     */
    bool is_dynamic_opval();
};



//[3]Message
//�������Sequence ��template
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

// FastDecoder������������Ҫ�õ����¼��ص��ӿ�
class Cb_FastDecoder
{
public:
	// ��ʼ����һ��Fast���ݶ�
	virtual void OnBegin(int nId) {}
	// �����һ��������
	virtual int OnData(int ulTagName, L2VAL * pVal, int precision) = 0;
	// һ��Fast���ݶν������
	virtual void OnEnd() {}
};

#endif