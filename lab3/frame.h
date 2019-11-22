#ifndef FRAME_H
#define FRAME_H
namespace NYU {
namespace OperatingSystems {
    // Union type used for specific page algorithms
    // A much better implementation that does not violate the open-closed principle
    // Would to have a base frame and everything but that's too much for this assignment
    union FrameData {
    };
    class Frame {
    public:
        int mappedProcess() const;
        int mappedPage() const;
        void mappedProcess(int newProcess);
        void mappedPage(int newPage);
        // Sets both mappedProcess and mappedPage to -1
        void unmap();
        // Checks if the frame is mapped
        bool mapped() const;
        FrameData& data();

    private:
        int d_mappedProcess = -1;
        int d_mappedPage = -1;
        FrameData d_data;
    };
}
}
#endif