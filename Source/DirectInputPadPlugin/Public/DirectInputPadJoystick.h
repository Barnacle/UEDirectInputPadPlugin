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
	void SetJoysticks(const TWeakPtr<FDirectInputJoystick>& Joystick);

public:
	UFUNCTION(BlueprintCallable, Category="DirectInputPad")
	EXInputPadKeyNames	GetKeyMap(EDirectInputPadKeyNames eDIKey);

	UFUNCTION(BlueprintCallable, Category="DirectInputPad")
	void				SetKeyMap(EDirectInputPadKeyNames eDIKey, EXInputPadKeyNames eXIKey);

private:
	TWeakPtr<FDirectInputJoystick> Joystick_;
};


// ���̊֐����g����UDirectInputPadJoystick���擾����
UCLASS()
class UDirectInputPadFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="DirectInputPad")
	static UDirectInputPadJoystick* GetDirectInputPadJoystick(int32 PlayerID);

public:
	static void ClearDirectInputPadJoystickMap();
};
