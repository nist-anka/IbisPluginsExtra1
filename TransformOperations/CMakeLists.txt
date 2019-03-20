# define sources
set( PluginSrc
        transformoperationsplugininterface.cpp transformoperationswidget.cpp
)
set( PluginHdr transformoperationsplugininterface.h  transformoperationswidget.h )

set( PluginHdrMoc transformoperationsplugininterface.h transformoperationswidget.h)

set( PluginUi transformoperationswidget.ui )


# Create plugin
DefinePlugin( "${PluginSrc}" "${PluginHdr}" "${PluginHdrMoc}" "${PluginUi}" )

