#pragma

#include "BaseRenderer.h"
#include "Component\PolylineRendererComponent.h"

namespace Engine
{
    class PolylineRenderer final : public BaseRenderer
    {
    public:
        PolylineRenderer();
        virtual ~PolylineRenderer() {}

		void DefineResources(DrawingResourceTable& resTable) override;
		void SetupBuffers(DrawingResourceTable& resTable) override;

        void BuildPass() override;

        void UpdatePrimitive(DrawingResourceTable& resTable) override;
        void Draw(DrawingResourceTable& resTable) override;

        void AttachSegments(std::vector<PolylineRendererComponent::LineSegmentGeom> geom, DrawingResourceTable& resTable);

	private:
		void BeginDrawPass() override {};
		void EndDrawPass() override {};

		void FlushData() override {};
		void ResetData() override {};

    public:
        // Define shader resource names
        FuncResourceName(LineSegmentVertexShader);
        FuncResourceName(LineSegmentPixelShader);
        // Define pipeline state names
        FuncResourceName(LineSegmentPipelineState);
        // Define effect resource names
        FuncResourceName(LineSegmentEffect);
        // Define stage resource names
        FuncResourceName(LineStage);
        // Define effect resource names
        FuncResourceName(LineSegmentPass);
        // Define geometry texture names
        FuncResourceName(LineSegmentTex);

    protected:
        static const uint32_t SIGNLE_SEGMENT_TEX_ELEMENT_COUNT  = 2;

        static const uint32_t MAX_SEGMENT_VERTEX_COUNT          = 16384 * 4;
        static const uint32_t MAX_SEGMENT_INDEX_COUNT           = 16384;

        static const uint32_t MAX_SEGMENT_ELEMENT_COUNT         = 16384;
        static const uint32_t MAX_SEGMENT_INSTANCE_COUNT        = 16384;

        const static uint32_t MAX_SEGMENT_DATA_SIZE             = 16384 * 24;

        static uint32_t m_sSegVertexID[MAX_SEGMENT_VERTEX_COUNT];
        static uint32_t m_sSegInstanceID[MAX_SEGMENT_INSTANCE_COUNT];

        static void InitVertexID();
        static void InitInstanceID();

        void DefineShaderResource(DrawingResourceTable& resTable);
        void DefinePipelineStateResource(DrawingResourceTable& resTable);

        void BindLineSegmentTexture(DrawingPass& pass, std::shared_ptr<std::string> pName);

    private:
        std::shared_ptr<DrawingPass> CreateSegmentPass(
            std::shared_ptr<std::string> pPassName,
            std::shared_ptr<std::string> pEffectName);
    };
}