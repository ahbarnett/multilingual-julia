module ArrModF
# example module doing something in julia to test calling from Fortran.
# Also tests julia multithreading. Also see: test_arrmodf.jl

using Base.Threads

export foomp2, foomp2_wrap

println("loaded ArrModF: num threads: ",Threads.nthreads())

"""foo demo func, writes elementwise exp of float array to 2nd array, Fortran-style, multithreaded"""
function foomp2(x,y)
    @assert length(x)==length(y)       # assumes y allocated
    @threads for i in eachindex(x)
        y[i] = exp(x[i])
    end
end

"""C-style wrapper to foomp2, passing in pointers and array lengths""" 
function foomp2_wrap(xptr,yptr,n)
#   @show xptr; @show yptr; @show n; println(typeof(n))      # debug
    x=unsafe_wrap(Array,xptr,(n,))         # make jl array, input
    y=unsafe_wrap(Array,yptr,(n,))         # output
    foomp2(x,y)
    nothing                                # needed for void
end

end   # module

