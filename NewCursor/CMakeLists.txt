# define sources
set( PluginSrc
        newcursorplugininterface.cpp cursorobject.cpp
)
set( PluginHdr newcursorplugininterface.h cursorobject.h )

set( PluginHdrMoc newcursorplugininterface.h cursorobject.h )


# Create plugin
DefinePlugin( "${PluginSrc}" "${PluginHdr}" "${PluginHdrMoc}" "${PluginUi}" )

