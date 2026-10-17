#!/usr/bin/env bash
# Run once by ONE team member, from inside this folder.
set -e
git init -b main
git add .
git commit -m "[setup] initial project scaffold"
git branch dev
echo
echo "Local repo ready. Now create the remote:"
echo "  Option 1 (GitHub CLI):  gh repo create cs3400-campus-system --private --source=. --push"
echo "                          git push -u origin dev"
echo "  Option 2 (website):     create an empty private repo on github.com, then:"
echo "                          git remote add origin <repo-url>"
echo "                          git push -u origin main && git push -u origin dev"
echo "Then add teammates under Settings > Collaborators."
