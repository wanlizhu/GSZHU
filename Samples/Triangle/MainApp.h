#pragma once

#include "Wanli.h"

class MainApp : public Wanli::IApplication
{
public:
    MainApp();
    virtual ~MainApp();

    virtual void Update() override;
};