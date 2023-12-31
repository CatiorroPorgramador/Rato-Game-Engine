@echo off

git add *
git commit -m "v0.2"
git branch -M development

git push -u origin development