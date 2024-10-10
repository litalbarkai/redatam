#!/bin/bash

# Make a temporary directory
mkdir -p /tmp/redatam

# Download the binaries
wget https://github.com/pachadotdev/open-redatam/releases/download/v0.1/redatam-macos.zip -O /tmp/redatam/redatam.zip

# Unzip the binaries
unzip /tmp/redatam/redatam.zip -d /tmp/redatam

# Install the binaries
sudo mkdir -p /usr/local/bin
sudo cp /tmp/redatam/redatam /usr/local/bin/
sudo cp /tmp/redatam/redatamgui /usr/local/bin/
sudo chmod +x /usr/local/bin/redatam
sudo chmod +x /usr/local/bin/redatamgui

# Create a desktop entry for Redatam GUI
echo "[Desktop Entry]
Version=1.0
Name=Redatam GUI
Exec=/usr/local/bin/redatamgui
Icon=utilities-terminal
Type=Application
Categories=Utility;" | sudo tee /usr/share/applications/redatamgui.desktop

echo "Installation complete. You can run 'redatam' and 'redatamgui' from the terminal or search for 'redatamgui' in your application menu."
