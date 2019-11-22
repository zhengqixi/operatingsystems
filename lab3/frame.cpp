#include "frame.h"
namespace NYU {
namespace OperatingSystems {
    int Frame::mappedProcess()
    {
        return d_mappedProcess;
    }
    int Frame::mappedPage()
    {
        return d_mappedPage;
    }
    void Frame::mappedProcess(int newProcess)
    {
        d_mappedProcess = newProcess;
    }
    void Frame::mappedPage(int newPage)
    {
        d_mappedPage = newPage;
    }
    // Sets both mappedProcess and mappedPage to -1
    void Frame::unmap()
    {
        d_mappedPage = -1;
        d_mappedProcess = -1;
    }
    bool Frame::mapped()
    {
        return !d_mappedProcess == -1;
    }
    FrameData& Frame::data()
    {
        return d_data;
    }
}
}