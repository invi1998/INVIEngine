#include "EngineMinimal.h"
#include "TimelineComponent.h"


void FTimeline::Tick(float DeltaTime)
{
	if (!bPlaying) return;

	if (Duration <= 0.0f) return;

	if (bReverse)	// ���򲥷�
	{
		CurrentTime -= DeltaTime;
		
		if (CurrentTime <= 0.0f)
		{
			if (bLoop)
			{
				CurrentTime = Duration;
				if (TimelineDelegate.IsBound())
				{
					TimelineDelegate.Execute(CurrentTime, Duration);
				}
			}
			else
			{
				CurrentTime = 0.0f;
				bPlaying = false;
				TimelineDelegate.ReleaseDelegate();
			}
		}
		else
		{
			if (TimelineDelegate.IsBound())
			{
				TimelineDelegate.Execute(CurrentTime, Duration);
			}
		}
	}
	else // ���򲥷�
	{
		CurrentTime += DeltaTime;
		if (CurrentTime >= Duration)
		{
			if (bLoop)
			{
				CurrentTime = 0.0f;

				if (TimelineDelegate.IsBound())
				{
					TimelineDelegate.Execute(CurrentTime, Duration);
				}
			}
			else
			{
				CurrentTime = Duration;
				bPlaying = false;

				TimelineDelegate.ReleaseDelegate();
			}
		}
		else
		{
			if (TimelineDelegate.IsBound())
			{
				TimelineDelegate.Execute(CurrentTime, Duration);
			}
		}
	}

}

void FTimeline::BindTimelineDelegate(const FTimelineDelegate& InDelegate, float InDuration, bool InLoop, bool InReverse)
{
	// ����֮ǰ�Ĵ���
	if (TimelineDelegate.IsBound())
	{
		TimelineDelegate.ReleaseDelegate();
	}

	TimelineDelegate = InDelegate;
	Duration = InDuration;
	bLoop = InLoop;
	bReverse = InReverse;
	bPlaying = true;
	CurrentTime = InReverse ? InDuration : 0.0f;
}
