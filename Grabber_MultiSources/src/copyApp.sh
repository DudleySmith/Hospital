#!/bin/sh

#  copyApp.sh
#  Grabber_MultiSources
#
#  Created by Dudley Smith on 21/02/2015.
#

# Empty releases folder
rm -vRf "/Volumes/Dude_Work/Code/_OF/0.8.4/apps/Hospital_OF/_Releases"
mkdir -v "/Volumes/Dude_Work/Code/_OF/0.8.4/apps/Hospital_OF/_Releases"

# copy App files and datas
cp -viR "/Volumes/Dude_Work/Code/_OF/0.8.4/apps/Hospital_OF/Grabber_MultiSources/bin" "/Volumes/Dude_Work/Code/_OF/0.8.4/apps/Hospital_OF/_Releases/Cam1"

