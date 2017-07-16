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
}

-- It might be added earlier in the file already. If so, just update it to have the name of your file.
add_plugin("ProcessExecutionDetector")
pluginsConfig.ProcessExecutionDetector = {
    moduleNames = {
        "<your file name here>",
    },
}
```

