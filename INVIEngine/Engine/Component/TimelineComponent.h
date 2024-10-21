#pragma once

// 时间轴单播代理 返回值为void 参数为float（持续时间）和float（当前帧时间）
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
	float CurrentTime = 0.0f;	// 当前时间
	float Duration = 0.0f;		// 持续时间
	bool bLoop = false;			// 是否循环
	bool bPlaying = true;		// 是否播放中
	bool bReverse = false;		// 是否反向播放

private:
	FTimelineDelegate TimelineDelegate;	// 时间轴代理
};

