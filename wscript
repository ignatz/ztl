#!/usr/bin/env python
import sys, os

def options(opt):
    pass

def configure(cfg):
    cfg.env.CXXFLAGS_0TL = [ '-std=c++0x' ]

def build(bld):
    bld(
        target          = 'ZTL',
        export_includes = '.',
        use             = '0TL',
    )
