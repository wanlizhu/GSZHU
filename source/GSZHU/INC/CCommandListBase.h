#pragma once

#include <GSZHU/ICommandList.h>
#include <GSZHU/DESC/SCommandListDesc.h>
#include "CDeviceObjectBase.h"

namespace GSZHU {
      template<typename INTERFACE, typename RENDER_DEVICE>
      class CCommandListBase : public CDeviceObjectBase<INTERFACE, RENDER_DEVICE> {
      public:
          using BASE = CDeviceObjectBase<INTERFACE, RENDER_DEVICE>;

          CCommandListBase(RENDER_DEVICE* Device)
              : BASE(Device, SCommandListDesc())
          {}
          ~CCommandListBase() {}
      };
}