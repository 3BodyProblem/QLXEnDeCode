#include "./templateReader.h"
#include <conio.h>


FIELDTYPE	fast_getType(const char * ptypebuf)
{
	FIELDTYPE dt = dt_null;
	if (NULL == ptypebuf )
	{
		return dt;
	}
	
	int len = strlen(ptypebuf);
	if (len ==0)
	{
		return dt;
	}
	
	if (stricmp(ptypebuf,"string") ==0 )
	{
		dt = dt_string;
	}
	else if (stricmp(ptypebuf,"length")==0)
	{
		dt = dt_int32;
	}
	else if (stricmp(ptypebuf,"int32")==0)
	{
		dt= dt_int32;
	}
	else if (stricmp(ptypebuf,"uInt32") ==0)
	{
		dt = dt_uint32;
	}
	else if (stricmp(ptypebuf,"int64")==0)
	{
		dt = dt_int64;
	}
	else if (stricmp(ptypebuf,"uInt64")==0)
	{
		dt = dt_uint64;
	}
	else if (stricmp(ptypebuf,"sequence")==0)
	{
		dt = dt_sequence;
	}
	return dt;
}

FIELDOP		fast_getOperator(const char * pfieldOP)
{
	FIELDOP fop = op_none;
	if (NULL == pfieldOP )
	{
		return fop;
	}
	
	int len = strlen(pfieldOP);
	if (len ==0)
	{
		return fop;
	}
	
	if (stricmp(pfieldOP,"constant")==0)
	{
		fop = op_constant;
	}
	else if (stricmp(pfieldOP,"copy")==0)
	{
		fop = op_copy;
	}
	else if (stricmp(pfieldOP,"delta")==0 )
	{
		fop = op_delta;
	}
	else if (stricmp(pfieldOP,"default")==0)
	{
		fop = op_default;
	}
	else if (stricmp(pfieldOP,"increment")==0)
	{
		fop = op_increment;
	}
	return fop;
}

FIELDPRESENCE	fast_getPresence(const char * pPresence)
{
	FIELDPRESENCE pr = pr_mandatory;
	if (NULL == pPresence)
	{
		return pr;
	}
	
	int len = strlen(pPresence);
	if (0 == len)
	{
		return pr;
	}
	
	if (stricmp(pPresence,"optional") ==0)
	{
		pr = pr_optional;
	}
	return pr;
}

//0表示ascii
//1表示unicode
int			fast_getCharset(const char * pCharset)
{
	if (NULL == pCharset)
	{
		return 0;
	}
	int len = strlen(pCharset);
	if (0 == len)
	{
		return 0;
	}

	if (stricmp(pCharset, "unicode") ==0)
	{
		return 1;
	}
	return 0;
}
//.............................................................................................
//.............................................................................................
//.............................................................................................
//.............................................................................................
//.............................................................................................
TemplateReader::TemplateReader()
{
}

TemplateReader::~TemplateReader()
{
	Release();
}

int TemplateReader::Instance( const char * file )
{
	Release();

	return LoadTemplate( file );
}

void	TemplateReader::Release()
{
	int i=0;
	int nCount = arrTemplates.size();
	for (i=0; i<nCount; i++)
	{
		delete arrTemplates[i];
	}
	
	arrTemplates.clear();
}

int		TemplateReader::LoadTemplate( const char * file)
{
	CMarkupSTL xmlparser;
	
	m_teplateFile = file;
	if( !xmlparser.Load( m_teplateFile.c_str() ) )
	{
		return -1;
	}
	
	if( !xmlparser.IsWellFormed() )
	{
		return -2;
	}
	
	//parser template
	L2_FAST_MSG * pTemplate =NULL;
	while( xmlparser.FindChildElem( "template" ) )
	{
		pTemplate = new L2_FAST_MSG();
		if (NULL == pTemplate)
		{
			continue;
		}
		
		//pTemplate->SetName(xmlparser.GetChildAttrib( "name" ).c_str() );
		pTemplate->TID = atol( xmlparser.GetChildAttrib( "id" ).c_str() );
		
		//1,分析里面的Field
		xmlparser.IntoElem();	//into Template
		string strname;
		while (xmlparser.FindChildElem(NULL) ) //find field
		{
			xmlparser.IntoElem(); //into Field
			parseonefield(xmlparser, pTemplate);
			xmlparser.OutOfElem();
		}
		
		xmlparser.OutOfElem();
		
		//2, 保存模版
		arrTemplates.push_back(pTemplate);
	}
	
	//3, 统计存在位图
	calculateslot();

	return 1;
}

L2_FAST_MSG*  TemplateReader::GetTemplateValue( long id )
{
	L2_FAST_MSG * pTemplate =NULL;

	int nsize = arrTemplates.size();
	for (int i=0; i<nsize; i++)
	{
		if (arrTemplates[i]->TID == id)
		{
			pTemplate = arrTemplates[i];
			break;
		}
	}
	return pTemplate;

}


//.......................................................................................
//.......................................................................................
//.......................................................................................
//.......................................................................................
//.......................................................................................
//.......................................................................................
void	TemplateReader::parseonefield(CMarkupSTL & xmlparser, L2_FAST_MSG* pTemplate)
{
	string strname = xmlparser.GetTagName();
	
	L2_FAST_Field * pField = new L2_FAST_Field();
	pField->type = fast_getType(strname.c_str() );

	if (pField->type == dt_sequence)
	{//sequence
		L2_FAST_MSG * pMsg = new L2_FAST_MSG();
		pMsg->TID = atol(xmlparser.GetAttrib("id").c_str() ) ;
		parseonesequence(xmlparser, pMsg);
		pField->pSequence = pMsg;
	}
	else
	{//field
		pField->id = atol( xmlparser.GetAttrib("id").c_str() );
		pField->presence = fast_getPresence(xmlparser.GetAttrib("presence").c_str() );
		pField->shift = atol( xmlparser.GetAttrib("decimalPlaces").c_str() );
        //pField->precision = atol(xmlparser.GetAttrib("precision").c_str());
        pField->precision = atol(xmlparser.GetAttrib("decimalPlaces").c_str());
		
		if (pField->type == dt_string)
		{//charset
			pField->typeEx = fast_getCharset(xmlparser.GetAttrib("charset").c_str() );
		}
		string strOperator ="";
		if (xmlparser.FindChildElem()) //find operator
		{
			strOperator= xmlparser.GetChildTagName();
			if (strOperator != "")
			{
				pField->opVal = xmlparser.GetChildAttrib("value").c_str() ; 
			}
		}
		
		pField->op = fast_getOperator(strOperator.c_str());
		if (pField->type != dt_null && pField->type != dt_string && pField->type != dt_sequence)
		{
			pField->opVal.ToInt32();
			if( pField->op == op_increment && pField->opVal == 0 )
			{
				pField->opVal = 1;
			}
		}
	}
	
	pTemplate->arrFields.push_back(pField);
}
	

void	TemplateReader::parseonesequence(CMarkupSTL & xmlparser,L2_FAST_MSG * pSequence)
{
	while (xmlparser.FindChildElem(NULL) ) //find field
	{
		xmlparser.IntoElem(); //into sequence
		parseonefield(xmlparser, pSequence);
		xmlparser.OutOfElem();
	}
	
	
	//把length 的信息，从child中取出 //上海使用
	int nChilds = pSequence->arrFields.size();
	if (nChilds>0)
	{
		L2_FAST_Field * pSeqLen = pSequence->arrFields[0];
		pSequence->TID = pSeqLen->id;

		pSequence->arrFields.erase(pSequence->arrFields.begin() );
	}
	
}

void TemplateReader::calculateslot()
{
	int nTemplateCount = arrTemplates.size();
	L2_FAST_MSG * pTemlate = NULL;

	int i=0;
	
	for ( i=0; i< nTemplateCount; i++)
	{
		pTemlate = arrTemplates[i];

		pTemlate->slotcount =1; ////(上海FAST 有个999，占第一个位置,所以后续从1开始计算)//pdkui

		countseqtemp(pTemlate);

	}
}

int TemplateReader::countseqtemp(L2_FAST_MSG * pTemplate)
{
	L2_FAST_Field * pField = NULL;
	int nFieldCount=pTemplate->arrFields.size();

	int j=0;
	for (j=0; j < nFieldCount; j++)
	{
		pField = pTemplate->arrFields[j];
		if (pField->id != 0)
		{
			if (pField->op == op_none || (pField->op == op_constant && pField->presence == pr_mandatory) ||pField->op == op_delta)
			{
				//不占位
				pField->slot = -1;
			}
			else
			{
				//占位
				pField->slot = pTemplate->slotcount; 
				
				pTemplate->slotcount++;
			}
		}
		else
		{
			L2_FAST_MSG * pMsg = pField->pSequence;
			if (pMsg)
			{
				pMsg->slotcount =0;
				countseqtemp(pMsg);

				//pdkui 
				//
				/*
 				if (pMsg->slotcount >0)		//如果Sequence 里面有占位的情况，那么外层的Template 或Sequence 的pmap里面也需要占位。
 				{
 					pField->slot = pTemplate->slotcount;
 
 					pTemplate->slotcount++ ;
				}
				*/

			}
			
		}
		
	}
	
	return 1;
}
//.......................................................................................
//.......................................................................................
//.......................................................................................
//.......................................................................................
//.......................................................................................
//.......................................................................................
#ifdef _DEBUG
void	TemplateReader::PrintTemplate()
{
	int nCount = arrTemplates.size();
	printf("\n\nprint template count = %d begin\n\n", nCount);

	L2_FAST_MSG * pMsg =NULL;
	for (int i=0; i<nCount; i++)
	{
		pMsg = arrTemplates[i];
		
		printf("========= template id=%d slotcount=%d=========\n",pMsg->TID, pMsg->slotcount);
		PrintOneTemplate(pMsg, 1);
		printf("template end!\n");

		_getch();
	}

	printf("\n\nprint template end\n\n");

}

void	TemplateReader::PrintOneTemplate(L2_FAST_MSG * pMsg, int nStep)
{
	int nCount = pMsg->arrFields.size();
	for (int i=0; i<nCount; i++)
	{
		PrintOneField(pMsg->arrFields[i], nStep);
	}
	
}

void	TemplateReader::PrintOneField(L2_FAST_Field * pField, int nStep)
{
	int i=0;
	for (i=0; i<nStep; i++)
	{
		printf("\t");
	}

	if (pField->id != 0)
	{
		printf("id=%d", pField->id );printf("\t");
		switch (pField->type)
		{
		case dt_null:printf("dt_null");break;
		case dt_uint32:printf("dt_uint32");break;
		case dt_int32:printf("dt_int32");break;
		case dt_uint64:printf("dt_uint64");break;
		case dt_int64:printf("dt_int64");break;
		case dt_string:printf("dt_string");break;
		case dt_sequence:printf("dt_sequence");break;
		default:break;
		}
		printf("\t");
		switch (pField->presence)
		{
		case pr_mandatory:printf("mandatory");break;
		case pr_optional:printf("optional");break;
		}
		printf("\t");
		switch (pField->op)
		{
		case op_none:printf("op_none");break;
		case op_default:
			{
				if (pField->type == dt_string)
				{
					printf("op_default %s", pField->opVal.String);
				}
				else
				{
					printf("op_default %d", pField->opVal.I4);
				}
			}
			
			break;
		case op_constant:
			{
				if (pField->type == dt_string)
				{
					printf("op_constant %s",pField->opVal.String);
				}
				else
				{
					printf("op_constant %d",pField->opVal.I4);
				}
			}
	
			break;
		case op_copy:printf("op_copy");break;
		case op_increment:printf("op_increment");break;
		}
		printf("\t");
		printf("shift=%d slot=%d", pField->shift, pField->slot);
	}
	else
	{
		L2_FAST_MSG * pMsg = pField->pSequence;
		printf("sequence begin slotcount=%d\n", pMsg->slotcount);
		if (pMsg)
		{
			PrintOneTemplate(pMsg, nStep+1);
		}

		for (i=0; i<nStep; i++)
		{
			printf("\t");
		}
		printf("sequence end\n");
	}
	printf("\n");
}


#endif
