#pragma once

#include <NTGS/Graphics/GraphicsEnums.h>

namespace NTGS {
    struct Viewport {
        FLOAT TopLeftX = 0.f;
        FLOAT TopLeftY = 0.f;
        FLOAT Width = 0.f;
        FLOAT Height = 0.f;
        FLOAT MinDepth = 0.f;
        FLOAT MaxDepth = 1.f;

        Viewport() noexcept {}
        Viewport(FLOAT _TopLeftX, FLOAT _TopLeftY, FLOAT _Width, FLOAT _Height, FLOAT _MinDepth = 0, FLOAT _MaxDepth = 1) noexcept
            : TopLeftX(_TopLeftX)
            , TopLeftY(_TopLeftY)
            , Width(_Width)
            , Height(_Height)
            , MinDepth(_MinDepth)
            , MaxDepth(_MaxDepth) 
        {}
    };

    struct Rect {
        INT Left = 0;
        INT Top = 0;
        INT Right = 0;
        INT Bottom = 0;

        Rect() noexcept {}
        Rect(INT _Left, INT _Top, INT _Right, INT _Bottom) noexcept 
            : Left(_Left)
            , Top(_Top)
            , Right(_Right)
            , Bottom(_Bottom)
        {}
    };

    struct Box {
        UINT MinX = 0;
        UINT MaxX = 0;
        UINT MinY = 0;
        UINT MaxY = 0;
        UINT MinZ = 0;
        UINT MaxZ = 1;

        Box() noexcept {}

        Box(UINT _MinX, UINT _MaxX,
            UINT _MinY, UINT _MaxY,
            UINT _MinZ, UINT _MaxZ) 
            : MinX(_MinX), MaxX(_MaxX)
            , MinY(_MinY), MaxY(_MaxY)
            , MinZ(_MinZ), MaxZ(_MaxZ)
        {}

        Box(UINT _MinX, UINT _MaxX,
            UINT _MinY, UINT _MaxY) :
            Box(_MinX, _MaxX, _MinY, _MaxY, 0, 1)
        {}

        Box(UINT _MinX, UINT _MaxX) :
            Box(_MinX, _MaxX, 0, 0, 0, 1)
        {}
    };
}