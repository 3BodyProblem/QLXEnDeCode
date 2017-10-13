
#include "FastBase.h"
#include <algorithm>
#include <functional>

//...................................................................................
//...................................................................................
//...................................................................................
//...................................................................................

L2VAL::L2VAL()
{
	vt = dt_null;
	memset(String, 0, MAXTAGLEN);
}

L2VAL::~L2VAL()
{
	
}

L2VAL::L2VAL(long	lValue)
{
	vt = dt_int32;
	I4 = lValue;
}

L2VAL::L2VAL(__int64 iValue)
{
	vt = dt_int64;
	I8 = iValue;
}

L2VAL::L2VAL(const char * str)
{
	vt = dt_string;
	if( str == NULL )
	{
		memset( String, 0, sizeof(String) );
	}
	else
	{
		strncpy( String, str, sizeof(String)-1 );
	}
}

L2VAL & L2VAL::operator = ( const char * str )
{
	vt = dt_string;
	if( str == NULL )
	{
		memset( String, 0, sizeof(String) );
	}
	else
	{
		strncpy( String, str, sizeof(String)-1 );
	}
	return *this;

}

L2VAL & L2VAL::operator = ( const long lValue )
{
	vt = dt_int32;
	I4 = lValue;

	return *this;
}

L2VAL & L2VAL::operator = ( L2VAL & b )
{
	vt = b.vt;
	memcpy( String, b.String, sizeof(String) );
	return *this;
}

L2VAL & L2VAL::SetInt64( const __int64 llValue )
{
    vt = dt_int64;
    I8 = llValue;
    return *this;
}

L2VAL& L2VAL::SetString(const char* str) {
	vt = dt_string;
	if (str == NULL) {
		memset(String, 0, sizeof String);
    } else {
        strncpy(String, str, sizeof String - 1);
	}
    return *this;
}

L2VAL& L2VAL::SetString(const char* str, size_t len) {
	vt = dt_string;
	if (str == NULL) {
		memset(String, 0, sizeof(String));
    } else {
        strncpy(String, str, sizeof(String) - 1 < len ? sizeof(String) - 1 : len);
	}
    return *this;
}

void L2VAL::ToInt32()
{
	vt = dt_int32;
	I4 = atol( String );
}

void L2VAL::clear() {
    vt = dt_null;
    memset(String, 0, sizeof(String));
}


bool L2VAL::operator == (const long & lValue)
{
	if (I4 == lValue)
	{
		return true;
	}
	return false;
}

//...................................................................................
//...................................................................................
//...................................................................................
//...................................................................................
L2_FAST_Field::L2_FAST_Field()
{
	id =0;
	op=op_none;
	type = dt_null;
	presence = pr_mandatory;
	slot =0;
	shift =0;

	pSequence=NULL;

	typeEx =0;
}

L2_FAST_Field::~L2_FAST_Field()
{
	if (pSequence)
	{
		delete pSequence;
		pSequence =0;
	}
}


//...................................................................................
//...................................................................................
//...................................................................................
//...................................................................................
L2_FAST_MSG::L2_FAST_MSG()
{
	//memset(pmap, 0, PMAPSIZE);
	//slot=0;

	TID=0;
	slotcount=0;
}

L2_FAST_MSG::~L2_FAST_MSG()
{
	int i=0;
	int nCount =arrFields.size();
	for (i=0; i<nCount; i++)
	{
		delete arrFields[i];
	}

	arrFields.clear();
}


bool L2_FAST_MSG::hasMap()
{
	return slotcount>0;
}

int L2_FAST_Field::reset_dynamic_opval() 
{
    if (type == dt_sequence) 
	{
        pSequence->reset_dynamic_opval();
    } 
	else if (is_dynamic_opval()) 
	{
        opVal.clear();
        currentvalue.clear();
    }
    return 0;
}

int L2_FAST_MSG::reset_dynamic_opval() 
{
    for_each(arrFields.begin(),
             arrFields.end(),
             mem_fun(&L2_FAST_Field::reset_dynamic_opval));
    return 0;
}

bool L2_FAST_Field::is_dynamic_opval() 
{
    if (type == dt_sequence) 
	{
        return false;
    }
    return (op == op_copy) || (op == op_increment) || (op == op_delta);
}

std::ostream & operator<<(std::ostream &os, const L2VAL &val) 
{
    if (val.vt == dt_null) 
	{
        os << "<null>";
    } 
	else if ((val.vt == dt_int32) || (val.vt == dt_uint32)) 
	{
        os << val.I4;
    } 
	else if ((val.vt == dt_int64) || (val.vt == dt_uint64)) 
	{
        char buf[20];
        sprintf(buf,"%I64d", val.I8);
        os << buf;
    } 
	else if (val.vt == dt_string) 
	{
        os << val.String;
    } 
	else if (val.vt == dt_sequence) 
	{
        os << "<sequence>";
    } 
	else 
	{
        os << "<unknown:" << val.vt << ">";
    }
    return os;
}
