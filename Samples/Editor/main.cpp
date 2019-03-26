#include "Application/IApplication.h"

using namespace NTGS;

int main() {
    WindowConfig config;
    auto app = IApplication::Create();
    app->Initialize();
    app->CreateNamedWindow("Editor", config, nullptr);
    app->Run();
    app->Finalize();

    return 0;
}