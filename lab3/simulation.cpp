#include "simulation.h"
#include "page.h"
#include "pagehandler.h"
#include "process.h"
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
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
    void Simulation::run(std::ostream& output, bool perInstOutput)
    {
        unsigned long currentInst = 0;
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
                continue;
            }
            if (inst == 'e') {
                processExist(currentProcess, output, perInstOutput);
                currentProcess = -1;
                ++currentInst;
                continue;
            }
            auto& page = d_processList[currentProcess][operand];
            if (!page.present()) {
                if (!d_processList[currentProcess].setPageBits(operand)) {
                    // SEGV
                    ++currentInst;
                    if (perInstOutput) {
                        output << " SEGV\n";
                    }
                    continue;
                } else {
                    auto frameIndex = d_faultHandler->selectFrame();
                    auto& frameEntry = (*d_faultHandler)[frameIndex];
                    if (frameEntry.mapped()) {
                        auto mappedProcess = frameEntry.mappedProcess();
                        auto mappedPage = frameEntry.mappedPage();
                        if (perInstOutput) {
                            output << " UNMAP " << mappedProcess << ':' << mappedPage << '\n';
                        }
                        auto& sadpage = d_processList[mappedProcess][mappedPage];
                        if (sadpage.modified()) {
                            if (sadpage.fileMapped()) {
                                if (perInstOutput) {
                                    output << " FOUT\n";
                                }
                            } else {
                                if (perInstOutput) {
                                    output << " OUT\n";
                                }
                            }
                            sadpage.modified(false);
                        }
                        sadpage.present(false);
                        sadpage.pagedOut(true);
                    }
                    frameEntry.mappedPage(operand);
                    frameEntry.mappedProcess(currentProcess);
                    page.assignedFrame(frameIndex);
                    if (page.pagedOut()) {
                        // Previously paged out
                        if (page.fileMapped()) {
                            // Filemapped, we bring it back from file
                            if (perInstOutput) {
                                output << " FIN\n";
                            }
                        } else {
                            // Fetch from swap
                            if (perInstOutput) {
                                output << " IN\n";
                            }
                        }
                    } else if (page.fileMapped()) {
                        if (perInstOutput) {
                            output << " FIN\n";
                        }
                        // File mapped, we bring it back from file
                    } else {
                        // ZERO new page
                        if (perInstOutput) {
                            output << " ZERO\n";
                        }
                    }
                    if (perInstOutput) {
                        output << " MAP " << frameIndex << '\n';
                    }
                }
            }
            page.referenced(true);
            if (inst == 'w') {
                if (page.writeProtected()) {
                    if (perInstOutput) {
                        output << " SEGPROT\n";
                    }
                } else {
                    page.modified(true);
                }
            }
            ++currentInst;
        }
    }
    void Simulation::processExist(int processNum, std::ostream& output, bool unmapOut)
    {
        auto& pageTable = d_processList[processNum].pageTable();
        for (int i = 0; i < pageTable.size(); ++i) {
            auto& pte = pageTable[i];
            if (pte.present()) {
                // Remove from global table
                d_faultHandler->freeFrame(pte.assignedFrame());
                if (unmapOut) {
                    output << " UNMAP " << processNum << ':' << i << '\n';
                    if (pte.fileMapped() && pte.modified()) {
                        output << " FOUT\n";
                    }
                }
            }
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