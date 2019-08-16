// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MediaCapture.h"
#include "AjaMediaOutput.h"
#include "HAL/CriticalSection.h"
#include "MediaIOCoreEncodeTime.h"
#include "Misc/FrameRate.h"
#include "AjaMediaCapture.generated.h"

class FEvent;
class UAjaMediaOutput;

/**
 * Output Media for AJA streams.
 * The output format could be any of EAjaMediaOutputPixelFormat.
 */
UCLASS(BlueprintType)
class AJAMEDIAOUTPUT_API UAjaMediaCapture : public UMediaCapture
{
	GENERATED_UCLASS_BODY()

	//~ UMediaCapture interface
public:
	virtual bool HasFinishedProcessing() const override;
protected:
	virtual bool ValidateMediaOutput() const override;
	virtual bool CaptureSceneViewportImpl(TSharedPtr<FSceneViewport>& InSceneViewport) override;
	virtual bool CaptureRenderTargetImpl(UTextureRenderTarget2D* InRenderTarget) override;
	virtual bool UpdateSceneViewportImpl(TSharedPtr<FSceneViewport>& InSceneViewport) override;
	virtual bool UpdateRenderTargetImpl(UTextureRenderTarget2D* InRenderTarget) override;
	virtual void StopCaptureImpl(bool bAllowPendingFrameToBeProcess) override;

	virtual void OnFrameCaptured_RenderingThread(const FCaptureBaseData& InBaseData, TSharedPtr<FMediaCaptureUserData, ESPMode::ThreadSafe> InUserData, void* InBuffer, int32 Width, int32 Height) override;

private:
	struct FAjaOutputCallback;
	friend FAjaOutputCallback;
	struct FAJAOutputChannel;

private:
	bool InitAJA(UAjaMediaOutput* InMediaOutput);
	void WaitForSync_RenderingThread() const;
	void ApplyViewportTextureAlpha(TSharedPtr<FSceneViewport> InSceneViewport);
	void RestoreViewportTextureAlpha(TSharedPtr<FSceneViewport> InSceneViewport);

private:
	/** AJA Port for outputting */
	FAJAOutputChannel* OutputChannel;
	FAjaOutputCallback* OutputCallback;

	/** Name of this output port */
	FString PortName;

	/** Option from MediaOutput */
	bool bWaitForSyncEvent;
	bool bLogDropFrame;
	bool bEncodeTimecodeInTexel;
	EAjaMediaOutputPixelFormat PixelFormat;
	bool UseKey;

	/** Saved IgnoreTextureAlpha flag from viewport */
	bool bSavedIgnoreTextureAlpha;
	bool bIgnoreTextureAlphaChanged;

	/** Selected FrameRate of this output */
	FFrameRate FrameRate;

	/** Critical section for synchronizing access to the OutputChannel */
	FCriticalSection RenderThreadCriticalSection;

	/** Event to wakeup When waiting for sync */
	FEvent* WakeUpEvent;
};
