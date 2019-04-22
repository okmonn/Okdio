#include "Okdio/Okdio.h"
#include "Okdio/Effector.h"
#include <Windows.h>
#define KEY

int main()
{
	//�G�t�F�N�^�[�̐���
	Effector effe(10, 2);
	//���C�u�����{��
	Okdio s("mtgx2.wav", &effe);
	//�G�t�F�N�g����
	TimeStretch time(2.0f);
	//�G�t�F�N�g�̒ǉ�
	//s.PushEffect(&time);
	//�Đ�
	s.Play();

	//�Đ��I���܂Ń��[�v
#ifdef KEY
	while (!(GetKeyState(VK_ESCAPE) & 0x80))
#else
	while(!s.IsPlayEnd())
#endif
	{

	}

	s.Stop();

	//�g�`�f�[�^�폜
	snd::Delete(s.GetName());

	return 0;
}
