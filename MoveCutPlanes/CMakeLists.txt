# define sources
set( PluginSrc
        movecutplanesplugininterface.cpp movecutplaneswidget.cpp
)
set( PluginHdr movecutplanesplugininterface.h movecutplaneswidget.h )

set( PluginHdrMoc movecutplanesplugininterface.h movecutplaneswidget.h )

set( PluginUi movecutplaneswidget.ui )


# Create plugin
DefinePlugin( "${PluginSrc}" "${PluginHdr}" "${PluginHdrMoc}" "${PluginUi}" )

