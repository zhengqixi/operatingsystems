#ifndef PAGE_H
#define PAGE_H
namespace NYU {
namespace OperatingSystems {
    class Page {
    public:
        Page();
        bool present() const;
        void present(bool set);
        bool writeProtected() const;
        void writeProtected(bool set);
        bool modified() const;
        void modified(bool set);
        bool referenced() const;
        void referenced(bool set);
        bool pagedOut() const;
        void pagedOut(bool set);
        unsigned int assignedFrame() const;
        void assignedFrame(unsigned int newFrame);
        bool fileMapped() const;
        void fileMapped(bool set);
        bool vma() const;
        void vma(bool set);

    private:
        unsigned int d_present : 1;
        unsigned int d_writeProtected : 1;
        unsigned int d_modified : 1;
        unsigned int d_referenced : 1;
        unsigned int d_pagedOut : 1;
        unsigned int d_assignedFrame : 7;
        unsigned int d_vma : 1;
        unsigned int d_fileMapped : 1;
        unsigned int d_padding : 18;
    };
}
} // namespace NYU

#endif