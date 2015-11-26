#pragma once

#include "DirectInputPadState.h"

#include "DirectInputPadJoystick.generated.h"

class FDirectInputJoystick;

// BP���J�pDIPad�N���X
// BP����́A���̃N���X����đ��삷��
UCLASS()
class UDirectInputPadJoystick : public UObject
{
	GENERATED_BODY()

public:
	// DIKey�ɐݒ肳��Ă���XIKey���擾����B�����ݒ肳��ĂȂ��Ƃ��́AXIGamePad_END
	UFUNCTION(BlueprintCallable, Category="DirectInputPad")
	EXInputPadKeyNames	GetKeyMap(EDirectInputPadKeyNames DIKey);

	// DIKey���AXIKey�Ƃ��Đݒ肷��
	UFUNCTION(BlueprintCallable, Category="DirectInputPad")
	void				SetKeyMap(EDirectInputPadKeyNames DIKey, EXInputPadKeyNames XIKey);

public:
	void SetJoysticks(const TWeakPtr<FDirectInputJoystick>& Joystick);

private:
	TWeakPtr<FDirectInputJoystick> Joystick_;
};


// ���̊֐����g����UDirectInputPadJoystick���擾����
UCLASS()
class UDirectInputPadFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 
	UFUNCTION(BlueprintCallable, Category="DirectInputPad")
	static UDirectInputPadJoystick* GetDirectInputPadJoystick(int32 PlayerID);

public:
	static void InitDirectInputPadJoystickLibrary();
	static void FinDirectInputPadJoystickLibrary();
};
