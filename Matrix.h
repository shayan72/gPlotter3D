/*
    Copyright 2012 Mohammad Razeghi , Shayan Salehian

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <gtk/gtk.h>
#include "Vector.h"

/** This method get a matrix and create a identity Matrix .
    @param mat input matrix that we want to make it identity */
inline void Identity(  gdouble mat[4][4] );

/** This method get a matrix and rotate it arround Y axis .
    @param mat input matrix that we want to rotate it around Y Axix
    @param ang angle to rotate */
void RotationY ( gdouble mat[4][4] , gdouble ang );

/** This method get a matrix and rotate it arround Z axis .
    @param mat input matrix that we want to rotate it around Z Axix
    @param ang angle to rotate */
void RotationZ ( gdouble mat[4][4] , gdouble ang );

/** This method get a transform matrix and rotate a transform a point with it  .
    @param f point that we want to transform
    @param mat input transofrm matrix
    @return  tranformed point with matrix f */
struct Vector Transform( struct Vector f , double mat[4][4]);

/** This method multiply two matrix
    @param mat_res result matrix of multiply
    @param mat1 first matrix for multiply
    @param mat2 second matrix for multiply  */
void mat_mult( double mat_res[4][4],  double mat1[4][4],  double mat2[4][4] ) ;

#endif // MATRIX_H
