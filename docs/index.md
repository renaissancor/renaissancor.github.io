﻿# Welcome to MkDocs

For full documentation visit [mkdocs.org](https://www.mkdocs.org).

## Commands

* `mkdocs new [dir-name]` - Create a new project.
* `mkdocs serve` - Start the live-reloading docs server.
* `mkdocs build` - Build the documentation site.
* `mkdocs -h` - Print help message and exit.

## Project layout

    mkdocs.yml    # The configuration file.
    docs/
        index.md  # The documentation homepage.
        ...       # Other markdown pages, images and other files.

## Common Commands 
```shell
mkdocs serve
mkdocs build
mkdocs gh-deploy
```

Visual Studio 

`Ctrl` + `F5` Program Execution 

`Ctrl` + `~`  Open Terminal 

`F5`          Program Debug 

Visual Studio Code 

```shell
svn update 
svn log -v -l 10 
```
Shows latest 10 logs of svn updated records containing file changes 

```shell
Get-Content "docs/AR250409.md" | Out-String | Select-String -Pattern '[^\x00-\x7F]' -AllMatches
``` 

Open git bash 
```bash 
iconv -f UTF-8 -t UTF-8 docs/AR250409.md > /dev/null
```