# SokS
> A simple *NIX based network scanner
<!--
ADD badges here
[![Build Status](https://travis-ci.com/alichtman/shallow-backup.svg?branch=master)](https://travis-ci.com/alichtman/shallow-backup)
 -->

`SokS` - Socket Scanner 

Contents
========

 * [Why?](#why)
 * [Installation(WIP)](#installation)
 * [Usage](#usage)




### Why?

Wanted a tool which can scan the network and graph it

### Installation
---
> This is still in alpha stages, so you can't install it.

- If you want to try it out, build it using this steps

1. Install graphviz library

	Debian based distros: `sudo apt install graphviz-dev`
    
    Fedora: `sudo dnf install graphviz-devel`
    
    Arch: `sudo pacman -S graphviz`
 
 2. Build the base
 	```
    make base 
    ```
 3. Installation
 
 > WIP

### Usage
---

```shell
❯ ./soks -h
SokS version 0.2
Usage: ./soks [Scan Types(s)] [Options] {Targets}
TARGET:
      can pass hostnames, IP addresses
      Example: google.com, 192,168.0.1
SCAN TECHNIQUES:
      -sU: UDP Scan (Slow/Unreliable)
      -sT: TCP Scan
PORT SPECIFICATIONS:
      -p <comma seperated ports>
      Example -p22; -p22,80,443
MISC:
      -V: Prints version
      -v: Increases Verbosity
      -d: Enables Debugging
      -h: Prints This help Summary
      -g: Makes a Pseudo Network Graph(WIP)
EXAMPLES:
      ./soks -v google.com
      ./soks -sT google.com -p 80,443
      ./soks -sU 192.168.0.1 -p 21,23
```




