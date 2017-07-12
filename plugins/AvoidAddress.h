#ifndef S2E_PLUGINS_AVOID_ADDRESS_H
#define S2E_PLUGINS_AVOID_ADDRESS_H

#include <s2e/CorePlugin.h>
#include <s2e/Plugin.h>
#include <s2e/Plugins/OSMonitors/Support/ProcessExecutionDetector.h>
#include <s2e/Utils.h>

namespace s2e {
namespace plugins {

class AvoidAddress : public Plugin {
    // Declares an S2E plugin
    S2E_PLUGIN

public:
    // Our constructor doesn't need to do anything
    AvoidAddress(S2E *s2e) : Plugin(s2e) { }

    // This will be called by S2E when registering and configuring the different plugins
    void initialize();

private:
    // The method signature corresponds to the onTranslateInstructionStart signal template in CorePlugin.h
    void onTranslateInstruction(ExecutionSignal *signal, S2EExecutionState *state, TranslationBlock *tb,
                                uint64_t pc);

    void onAvoid(S2EExecutionState *state, uint64_t pc);
    typedef std::tr1::unordered_set<uint64_t> IntegerSet;
    IntegerSet m_trackedAddresses;

    ProcessExecutionDetector *m_procDetector;

};

} // namespace plugins
} // namespace s2e

#endif
