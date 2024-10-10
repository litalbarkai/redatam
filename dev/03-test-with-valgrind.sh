#!/bin/bash

valgrind --leak-check=full --track-origins=yes --log-file=valgrind.txt ./redatam downloads/CP2011URY/BaseRPub/CPV2011_uruguay_publica.dicx downloads/CP2011URY/open-redatam-cpp/
