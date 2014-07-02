@echo off

::%~p0 is the location of the batch file and ends in a \

::e.g. users\self\documents\engine\

::put the rest of your path directly after with no space between the %~p0 and the rest of your path

path = "%~p0GameSolution\GameSolution.sln"

echo %path%

start "" %path%