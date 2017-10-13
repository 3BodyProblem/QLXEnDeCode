#ifndef __SHLFAST_TEMPLATEREADER_H__
#define __SHLFAST_TEMPLATEREADER_H__


#pragma warning(disable : 4996)
#include "MarkupSTL.h"
#include "FastBase.h"


class TemplateReader
{
public:
	TemplateReader();
	virtual ~TemplateReader();
	
public:
	int			  Instance( const char * file );
	void		  Release();
	
public:
	int			  LoadTemplate( const char * file );
	
	L2_FAST_MSG*  GetTemplateValue( long id );
	
protected:
	void		  parseonefield(CMarkupSTL & xmlparser, L2_FAST_MSG* pTemplate);
	
	void		  parseonesequence(CMarkupSTL & xmlparser, L2_FAST_MSG * pSequence);
	
	void		  calculateslot();
	
	int			  countseqtemp(L2_FAST_MSG * pTemplate);

public:
	vector<L2_FAST_MSG * > arrTemplates;	//Template array 
	
protected:
	string					m_teplateFile;
	
	
#ifdef _DEBUG
public:
	void		  PrintTemplate();
protected:
	void		  PrintOneTemplate(L2_FAST_MSG * pMsg, int nStep);
	
	void		  PrintOneField(L2_FAST_Field * pField, int nStep);
#endif
	
};


#endif//__SHLFAST_TEMPLATEREADER_H__