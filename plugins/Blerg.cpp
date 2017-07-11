#include <s2e/S2E.h>

#include "Blerg.h"

#include <s2e/ConfigFile.h>

namespace s2e {
namespace plugins {

//Define a plugin whose class is InstructionTracker and called "InstructionTracker".
//The plugin does not have any dependency.
S2E_DEFINE_PLUGIN(Blerg, "Tutorial - Blerg", "Blerg",);

void Blerg::initialize() {
    //m_address = (uint64_t) s2e()->getConfig()->getInt(getConfigKey() + ".addressToTrack");

    s2e()->getCorePlugin()->onTranslateInstructionStart.connect(
        sigc::mem_fun(*this, &Blerg::onTranslateInstruction));

}

void Blerg::onTranslateInstruction(ExecutionSignal *signal,
                                                S2EExecutionState *state,
                                                TranslationBlock *tb,
                                                uint64_t pc) {
    if(pc == 0x00400636) {
        // When we find an interesting address, ask S2E to invoke our
        // callback when the address is actually executed.
        signal->connect(sigc::mem_fun(*this, &Blerg::onInstructionExecution));
    }
}

// This callback is called only when the instruction at our address is executed.
// The callback incurs zero overhead for all other instructions.
void Blerg::onInstructionExecution(S2EExecutionState *state, uint64_t pc) {
    s2e()->getDebugStream() << "Executing blerg instruction at " << hexval(pc) << '\n';   
    // The plugins can arbitrarily modify/observe the current execution state via
    // the execution state pointer.
    // Plugins can also call the s2e() method to use the S2E API.
}

} // namespace plugins
} // namespace s2e

