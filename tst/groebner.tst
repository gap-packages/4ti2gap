gap> cuww4:=[ [ 13211, 13212, 39638, 52844, 66060, 79268, 92482 ] ];
[ [ 13211, 13212, 39638, 52844, 66060, 79268, 92482 ] ]
gap> cuww4cost:=[ [ 1, 0, 0, 0, 0, 0, 0 ] ];
[ [ 1, 0, 0, 0, 0, 0, 0 ] ]
gap> GroebnerBasis4ti2(cuww4, cuww4cost);
[ [ 0, -10, 2, 1, 0, 0, 0 ], [ 0, -9, 2644, -1981, 0, 0, 0 ], [ 0, -5, 0, 0, 1, 0, 0 ], [ 0, -2, 0, -1, 0, 1, 0 ],
  [ 0, -1, -2642, 1982, 0, 0, 0 ], [ 0, 0, -1, -1, 0, 0, 1 ], [ 2, -5, 1, 0, 0, 0, 0 ], [ 2, 4, -2643, 1981, 0, 0, 0 ],
  [ 2, 5, -1, -1, 0, 0, 0 ], [ 4, 0, 0, -1, 0, 0, 0 ] ]
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
