#ifndef _H_ELAG
#define _H_ELAG

//
// enum Elag int - ��������ֵȫ��״̬��
// >= 0 ��ʶSuccess״̬, < 0 ��ʶError״̬
//
enum Elag {
	ErrParse    = -8, //Э���������
	ErrClose    = -7, //�����ʧ��, ��ȡ���Ҳ�������
	ErrEmpty    = -6, //��������Ϊ��
	ErrTout     = -5, //������ʱ����
	ErrFd       = -4, //�ļ���ʧ��
	ErrAlloc    = -3, //�ڴ�������
	ErrParam    = -2, //�����������
	ErrBase     = -1, //�����������, ����λ�ô��󶼿�����

	SufBase     = +0, //������ȷ����
};

#endif//_H_ELAG