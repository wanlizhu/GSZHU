#include "Utils/Platform/IProgressBar.h"
#include "ProgressBar_Win32.h"

namespace GS
{
	IProgressBar::SharedPtr IProgressBar::Create(const IProgressBar::Desc& desc)
	{
		return ProgressBar_Win32::Create(desc);
	}

	IProgressBar::IProgressBar(const std::string& name)
		: Object(name)
	{}

	IProgressBar::~IProgressBar()
	{}
}