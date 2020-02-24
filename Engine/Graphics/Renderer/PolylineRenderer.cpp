#include "PolylineRenderer.h"
#include "DrawingDevice.h"

using namespace Engine;

uint32_t PolylineRenderer::m_sSegVertexID[MAX_SEGMENT_VERTEX_COUNT] = { 0 };
uint32_t PolylineRenderer::m_sSegInstanceID[MAX_SEGMENT_INSTANCE_COUNT] = { 0 };

PolylineRenderer::PolylineRenderer() : BaseRenderer()
{
    InitVertexID();
    InitInstanceID();

    m_pData = new char[MAX_SEGMENT_DATA_SIZE];
    reset(MAX_SEGMENT_DATA_SIZE);
}

void PolylineRenderer::DefineResources(DrawingResourceTable& resTable)
{
    DefineDefaultResources(resTable);
    DefineShaderResource(resTable);
    DefinePipelineStateResource(resTable);

    DefineStaticVertexBuffer(VB_VertexID(), 4, MAX_SEGMENT_VERTEX_COUNT, &m_sSegVertexID, MAX_SEGMENT_VERTEX_COUNT * 4, resTable);
    DefineStaticVertexBuffer(VB_InstanceID(), 4, MAX_SEGMENT_INSTANCE_COUNT, &m_sSegInstanceID, MAX_SEGMENT_INSTANCE_COUNT * 4, resTable);
    
    DefineDynamicTextureWithInit(LineSegmentTex(), eFormat_R32G32B32_FLOAT, MAX_SEGMENT_ELEMENT_COUNT * SIGNLE_SEGMENT_TEX_ELEMENT_COUNT, m_pData, m_size, resTable);
}

void PolylineRenderer::BuildPass()
{
    //auto pStage = CreateStage(LineStage());
    //pStage->AppendDrawingPass(CreateSegmentPass(LineSegmentPass(), LineSegmentEffect()));
    //m_stageTable.AddDrawingStage(pStage->GetName(), pStage);

	auto& pMainPass = CreateSegmentPass(LineSegmentPass(), LineSegmentEffect());
	m_passTable[LineSegmentPass()] = pMainPass;
}

void PolylineRenderer::SetupBuffers(DrawingResourceTable& resTable)
{
}

void PolylineRenderer::UpdatePrimitive(DrawingResourceTable& resTable)
{
    UpdateConstants(resTable);

    auto pEntry = resTable.GetResourceEntry(DefaultPrimitive());
    if (pEntry == nullptr)
        return;

    auto pPrimitive = std::dynamic_pointer_cast<DrawingPrimitive>(pEntry->GetResource());
    if (pPrimitive == nullptr)
        return;

    pPrimitive->SetPrimitiveType(ePrimitive_TriangleStrip);
    pPrimitive->SetVertexCount(m_instanceCount > 0 ? 4U : 0);
    pPrimitive->SetIndexCount(0);
    pPrimitive->SetInstanceCount(m_instanceCount);

    pPrimitive->SetVertexOffset(0);
    pPrimitive->SetIndexOffset(0);
    pPrimitive->SetInstanceOffset(0);
}

void PolylineRenderer::Draw(DrawingResourceTable& resTable)
{
    FlushData_Legacy(resTable, LineSegmentTex());

    UpdatePrimitive(resTable);
    //FlushStage(LineStage());
}

void PolylineRenderer::AttachSegments(std::vector<PolylineRendererComponent::LineSegmentGeom> geom, DrawingResourceTable& resTable)
{
    reset(MAX_SEGMENT_DATA_SIZE);

    const static uint32_t size = sizeof(PolylineRendererComponent::LineSegmentGeom);
    for (int i = 0; i < geom.size(); ++i)
        FillInstanceData(&geom[i], size, MAX_SEGMENT_DATA_SIZE, resTable, LineSegmentTex());
}

void PolylineRenderer::DefineShaderResource(DrawingResourceTable& resTable)
{
	switch (gpGlobal->GetConfiguration<GraphicsConfiguration>().GetDeviceType())
	{
	case eDevice_D3D11:
	case eDevice_D3D12:
		DefineVertexShader(LineSegmentVertexShader(), strPtr("Asset\\Shader\\HLSL\\Geometry\\segment.vs"), strPtr("Segment_VS"), resTable);
		DefinePixelShader(LineSegmentPixelShader(), strPtr("Asset\\Shader\\HLSL\\Geometry\\segment.ps"), strPtr("Segment_PS"), resTable);
		break;
	case eDevice_Vulkan:
		DefineVertexShader(LineSegmentVertexShader(), strPtr("Asset\\Shader\\SPIRV\\segment_vert.spv"), strPtr("Segment_VS"), resTable);
		DefinePixelShader(LineSegmentPixelShader(), strPtr("Asset\\Shader\\SPIRV\\segment_frag.spv"), strPtr("Segment_PS"), resTable);
		break;
	default:
		throw std::runtime_error("Shader resource path is undefined for this device type.");
		break;
	}

    DefineLinkedEffect(LineSegmentEffect(), LineSegmentVertexShader(), LineSegmentPixelShader(), resTable);
}

void PolylineRenderer::DefinePipelineStateResource(DrawingResourceTable& resTable)
{
    DefinePipelineState(NullInputVertexFormat(),
                        LineSegmentPipelineState(),
                        DefaultPrimitive(),
                        LineSegmentEffect(),
                        DefaultBlendState(),
                        DefaultRasterState(),
                        DefaultDepthState(),
                        ScreenTarget(),
                        resTable);
}

void PolylineRenderer::BindLineSegmentTexture(DrawingPass& pass, std::shared_ptr<std::string> pName)
{
    auto vertex_tex_slot = strPtr("VertexTex");
    AddTextureSlot(pass, vertex_tex_slot, strPtr("gLineSegmentTex"));
    BindResource(pass, vertex_tex_slot, pName);
}

std::shared_ptr<DrawingPass> PolylineRenderer::CreateSegmentPass(
    std::shared_ptr<std::string> pPassName,
    std::shared_ptr<std::string> pEffectName)
{
    auto pPass = CreatePass(pPassName);

    BindEffect(*pPass, pEffectName);
    BindPipelineState(*pPass, LineSegmentPipelineState());
    BindVertexInputs(*pPass);
    BindStates(*pPass);
    BindOutput(*pPass);
    BindPrimitive(*pPass, DefaultPrimitive());
    BindVaringStates(*pPass, DefaultVaringStates());
    BindConstants(*pPass);
    BindLineSegmentTexture(*pPass, LineSegmentTex());

    return pPass;
}

void PolylineRenderer::InitVertexID()
{
    for (uint32_t i = 0; i < MAX_SEGMENT_VERTEX_COUNT; ++i)
        m_sSegVertexID[i] = i;
}

void PolylineRenderer::InitInstanceID()
{
    for (uint32_t i = 0; i < MAX_SEGMENT_INSTANCE_COUNT; ++i)
        m_sSegInstanceID[i] = i;
}