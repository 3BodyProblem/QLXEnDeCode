#ifndef __QLX_PYD_EXPORT_H__
#define __QLX_PYD_EXPORT_H__

#if _DEBUG
#define	EXPORT_PYTHON_PYD_MODULE			1			///< �Ƿ���Ҫ�ṩ֧��pyd����֧�ֵ��趨
#endif

#ifdef	EXPORT_PYTHON_PYD_MODULE


#define	BOOST_PYTHON_STATIC_MODULE						///< ֧�־�̬��
#define	BOOST_PYTHON_STATIC_LIB							///< ���ӵ�python.lib��̬��
#include "boost/python.hpp"								///< boost.pythonͷ�ļ�֧��
#include "boost/utility.hpp"
#include "boost/noncopyable.hpp"
#include "boost/python/class.hpp"
#include "boost/python/module.hpp"
#include "boost/python/wrapper.hpp"						///< boost.python
#include "boost/shared_ptr.hpp"




#endif

#endif






