#pragma once

#include "AllowWindowsPlatformTypes.h"

#define DIRECTINPUT_VERSION 0x0800 
#define _CRT_SECURE_NO_DEPRECATE

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <windows.h>

#pragma warning(push) 
#pragma warning(disable:6000 28251) 
#include <dinput.h> 
#pragma warning(pop) 

#include <dinputd.h>

class FDirectInputJoystick;

//! DirectInput�h���C�o�[�N���X
class FDirectInputDriver
{
public:
	FDirectInputDriver() :pDriver_(nullptr) {}
	~FDirectInputDriver() { Fin(); }

	bool Init();
	void Fin();

	LPDIRECTINPUT8 driver()const { return pDriver_; }

private:
	LPDIRECTINPUT8 pDriver_;
};

/*! @brief �g�p�\�ȃW���C�X�e�B�b�N���Ǘ�����N���X
 *
 *	�g�p�\�ȃW���C�X�e�B�b�N�̏����W�߂Ă����A�K�v�ɉ�����
 *	���̏���Ԃ�
 */
class FDirectInputJoystickEnum
{
public:
	FDirectInputJoystickEnum(){  vecJoyStickInfo_.Reserve(8); }

	bool Init(FDirectInputDriver& adapter);

	//! ���o����XInput�f�o�C�X�̐�
	uint32 GetXInputDeviceNum()const{ return nXInputDeviceNum_; }

	//! �g�p�\�ȃW���C�X�e�B�b�N�̐�
	uint32 EnabledJoystickNum()const{ return vecJoyStickInfo_.Num(); }

	//! @brief �g�p�\�ȃW���C�X�e�B�b�N�̏����擾����
	/*! @param[in] nJoyNo �����擾����W���C�X�e�B�b�N�ԍ��B0�ȏ�enable_joystick_num()�ȉ� 
	 *  @return �W���C�X�e�B�b�N����optioanl�B�W���C�X�e�B�b�N�����݂��Ă���ΊY���̒l�������Ă��� */
	const DIDEVICEINSTANCE* GetJoystickInfo(uint32_t nJoyNo)const;

private:
	//! DirectInput::EnumDevice�ɓn���R�[���o�b�N�֐�
	static BOOL CALLBACK OnEnumDevice(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

private:
	//! �W���C�X�e�B�b�N���z��
	TArray<DIDEVICEINSTANCE> vecJoyStickInfo_;

	uint32 nXInputDeviceNum_ = 0;
};


/*! @brief FDirectInputJoystick���쐬�Ǘ�����N���X
 *
 *  FDirectInputJoystick_enum/FDirectInputJoystick���܂Ƃ߂�B
 *  Joystick���擾���鎞�͂�����g���Ɨǂ�
 */
class FDirectInputJoystickFactory
{
public:
	typedef TMap<uint32, TSharedPtr<FDirectInputJoystick>> joy_map;

public:
	FDirectInputJoystickFactory();
	~FDirectInputJoystickFactory(){ Fin(); }

	bool Init(HWND hWnd, const TSharedPtr<FDirectInputDriver>& pDriver, bool bBackGround=false);
	void Fin();

	//! ���o����XInput�f�o�C�X�̐�
	uint32 GetXInputDeviceNum()const{ return joyEnum_.GetXInputDeviceNum(); }

	//! �g�p�\�ȃW���C�X�e�B�b�N�̐�
	uint32 EnabledJoystickNum()const{ return joyEnum_.EnabledJoystickNum(); }

	//! @brief �w��̔ԍ���FDirectInputJoystick���擾����
	/*! @return ���łɑ��݂����炻���Ԃ��B���݂��Ȃ�joystick�������琶������B���������s������nullptr���Ԃ� */
	TSharedPtr<FDirectInputJoystick> GetJoystick(uint32 nNo);

private:
	TSharedPtr<FDirectInputDriver>	pAdapter_;
	HWND							hWnd_;
	bool							bBackGround_;

	FDirectInputJoystickEnum	joyEnum_;
	joy_map						mapJoy_;
};

#include "HideWindowsPlatformTypes.h"
