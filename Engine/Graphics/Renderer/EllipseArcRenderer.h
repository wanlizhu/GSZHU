#pragma

#include "BaseRenderer.h"
#include "Component\EllipseArcRendererComponent.h"

namespace Engine
{
    class EllipseArcRenderer final : public BaseRenderer
    {
    public:
        EllipseArcRenderer();
        virtual ~EllipseArcRenderer() {}

        void DefineResources(DrawingResourceTable& resTable) override;
		void SetupBuffers(DrawingResourceTable& resTable) override;

        void BuildPass() override;

        void UpdatePrimitive(DrawingResourceTable& resTable) override;
        void Draw(DrawingResourceTable& resTable) override;

        void AttachEllipseArcs(std::vector<EllipseArcRendererComponent::EllipseArcGeom> geom, DrawingResourceTable& resTable);

	private:
		void BeginDrawPass() override {};
		void EndDrawPass() override {};

		void FlushData() override {};
		void ResetData() override {};

    public:
        // Define shader resource names
        FuncResourceName(EllipseVertexShader);
        FuncResourceName(EllipsePixelShader);
        // Define pipeline state names
        FuncResourceName(EllipseArcPipelineState);
        // Define effect resource names
        FuncResourceName(EllipseArcEffect);
        // Define stage resource names
        FuncResourceName(EllipseArcStage);
        // Define effect resource names
        FuncResourceName(EllipseArcPass);
        // Define geometry texture names
        FuncResourceName(EllipseArcTex);

    protected:
        static const uint32_t TEX_ELEMENT_COUNT_PER_ELLIPSE_ARC = 3;

        static const uint32_t MAX_ELLIPSE_ARC_VERTEX_COUNT      = 16384 * 32;
        static const uint32_t MAX_ELLIPSE_ARC_INDEX_COUNT       = 16384;

        static const uint32_t MAX_ELLIPSE_ARC_ELEMENT_COUNT     = 16384;
        static const uint32_t MAX_ELLIPSE_ARC_INSTANCE_COUNT    = 16384;

        const static uint32_t MAX_ELLIPSE_ARC_DATA_SIZE         = 16384 * 36;

        static uint32_t m_sEllipseArcVertexID[MAX_ELLIPSE_ARC_VERTEX_COUNT];
        static uint32_t m_sEllipseArcInstanceID[MAX_ELLIPSE_ARC_INSTANCE_COUNT];

        static void InitVertexID();
        static void InitInstanceID();

        void DefineShaderResource(DrawingResourceTable& resTable);
        void DefinePipelineStateResource(DrawingResourceTable& resTable);

        void BindEllipseArcTexture(DrawingPass& pass, std::shared_ptr<std::string> pName);

    private:
        std::shared_ptr<DrawingPass> CreateEllipseArcPass(
            std::shared_ptr<std::string> pPassName,
            std::shared_ptr<std::string> pEffectName);
    };
}