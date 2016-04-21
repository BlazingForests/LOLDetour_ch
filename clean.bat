color fc&title 清理VS2010编译垃圾文件批处理
@echo 清理开始...
rd /s /q Debug
rd /s /q Release
rd /s /q LOLDetour\Debug
rd /s /q LOLDetour\Release
rd /s /q Main\Debug
rd /s /q Main\Release
rd /s /q ipch
del /f /s /q *.sdf
@pause