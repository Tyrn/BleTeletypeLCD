#/usr/bin/env bash

# Configure/reconfigure the project and the language server.

pio project init --ide=vim
pio run -t compiledb
#curl -s https://anurag3301.com/files/GrDSa.py | python
python fix_cmdcompile.py
