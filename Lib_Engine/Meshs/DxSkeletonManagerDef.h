#pragma once

enum EMBODY
{
	EMBODY_ROOT		= 1,	// Root���� BIP01 ����
	EMBODY_UPBODY	= 2,	// ��ü								- ���� ��ü�� �����̰� ���� ���� �����Ѵ�.
	EMBODY_DOWNBODY	= 4,	// ��ü								- ��ü�� �����ϴ� ���� ����. < �ִٸ� ���� ������ �ؾ��� >

	EMBODY_DEFAULT	= EMBODY_ROOT|EMBODY_UPBODY|EMBODY_DOWNBODY,
};