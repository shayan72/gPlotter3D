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

#include "Matrix.h"
/////////////////////////////////////////////////////////////////////
inline void
Identity(  gdouble mat[4][4] )
{
    int  i = 0 , j = 0 ;
    for ( i = 0; i < 4; ++i)
    {
        for ( j = 0; j < 4; ++j)
        {
            if ( i == j )
                mat[i][j] = 1.0;
            else
                mat[i][j] = 0.0 ;
        }
    }
}
/////////////////////////////////////////////////////////////////////
void
RotationY ( gdouble mat[4][4] ,
            gdouble ang )
{
    double c=cos(ang), s=sin(ang);
    Identity(mat);
    mat[0][0] = c;
    mat[0][2] = -s;
    mat[2][0] = s;
    mat[2][2] = c;
}
/////////////////////////////////////////////////////////////////////
void
RotationZ ( gdouble mat[4][4] ,
            gdouble ang )
{
    double c=cos(ang), s=sin(ang);
    Identity(mat);
    mat[0][0] = c;
    mat[0][1] =- s;
    mat[1][0] = s;
    mat[1][1] = c;
}
/////////////////////////////////////////////////////////////////////
struct Vector
Transform( struct Vector f ,
        double mat[4][4] )
{
    gdouble x = f.x ;
    gdouble y = f.y ;
    gdouble z = f.z ;

    struct Vector v ;

    v.x = x*mat[0][0] + y*mat[1][0] + z*mat[2][0];
    v.y = x*mat[0][1] + y*mat[1][1] + z*mat[2][1];
    v.z = x*mat[0][2] + y*mat[1][2] + z*mat[2][2];

    return v;
}
/////////////////////////////////////////////////////////////////////
void
mat_mult(
    double mat_res[4][4],
    double mat1[4][4],
    double mat2[4][4] )
{
    int i, j, k;
    double mat_res_temp[4][4];

    for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++) {
        mat_res_temp[i][j] = 0;
        for (k = 0; k < 4; k++)
        mat_res_temp[i][j] += mat1[i][k] * mat2[k][j];
    }
    for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
        mat_res[i][j] = mat_res_temp[i][j];
}
