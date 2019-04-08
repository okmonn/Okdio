#include "Okdio/Okdio.h"
#include "Okdio/Effector.h"
#include <Windows.h>

int main()
{
	//�G�t�F�N�^�[�̐���
	Effector effe(10, 2);
	//���C�u�����{��
	Okdio s("mtgx.wav", &effe);
	//�G�t�F�N�g����
	Pan pan(90.0f);
	//�G�t�F�N�g�̒ǉ�
	s.PushEffect(&pan);
	//�Đ�
	s.Play();

	//�Đ��I���܂Ń��[�v
	while (!s.IsPlayEnd())
	{

	}

	//�g�`�f�[�^�폜
	snd::Delete(s.GetName());

	return 0;
}
