#ifndef FRAME_H
#define FRAME_H
namespace NYU {
namespace OperatingSystems {
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

    private:
        int d_mappedProcess = -1;
        int d_mappedPage = -1;
    };
}
}
#endif