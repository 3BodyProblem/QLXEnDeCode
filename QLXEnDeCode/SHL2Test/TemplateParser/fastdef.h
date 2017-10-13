
#ifndef _FAST_BASE_DEFINE_H
#define _FAST_BASE_DEFINE_H

//��������
typedef enum tagFieldType
{
	dt_null=0,
	dt_string,
	dt_int32,
	dt_uint32,
	dt_int64,
	dt_uint64,
	dt_sequence,

}FIELDTYPE;

//���ݲ�����
typedef enum tagFieldOP
{
	op_none=0,
	op_constant,
	op_default,
	op_copy,
	op_increment,
	op_delta,
}FIELDOP;

//���ڷ�
typedef enum tagFieldPresence
{
	pr_mandatory=0,
	pr_optional,

}FIELDPRESENCE;


#endif