#!/usr/bin/python
# Test Julia module ArrMod.jl calling from Python, via pyjulia
# See: https://pyjulia.readthedocs.io/en/latest/index.html
# Barnett 7/24/22

# set up py-calling-jl interface:
import julia
julia.install()   # optional; use to set up

# following two lines needed for me since libpython is not dynamically linked...
# (see docs for pyjulia). They don't seem to work in a script, only REPL!
from julia.api import Julia
jl = Julia(compiled_modules=False)
# & takes several seconds from Python REPL

# Warm-up 1: test something from Base...
from julia import Base
import numpy as np
x = 1.0
print("pyjulia interface Base test error: ",Base.sin(x) - np.sin(x))

# Warm-up 2: test an installed jl pkg... (you'll need to install it)
from julia import SpecialFunctions
import scipy.special as sp
nu=0; x=1.0
print("pyjulia interface SpecialFunctions test error: ",abs(SpecialFunctions.besselh(nu,x)-sp.hankel1(nu,x)))
# (note: complex outputs; takes a few secs to run the first time)

# Actual task: call self-contained user-defined module in this dir...
# see https://discourse.julialang.org/t/calling-multiple-user-defined-julia-functions-from-python/11887/2
from julia import Main
Main.include("ArrMod.jl")
x = np.random.rand(10000000)
y = Main.ArrMod.foomp(x)   # test version passing out array
y2 = np.exp(x)             # check the math on py side
print("pyjulia ArrMod.foomp test error: ",np.linalg.norm(y-y2)/np.linalg.norm(y2))

# Can modify a python array from julia, via ctypes, a bit messy:
# https://github.com/JuliaPy/pyjulia/issues/385

# timings for above (py 2.5x faster than jl; since no attempt to SIMD in jl)
import time
t0 = time.time()
y = Main.ArrMod.foomp(x)   # test version passing out array
print("time to call julia      ",time.time()-t0)
t0 = time.time()
y2 = np.exp(x)             # check the math on py side
print("time using python numpy ",time.time()-t0)
