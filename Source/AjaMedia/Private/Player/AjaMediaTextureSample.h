// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MediaIOCoreTextureSampleBase.h"
#include "MediaShaders.h"

/**
 * Implements a media texture sample for AjaMedia.
 */
class FAjaMediaTextureSample
	: public FMediaIOCoreTextureSampleBase
{
	using Super = FMediaIOCoreTextureSampleBase;

public:

	/**
	 * Initialize the sample.
	 *
	 * @param InVideoData The video frame data.
	 * @param InSampleFormat The sample format.
	 * @param InTime The sample time (in the player's own clock).
	 * @param InFrameRate The framerate of the media that produce the sample.
	 * @param InTimecode The sample timecode if available.
	 */
	bool InitializeProgressive(const AJA::AJAVideoFrameData& InVideoData, EMediaTextureSampleFormat InSampleFormat, FTimespan InTime, const FFrameRate& InFrameRate, const TOptional<FTimecode>& InTimecode, bool bInIsSRGB)
	{
		bIsSRGBInput = bInIsSRGB;

		return Super::Initialize(InVideoData.VideoBuffer
			, InVideoData.VideoBufferSize
			, InVideoData.Stride
			, InVideoData.Width
			, InVideoData.Height
			, InSampleFormat
			, InTime
			, InFrameRate
			, InTimecode);
	}

	/**
	 * Initialize the sample.
	 *
	 * @param InVideoData The video frame data.
	 * @param InSampleFormat The sample format.
	 * @param InTime The sample time (in the player's own clock).
	 * @param InFrameRate The framerate of the media that produce the sample.
	 * @param InTimecode The sample timecode if available.
	 * @param bEven Only take the even frame from the image.
	 */
	bool InitializeInterlaced_Halfed(const AJA::AJAVideoFrameData& InVideoData, EMediaTextureSampleFormat InSampleFormat, FTimespan InTime, const FFrameRate& InFrameRate, const TOptional<FTimecode>& InTimecode, bool bInEven, bool bInIsSRGB)
	{
		bIsSRGBInput = bInIsSRGB;

		return Super::InitializeWithEvenOddLine(bInEven
			, InVideoData.VideoBuffer
			, InVideoData.VideoBufferSize
			, InVideoData.Stride
			, InVideoData.Width
			, InVideoData.Height
			, InSampleFormat
			, InTime
			, InFrameRate
			, InTimecode);
	}

	/**
	 * Set the sample properties.
	 *
	 * @param InStride The number of channel of the video buffer.
	 * @param InWidth The sample rate of the video buffer.
	 * @param InHeight The sample rate of the video buffer.
	 * @param InSampleFormat The sample format of the video buffer.
	 * @param InTime The sample time (in the player's own clock).
	 * @param InFrameRate The framerate of the media that produce the sample.
	 * @param InTimecode The sample timecode if available.
	 * @param bInIsSRGB Whether the sample is in sRGB space.
	 */
	bool UpdateProperties(uint32 InStride, uint32 InWidth, uint32 InHeight, EMediaTextureSampleFormat InSampleFormat, FTimespan InTime, const FFrameRate& InFrameRate, const TOptional<FTimecode>& InTimecode, bool bInIsSRGB)
	{
		bIsSRGBInput = bInIsSRGB;

		return Super::SetProperties(InStride, InWidth, InHeight, InSampleFormat, InTime, InFrameRate, InTimecode);
	}
	
	/**
	 * Is output Srgb
	 *
	 * @return false
	 */
	virtual bool IsOutputSrgb() const override
	{
		return bIsSRGBInput;
	}

	/**
	 * Get YUV to RGB conversion matrix
	 *
	 * @return MediaIOCore Yuv To Rgb matrix
	 */
	virtual const FMatrix& GetYUVToRGBMatrix() const override
	{
		return MediaShaders::YuvToRgbRec709Full;
	}

protected:

	/** Wheter the sample is in sRGB space and requires an explicit conversion to linear */
	bool bIsSRGBInput;
};

/*
 * Implements a pool for AJA texture sample objects.
 */
class FAjaMediaTextureSamplePool : public TMediaObjectPool<FAjaMediaTextureSample> { };
