//
//  Copyright 2013 by Autodesk, Inc. All rights reserved.
//
// This computer source code and related instructions and comments
// are the unpublished confidential and proprietary information of
// Autodesk, Inc. and are protected under applicable copyright and
// trade secret law. They may not be disclosed to, copied or used
// by any third party without the prior written consent of Autodesk, Inc.
//
//

#ifndef AG_DEVICE_IMPL_OGS__
#define AG_DEVICE_IMPL_OGS__

#include <Analytic/AGTypes.h>

#ifdef _GI_OGS_

#include <Analytic/AGDeviceInterface.h>
#include <Analytic/AGDeviceCapsImpl.h>
#include <Analytic/AGGraphicsLevel.h>


namespace AutoCAD { namespace Graphics { namespace Engine {

    class AGDeviceFactory_OGS : public AGDeviceFactory
    {
    public:
        static AGDeviceFactory_OGS* Instance();

        virtual AGDeviceInterface* CreateDevice(AGVOID* devHandle);
    };

    // it's an OGS implementation of abstract device
    class AGDeviceImpl_OGS : public AGDeviceInterface
    {
    public:
        AGDeviceImpl_OGS(AGNativeDevice* pDevice);
        virtual ~AGDeviceImpl_OGS();

        // get device 
        virtual AGNativeDevice* GetNativeDevice();

        // query device capabilities.
        virtual const AGDeviceCaps& GetDeviceCaps() const
        {
            return *mpDeviceCaps;
        }

        // query graphics level.
        virtual const AGGraphicsLevel& GetGraphicsLevel() const
        {
            return *mpGraphicsLevel;
        }

        // create operations.
        virtual AGBOOL CreateVertexFormat(const AGVertexFormatDesc*pDesc, AGVertexFormat*& pRes);
        virtual AGBOOL CreateBlendState(const AGBlendStateDesc*pDesc, AGBlendState*& pRes);
        virtual AGBOOL CreateRasterState(const AGRasterStateDesc*pDesc, AGRasterState*& pRes);
        virtual AGBOOL CreateDepthState(const AGDepthStateDesc*pDesc, AGDepthState*& pRes);
        virtual AGBOOL CreateSamplerState(const AGSamplerStateDesc*pDesc, AGSamplerState*& pRes);
        virtual AGBOOL CreateTarget(const AGTargetDesc* pDesc, AGTarget*& pRes);
        virtual AGBOOL CreateZBuffer(const AGZBufferDesc* pDesc, AGZBuffer*& pRes);

        virtual AGBOOL ResizeSwapChain(const AGTargetDesc* /*pDesc*/, AGTarget*& /*pRes*/) { return AGFalse; }	//not implemented
        virtual AGBOOL LoadSwapChainFromHandle(AGVOID* /*handle*/, AGTarget*& /*pRes*/) { return AGFalse;} //not implemented
        virtual AGBOOL LoadRawZBufferFromHandle(AGVOID* handle, AGRawZBuffer*& pRes) { return AGFalse; } //not implemented
        virtual AGBOOL LoadRawTargetFromHandle(AGVOID* rtHandle, AGVOID* texHandle, AGRawTarget*& pRes) { return AGFalse; } //not implemented
        virtual AGBOOL LoadRawBufferFromHandle(AGVOID* bufHandle, AGVOID* writeHandle, AGVOID* readHandle,
            AGUInt32 sizeInBytes, AGUInt32 stride, AGUInt32 offset, AGRawBuffer*& pRes) { return AGFalse;} //not implemented
        virtual AGBOOL LoadRawTextureFromHandle(AGVOID* texHandle, AGRawTexture*& pRes) { return AGFalse; } //not implemented
        
        virtual AGBOOL CreateRWBuffer(const AGRWBufferDesc* pDesc, AGRWBuffer*& pRes,
            const AGVOID *pData = AGNULL, AGUInt32 dataSize = 0);
        virtual AGBOOL CreateVertexBuffer(const AGVertexBufferDesc* pDesc, AGVertexBuffer*& pRes,
            AGDeviceResource* pRefRes = AGNULL, const AGVOID *pData = AGNULL, AGUInt32 dataSize = 0);
        virtual AGBOOL CreateIndexBuffer(const AGIndexBufferDesc* pDesc, AGIndexBuffer*& pRes,
            AGDeviceResource* pRefRes = AGNULL, const AGVOID *pData = AGNULL, AGUInt32 dataSize = 0);
        virtual AGBOOL CreateTexture(const AGTextureDesc* pDesc, AGTexture*& pRes,
            AGDeviceResource* pSampler = AGNULL, AGDeviceResource*  pRefRes = AGNULL,
            const AGVOID *const pData[] = AGNULL, AGUInt32 dataSize[] = AGNULL, AGUInt32 dataSlices = 0);
        virtual AGBOOL CreateTexBuffer(const AGTexBufferDesc* pDesc, AGTexBuffer*& pRes,
            AGDeviceResource*  pRefRes = AGNULL,
            const AGVOID *pData = AGNULL, AGUInt32 dataSize = 0);

        virtual AGBOOL CreateEffectFromString(const AGKStringA& effectStr, 
            const AGEffectDesc* pDesc, AGEffect*& pEffect);
        virtual AGBOOL CreateEffectFromBuffer(const AGVOID *pData, AGUInt32 len, 
            const AGEffectDesc* pDesc, AGEffect*& pEffect);
        virtual AGBOOL CreateEffectFromFile(
            const AGEffectDesc* pDesc, AGEffect*& pEffect);

        virtual AGBOOL CreateGraphicsEffect(const AGEffectDesc* pDesc, const AGVertexShader* pVertexShader,
            const AGPixelShader* pPixelShader, AGEffect*& pEffect);
        virtual AGBOOL CreateGraphicsEffect(const AGEffectDesc* pDesc, const AGArray<AGShader*>& aShaders, AGEffect*& pEffect);
        virtual AGBOOL CreateComputeEffect(const AGEffectDesc* pDesc, const AGComputeShader* apComputeShader, AGEffect*& apEffect);

        virtual AGBOOL CreateVertexShaderFromString(const AGKStringA& shaderStr,
            const AGVertexShaderDesc* pDesc, AGVertexShader*& pShader);
        virtual AGBOOL CreateVertexShaderFromBuffer(const AGVOID *pData, AGUInt32 len,
            const AGVertexShaderDesc* pDesc, AGVertexShader*& pShader);
        virtual AGBOOL CreateVertexShaderFromFile(
            const AGVertexShaderDesc* pDesc, AGVertexShader*& pShader);

        virtual AGBOOL CreatePixelShaderFromString(const AGKStringA& shaderStr,
            const AGPixelShaderDesc* pDesc, AGPixelShader*& pShader);
        virtual AGBOOL CreatePixelShaderFromBuffer(const AGVOID *pData, AGUInt32 len,
            const AGPixelShaderDesc* pDesc, AGPixelShader*& pShader);
        virtual AGBOOL CreatePixelShaderFromFile(
            const AGPixelShaderDesc* pDesc, AGPixelShader*& pShader);

        virtual AGBOOL CreatePipelineState(const AGPipelineStateDesc* pDesc, AGPipelineState*& pState);

        virtual AGBOOL CreateComputeShaderFromString(const AGKStringA& shaderStr,
            const AGComputeShaderDesc* pDesc, AGComputeShader*& pShader);
        virtual AGBOOL CreateComputeShaderFromBuffer(const AGVOID* pData, AGUInt32 len,
            const AGComputeShaderDesc* pDesc, AGComputeShader*& pShader);
        virtual AGBOOL CreateComputeShaderFromFile(
            const AGComputeShaderDesc* pDesc, AGComputeShader*& pShader);

        virtual AGVOID CleanupResource(AGDeviceResource* pRes);

        // clear operation
        virtual AGVOID ClearTarget(AGTarget& target, const AGFVec4&  color);
        virtual AGVOID ClearDepthStencil(AGZBuffer& zbuffer, AGFP32 depth, AGUInt8 stencil, AGUInt32 flag);
        virtual AGVOID ClearRWBuffer(AGRWBuffer& rwBuffer, const AGVOID *pVal, AGUInt32 flag);

        // set pipeline state
        virtual AGVOID SetPipelineState(AGPipelineState* pState);

        // device states operations
        virtual AGVOID SetBlendState(AGBlendState* pBlend, AGFVec4 blendFactor, AGUInt32 sampleMask);
        virtual AGVOID SetDepthState(AGDepthState* pDepth, AGUInt32 stencilRef);
        virtual AGVOID SetRasterState(AGRasterState* pRaster);

        virtual AGVOID PushBlendState();
        virtual AGVOID PopBlendState();

        virtual AGVOID PushDepthState();
        virtual AGVOID PopDepthState();

        virtual AGVOID PushRasterState();
        virtual AGVOID PopRasterState();

        virtual AGVOID SetVertexFormat(AGVertexFormat* pFormat);
        virtual AGVOID SetIndexBuffer(AGIndexBuffer* pIB);
        virtual AGVOID SetVertexBuffers(AGInt32 count, AGVertexBuffer* vbs[]);

        virtual AGVOID SetViewport(AGBox2* vp);
        virtual AGBox2 GetViewport() const;

        virtual AGVOID SetScissorBox(AGBox2* box);
        virtual AGBox2 GetScissorBox() const;

        virtual AGVOID ApplyTargets();
        virtual AGVOID SetTargets(AGUInt32 max_targets, AGTarget* targets[], AGZBuffer* zBuffer, 
            AGUInt32 max_rwbs, AGRWBuffer* rwbs[]);

        virtual AGVOID BackupTargets();
        virtual AGVOID RestoreTargets();

          // effect operations
        virtual AGBOOL UpdateEffectParameter(AGParameter* pParam, AGEffect* pEffect);
        virtual AGBOOL UpdateEffectTexture(AGTexture* pTex, AGUString* pName, AGEffect* pEffect);
        virtual AGBOOL UpdateEffectBuffer(AGTexBuffer* pBuffer, AGUString* pName, AGEffect* pEffect);
        virtual AGBOOL UpdateEffectSampler(AGSamplerState* pSampler, AGUString* pName, AGEffect* pEffect);
        virtual AGBOOL UpdateEffectTexBuffer(AGTexBuffer* pTexBuffer, AGUString* pName, AGEffect* pEffect);
        virtual AGBOOL UpdateEffectTexBuffer(AGRWBuffer* pRWBuffer, AGUString* pName, AGEffect* pEffect);
        virtual AGBOOL UpdateEffectInputRWBuffer(AGRWBuffer* pRWBuffer, AGUString* pName, AGEffect* pEffect);
        virtual AGBOOL UpdateEffectOutputRWBuffer(AGRWBuffer* pRWBuffer, AGUString* pName, AGEffect* pEffect);

        
        // draw operations
        virtual AGVOID BeginEffect(AGDrawContext& dc, AGEffect* pEffect);
        virtual AGVOID EndEffect(AGDrawContext& dc, AGEffect* pEffect);

        virtual AGInt32 DrawPrimitive(AGDrawContext& dc, AGEffect* pEffect, AGPrimInfo* pInfo);

        virtual AGBOOL Dispatch(const AGComputeShaderInfo* aComputeShaderInfo, AGDeviceResource* apRes) const;

        // present operations
        virtual AGVOID FlushCommands();
        virtual AGVOID FlushCommandsAndWait();
        virtual DeviceStatus Present(AGTarget& target, AGUInt32 syncInterval, AGUInt32 flag);

        // copy resource operations
        virtual AGVOID* Map(AGDeviceResource* pRes, AGInt32 subID, 
            AGResourceAccess flag, AGInt32& rowPitch, AGInt32& slicePitch,
            AGUInt32 offset = 0, AGUInt32 sizeInBytes = 0);
        virtual AGVOID  UnMap(AGDeviceResource* pRes, AGInt32 subID);

        virtual AGBOOL CopyBuffer(AGDeviceResource* pDstRes, AGInt32 dstSubID, 
            AGDeviceResource* pSrcRes, AGInt32 srcSubID,
            AGUInt32 dstStartInBytes, AGUInt32 srcStartInBytes, AGUInt32 sizeInBytes);
        virtual AGBOOL CopyTexture(AGDeviceResource* pDstRes, AGInt32 dstSubID,
            AGDeviceResource* pSrcres, AGInt32 srcSubID,
            const AGIVec3& srcMin, const AGIVec3& srcMax, const AGIVec3& dstOrigin);


        // utilities
        virtual AGInt32 FormatBytes(AGFormat format);
        virtual DeviceStatus GetDeviceStatus();


    private:
        // translate primitive type and vertex count into primitive count.
        static AGUInt32 GetPrimitiveCount(AGPrimType type, AGUInt32 vertex_count);

        // clean-up 
        AGVOID ClearDevice();
        AGVOID ClearTempStates();

        // vertex format helpers
        AGVOID InitUsageTable();
        AGVOID ClearUsageTable();
        OGS::Devices::EVertexFieldUsage GetUsage(AGUString* pName, AGUInt32 index);

        // create a fake vertex fomrat for vertex buffer
        // since OGS requires a vertex format for vertex buffer creation
        OGS::Devices::AVertexFormat* CreateFakeFormat(AGUInt32 strideInBytes);


        // effect helpers
        AGVOID BuildMacros(const AGEffectDesc* pDesc, OGS::Devices::ShaderMacro*& pMacros, AGUInt32& numMacros);
        AGVOID FreeMacros( OGS::Devices::ShaderMacro* pMacros, AGUInt32 numMacros);

        AGEffect* CreateEffect(AGNativeEffect* pNativeEffect,  AGUString* pName);

        // map helpers
        AGVOID* MapTexture(AGTexture* pTex, AGInt32 subID, AGResourceAccess flag, AGInt32& rowPitch, AGInt32& slicePitch);
        AGVOID UnMapTexture(AGTexture* pTex, AGInt32 subID);

        AGVOID* MapVertexBuffer(AGVertexBuffer* pBuf, AGResourceAccess flag);
        AGVOID UnMapVertexBuffer(AGVertexBuffer* pBuf);

        AGVOID* MapIndexBuffer(AGIndexBuffer* pBuf, AGResourceAccess flag);
        AGVOID UnMapIndexBuffer(AGIndexBuffer* pBuf);

        AGVOID* MapTexBuffer(AGTexBuffer* pBuf, AGResourceAccess flag);
        AGVOID UnMapTexBuffer(AGTexBuffer* pBuf);

        // copy helpers
        AGBOOL CopyTextureData(AGTexture* pDstTex, AGInt32 dstSubID, AGTexture* pSrcTex, AGInt32 srcSubID, 
            const AGIVec3& srcMin, const AGIVec3& srcMax, const AGIVec3& dstOrigin);
        AGBOOL CopyVertexBuffer(AGVertexBuffer* pDstBuf, AGVertexBuffer* pSrcBuf,
            AGUInt32 dstStartInBytes, AGUInt32 srcStartInBytes, AGUInt32 sizeInBytes);
        AGBOOL CopyIndexBuffer(AGIndexBuffer* pDstBuf, AGIndexBuffer* pSrcBuf,
            AGUInt32 dstStartInBytes, AGUInt32 srcStartInBytes, AGUInt32 sizeInBytes);
        AGBOOL CopyTexBuffer(AGTexBuffer* pDstBuf, AGTexBuffer* pSrcBuf,
            AGUInt32 dstStartInBytes, AGUInt32 srcStartInBytes, AGUInt32 sizeInBytes);
        

        AGVOID ClearTexMapperTable();
        OGS::Devices::IMappableRaster* GetTextureMapper(AGTexture* pTex, AGInt32 subID);
        OGS::Devices::IMappableRaster* CreateTextureMapper(AGTexture* pTex, AGInt32 subID);

        // draw helpers
        AGVOID DoDrawPrimitive(AGDrawContext& dc, AGEffect* pEffect, AGPrimType primType,
          AGUInt32 vertexOffset, AGUInt32 primCount, AGUInt32 instCount);
        AGVOID backupDeviceViewportAndScissor();
        AGVOID resetDeviceViewportAndScissor();

		//obtain api name
		virtual const AGUString* GetApiName() const override;

    private:
        AGAutoPtr<AGNativeDevice> mpDevice;

        // usage name table: workaround for OGS format parsing logic.
        AGHashMap<OGS::Devices::EVertexFieldUsage> mUsageTable;

        // temporary state objects: woraround for OGS state definition issue.
        AGAutoPtr<AGRawBlendState> mpTempBlendState;
        AGAutoPtr<AGRawDepthState> mpTempDepthState;

         // backup viewports: workaround for OGS set target issue.
        AGBox2* m_backupVp;
        AGBox2  m_backupVpVal;
    
        AGBox2* m_backupClipRect;
        AGBox2  m_backupClipRectVal;

        // texture mappable raster list
        AGArray<OGS::Devices::IMappableRaster*> mTexMapperTable;

        // device caps
        AGAutoPtr<AGDeviceCapsImpl_OGS> mpDeviceCaps;

        // graphics level
        AGAutoPtr<AGGraphicsLevel> mpGraphicsLevel;

        OGS::Devices::ATarget* mBackupTargets[MAX_TARGETS] = { AGNULL };
        int mBackupTargetCount = 0;

        OGS::Devices::IBuffer* mBackupUAVs[MAX_UAVS] = { AGNULL };
        int mBackupUAVCount = 0;

        OGS::Devices::ATarget* mBackupZBuffer = AGNULL;
    };


} } }

#endif

#endif