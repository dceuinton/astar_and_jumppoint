@echo off
git add .
git status
set /p message=Enter a commit message: 
git commit -m "%message%"
