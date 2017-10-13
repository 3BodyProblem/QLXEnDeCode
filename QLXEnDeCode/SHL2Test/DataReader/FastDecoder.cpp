#include "FastDecoder.h"
#include "ConvertStr.h"
#include "../templateparser/templatereader.h"


TemplateReader	Global_TempalteReader;


FastDecoder::FastDecoder()
{
	m_callback = NULL;
}

FastDecoder::~FastDecoder()
{

}

int	 FastDecoder::Instance(Cb_FastDecoder *callback)
{
	m_callback = callback;

	return 1;
}

void FastDecoder::Release()
{
	m_callback = NULL;
}
//..........................................................................................
//..........................................................................................
//..........................................................................................
//..........................................................................................
//..........................................................................................
// 解析FAST协议流
int	FastDecoder::DecodeFastStream(const char * buf, int len , int tag35)
{
	L2_FAST_MSG * pTemplate = GetTemplate(tag35);
	if (NULL == pTemplate)
	{
		//比较严重的ERROR
		printf("error template id=%d \n", tag35);
		return -1;
	}

    // 首先复位动态opval
    pTemplate->reset_dynamic_opval();

	int npos =0; 
	int nsize=0;
	int noffset =0;
	while(npos < len)
	{
		nsize = DecodeFastMsg(buf + noffset, len - noffset);
		if (nsize <0)
		{
			break; //如果有处理出错，返回//pdkui
		}
		noffset += nsize;

		npos += nsize;
	}

	return npos;
}

L2_FAST_MSG * FastDecoder::GetTemplate(int tag999)
{
	L2_FAST_MSG * pTemplate = Global_TempalteReader.GetTemplateValue(tag999);

	return pTemplate;
}

// 解析One FAST协议
int FastDecoder::DecodeFastMsg(const char * buf, int len)
{
	int nsize =0;
	int noffset=0;

	unsigned char pmap[PMAPSIZE]={0};
	nsize = DecodePMAP(buf+ noffset, pmap, 1);
	noffset+= nsize;

	L2VAL temID;
	{
		temID.vt = dt_uint32;
		nsize = DecodeTemplateID(buf+noffset,&temID);
		noffset += nsize;
	}
	
	L2_FAST_MSG * pTemplate = GetTemplate(temID.I4);
	if (NULL == pTemplate)
	{
		//比较严重的ERROR
		printf("error template id=%d \n", temID.I4);
		return -1;
	}

	m_callback->OnBegin(temID.I4);

	int nCount = pTemplate->arrFields.size();
	for (int i=0; i<nCount; i++)
	{
		L2_FAST_Field * pField = pTemplate->arrFields[i];
		nsize = DecodeField(buf+noffset, pmap, pField);
		if (nsize>0)
		{
			noffset += nsize;
		}
		else if (nsize <0)
		{
			//break; //处理有错//pdkui
			return -1;
		}
	}
	m_callback->OnEnd();

	return noffset;
}

int FastDecoder::DecodeFastSeqMsg(const char * buf, L2_FAST_MSG * pTemplate)
{
	int nsize =0;
	int noffset=0;

	unsigned char pmap[PMAPSIZE]={0};
	if (pTemplate->hasMap() )
	{
		nsize = DecodePMAP(buf+ noffset, pmap, pTemplate->slotcount);
		noffset+= nsize;
	}
	else
	{
		memset(pmap, 0xFF, PMAPSIZE);
	}
	
	int nCount = pTemplate->arrFields.size();
	for (int i=0; i<nCount; i++)
	{
		nsize = DecodeField(buf+noffset, pmap, pTemplate->arrFields[i]);
		if (nsize>0)
		{
			noffset += nsize;
		}
		else if (nsize<0)
		{
			break;//处理有错//pdkui
		}
	}
	
	return noffset;
}
//..........................................................................................
//..........................................................................................
//..........................................................................................
//..........................................................................................
//..........................................................................................
int FastDecoder::DecodeTemplateID(const char *buf, L2VAL * pVal)
{
	//TemplateID(999)  
	pVal->vt = dt_int32;
	int size = DecodeInt32(buf, pr_mandatory, pVal);
	return size;
}

int FastDecoder::DecodeField(const char * buf,unsigned char * pmap, L2_FAST_Field * pField)
{
	int nsize =0;
	if (pField->id != 0)
	{
		nsize = DecodeNormalField(buf, pmap, pField);

		m_callback->OnData(pField->id, &(pField->currentvalue), pField->precision );
	}
	else
	{
		nsize = DecodeSequenceField(buf, pmap, pField);
	}
	return nsize;
}

int FastDecoder::DecodeSequenceField(const char * buf,unsigned char * pmap, L2_FAST_Field * pField)
{
	int size =0;
	int noffset=0;

	//1,根据luozn的代码改写
	{
		//(1)取出length
		L2VAL iSeqLen;
		iSeqLen.vt = dt_uint32; //深圳的FAST是uint32//pdkui
		size = DecodeInt32(buf +noffset, pr_optional, &iSeqLen);
		noffset += size;

		m_callback->OnData(pField->pSequence->TID, &iSeqLen, 0);

		for (int i=0; i< iSeqLen.I4; i++)
		{
			size = DecodeFastSeqMsg(buf +noffset, pField->pSequence);
			if (size>0)
			{
				noffset += size;
			}
			else if (size <0)
			{
				break; //如果出错//pdkui
			}
		}
	}

	//2,FAST1.1协议上?
	{

	}


	//3,上交所培训资料上拷的
	/*
	{
		if (pField->slot <0 || GetPAMPSlot(pmap, pField->slot))
		{
			//(1)取出length
			L2VAL iSeqLen;
			iSeqLen.vt = dt_uint32;
			size = DecodeInt32(buf +noffset, pr_mandatory, &iSeqLen);
			noffset += size;
			
			if (m_pOnParseField)
			{
				m_pOnParseField(pField->pSequence->TID, &iSeqLen);
			}
			
			//(2)
			for (int i=0; i<iSeqLen.I4; i++)
			{
				size = DecodeFastSeqMsg(buf +noffset, pField->pSequence);
				noffset += size;
			}
		}
	}
	*/

	return noffset;
}

int FastDecoder::DecodeNormalField(const char * buf,unsigned  char  * pmap, L2_FAST_Field * pField)
{
	int size=0;
	switch (pField->op)
	{
	case op_none:
		{
			size = Fast_Decode_Field_Value(buf, pField); //
		}
		break;
	case op_constant:
		{
			if (pField->slot <0 || GetPAMPSlot(pmap, pField->slot))
			{
				pField->currentvalue = pField->opVal;
			}
			else
			{
				pField->currentvalue.vt = dt_null; //NULL
			}
		}
		break;
	case op_default:
		{
			if (GetPAMPSlot(pmap, pField->slot))
			{
				size = Fast_Decode_Field_Value(buf, pField);
			}
			else
			{
				pField->currentvalue = pField->opVal;
			}
		}
		break;


	case op_copy:
		{
			if (GetPAMPSlot(pmap, pField->slot))
			{
				size = Fast_Decode_Field_Value(buf, pField);
				if (pField->currentvalue.vt != dt_null)
				{
					pField->opVal = pField->currentvalue;	//记住作为前值//pdkui
				}
			}
			else
			{
				pField->currentvalue = pField->opVal;
			}
		
		}
		break;
	case op_increment:
		{
			if (GetPAMPSlot(pmap, pField->slot))
			{
				size = Fast_Decode_Field_Value(buf, pField);
				if (pField->currentvalue.vt != dt_null)
				{
					pField->opVal = pField->currentvalue;	//记住作为前值//pdkui
				}
			}
			else
			{
				if (pField->opVal.vt != dt_null)
				{
					if (pField->currentvalue.vt == dt_int32 || pField->currentvalue.vt == dt_uint32)
					{
						pField->currentvalue.I4 = pField->opVal.I4 +1;
						pField->opVal = pField->currentvalue; //记住作为前值//pdkui
					}
					else if (pField->currentvalue.vt == dt_int64 || pField->currentvalue.vt == dt_uint64)
					{
						pField->currentvalue.I8 = pField->opVal.I8 +1;
						pField->opVal = pField->currentvalue; //记住作为前值//pdkui
					}
				}
				else
				{
					pField->currentvalue.vt = dt_null;
				}
				
			}

		}
		break;
	case op_delta:
		{
			//前值作为基础值
			
			size = Fast_Decode_Field_Value(buf, pField);
			if (pField->opVal.vt == dt_null)
			{
				if (pField->currentvalue.vt != dt_null)
				{
					pField->opVal = pField->currentvalue;
				}
			}
			else
			{
				if (pField->currentvalue.vt == dt_null)
				{
					pField->currentvalue = pField->opVal;
				}
				else if (pField->currentvalue.vt == dt_int32 || pField->currentvalue.vt == dt_uint32)
				{
					pField->currentvalue.I4 += pField->opVal.I4;
					pField->opVal = pField->currentvalue;	//记住作为前值//pdkui
				}
				else if (pField->currentvalue.vt == dt_int64 || pField->currentvalue.vt == dt_uint64)
				{
					pField->currentvalue.I8 += pField->opVal.I4;
					pField->opVal = pField->currentvalue;	//记住作为前值//pdkui
				}
				
			}
		}
        break;
	}
	return size;
}
//..........................................................................................
//..........................................................................................
//..........................................................................................
//..........................................................................................
//..........................................................................................
int	FastDecoder::Fast_Decode_Field_Value(const char* buf, L2_FAST_Field* pField)
{
	int size=0;
	switch (pField->type)
	{
	case dt_int32:
	case dt_uint32:
		{
			pField->currentvalue.vt = pField->type;
			size= DecodeInt32(buf,pField->presence, &pField->currentvalue );
			if (size<0) 
				return -1;
		}
		break;
		
	case dt_int64:
	case dt_uint64:
		{
			pField->currentvalue.vt = pField->type;
			size=DecodeInt64(buf, pField->presence, &pField->currentvalue );
			if (size<0) 
				return -1;
		}
		break;
		
	case dt_string:
		{
			pField->currentvalue.vt = pField->type;

			if (pField->typeEx == 1)
			{
				size = DecodeUNICODEString(buf, pField->presence, &pField->currentvalue );
			}
			else
			{
				size=DecodeASCIIString(buf,  pField->presence, &pField->currentvalue );
			}
			if (size<0) 
				return -1;
		}
		break;
	}
	
	return size;
}

int FastDecoder::DecodePMAP(const char* buf,unsigned char *pmap,int TMsgSlot)
{
    int size=0;
    if (TMsgSlot>0)
    {
        char Inbuf[PMAPSIZE]={0};
        size=DecodeBytes(buf, Inbuf, PMAPSIZE);

        //重整一下
        //(0)110 0001 (1)100 0000 ===> [110 0001] [1000000]
        //重整的结果是往左对齐:[1100 0011] [000000 ]

        //代码有BUG:8Byte编码，可以左移为7Byte. 如果存在56bit以上的存在位图,会有问题 //pdkui
        /*
        unsigned char cLastChar = 0;
        for(int nRetVal = size-1; nRetVal >= 0 ; --nRetVal )
        {
            pmap[nRetVal] = (Inbuf[nRetVal]<<(nRetVal+1)) | (cLastChar>>(8-nRetVal-1));
            cLastChar = Inbuf[nRetVal]<<1;
        }
        */

        //pdkui 2013-4-28
        int offset =0;
        int i=0;
        int opp=0;
        unsigned char cNext = 0;
        unsigned char cChar =0;
        do
        {
            opp = offset%7;
            cChar = Inbuf[i];
            if (i<size)
            cNext = Inbuf[i+1];
            else
            cNext =0;

            pmap[offset++] = cChar<<(opp+1)| cNext>>(6-opp);
            if (opp ==6)
            {
                i++;
            }
            i++;
        } while (i<size);
    }
    return size;
} 

bool FastDecoder::GetPAMPSlot(unsigned char * pmap, int slot)
{
	int n = slot;

	int nBit = n&0x7;

	unsigned int result = (pmap[n/8]) & ( 0x01<<(7-nBit) );
	return result != 0;
}
//..........................................................................................
//..........................................................................................
//..........................................................................................
//..........................................................................................
//..........................................................................................
//按照Stop bit 规则获取字符序列
int	FastDecoder::DecodeBytes(const char* buf, char* b,int size)
{
	for (int i=0;i<size;i++)
	{
		if ( (buf[i]&0x80) != 0 ) 
		{
			b[i]=buf[i] & 0x7f;
			return i+1;
		}
		else
		{
			b[i]=buf[i];
		}

	}

	return -1;
}

//解析int32 和uint32 
int	FastDecoder::DecodeInt32(const char* buf, FIELDPRESENCE presence, L2VAL *pVal)
{
	char b[8]={0};
	char sign_mask=0x00;
	int size= DecodeBytes(buf, b, 5);
	if (size>0)
	{
		if (pVal->vt== dt_int32 ) 	
		{
			sign_mask = 0x40;
		}
		
		if (b[0] & sign_mask)	
		{
			pVal->I4 = -1;
		}
		else	
		{
			pVal->I4 = 0;
		}
		
		for (int i = 0 ; i< size ; i ++)
			pVal->I4= (pVal->I4 << 7) | b[i];
		
		if (presence==pr_optional) 
		{
			if (pVal->I4>0)
			{
				pVal->I4--;
			}
			else if (pVal->I4==0)
			{
				pVal->vt=dt_null;
			}
		}
		
	}
	return size;
}

//解析int64 和uint64
int	FastDecoder::DecodeInt64(const char* buf, FIELDPRESENCE presence, L2VAL *pVal)
{
	char b[12]={0};
	char sign_mask=0x00;
	int size= DecodeBytes(buf,b,10);
	if (size>0)
	{
		if (pVal->vt== dt_int64)	
			sign_mask=0x40;
		
		if (b[0] & sign_mask) 
			pVal->I8 = -1;
		else	
			pVal->I8 = 0;
		
		for (int i = 0 ; i< size ; i ++)
		{
			pVal->I8= (pVal->I8<< 7) | b[i];
		}
		
		if (presence== pr_optional) 
		{
			if (pVal->I8 > 0)	
			{
				pVal->I8--;
			}
			else if (pVal->I8 == 0)
			{
				pVal->vt = dt_null;
			}
		}
		
	}
	
	return size;
}

int	FastDecoder::DecodeASCIIString(const char* buf, FIELDPRESENCE presence, L2VAL *pVal)
{
	char b[MAXTAGLEN];
	int size= DecodeBytes(buf,b,MAXTAGLEN);
	if (size>0)
	{
		memcpy(pVal->String,b,size);
		pVal->String[size]=0;
		if (presence==pr_optional)
		{
			if (size==1 && pVal->String[0] == 0)
			{
				pVal->vt = dt_null;
			}

		}

	}
	return size;
}

int FastDecoder::DecodeUNICODEString(const char * buf, FIELDPRESENCE fpr, L2VAL * pVal)
{
	L2VAL iStrLen;
	iStrLen.vt = dt_int32;
	int size = DecodeInt32(buf, pr_mandatory, &iStrLen);
	if (size >0)
	{
		if (iStrLen.I4 >0)
		{
			char szUTF8[128]={0};
			memcpy(szUTF8, buf+size, iStrLen.I4);

			vector<char> aa;
			UTF8ToMB(aa, szUTF8, iStrLen.I4);
			int i=0;
			for (i=0; i<aa.size(); i++)
			{
				pVal->String[i] = aa[i];
			}
			pVal->String[i]=0;
			
			size += iStrLen.I4;
		}
	
	}
	
	return size;
}



