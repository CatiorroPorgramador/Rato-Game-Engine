@echo off

git add *
git commit -m "v0.1"
git branch -M development

git push -u origin development