#include "EllipseArcRenderer.h"
#include "DrawingDevice.h"

using namespace Engine;

uint32_t EllipseArcRenderer::m_sEllipseArcVertexID[MAX_ELLIPSE_ARC_VERTEX_COUNT] = { 0 };
uint32_t EllipseArcRenderer::m_sEllipseArcInstanceID[MAX_ELLIPSE_ARC_INSTANCE_COUNT] = { 0 };

EllipseArcRenderer::EllipseArcRenderer() : BaseRenderer()
{
    InitVertexID();
    InitInstanceID();

    m_pData = new char[MAX_ELLIPSE_ARC_DATA_SIZE];
    reset(MAX_ELLIPSE_ARC_DATA_SIZE);
}

void EllipseArcRenderer::DefineResources(DrawingResourceTable& resTable)
{
    DefineDefaultResources(resTable);
    DefineShaderResource(resTable);
    DefinePipelineStateResource(resTable);

    DefineStaticVertexBuffer(VB_VertexID(), 4, MAX_ELLIPSE_ARC_VERTEX_COUNT, &m_sEllipseArcVertexID, MAX_ELLIPSE_ARC_VERTEX_COUNT * 4, resTable);
    DefineStaticVertexBuffer(VB_InstanceID(), 4, MAX_ELLIPSE_ARC_INSTANCE_COUNT, &m_sEllipseArcInstanceID, MAX_ELLIPSE_ARC_INSTANCE_COUNT * 4, resTable);

    DefineDynamicTextureWithInit(EllipseArcTex(), eFormat_R32G32B32_FLOAT, MAX_ELLIPSE_ARC_ELEMENT_COUNT * TEX_ELEMENT_COUNT_PER_ELLIPSE_ARC, m_pData, m_size, resTable);
}

void EllipseArcRenderer::BuildPass()
{
    //auto pStage = CreateStage(EllipseArcStage());
    //pStage->AppendDrawingPass(CreateEllipseArcPass(EllipseArcPass(), EllipseArcEffect()));
    //m_stageTable.AddDrawingStage(pStage->GetName(), pStage);

	auto& pMainPass = CreateEllipseArcPass(EllipseArcPass(), EllipseArcEffect());
	m_passTable[EllipseArcPass()] = pMainPass;
}

void EllipseArcRenderer::SetupBuffers(DrawingResourceTable& resTable)
{
}

void EllipseArcRenderer::UpdatePrimitive(DrawingResourceTable& resTable)
{
    UpdateConstants(resTable);

    auto pEntry = resTable.GetResourceEntry(DefaultPrimitive());
    if (pEntry == nullptr)
        return;

    auto pPrimitive = std::dynamic_pointer_cast<DrawingPrimitive>(pEntry->GetResource());
    if (pPrimitive == nullptr)
        return;

    pPrimitive->SetPrimitiveType(ePrimitive_TriangleStrip);
    pPrimitive->SetVertexCount(m_instanceCount > 0 ? 32U : 0);
    pPrimitive->SetIndexCount(0);
    pPrimitive->SetInstanceCount(m_instanceCount);

    pPrimitive->SetVertexOffset(0);
    pPrimitive->SetIndexOffset(0);
    pPrimitive->SetInstanceOffset(0);
}

void EllipseArcRenderer::Draw(DrawingResourceTable& resTable)
{
    FlushData_Legacy(resTable, EllipseArcTex());

    UpdatePrimitive(resTable);
    //FlushStage(EllipseArcStage());
}

void EllipseArcRenderer::AttachEllipseArcs(std::vector<EllipseArcRendererComponent::EllipseArcGeom> geom, DrawingResourceTable& resTable)
{
    reset(MAX_ELLIPSE_ARC_DATA_SIZE);

    const static uint32_t size = sizeof(EllipseArcRendererComponent::EllipseArcGeom);
    for (int i = 0; i < geom.size(); ++i)
        FillInstanceData(&geom[i], size, MAX_ELLIPSE_ARC_DATA_SIZE, resTable, EllipseArcTex());
}

void EllipseArcRenderer::DefineShaderResource(DrawingResourceTable& resTable)
{
	switch (gpGlobal->GetConfiguration<GraphicsConfiguration>().GetDeviceType())
	{
	case eDevice_D3D11:
	case eDevice_D3D12:
		DefineVertexShader(EllipseVertexShader(), strPtr("Asset\\Shader\\HLSL\\Geometry\\ellipse.vs"), strPtr("Ellipse_Dash_VS"), resTable);
		DefinePixelShader(EllipsePixelShader(), strPtr("Asset\\Shader\\HLSL\\Geometry\\ellipse.ps"), strPtr("Ellipse_Dash_PS"), resTable);
		break;
	case eDevice_Vulkan:
		DefineVertexShader(EllipseVertexShader(), strPtr("Asset\\Shader\\SPIRV\\ellipse_vert.spv"), strPtr("Ellipse_Dash_VS"), resTable);
		DefinePixelShader(EllipsePixelShader(), strPtr("Asset\\Shader\\SPIRV\\ellipse_frag.spv"), strPtr("Ellipse_Dash_PS"), resTable);
		break;
	default:
		throw std::runtime_error("Shader resource path is undefined for this device type.");
		break;
	}

    DefineLinkedEffect(EllipseArcEffect(), EllipseVertexShader(), EllipsePixelShader(), resTable);
}

void EllipseArcRenderer::DefinePipelineStateResource(DrawingResourceTable& resTable)
{
    DefinePipelineState(NullInputVertexFormat(),
                        EllipseArcPipelineState(),
                        DefaultPrimitive(),
                        EllipseArcEffect(),
                        DefaultBlendState(),
                        DefaultRasterState(),
                        DefaultDepthState(),
                        ScreenTarget(),
                        resTable);
}

void EllipseArcRenderer::BindEllipseArcTexture(DrawingPass& pass, std::shared_ptr<std::string> pName)
{
    auto circle_arc_tex_slot = strPtr("EllipseArcTex");
    AddTextureSlot(pass, circle_arc_tex_slot, strPtr("gEllipseArcTex"));
    BindResource(pass, circle_arc_tex_slot, pName);
}

std::shared_ptr<DrawingPass> EllipseArcRenderer::CreateEllipseArcPass(
    std::shared_ptr<std::string> pPassName,
    std::shared_ptr<std::string> pEffectName)
{
    auto pPass = CreatePass(pPassName);

    BindEffect(*pPass, pEffectName);
    BindPipelineState(*pPass, EllipseArcPipelineState());
    BindVertexInputs(*pPass);
    BindStates(*pPass);
    BindOutput(*pPass);
    BindPrimitive(*pPass, DefaultPrimitive());
    BindVaringStates(*pPass, DefaultVaringStates());
    BindConstants(*pPass);
    BindEllipseArcTexture(*pPass, EllipseArcTex());

    return pPass;
}

void EllipseArcRenderer::InitVertexID()
{
    for (uint32_t i = 0; i < MAX_ELLIPSE_ARC_VERTEX_COUNT; ++i)
        m_sEllipseArcVertexID[i] = i;
}

void EllipseArcRenderer::InitInstanceID()
{
    for (uint32_t i = 0; i < MAX_ELLIPSE_ARC_INSTANCE_COUNT; ++i)
        m_sEllipseArcInstanceID[i] = i;
}