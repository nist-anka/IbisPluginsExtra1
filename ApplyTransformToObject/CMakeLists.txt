# define sources
set( PluginSrc
        applytransformtoobjectplugininterface.cpp
)
set( PluginHdr applytransformtoobjectplugininterface.h )

set( PluginHdrMoc applytransformtoobjectplugininterface.h )


# Create plugin
DefinePlugin( "${PluginSrc}" "${PluginHdr}" "${PluginHdrMoc}" "${PluginUi}" )

