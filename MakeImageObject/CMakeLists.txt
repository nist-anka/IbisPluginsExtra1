# define sources
set( PluginSrc
        makeimageobjectplugininterface.cpp
)
set( PluginHdr makeimageobjectplugininterface.h )

set( PluginHdrMoc makeimageobjectplugininterface.h )


# Create plugin
DefinePlugin( "${PluginSrc}" "${PluginHdr}" "${PluginHdrMoc}" "${PluginUi}" )

