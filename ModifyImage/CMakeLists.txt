# define sources
set( PluginSrc
        modifyimageplugininterface.cpp
)
set( PluginHdr modifyimageplugininterface.h )

set( PluginHdrMoc modifyimageplugininterface.h )


# Create plugin
DefinePlugin( "${PluginSrc}" "${PluginHdr}" "${PluginHdrMoc}" "${PluginUi}" )

