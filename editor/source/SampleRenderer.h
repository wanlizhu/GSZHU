#pragma once

#include "GSZHU/IRenderer.h"

class SampleRenderer : public GSZHU::IRenderer {
public:
    using Pointer = std::shared_ptr<SampleRenderer>;

    SampleRenderer() = default;
    virtual ~SampleRenderer();

    static SampleRenderer::Pointer Create();

    virtual bool Initialize(GSZHU::WINDOW_HANDLE win) override;
    virtual void Destroy() override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void Present() override;
    virtual void Resize(int width, int height) override;

private:

};