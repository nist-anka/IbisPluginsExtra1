# define sources
set( PluginSrc
        hideobjectsplugininterface.cpp userobjects.cpp
)
set( PluginHdr hideobjectsplugininterface.h userobjects.h )

set( PluginHdrMoc hideobjectsplugininterface.h userobjects.h )

set( PluginUi userobjects.ui )


# Create plugin
DefinePlugin( "${PluginSrc}" "${PluginHdr}" "${PluginHdrMoc}" "${PluginUi}" )

