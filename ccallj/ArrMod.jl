module ArrMod
# example module doing something to an array in julia, to test calling from C.
# For usage see: test_arrmod.jl

using Base.Threads                     # use OMP

export foo, foomp, foomp2, foomp2_wrap

println("loaded ArrMod: num threads: ",Threads.nthreads())

"""foo demo func, returns elementwise exp of a float array."""
foo(x) = exp.(x)

"""foo demo func, alloc & returns elementwise exp of float array, multithreaded (well, that segfaults, so not yet)"""
function foomp(x)       # ::Array{Float64,1})
    y = similar(x)                 # alloc but don't fill output
#    @threads for i in eachindex(x)       # segfaults when called from C!
    for i in eachindex(x)                 # works when called from C
        y[i] = exp(x[i])
    end
    y
end

"""foo demo func, writes elementwise exp of float array to 2nd array, Fortran-style, multithreaded works"""
function foomp2(x,y)
    @assert length(x)==length(y)      # assumes y allocated
    @threads for i in eachindex(x)         # use OMP
        y[i] = exp(x[i])
    end
end

"""C-style wrapper to foomp2, passing in pointers and array lengths""" 
function foomp2_wrap(xptr,yptr,n)
#   @show xptr; @show yptr; @show n;       # for debug
    x=unsafe_wrap(Array,xptr,(n,))         # input
    y=unsafe_wrap(Array,yptr,(n,))         # output
    foomp2(x,y)
    nothing
end

end   # module

