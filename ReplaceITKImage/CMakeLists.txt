# define sources
set( PluginSrc
        replaceitkimageplugininterface.cpp
)
set( PluginHdr replaceitkimageplugininterface.h )

set( PluginHdrMoc replaceitkimageplugininterface.h )


# Create plugin
DefinePlugin( "${PluginSrc}" "${PluginHdr}" "${PluginHdrMoc}" "${PluginUi}" )

