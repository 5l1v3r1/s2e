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

    m_detector = static_cast<ModuleExecutionDetector *>(s2e()->getPlugin("ModuleExecutionDetector"));
    m_relative = s2e()->getConfig()->getBool(getConfigKey() + ".relative", false, NULL);

    m_monitor = static_cast<OSMonitor *>(s2e()->getPlugin("OSMonitor"));

    if ( m_relative )
        getDebugStream() << "AvoidAddress is using relative addresses.\n";
    else
        getDebugStream() << "AvoidAddress is using absolute addresses.\n";

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

    // Grab process addresses as they load
    //m_monitor->onModuleLoad.connect(sigc::mem_fun(*this, &AvoidAddress::onModuleLoad));

}


void AvoidAddress::onTranslateInstruction(ExecutionSignal *signal, S2EExecutionState *state,
                                                  TranslationBlock *tb, uint64_t pc) {

    // The processes to track are declared in the S2E LUA configuration file
    if (!m_procDetector->isTracked(state)) {
        return;
    }

    //getDebugStream(state) << "AvoidAddress: onTranslateInstruction in tracked PC of " << hexval(pc) << "\n";

    // ModuleDescriptor to help figure out the real address
    const ModuleDescriptor *md = m_detector->getCurrentDescriptor(state);
    if (!md) {
        return;
    }

    //getDebugStream(state) << "AvoidAddress: Got a descriptor, name: " << md->Name << "\n";

    // A little backwards since we're defining the address first... But adjust for the base.
    if ( m_relative ) {
	pc -= md->LoadBase;
    }

    //getDebugStream(state) << "AvoidAddress: Checking if we want to hook address " << hexval(pc) << " ... ";

    // If this address is in our set
    if (m_trackedAddresses.find(pc) != m_trackedAddresses.end()) {
	getDebugStream(state) << "AvoidAddress: Hooking address " << hexval(pc) << "\n";
        // Register a handler for when the "avoid" code is executed
        signal->connect(sigc::mem_fun(*this, &AvoidAddress::onAvoid));
    }
}

void AvoidAddress::onAvoid(S2EExecutionState *state, uint64_t pc) {
    getDebugStream(state) << "Avoiding address " << hexval(pc) << "\n";
    // There is no reason to continue execution any further. So kill the state
    s2e()->getExecutor()->terminateStateEarly(*state, "AvoidAddress avoided path.");
}

/*
void AvoidAddress::onModuleLoad(S2EExecutionState *state, const ModuleDescriptor &module) {

    // Ignore modules that are not ours.
    if (!m_procDetector->isTracked(state)) {
        return;
    }

    const ModuleDescriptor *md = m_detector->getCurrentDescriptor(state);
    if (!md) {
        return;
    }

    getDebugStream(state) << "AvoidAddress: onModuleLoad hit. saving module \"" << md->Name << "\"\n";

}
*/

} // namespace plugins
} // namespace s2e
