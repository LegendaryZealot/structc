#ifndef _H_ELAG
#define _H_ELAG

//
// enum Elag int - ��������ֵȫ��״̬��
// >= 0 ��ʶSuccess״̬, < 0 ��ʶError״̬
//
enum Elag {
	EParse    = -8, //Э���������
	EClose    = -7, //�����ʧ��, ��ȡ���Ҳ�������
	EEmpty    = -6, //��������Ϊ��
	ETout     = -5, //������ʱ����
	EFd       = -4, //�ļ���ʧ��
	EAlloc    = -3, //�ڴ�������
	EParam    = -2, //�����������
	EBase     = -1, //�����������, ����λ�ô��󶼿�����

	SBase     = +0, //������ȷ����
};

#endif//_H_ELAG
