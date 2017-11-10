#ifndef __QLX_PYD_EXPORT_H__
#define __QLX_PYD_EXPORT_H__

#if _DEBUG
#define	EXPORT_PYTHON_PYD_MODULE			1			///< 是否需要提供支持pyd导出支持的设定
#endif

#ifdef	EXPORT_PYTHON_PYD_MODULE


#define	BOOST_PYTHON_STATIC_MODULE						///< 支持静态库
#define	BOOST_PYTHON_STATIC_LIB							///< 链接到python.lib静态库
#include "boost/python.hpp"								///< boost.python头文件支持
#include "boost/utility.hpp"
#include "boost/noncopyable.hpp"
#include "boost/python/class.hpp"
#include "boost/python/module.hpp"
#include "boost/python/wrapper.hpp"						///< boost.python
#include "boost/shared_ptr.hpp"




#endif

#endif






