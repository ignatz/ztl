#!/usr/bin/env python
import sys, os

def check_version_cxx(cfg):
    error = "g++-4.7, clang-3.1 or newer required"

    cfg.check_cxx(
        msg      = "Checking compiler version",
        okmsg    = "ok",
        errmsg   = error,
        type     = "cxx",
        cxxflags = "-std=c++0x",
        execute  = False,
        fragment = """
                       #if __cplusplus != 201103L
                         #error %(MSG)s
                       #endif
                   """ % { "MSG" : error })

def options(opt):
    opt.load('g++')

def configure(cfg):
    cfg.load('g++')
    check_version_cxx(cfg)

    cfg.env.CXXFLAGS_0TL = [ '-std=c++0x' ]

def build(bld):
    bld(
        target          = 'ZTL',
        export_includes = '.',
        use             = '0TL',
    )
