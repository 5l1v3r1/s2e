#ifndef S2E_PLUGINS_INSTRTRACKER_H
#define S2E_PLUGINS_INSTRTRACKER_H

#include <s2e/Plugin.h>
#include <s2e/CorePlugin.h>
#include <s2e/S2EExecutionState.h>
#include <s2e/S2E.h>
#include <s2e/Utils.h>

namespace s2e {
namespace plugins {

class Blerg : public Plugin {
    S2E_PLUGIN

private:
    /*
    ModuleExecutionDetector *m_executionDetector;
    ExecutionTracer *m_executionTracer;

    TranslationBlock *m_tb;
    sigc::connection m_tbConnection;
    */
    void onTranslateInstruction(ExecutionSignal *signal, S2EExecutionState *state, TranslationBlock *tb, uint64_t pc);
    void onInstructionExecution(S2EExecutionState *state, uint64_t pc);

public:
    Blerg(S2E *s2e) : Plugin(s2e) {}

    void initialize();
};


} // namespace plugins
} // namespace s2e

#endif

