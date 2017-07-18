# Overview
The goal for the `AvoidAddress` plugin is to simply accept a list of addresses that you wish to avoid (likely discovered through static or other methods). It will then kill any state that hits this address.

# Configuration

Edit the `s2e-config.lua` file to add the following:

```lua
add_plugin("AvoidAddress")
pluginsConfig.AvoidAddress = {
    addressList = {
        0x00400636,
        <address 2 to avoid>
    },
    relative = false,
}

-- It might be added earlier in the file already. If so, just update it to have the name of your file.
add_plugin("ProcessExecutionDetector")
pluginsConfig.ProcessExecutionDetector = {
    moduleNames = {
        "<your file name here>",
    },
}
```

__addressList__ == This is a list of addresses to "avoid". You can actually use this for the case you want to find as well, since by default S2E will dump out example constraints.

__relative__ == bool value if the addresses in the list are relative. Use this when you're breaking on addresses in a position independent executable (PIE) binary, since you will not know ahead of time where it will be loaded.

# Issues
It appears right now that there's a bug in S2E's handling of amd64 PIE binaries when it comes to the `ProcessExecutionDetector` plugin that this plugin utilizes. For now, amd64 PIE ELF binaries will likely not address avoid correctly. https://github.com/S2E/libs2eplugins/issues/10

This is untested on PE and MACH-O
