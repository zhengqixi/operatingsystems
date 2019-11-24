#include "simulation.h"
#include "page.h"
#include "pagehandler.h"
#include "process.h"
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    Simulation::Simulation(std::istream& input, PageHandler* faultHandler, int numPages)
        : d_faultHandler{ faultHandler }
        , d_input{ input }
    {
        // Create process list
        // No need to worry about bad inputs :)
        int processCount = std::stoi(nextLine());
        d_processList.reserve(processCount);
        for (int i = 0; i < processCount; ++i) {
            d_processList.push_back(Process(numPages));
            int vmaCount = std::stoi(nextLine());
            for (int j = 0; j < vmaCount; ++j) {
                std::stringstream lineParser(nextLine());
                int start, end, writeProtected, fileMapped;
                lineParser >> start >> end >> writeProtected >> fileMapped;
                d_processList[i].addVMA(start, end,
                    static_cast<bool>(fileMapped),
                    static_cast<bool>(writeProtected));
            }
        }
    }
    void Simulation::run(std::ostream& output, bool perInstOutput, bool finalFrameTable, bool finalPageTable, bool processSummary, bool summary)
    {
        unsigned long long cycleCost = 0;
        unsigned long long contextSwitches = 0;
        unsigned long long processExists = 0;
        unsigned long long currentInst = 0;
        int currentProcess = -1;
        char inst = '\0';
        int operand = -1;
        while (nextInstruction(inst, operand)) {
            if (perInstOutput) {
                output << currentInst << ": ==> " << inst
                       << ' ' << operand << '\n';
            }
            if (inst == 'c') {
                currentProcess = operand;
                ++currentInst;
                ++contextSwitches;
                cycleCost += d_contextSwitch;
                continue;
            }
            if (inst == 'e') {
                auto pageData = processExist(currentProcess, output, perInstOutput);
                int unmapedPages = pageData.first;
                int fouted = pageData.second;
                d_processList[currentProcess].unmaps() += unmapedPages;
                d_processList[currentProcess].fouts() += fouted;
                cycleCost += d_maps * unmapedPages;
                cycleCost += d_fileInOut * fouted;
                cycleCost += d_processExist;
                currentProcess = -1;
                ++currentInst;
                ++processExists;
                continue;
            }
            auto& page = d_processList[currentProcess][operand];
            if (!page.present()) {
                if (!d_processList[currentProcess].setPageBits(operand)) {
                    // SEGV
                    ++currentInst;
                    cycleCost += d_segV;
                    ++d_processList[currentProcess].segvs();
                    if (perInstOutput) {
                        output << " SEGV\n";
                    }
                    continue;
                } else {
                    auto frameIndex = d_faultHandler->selectFrame(currentInst, d_processList);
                    auto& frameEntry = (*d_faultHandler)[frameIndex];
                    if (frameEntry.mapped()) {
                        auto mappedProcess = frameEntry.mappedProcess();
                        auto mappedPage = frameEntry.mappedPage();
                        cycleCost += d_maps;
                        ++d_processList[mappedProcess].unmaps();
                        if (perInstOutput) {
                            output << " UNMAP " << mappedProcess << ':' << mappedPage << '\n';
                        }
                        auto& sadpage = d_processList[mappedProcess][mappedPage];
                        if (sadpage.modified()) {
                            if (sadpage.fileMapped()) {
                                cycleCost += d_fileInOut;
                                ++d_processList[mappedProcess].fouts();
                                if (perInstOutput) {
                                    output << " FOUT\n";
                                }
                            } else {
                                cycleCost += d_pageInOut;
                                ++d_processList[mappedProcess].outs();
                                if (perInstOutput) {
                                    output << " OUT\n";
                                }
                                sadpage.pagedOut(true);
                            }
                            sadpage.modified(false);
                        }
                        sadpage.present(false);
                    }
                    frameEntry.mappedPage(operand);
                    frameEntry.mappedProcess(currentProcess);
                    page.assignedFrame(frameIndex);
                    if (page.pagedOut()) {
                        // Previously paged out
                        // Fetch from swap
                        cycleCost += d_pageInOut;
                        ++d_processList[currentProcess].ins();
                        if (perInstOutput) {
                            output << " IN\n";
                        }
                    } else if (page.fileMapped()) {
                        cycleCost += d_fileInOut;
                        ++d_processList[currentProcess].fins();
                        if (perInstOutput) {
                            output << " FIN\n";
                        }
                        // File mapped, we bring it back from file
                    } else {
                        // ZERO new page
                        cycleCost += d_zero;
                        ++d_processList[currentProcess].zeros();
                        if (perInstOutput) {
                            output << " ZERO\n";
                        }
                    }
                    cycleCost += d_maps;
                    ++d_processList[currentProcess].maps();
                    if (perInstOutput) {
                        output << " MAP " << frameIndex << '\n';
                    }
                }
            }
            page.referenced(true);
            cycleCost += d_access;
            if (inst == 'w') {
                if (page.writeProtected()) {
                    cycleCost += d_segProt;
                    ++d_processList[currentProcess].segprots();
                    if (perInstOutput) {
                        output << " SEGPROT\n";
                    }
                } else {
                    page.modified(true);
                }
            }
            ++currentInst;
        }
        if (finalPageTable) {
            printAllPageTables(output);
        }
        if (finalFrameTable) {
            printFrametable(output);
        }
        if (processSummary) {
            printAllProcessSummary(output);
        }
        if (summary) {
            output << "TOTALCOST " << currentInst << ' ' << contextSwitches
                   << ' ' << processExists << ' ' << cycleCost << '\n';
        }
    }
    std::pair<int, int> Simulation::processExist(int processNum, std::ostream& output, bool unmapOut)
    {
        int unmappedPages = 0;
        int fout = 0;
        if (unmapOut) {
            output << "EXIT current process " << processNum << '\n';
        }
        auto& pageTable = d_processList[processNum].pageTable();
        for (int i = 0; i < pageTable.size(); ++i) {
            auto& pte = pageTable[i];
            if (pte.present()) {
                // Remove from global table
                ++unmappedPages;
                d_faultHandler->freeFrame(pte.assignedFrame());
                if (unmapOut) {
                    output << " UNMAP " << processNum << ':' << i << '\n';
                    if (pte.fileMapped() && pte.modified()) {
                        ++fout;
                        output << " FOUT\n";
                    }
                }
            }
            pte.clear();
        }
        return std::make_pair(unmappedPages, fout);
    }
    void Simulation::printFrametable(std::ostream& output)
    {
        output << "FT:";
        auto& frameTable = d_faultHandler->globalFrames();
        for (int i = 0; i < frameTable.size(); ++i) {
            output << " ";
            if (frameTable[i].mapped()) {
                output << frameTable[i].mappedProcess() << ':' << frameTable[i].mappedPage();
            } else {
                output << '*';
            }
        }
        output << '\n';
    }
    void Simulation::printProcessPageTable(int processNum, std::ostream& output)
    {
        output << "PT[" << processNum << "]:";
        auto& pageList = d_processList[processNum].pageTable();
        for (int i = 0; i < pageList.size(); ++i) {
            Page& pte = pageList[i];
            output << ' ';
            if (!pte.present() && !pte.pagedOut()) {
                output << '*';
                continue;
            }
            if (!pte.present() && pte.pagedOut()) {
                output << '#';
                continue;
            }
            output << i << ':';
            char referenced = pte.referenced() ? 'R' : '-';
            char modified = pte.modified() ? 'M' : '-';
            char hasSwap = pte.pagedOut() ? 'S' : '-';
            output << referenced << modified << hasSwap;
        }
        output << '\n';
    }
    void Simulation::printAllPageTables(std::ostream& output)
    {
        for (int i = 0; i < d_processList.size(); ++i) {
            printProcessPageTable(i, output);
        }
    }
    void Simulation::printProcessSummary(int processNum, std::ostream& output)
    {
        Process& process = d_processList[processNum];
        output << "PROC[" << processNum << "]: U=" << process.unmaps()
               << " M=" << process.maps() << " I=" << process.ins() << " O=" << process.outs()
               << " FI=" << process.fins() << " FO=" << process.fouts() << " Z=" << process.zeros()
               << " SV=" << process.segvs() << " SP=" << process.segprots() << '\n';
    }
    void Simulation::printAllProcessSummary(std::ostream& output)
    {
        for (int i = 0; i < d_processList.size(); ++i) {
            printProcessSummary(i, output);
        }
    }
    bool Simulation::nextInstruction(char& inst, int& operand)
    {
        std::string instruction(nextLine());
        if (instruction.empty()) {
            return false;
        }
        std::stringstream instParser(instruction);
        instParser >> inst >> operand;
        return true;
    }
    std::string Simulation::nextLine()
    {
        std::string line;
        while (std::getline(d_input, line)) {
            if (line.front() == '#') {
                continue;
            }
            return line;
        }
        return "";
    }
}
}