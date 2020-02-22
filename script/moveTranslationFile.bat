@echo off
chcp 65001 > nul
if /i "%processor_architecture%"=="AMD64" GOTO AMD64
if /i "%PROCESSOR_ARCHITEW6432%"=="AMD64" GOTO AMD64
if /i "%processor_architecture%"=="x86" GOTO x86
GOTO arch_unknown

:: $(MSBuildProjectDirectory)가 working directory로 설정됩니다

:AMD64
	copy ".\asset\translation\ko-kr.json" "C:\Program Files (x86)\steam\steamapps\common\Geometry Dash"  && (
  		exit
	) || (
 		GOTO inform_error
	)

:x86
	copy ".\asset\translation\ko-kr.json" "C:\Program Files\steam\steamapps\common\Geometry Dash"  && (
  		exit
	) || (
 		GOTO inform_error
	)

:arch_unknown
	echo Error - Unsupported architecture
	exit 1

:inform_error
	echo Error - Failed to copy translation file to game directory
	exit 1


