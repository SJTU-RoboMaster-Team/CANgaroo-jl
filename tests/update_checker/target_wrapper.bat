@echo off
SetLocal EnableDelayedExpansion
(set PATH=D:\msys64\ucrt64\bin;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=D:\msys64\ucrt64\share\qt6\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=D:\msys64\ucrt64\share\qt6\plugins
)
%*
EndLocal
