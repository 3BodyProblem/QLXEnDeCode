# -*- coding: utf-8 -*-
"""
	@brief	Sh level2 乾隆自定义推送数据编码/解码
	@note	以独占方式处理文件
"""


############# configuration ##########################
### to do:
######################################################


import QLXEnDeCode as X


if __name__ == '__main__':
	try:
		X.RunAllDemoTest_Encode( "./QLXDemoTemplate.xml" )
		X.RunSHL2Test_Encode( "./template.2.13.xml", "./data" )
		print( '[Mission Complete!]--------------------------------' )
	except Exception as e:
		print( e )
	finally:
		print( '###################################################\n\n' )







