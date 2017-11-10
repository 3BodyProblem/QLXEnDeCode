#include "PYDExport.h"
#include "Encode.h"
#include "Decode.h"
#include "UnitTest.h"


#ifdef	EXPORT_PYTHON_PYD_MODULE			///< 判断是否需要提供支持pyd导出支持


void PrintVersion()
{
	::printf( "Module Version : %s", GetModuleVersion() );
}


BOOST_PYTHON_FUNCTION_OVERLOADS( RunAllDemoTest_Encode_Overloads, RunAllDemoTest_Encode, 1, 7 )

/**
 * @brief			导出 python module
 * @note			官方做法的定义如下：	BOOST_PYTHON_MODULE(QLXEnDeCode)
 */
void BOOST_PP_CAT(init_module_,QLXEnDeCode)();                               
extern "C" __declspec(dllexport) _BOOST_PYTHON_MODULE_INIT(QLXEnDeCode)
{
	using namespace boost::python;
	def( "PrintVersion", PrintVersion, "Print module version of \"QLXEnDeCode.pyd\"." );
	def( "RunAllDemoTest_Encode", RunAllDemoTest_Encode, RunAllDemoTest_Encode_Overloads() );
/*								, "Active all encode unit tests. \n \
								def RunAllDemoTest_Encode( pszEnDeCodeXml, sParam1, sParam2, sParam3, sParam4, sParam5, sParam6 ):" );
*/
/*	def( "GetMarketControlApi", (I_QLXMarketControl*(*)())GetMarketControlApi, return_value_policy<manage_new_object>(), "Shanghai Level2 fast/step market data analyze module. \nThis function return a module control pointer of \'I_QLXMarketControl*\'. " );

	class_< I_DataPush_Wrapper, boost::noncopyable >( "I_QLXMarketDataPush" )
		.def( "OnRealTimeData", pure_virtual( &I_QLXMarketDataPush::OnRealTimeData ) )
		.def( "OnFullImageData", pure_virtual( &I_QLXMarketDataPush::OnFullImageData ) )
//		.def( "OnMarketDataStatus", pure_virtual( &I_QLXMarketDataPush::OnMarketDataStatus ) )
//		.def( "OnLog", pure_virtual( &I_QLXMarketDataPush::OnLog ) )
		;

	class_< I_Control_Wrapper, boost::noncopyable >( "I_QLXMarketControl", no_init )
		.def( "Initialize", pure_virtual( &I_QLXMarketControl::Initialize ) )
		.def( "Release", pure_virtual( &I_QLXMarketControl::Release ) )
		.def( "RequestFullImage", pure_virtual( &I_QLXMarketControl::RequestFullImage ) )
		.def( "Command", pure_virtual( &I_QLXMarketControl::Command ) )
//		.def( "GetVersion", pure_virtual( &I_QLXMarketControl::GetVersion ) )
		;
*/
}


#endif









