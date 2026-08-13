// Copyright 2026 W.H.A.T. I.F. Project. All Rights Reserved.
// WhatIfEditor module implementation

#include "WhatIfEditor.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogWhatIfEditor);

class FWhatIfEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        UE_LOG(LogWhatIfEditor, Log, TEXT("WhatIfEditor module has started."));
    }

    virtual void ShutdownModule() override
    {
        UE_LOG(LogWhatIfEditor, Log, TEXT("WhatIfEditor module has shut down."));
    }
};

IMPLEMENT_MODULE(FWhatIfEditorModule, WhatIfEditor);
