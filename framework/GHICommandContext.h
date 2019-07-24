//=================================================================================================
//
//
//  All code licensed under the MIT license
//
//=================================================================================================

#pragma once

#include "GHIResources.h" 

namespace SimpleFramework
{
	
    enum EResourceView
    {
        RTV,
        UAV,
        SRV,
        NUM
    };

    enum EShaderStage
    {
        VS,
        PS,
        CS,
    };

	class IGHIComputeCommandCotext
	{
	public:
        virtual GHIBuffer* CreateConstBuffer(int size, const void* initData) = 0;

		virtual void UpdateBuffer(GHIBuffer*buffer, void* data, int size) = 0;
        virtual void SetShaderResource(GHITexture *resource, int slot, GHISRVParam view) = 0;
        virtual void SetShaderResource(GHITexture *resource, int slot, GHIUAVParam view) = 0;
        virtual void SetConstBuffer(GHIBuffer *resource, int slot) = 0;
        virtual GHISampler* CreateSampler(const GHISamplerDesc  &desc) = 0;
        virtual void SetSampler(GHISampler *resource, int slot, EShaderStage stage) = 0;
		virtual void Dispatch(int nX, int nY, int nZ) = 0;
	};
}