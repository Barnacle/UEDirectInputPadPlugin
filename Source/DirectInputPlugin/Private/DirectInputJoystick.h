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

#include "Core.h"

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
};


//! PVO�Ǝ��̓��͕����������񋓎q
enum EDirectInputArrow : uint8
{
	// �ʏ�̃{�^���̎�����̔ԍ���U��
	POV_NONE = 32,	//!< POV����������Ă��Ȃ�
	POV_UP,			//!< POV��
	POV_RIGHT,		//!< POV��
	POV_DOWN,		//!< POV��
	POV_LEFT,		//!< POV��
	AXIS_NONE,		//!< XY������������Ă��Ȃ�
	AXIS_UP,		//!< ��Y��
	AXIS_RIGHT,		//!< ��X��
	AXIS_DOWN,		//!< ��Y��
	AXIS_LEFT,		//!< ��X��
	ARROW_END,
};


/*! @brief �W���C�X�e�B�b�N���\���N���X
 *
 *	FDirectInputJoystick_enum����A�擾�����������Ƀf�o�C�X���쐬��
 *	���ۂɓ��͏������s��
 *  ���E��]�͈̔͂́A-1.0�`1.0 �����B�����т͈̔͂ɂ��鎞��0.0���Ԃ�
 *
 *  AD�ϊ���ON�ɂ���ƁAXY���f�W�^����POV�Ƃ��āAPOV��XY���f�W�^�����͂Ƃ��Ă�������
 *  XY����is_pov�n���\�b�h�ŁAPOV�n��is_axis�n���\�b�h�Ŏ擾�ł���悤�ɂȂ�
 */
class FDirectInputJoystick
{
public:
	static const int32 MAX_AXIS_VALUE;

public:
	FDirectInputJoystick():pDevice_(nullptr),nCurIndex_(0),bAcquire_(false),
				  bADConv_(true),
				  nX_Threshold_(300),nY_Threshold_(300),nZ_Threshold_(300),
				  nXrot_Threshold_(300),nYrot_Threshold_(300),nZrot_Threshold_(300),
				  bGuard_(false){ ClearBuf(); }

	~FDirectInputJoystick(){ Fin(); }

	//! @brief ������
	/*! @param[in] joyins �f�o�C�X���쐬����f�o�C�X�C���X�^���X
		@param[in] adaptar DirectInput�A�_�v�^�[�C���X�^���X
	    @param[in] hWnd �L�[�{�[�h���͂��󂯕t����E�C���h�E�n���h��
		@param[in] bBackGraound true���Ɣ�A�N�e�B�u�ł��L�[���͂��󂯕t����悤�ɂȂ� */
	bool Init(const DIDEVICEINSTANCE& joyins, FDirectInputDriver& adapter, HWND hWnd, bool bBackGround=false);

	//! �I������
	void Fin();

	//! �W���C�X�e�B�b�N���͂��擾
	bool Input();

	//! AD�ϊ����L����
	bool IsAdConvFlag()const{ return bADConv_; }
	//! AD�ϊ��t���O�ݒ�
	bool SetAdConvFlag(bool bAD){ bADConv_ = bAD; }

	//! �A�i���O���̂����т�臒l�ݒ�B���̒l�ȉ��̓��͖͂�������
	void SetAxisThreshold(uint32 nX, uint32 nY, uint32 nZ){ nX_Threshold_=nX; nY_Threshold_=nY; nZ_Threshold_=nZ; }
	//! �A�i���O��]�̂����т�臒l�ݒ�B���̒l�ȉ��̓��͖͂�������
	void SetRotThreshold(uint32 nX, uint32 nY, uint32 nZ){ nXrot_Threshold_=nX; nYrot_Threshold_=nY; nZrot_Threshold_=nZ; }

	//! x���̒l
	float	X()const;
	//! y���̒l
	float	Y()const;
	//! z���̒l
	float	Z()const;

	//! x����1�O�̒l
	float	PrevX()const;
	//! y����1�O�̒l
	float	PrevY()const;
	//! z����1�O�̒l
	float	PrevZ()const;

	//! x��]�̒l
	float	RotX()const;
	//! y��]�̒l
	float	RotY()const;
	//! z��]�̒l
	float	RotZ()const;

	//! x��]��1�O�̒l
	float	RotPrevX()const;
	//! y��]��1�O�̒l
	float	RotPrevY()const;
	//! z��]��1�O�̒l
	float	RotPrevZ()const;

	//! pov�l
	int32 Pov()const;
	//! pov��1�O�̒l
	int32 PrevPov()const;

	//! �{�^����������Ă���B0�`31�̓{�^���B32�`��POV�Ǝ�
	bool IsPress(uint32 nBtn)const;
	//! �{�^���������ꂽ�u�ԁB0�`31�̓{�^���B32�`��POV�Ǝ�
	bool IsPush(uint32 nBtn)const;
	//! �{�^�������ꂽ�u�ԁB0�`31�̓{�^���B32�`��POV�Ǝ�
	bool IsRelease(uint32 nBtn)const;

	// ���̓K�[�h
	void SetGuard(bool bGuard);
	bool IsGuard()const{ return bGuard_; }

	//! �o�b�t�@���N���A����
	void ClearBuf(){ nCurIndex_=0; ::ZeroMemory(joyBuf_, sizeof(joyBuf_)); }

	//! ���݂̃o�b�t�@���N���A����
	void ClearCurBuf(){ ::ZeroMemory(&joyBuf_[nCurIndex_], sizeof(DIJOYSTATE)); }

private:
	//! POV��������Ă���
	bool IsPovPress(enum EDirectInputArrow eArrow)const;
	//! POV�������ꂽ�u��
	bool IsPovPush(enum EDirectInputArrow eArrow)const;
	//! POV�����ꂽ�u��
	bool IsPovRelease(enum EDirectInputArrow eArrow)const;


	//! POV��������Ă��邩�ǂ����̃`�F�b�N
	/*! @param[in] eArrow �`�F�b�N�������
		@param[in] index �`�F�b�N����o�b�t�@�C���f�b�N�X */
	bool IsPovPressInner(enum EDirectInputArrow eArrow, uint32 nIndex)const;

	//! XY�����f�W�^���I�ɉ�����Ă���
	bool IsAxisPress(enum EDirectInputArrow eArrow)const;
	//! XY�����f�W�^���I�ɉ����ꂽ�u��
	bool IsAxisPush(enum EDirectInputArrow eArrow)const;
	//! XY�����f�W�^���I�ɗ��ꂽ�u��
	bool IsAxisRelease(enum EDirectInputArrow eArrow)const;

	//! XY�����f�W�^���I�ɉ�����Ă��邩�ǂ����̃`�F�b�N
	/*! @param[in] eArrow �`�F�b�N�������
		@param[in] index �`�F�b�N����o�b�t�@�C���f�b�N�X */
	bool IsAxisPressInner(enum EDirectInputArrow eArrow, uint32 nIndex)const;

	//! POV_*��AXIS_* �𑊌ݕϊ�����
	enum EDirectInputArrow SwapPovAxis(enum EDirectInputArrow eArrow)const;

private:
	//! DirectInputDevice::EnumObjects�ɓn���R�[���o�b�N�֐�
	static BOOL CALLBACK OnEnumAxis(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

private:
	//! �f�o�C�X
	LPDIRECTINPUTDEVICE8	pDevice_;

	//! �W���C�X�e�B�b�N���̓o�b�t�@
	DIJOYSTATE	joyBuf_[2];
	//! �W���C�X�e�B�b�N�o�b�t�@�ʒu
	uint32		nCurIndex_;

	//! �f�o�C�X���l�����Ă��邩�ǂ���
	bool		bAcquire_;

	//! �A�i���OX,Y��POV(�\���L�[)�Ƃ��Č��Ȃ����ǂ��� 
	bool		bADConv_;

	// joystick�̃A�i���O�̗V�т̒l�@-threshold�`threshold �͈̔͂ɂȂ�
	// ����ȉ��̒l�̎��͓��͂Ȃ�����

	uint32		nX_Threshold_;
	uint32		nY_Threshold_;
	uint32		nZ_Threshold_;

	uint32		nXrot_Threshold_;
	uint32		nYrot_Threshold_;
	uint32		nZrot_Threshold_;

	//! @brief ���̓K�[�h�t���O
	/*! true�ɂȂ���Ɠ��̓o�b�t�@���N���A����Ainput���Ă�ł��X�V����Ȃ�
	 *  �܂�A���͂���ĂȂ������ɂȂ� */
	bool bGuard_;
};


/*! @brief FDirectInputJoystick���쐬�Ǘ�����N���X
 *
 *  FDirectInputJoystick_enum/FDirectInputJoystick���܂Ƃ߂�B
 *  Joystick���擾���鎞�͂�����g���Ɨǂ�
 */
class FDirectInputJoystick_factory
{
public:
	typedef TMap<uint32, TSharedPtr<FDirectInputJoystick>> joy_map;

	FDirectInputJoystick_factory(){}
	~FDirectInputJoystick_factory(){ Fin(); }

	bool Init(HWND hWnd, const TSharedPtr<FDirectInputDriver>& pDriver, bool bBackGround=false);
	void Fin();

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
