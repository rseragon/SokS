# SokS (Socket Scanner)
> A simple *NIX based network scanner


![Version](https://img.shields.io/badge/Version-0.8-blue)
![GitHub Workflow Status](https://img.shields.io/github/workflow/status/rseragon/SokS/C%20CI)
<p align="center">
  <img align="center" width='400' src="https://i.ibb.co/S0vdvKR/soks.png">
</p>



Contents
========

 * [Why?](#why)
 * [Features](#features)
 * [Installation](#installation)
 * [Usage](#usage)




### Why?

Cuz Why Not

### Features

+ Check for alive hosts in a network
+ Scan for open TCP/UDP ports
+ Graph alive targets in the network
+ more coming soon....

### Installation
---
> Install at YOUR own risk

Pre-requisite:

1. Install graphviz library

	Debian based distros: `sudo apt install graphviz-dev`
    
    Fedora: `sudo dnf install graphviz-devel`
    
    Arch: `sudo pacman -S graphviz`
 
 2. Build the base
 	```
    make  
    ```
 3. Installation
 
	```
	sudo make install
	```

### Usage
---

```shell
❯ soks
SokS version 0.8
Usage: soks [Scan Types(s)] [Options] {Targets}
TARGET:
      can pass hostnames, IP, CIDR addresses, IP ranges(Only to the last octet)
      Example: google.com, 192.168.0.1, 10.10.10.10/24, 1.1.1.1-10
SCAN TECHNIQUES:
      -sU: UDP Scan (Slow/Unreliable)
      -sT: TCP Scan
      -sL: List the hosts
PORT SPECIFICATIONS:
      -p <comma seperated ports>
      Example -p22; -p22,80,443
GRAPH:
      -g <GraphName>.jpg
      Note: The extension name doesn't matter
MISC:
      -V: Prints version
      -v: Increases Verbosity
      -d: Enables Debugging
      -h: Prints This help Summary
EXAMPLES:
      soks -v google.com
      soks -sT google.com -p 80,443
      soks -sU 192.168.0.1 -p 21,23
      soks 192.168.0.2/24 -g graph.jpg
```






