#! /usr/bin/env python3

# File extensions
QRC = 'qrc' # Qt resource definition file
RCC = 'rcc' # Qt binary resource file
WSD = 'wsd' # Warmonger Surface Definition
WSM = 'wsm' # Warmonger Surface Metadata
WSP = 'wsp' # Warmonger Surface Package

import json
import pathlib
import sys
import subprocess
import tarfile

if len(sys.argv) != 2:
    print("Usage: makesurfacepkg.py path/to/surface/dir\n")
    exit(1)

path = sys.argv[1]

surface_dir = pathlib.Path(path);
definition_files = list(surface_dir.glob('*.' + WSD))

if len(definition_files) != 1:
    print("Ambiguous surface path, more than one .wsd files found")
    sys.exit(1)

definition_file = definition_files[0]

if definition_file.is_dir():
    print("Definition file " + str(definition_file) + "is a directory")
    sys.exit(1)

with open(str(definition_file), 'r') as f:
    surface = json.load(f)

surface_name = surface['objectName']

wsm_file = definition_file.with_name(surface_name + '.' + WSM)
metadata = {
    'objectName': surface['objectName'],
    'displayName': surface['displayName'],
    'description': surface['description']
}

with open(str(wsm_file), 'w') as f:
    json.dump(metadata, f)

qrc_file = definition_file.with_name(surface_name + '.' + QRC)
rcc_file = definition_file.with_name(surface_name + '.' + RCC)

rcc_cmd = ['rcc', '--binary', '--root', '/surface', '--output', str(rcc_file), str(qrc_file)]
rcc = subprocess.Popen(rcc_cmd)
ret_code = rcc.wait()

if ret_code:
    print("Resource compilation failed")
    sys.exit(1)

package_file = definition_file.with_name(surface_name + '.' + WSP)
with tarfile.open(str(package_file), 'w') as pkg:
    pkg.add(str(wsm_file))
    pkg.add(str(rcc_file))
