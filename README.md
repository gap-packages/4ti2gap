The GAP 4 package `4ti2gap`
===========================

1 Package description
---------------------
This package intends to use the tools included in 4ti2 from the gap shell. 
So far it provides (limited) access to

* grobner,
* hilbert,
* graver,
* zsolve.


2 Installation preparation
--------------------------

To run this package you need 4ti2 installed with the components groebner and zsolve enabled, configured with the option --enable-shared.

In order to use integer multiple precision arithmetic, the library GMP must be installed with the c++ option enabled, and also 4ti2 and GAP should be installed with multiple precision arithmetic enabled. If not, the precission of 4ti2gap will be that of your system architecture.

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

	./configure --with-gaproot=whereyouinstalledgap --with-4ti2=whereyouinstalled4ti2 --with-gmp=whereyouinstalledgmp
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


### Hilbert bases

hilbert actually only returns the homogenous output. It is called in the following way:

`HilbertBasis4ti2( problem )`, where problem is a list to specify the input components of the "problem" (see the 4ti2 manual):

```gap
gap> problem:=[ "mat", [ [ 1, -3, 9, -1, 81 ], 
[ -9, 27, 81, -9, 1 ], [ 1, -3, 9, 1, -81 ] ], 
"rel", [ "=", "<", ">"], "sign", [1, 0, 1, 0, -1] ];; 
HilbertBasis4ti2(problem);

[ [ 3, 1, 0, 0, 0 ], [ 0, -364, 0, -1095, -27 ], [ 1, 0, 0, 1, 0 ], 
  [ 1, -121, 0, -365, -9 ], [ 0, -1, 0, 3, 0 ], [ 0, -27, 0, -81, -2 ], 
  [ 0, -14, 0, -39, -1 ], [ 2, -13, 0, -40, -1 ], [ 0, 0, 1, 9, 0 ] ]

gap> problem:=["mat", [[1, 1, 1, -1, -1, -1, 0, 0, 0 ], 
[1, 1, 1, 0, 0, 0, -1, -1, -1], [0, 1, 1, -1, 0, 0, -1, 0, 0], 
[1, 0, 1, 0, -1, 0, 0, -1,  0], [ 1, 1, 0, 0, 0, -1, 0, 0, -1], 
[ 0, 1, 1, 0, -1, 0, 0, 0, -1], [ 1, 1, 0, 0, -1, 0, -1, 0, 0]], 
"rel", [[0, 0 ,0 ,0, 0, 0, 0]], 
"sign", [[0, 0, 0, 0, 0, 0, 0, 0, 0]]];; 
HilbertBasis4ti2(problem);

[ ]

gap> problem:=["mat", [[1, -31, -1, 1], [-111, 5, 10, 25]]];;
HilbertBasis4ti2(problem);

[ [ 35, 0, 136, 101 ], [ 15, 1, 36, 52 ], [ 195, 34, 0, 859 ], 
  [ 110, 19, 4, 483 ], [ 25, 4, 8, 107 ] ]
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
gap> problem:=["mat",[[0, 0, 0, 0, 0, 3, -4, -1, 2], [0, 0, 0, 0, 1, -1, 1, 0, -1], [0, 0, 0, 1, 2, 0, 0, -1, -2], [0, 0, 1, 0, 1, 0, 0, -1, -1], [0, 1, 2, 0, 0, 0, 0, -1, -2], [1, 0, 2, 0, 0, 0, 0, -2, -1], [-2, 0, -2, 0, 0, 0, 0, 3, 0], [-2, 0, 0, 0, 0, 0, 0, 1, 0], [0, 0, -2, 0, 0, 0, 0, 1, 0], [0, 0, 0, 0, 0, 0, 0, -1, 0]], "rel", [[0, 0, 0, 0, 0, 0, -1, -1, -1, -1]], "sign", [[0, 0, 0, 0, 0, 0, 0, 0, 0]]];; ZSolve4ti2(problemzso);

rec( zhom := [ [ 1, 0, 2, 2, 1, 0, 0, 2, 1 ], [ 2, 0, 1, 0, 1, 2, 1, 2, 0 ], 
      [ 1, 2, 0, 0, 1, 2, 2, 0, 1 ], [ 1, 1, 1, 1, 1, 1, 1, 1, 1 ], 
      [ 0, 2, 1, 2, 1, 0, 1, 0, 2 ] ], 
  zinhom := [ [ 0, 0, 0, 0, 0, 0, 0, 0, 0 ] ] )

gap> ZSolve4ti2(["mat",[[0, 0]],"sign",[[0,0]]]);

rec( zfree := [ [ 1, 0 ], [ 0, 1 ] ], zhom := [  ], zinhom := [ [ 0, 0 ] ] )
```

If GMP is installed, `ZSolve4ti2gmp( problem )` uses integer multiple precision arithmetic.

4 Roadmap
---------
1.- See if we can get rid of --enable-shared

2.- Produce a 32 bit version that uses gmp and can be compiled under cygwin.

