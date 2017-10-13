# -*- coding: utf-8 -*-
"""
	@brief	encode/decode模块单元测试脚本

	@usage
		运行内置测试数据
		X.RunAllDemoTest_Encode( "./QLXDemoTemplate.xml", "*Demo*" )

		运行外部导入的上海Level2数据
		X.RunAllDemoTest_Encode( "./QLXDemoTemplate.xml", "*SHL2*", "./template.2.13.xml", "./fastdata" )
"""


############# configuration ##########################
### to do:
######################################################


import QLXEnDeCode as X


if __name__ == '__main__':
	try:
		X.RunAllDemoTest_Encode( "./QLXDemoTemplate.xml", "*SHL2*", "./template.2.13.xml", "./fastdata" )
		print( '[Mission Complete!]--------------------------------' )
	except Exception as e:
		print( e )
	finally:
		print( '###################################################\n\n' )







