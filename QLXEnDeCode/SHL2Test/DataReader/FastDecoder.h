
#ifndef _FAST_DECODER_H
#define _FAST_DECODER_H

#include "../templateParser/FastBase.h"

class FastDecoder
{
public:
	FastDecoder();
	virtual ~FastDecoder();

public:
	int			Instance(Cb_FastDecoder *callback);
	void		Release();
public:	
	int			DecodeFastStream(const char * buf, int len , int tag35);

	L2_FAST_MSG * GetTemplate(int tag999);

	int			DecodeFastMsg(const char * buf, int len);

	int			DecodeFastSeqMsg(const char * buf, L2_FAST_MSG * pTemplate);
protected:
	int			Fast_Decode_Field_Value(const char * buf,L2_FAST_Field* pField);

	bool		GetPAMPSlot(unsigned char * pmap, int slot);

	int			DecodeTemplateID(const char *buf, L2VAL * pVal);

	int			DecodeField(const char * buf,unsigned char * pmap, L2_FAST_Field * pField);

	int			DecodeNormalField(const char * buf,  unsigned char  * pmap, L2_FAST_Field * pField);
	int			DecodeSequenceField(const char * buf, unsigned char * pmap, L2_FAST_Field * pField);
private:
	// @return size =0 表示没有 >0 表示ok <0表示出错
	int			DecodePMAP( const char* buf, unsigned char *pmap,int TMsgSlot);

	// @return size, <0表示出错
	int			DecodeInt32(const char* buf, FIELDPRESENCE fpr, L2VAL *pVal);
	int			DecodeInt64(const char* buf, FIELDPRESENCE fpr, L2VAL *pVal);
	int			DecodeASCIIString(const char* buf, FIELDPRESENCE fpr, L2VAL *pVal);
	int			DecodeUNICODEString(const char * buf, FIELDPRESENCE fpr, L2VAL * pVal);

	// @return size, <0 表示出错
	int			DecodeBytes(const char* buf, char* b,int size);

private:
	Cb_FastDecoder		*m_callback;

};



#endif