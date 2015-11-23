#pragma once

#include "InputCoreTypes.h"
#include "InputDevice.h"

class FDirectInputDriver;
class FDirectInputJoystickFactory;
class FDirectInputJoystick;

class FDirectInputPadDevice : public IInputDevice
{
public:
	bool Init(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler);
	void Fin();

public:
	/** Poll for controller state and send events if needed */
	virtual void SendControllerEvents()override;

	/** Set which MessageHandler will get the events from SendControllerEvents. */
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)override{ MessageHandler_=InMessageHandler; }

public:
	virtual void Tick(float DeltaTime){}
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar){ return true; }
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value){}
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values){}

private:
	TSharedPtr<FGenericApplicationMessageHandler>	MessageHandler_;

	TSharedPtr<FDirectInputDriver>					DDriver_;
	TSharedPtr<FDirectInputJoystickFactory>			DFactory_;
	TArray<TWeakPtr<FDirectInputJoystick>>			DJoysticks_; // �K��MAX_JOYSITCKS���m�ۂ����BIndex��PlayerID�ƈ�v���Ă���

	uint32											XInputDeviceNum_ = 0; // �Ȃ����Ă���XInput�f�o�C�X���A�ő�4
};
