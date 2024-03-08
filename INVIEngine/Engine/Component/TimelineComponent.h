#pragma once
#include "Core/CoreObject/CoreMinimalObject.h"

// ʱ���ᵥ������ ����ֵΪvoid ����Ϊfloat������ʱ�䣩��float����ǰ֡ʱ�䣩
DEFINITION_SIMPLE_SINGLE_DELEGATE(FTimelineDelegate, void, float, float);

class FTimeline
{
public:
	FTimeline() = default;

	void Tick(float DeltaTime);

	FORCEINLINE float GetDuration() const { return Duration; }
	FORCEINLINE float GetCurrentTime() const { return CurrentTime; }
	FORCEINLINE bool IsPlaying() const { return bPlaying; }
	FORCEINLINE bool IsLoop() const { return bLoop; }
	FORCEINLINE bool IsReverse() const { return bReverse; }

	FORCEINLINE void SetDuration(float InDuration) { Duration = InDuration; }
	FORCEINLINE void SetLoop(bool InLoop) { bLoop = InLoop; }
	FORCEINLINE void SetReverse(bool InReverse) { bReverse = InReverse; }
	FORCEINLINE void SetPlaying(bool InPlaying) { bPlaying = InPlaying; }
	FORCEINLINE void SetCurrentTime(float InCurrentTime) { CurrentTime = InCurrentTime; }

	void BindTimelineDelegate(const FTimelineDelegate& InDelegate, float InDuration, bool InLoop, bool InReverse);

private:
	float CurrentTime = 0.0f;	// ��ǰʱ��
	float Duration = 0.0f;		// ����ʱ��
	bool bLoop = false;			// �Ƿ�ѭ��
	bool bPlaying = true;		// �Ƿ񲥷���
	bool bReverse = false;		// �Ƿ��򲥷�

private:
	FTimelineDelegate TimelineDelegate;	// ʱ�������
};

