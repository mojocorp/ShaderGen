#pragma once

#if TODO
#include "SGFrame.h"

class App : public QApplication
{
public:

    App();

    SGFrame* GetFrame() {return frame;}
    // C-style output methods
    //Callers of Errorf should include a period and a linefeed
    void Errorf(const char* format, ...);
    void Printf(const char* format, ...);
    void Statusf(const char* format, ...);
    void Ready(){frame->SetStatusText("Ready.", 0);}
private:
    SGFrame * frame;
};
#endif
