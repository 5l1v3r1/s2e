#include <s2e/S2E.h>

#include "AvoidAddress.h"

namespace s2e {
namespace plugins {

S2E_DEFINE_PLUGIN(AvoidAddress, // Plugin class
                  "General use plugin to stop state execution if given addresses are hit.", // Plugin description
                  "", // Unused
                  "ProcessExecutionDetector"); // Plugin dependencies

void AvoidAddress::initialize() {
    // Register for callbacks
    s2e()->getCorePlugin()->onTranslateInstructionStart.connect(
        sigc::mem_fun(*this, &AvoidAddress::onTranslateInstruction));

    bool ok;                                                                                                                                                              
    ConfigFile *cfg = s2e()->getConfig();
                                                                                                                                                                          
    ConfigFile::integer_list addressList =                                                                                                                                
        cfg->getIntegerList(getConfigKey() + ".addressList", ConfigFile::integer_list(), &ok);                                                                            
                                                                                                                                                                          
    if (addressList.empty()) {                                                                                                                                            
        getWarningsStream() << "no addresses configured to avoid\n";                                                                                                      
        exit(-1);                                                                                                                                                         
    }                                                                                                                                                                     

    // Add these addresses to our set
    foreach2 (it, addressList.begin(), addressList.end()) { m_trackedAddresses.insert(*it); }
    m_procDetector = s2e()->getPlugin<ProcessExecutionDetector>();

}


void AvoidAddress::onTranslateInstruction(ExecutionSignal *signal, S2EExecutionState *state,
                                                  TranslationBlock *tb, uint64_t pc) {

    
    // The processes to track are declared in the S2E LUA configuration file
    if (!m_procDetector->isTracked(state)) {
        return;
    }


    // If this address is in our set
    if (m_trackedAddresses.find(pc) != m_trackedAddresses.end()) {
        // Register a handler for when the "avoid" code is executed
        signal->connect(sigc::mem_fun(*this, &AvoidAddress::onAvoid));
    }
}

void AvoidAddress::onAvoid(S2EExecutionState *state, uint64_t pc) {
    getDebugStream(state) << "Avoiding address " << hexval(pc);
    // There is no reason to continue execution any further. So kill the state
    s2e()->getExecutor()->terminateStateEarly(*state, "AvoidAddress avoided path.");
}


} // namespace plugins
} // namespace s2e
