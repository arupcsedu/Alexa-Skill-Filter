# Alexa-Skill-Filter
An automatic tools for the identifying the vulnerable skills and utterances

1.	Install Visual Studio 2022. 
2.	Clone the project from the repository: https://github.com/arupcsedu/Alexa-Skill-Filter.git
3.	Import project or open SkillFilter.sln from the source directory.
4.	Change line number 277 of SkillFilter.cpp with your local skill files. You will get it from the project repo.
5.	Import dirent.h if it is not included.
6.	Build the project.
7.	If you see build failure for the dependencies of build tools, then either, install build tool that is recommend for your visual studio. You will see the version name from the error message. Or, use the existing one from the settings. Right click to Project  Properties  Configuration Properties at the left panel  General   Platform toolset0-> Select the installed on.
8.	You will see error, mentioned in steps 7, if you don’t use visual studio 22.
9.	Build and run the app. 
10.	output.txt will be generated in the same project directory and check the results for the better analysis.

