#include "Okdio/Okdio.h"
#include "Okdio/Effector.h"
#include <Windows.h>
#define KEYA

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
