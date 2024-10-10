#!/bin/bash

# the original link https://redatam.org/es/microdatos
# is broken since Sep 1, 2024

# We use the Wayback Machine to get the latest snapshot
# https://web.archive.org/web/20240420134748/https://redatam.org/es/microdatos
# the download links are still valid as of Sep 10, 2024

# Argentina
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2010ARG.zip -O downloads/CP2010ARG.zip

# Bolivia
wget https://redatam.org/cdr/descargas/censos/poblacion/Cp2001BOL.zip -O downloads/CP2001BOL.zip
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2012BOL.zip -O downloads/CP2012BOL.zip

# Chile
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2017CHL.zip -O downloads/CP2017CHL.zip

# Ecuador
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2010ECU.zip -O downloads/CP2010ECU.zip

# El Salvador
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2007SLV.zip -O downloads/CP2007SLV.zip

# Guatemala
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2018GTM.zip -O downloads/CP2018GTM.zip

# Mexico
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2000MEX.zip -O downloads/CP2000MEX.zip
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2010MEX.zip -O downloads/CP2010MEX.zip
wget https://redatam.org/cdr/descargas/censos/poblacion/ECI2015MEX.zip -O downloads/ECI2015MEX.zip
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2020MEX.zip -O downloads/CP2020MEX.zip

# Myammar
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2014MMR.zip -O downloads/CP2014MMR.zip

# Peru
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2007PER.zip -O downloads/CP2010PER.zip
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2017PER.zip -O downloads/CP2017PER.zip

# Republica Dominicana
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2002DOM.zip -O downloads/CP2002DOM.zip

# Uruguay
wget https://redatam.org/cdr/descargas/censos/poblacion/CP2011URY.zip -O downloads/CP2011URY.zip
