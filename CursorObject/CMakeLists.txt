# define sources
set( PluginSrc
        cursorobjectplugininterface.cpp cursorobject.cpp
)
set( PluginHdr cursorobjectplugininterface.h cursorobject.h )

set( PluginHdrMoc cursorobjectplugininterface.h cursorobject.h )


# Create plugin
DefinePlugin( "${PluginSrc}" "${PluginHdr}" "${PluginHdrMoc}" "${PluginUi}" )

