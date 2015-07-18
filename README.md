The GAP 4 package `4ti2gap`
===========================

1 Package description
---------------------
This package intends to use the tools included in 4ti2 from the gap shell.
So far it provides (limited) access to

* grobner,
* minimize,
* normalform,
* hilbert,
* graver,
* zsolve.

We took as initial template [NormalizInterface](https://github.com/fingolfin/NormalizInterface), and got plenty of help from Sebastian Gutshe.

2 Installation preparation
--------------------------

To run this package you need 4ti2 installed with the components groebner and zsolve enabled, configured with the option --enable-shared.

In case you want to use this package with *hpc-gap*, you should append the option --with-hpcgap, to the `configure` command used in order to build the *4ti2gap* binaries.

In order to use integer multiple precision arithmetic, the library GMP must be installed with the c++ option enabled, and also 4ti2 and GAP should be installed with multiple precision arithmetic enabled. If not, the precision of 4ti2gap will be that of your system architecture.

If you have GMP and 4ti2 installed in your system, you can try to build 4ti2gap in its folder. The following commands will try to find a GMP installed in /usr or /usr/local. (Note that the installation folders for GAP and 4ti2 should be specified with the appropiate options.)

	./configure --with-gaproot=whereyouinstalledgap --with-4ti2=whereyouinstalled4ti2 --with-gmp=yes
	make

Alternativelly, if GMP is installed in another folder, the value of --with-gmp should be this path of this folder.

If you want to use 4ti2gap with GMP, best way to get it working is the following. Compile gmp with

	./configure --prefix=whereyouwanttoinstallgmp --enable-cxx

Compile gap with

	./configure --with-gmp=whereyouinstalledgmp
	make

Then compile 4ti2 with

	./configure --enable-shared --prefix=whereyouwanttoinstall4ti2 --with-gmp=whereyouinstalledgmp
	make
	make install

In some systems 4ti2 does not find the Gnu Linear Programming Kit needed by the groebner component. Adding the path to GLPK to the previous configure command line may help:

	...$ ./configure --enable-shared --prefix=whereyouwanttoinstall4ti2 --with-gmp=whereyouinstalledgmp --with-glpk=whereglpkisinstalled

Finally go to 4ti2gap folder and do

	./autogen.sh

and next

	./configure --with-gaproot=whereyouinstalledgap --with-4ti2=whereyouinstalled4ti2 --with-gmp=whereyouinstalledgmp

	make

If the compilation process fails with the error `fatal error: 'glpk.h' file not found`, then it is necessary to specify the glpk path to include files before the configure command using CPPFLAGS:

  CPPFLAGS="-I whereitisinstalledglpk/include" ./configure --with-gaproot=whereyouinstalledgap --with-4ti2=whereyouinstalled4ti2 --with-gmp=whereyouinstalledgmp

	make

3 Documentation and tests
-------------------------

So far a limited groebner and hilbert are included.

### Gröbner bases

groebner is called in the following way:

`GroebnerBasis4ti2( matrix , order )`, where matrix is a matrix with integer entries and order is either an order matrix or "lex", "grlex" "grevlex".

```gap
gap> GroebnerBasis4ti2([[3,5,7]],[[3,5,7]]);

[ [ -4, 1, 1 ], [ -3, -1, 2 ], [ -1, 2, -1 ] ]

gap> GroebnerBasis4ti2([[3,5,7]],"lex");

[ [ 0, 7, -5 ], [ 1, -2, 1 ], [ 1, 5, -4 ], [ 2, 3, -3 ], [ 3, 1, -2 ],
  [ 4, -1, -1 ] ]

gap> GroebnerBasis4ti2([[3,5,7]],"grevlex");

[ [ -1, 2, -1 ], [ 3, 1, -2 ], [ 4, -1, -1 ] ]

gap> GroebnerBasis4ti2([[3,5,7]],"grlex");  

[ [ 0, 7, -5 ], [ 1, -2, 1 ], [ 1, 5, -4 ], [ 2, 3, -3 ], [ 3, 1, -2 ],
  [ 4, -1, -1 ] ]
```
If GMP is installed, `GroebnerBasis4ti2( matrix , order )` uses integer multiple precision arithmetic.

### Integer linear program minimal solution

minimize is called in the following way:

`Minimize4ti2( matrix, lattice, cost, sol, sign )`, where

- matrix is optional as an empty list [], only if lattice basis is given,

- lattice basis also optional as an empty list [], only if matrix is given, (both matrix and lattice can be given)

- the cost vector should be a matrix with 1 row or a flat list,

- sol is an integer solution to specify a fiber (same format as cost vector),

- and the sign are the constraints of the variables (1 o 0 for non-negative and free variable, resp., optional as an empty list []).


```gap
gap> Minimize4ti2([[3,5,7]],[],[[1,1,1]],[3,6,9],[1,1,1]);
[ [ 2, 1, 13 ] ]
gap> GroebnerBasis4ti2([[3,5,7]]);
[ [ -4, 1, 1 ], [ -3, -1, 2 ], [ -1, 2, -1 ] ]
gap> Minimize4ti2([],last,[[1,1,1]],[3,6,9],[1,1,1]);
[ [ 2, 1, 13 ] ]
```

### Normal form

normalform is called in the following way:

`Normalform4ti2(matrix, lattice, groebner, cost, feas, sign)` where TO EXPLAIN

```gap
gap> EXAMPLE
```

### Hilbert bases

hilbert actually only returns the homogenous output. It is called in the following way:

`HilbertBasis4ti2( problem )`, where problem is a list to specify the input components of the "problem" (see the 4ti2 manual):

```gap
gap> problem:=[ "mat", [ [ 1, -3, 9, -1, 81 ],
[ -9, 27, 81, -9, 1 ], [ 1, -3, 9, 1, -81 ] ],
"rel", [ "=", "<", ">"], "sign", [1, 0, 1, 0, -1] ];;
gap> HilbertBasis4ti2(problem);
rec( zhom := [ [ 3, 1, 0, 0, 0 ], [ 0, -364, 0, -1095, -27 ], [ 1, 0, 0, 1, 0 ],
	[ 1, -121, 0, -365, -9 ], [ 0, -1, 0, 3, 0 ], [ 0, -27, 0, -81, -2 ],
  [ 0, -14, 0, -39, -1 ], [ 2, -13, 0, -40, -1 ], [ 0, 0, 1, 9, 0 ] ] )

gap> problem:=["mat", [[1, 1, 1, -1, -1, -1, 0, 0, 0 ],
[1, 1, 1, 0, 0, 0, -1, -1, -1], [0, 1, 1, -1, 0, 0, -1, 0, 0],
[1, 0, 1, 0, -1, 0, 0, -1,  0], [ 1, 1, 0, 0, 0, -1, 0, 0, -1],
[ 0, 1, 1, 0, -1, 0, 0, 0, -1], [ 1, 1, 0, 0, -1, 0, -1, 0, 0]],
"rel", [[0, 0 ,0 ,0, 0, 0, 0]],
"sign", [[0, 0, 0, 0, 0, 0, 0, 0, 0]]];;
gap> HilbertBasis4ti2(problem);
rec( zfree := [ [ 2, 2, -1, -2, 1, 4, 3, 0, 0 ],
	[ 0, -1, 1, 1, 0, -1, -1, 1, 0 ], [ -1, 0, 1, 2, 0, -2, -1, 0, 1 ] ],
	zhom := [  ] )

gap> problem:=["mat", [[1, -31, -1, 1], [-111, 5, 10, 25]]];;
gap> HilbertBasis4ti2(problem);
rec( zhom := [ [ 35, 0, 136, 101 ], [ 15, 1, 36, 52 ], [ 195, 34, 0, 859 ],
	[ 110, 19, 4, 483 ], [ 25, 4, 8, 107 ] ] )
```

If GMP is installed, `HilbertBasis4ti2gmp( problem )` uses integer multiple precision arithmetic.

### Graver bases

graver actually only returns the homogenous output. It is called in the following way:
GraverBasis4ti2( problem ), where problem is a list to specify the input components of the "problem" (see the 4ti2 manual):

```gap
gap> problem:=["mat", [[1, 1, 1, 1, 1], [1, 2, 3, 4, 5]]];; GraverBasis4ti2( problem );

[ [ 1, -2, 1, 0, 0 ], [ 2, -3, 0, 1, 0 ], [ 3, -4, 0, 0, 1 ],
  [ 1, -1, -1, 1, 0 ], [ 2, -2, -1, 0, 1 ], [ 1, -1, 0, -1, 1 ],
  [ 0, 1, -2, 1, 0 ], [ 0, 1, -1, -1, 1 ], [ 1, 0, -2, 0, 1 ],
  [ 1, -2, 0, 2, -1 ], [ 0, 0, 1, -2, 1 ], [ 1, 0, -1, -2, 2 ],
  [ 0, 1, 0, -3, 2 ], [ 1, 0, -3, 2, 0 ], [ 1, 0, 0, -4, 3 ],
  [ 0, 2, -3, 0, 1 ] ]
```

If GMP is installed, `GraverBasis4ti2gmp( problem )` uses integer multiple precision arithmetic.

### ZSolve

zsolve performs the same computations as 4ti2 the command line version. Its input is expressed the same way as GraverBasis4ti2 and HilbertBasis4ti2. The output is a record with the components that gives the explicit description of all solutions (see the 4ti2 manual):

```gap
gap> problem:=["mat",[[0, 0, 0, 0, 0, 3, -4, -1, 2], [0, 0, 0, 0, 1, -1, 1, 0, -1], [0, 0, 0, 1, 2, 0, 0, -1, -2], [0, 0, 1, 0, 1, 0, 0, -1, -1], [0, 1, 2, 0, 0, 0, 0, -1, -2], [1, 0, 2, 0, 0, 0, 0, -2, -1], [-2, 0, -2, 0, 0, 0, 0, 3, 0], [-2, 0, 0, 0, 0, 0, 0, 1, 0], [0, 0, -2, 0, 0, 0, 0, 1, 0], [0, 0, 0, 0, 0, 0, 0, -1, 0]], "rel", [[0, 0, 0, 0, 0, 0, -1, -1, -1, -1]], "sign", [[0, 0, 0, 0, 0, 0, 0, 0, 0]]];; ZSolve4ti2(problem);

rec( zhom := [ [ 1, 0, 2, 2, 1, 0, 0, 2, 1 ], [ 2, 0, 1, 0, 1, 2, 1, 2, 0 ],
      [ 1, 2, 0, 0, 1, 2, 2, 0, 1 ], [ 1, 1, 1, 1, 1, 1, 1, 1, 1 ],
      [ 0, 2, 1, 2, 1, 0, 1, 0, 2 ] ],
  zinhom := [ [ 0, 0, 0, 0, 0, 0, 0, 0, 0 ] ] )

gap> ZSolve4ti2(["mat",[[0, 0]],"sign",[[0,0]]]);

rec( zfree := [ [ 1, 0 ], [ 0, 1 ] ], zhom := [  ], zinhom := [ [ 0, 0 ] ] )

gap> ZSolve4ti2(["mat",TransposedMat([[2,0],[0,2],[1,2],[2,1]]),"sign",
[1,1,1,1],"rhs",[[15,15]]]);
rec( zhom := [  ], zinhom := [ [ 0, 3, 1, 7 ], [ 2, 4, 1, 5 ], [ 4, 5, 1, 3 ],
[ 6, 6, 1, 1 ], [ 1, 2, 3, 5 ], [ 3, 3, 3, 3 ], [ 3, 0, 7, 1 ], [ 5, 4, 3, 1 ],
[ 4, 2, 5, 1 ], [ 2, 1, 5, 3 ], [ 0, 0, 5, 5 ] ] )

```

If GMP is installed, `ZSolve4ti2gmp( problem )` uses integer multiple precision arithmetic.


4 Roadmap
---------
1.- See if we can get rid of --enable-shared

2.- Produce a 32 bit version that uses gmp and can be compiled under cygwin.
