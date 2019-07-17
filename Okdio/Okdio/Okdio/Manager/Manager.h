#pragma once
#include "../etc/Singleton.h"

struct _GUID;
typedef _GUID GUID;
class Okdio;

class Manager :
	public Singleton<Manager>
{
	friend Singleton<Manager>;
public:
	// �������m��
	void Malloc(void);

	// �I�u�W�F�N�g����
	long Create(const GUID& id, void** obj);

	// �������
	void Release(void);

private:
	// �R���X�g���N�^
	Manager();
	// �f�X�g���N�^
	~Manager();

	// �T�E���h���C�u����
	Okdio* okdio;
};
