//=============================================================================
// zapożyczone od J. Ganczarskiego
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <cmath>

#ifndef __VECMATQUAT__H__
#define __VECMATQUAT__H__

//=============================================================================
// szablon klasy obsługującej wektor/punkt 3D
//=============================================================================
template <class T = GLfloat> class vector3
{
protected:
    T xyz[3];
public:
    // konstruktory
    vector3();
    vector3( const T x, const T y, const T z );
    vector3( const T *v );
    vector3( const vector3<T> &v );

    // pobieranie i zmiana danych
    const T *data() const;
    T &operator[] ( unsigned int i );
    const T &operator[] ( unsigned int i ) const;
    const T x() const;
    const T y() const;
    const T z() const;
    void set( const T x, const T y, const T z );
    void setX( const T x );
    void setY( const T y );
    void setZ( const T z );

    // operatory arytmetyczne
    vector3<T> &operator += ( const vector3<T> &v );
    vector3<T> &operator -= ( const vector3<T> &v );
    vector3<T> operator - ();
    vector3<T> &operator *= ( const T f );
    vector3<T> &operator /= ( const T f );
    vector3<T> operator * ( const T f );

    // operacje matematyczne
    const T length() const;
    vector3<T> normalize();
};

//=============================================================================
// operatory i operacje matematyczne klasy vector3
//=============================================================================
template <class T> vector3<T> normalize( const vector3<T> &v );
template <class T> const T dot3( const vector3<T> &v1, const vector3<T> &v2 );
template <class T> vector3<T> cross3( const vector3<T> &v1, const vector3<T> &v2 );
template <class T> vector3<T> operator + ( const vector3<T> &v1, const vector3<T> &v2 );
template <class T> vector3<T> operator - ( const vector3<T> &v1, const vector3<T> &v2 );
template <class T> vector3<T> operator * ( const T f, const vector3<T> &v );
template <class T> vector3<T> operator * ( const vector3<T> &v, const T f );
template <class T> vector3<T> operator / ( const vector3<T> &v, const T f );
template <class T> bool operator == ( const vector3<T> &v1, const vector3<T> &v2 );
template <class T> bool operator != ( const vector3<T> &v1, const vector3<T> &v2 );


//=============================================================================
// konstruktor domylny
//=============================================================================
template <class T> vector3<T>::vector3()
{
    xyz[0] = static_cast<T> ( 0.0 );
    xyz[1] = static_cast<T> ( 0.0 );
    xyz[2] = static_cast<T> ( 0.0 );
}

//=============================================================================
// konstruktor trzyelementowy
//=============================================================================
template <class T> vector3<T>::vector3( const T x, const T y, const T z )
{
    xyz[0] = x;
    xyz[1] = y;
    xyz[2] = z;
}

//=============================================================================
// konstruktor wektorowy
//=============================================================================
template <class T> vector3<T>::vector3( const T *v )
{
    xyz[0] = v[0];
    xyz[1] = v[1];
    xyz[2] = v[2];
}

//=============================================================================
// konstruktor kopiujący
//=============================================================================
template <class T> vector3<T>::vector3( const vector3<T> &v )
{
    xyz[0] = v[0];
    xyz[1] = v[1];
    xyz[2] = v[2];
}

//=============================================================================
// pobranie danych
//=============================================================================
template <class T> const T *vector3<T>::data() const
{
    return xyz;
}

//=============================================================================
// operator tablicowy
//=============================================================================
template <class T> T &vector3<T>::operator[] ( unsigned int i )
{
    return xyz[i];
}

//=============================================================================
// operator tablicowy
//=============================================================================
template <class T> const T &vector3<T>::operator[] ( unsigned int i ) const
{
    return xyz[i];
}

//=============================================================================
// pobranie pierwszej składowej
//=============================================================================
template <class T> const T vector3<T>::x() const
{
    return xyz[0];
}

//=============================================================================
// pobranie drugiej składowej
//=============================================================================
template <class T> const T vector3<T>::y() const
{
    return xyz[1];
}

//=============================================================================
// pobranie trzeciej składowej
//=============================================================================
template <class T> const T vector3<T>::z() const
{
    return xyz[2];
}

//=============================================================================
// zapis danych wszystkich składowych
//=============================================================================
template <class T> void vector3<T>::set( const T x, const T y, const T z )
{
    xyz[0] = x;
    xyz[1] = y;
    xyz[2] = z;
}

//=============================================================================
// zapis danych pierwszej składowej
//=============================================================================
template <class T> void vector3<T>::setX( const T x )
{
    xyz[0] = x;
}

//=============================================================================
// zapis danych drugiej składowej
//=============================================================================
template <class T> void vector3<T>::setY( const T y )
{
    xyz[1] = y;
}

//=============================================================================
// zapis danych trzeciej składowej
//=============================================================================
template <class T> void vector3<T>::setZ( const T z )
{
    xyz[2] = z;
}

//=============================================================================
// mnożenie przez stałą
//=============================================================================
template <class T> vector3<T> operator * ( const T f, const vector3<T> &v )
{
    return vector3<T>( v[0] * f, v[1] * f, v[2] * f );
}

//=============================================================================
// mnożenie przez stałą
//=============================================================================
template <class T> vector3<T> operator * ( const vector3<T> &v, const T f )
{
    return vector3<T>( v[0] * f, v[1] * f, v[2] * f );
}

//=============================================================================
// mnożenie przez stałą
//=============================================================================
template <class T> vector3<T> &vector3<T>::operator *= ( const T f )
{
    xyz[0] *= f;
    xyz[1] *= f;
    xyz[2] *= f;
    return *this;
}

//=============================================================================
// dzielenie przez stałą
//=============================================================================
template <class T> vector3<T> &vector3<T>::operator /= ( const T f )
{
    xyz[0] /= f;
    xyz[1] /= f;
    xyz[2] /= f;
    return *this;
}

//=============================================================================
// dzielenie przez stałą
//=============================================================================
template <class T> vector3<T> operator / ( const vector3<T> &v, const T f )
{
    return vector3<T>( v[0] / f, v[1] / f, v[2] / f );
}

//=============================================================================
// przypisanie - operator +=
//=============================================================================
template <class T> vector3<T> &vector3<T>::operator += ( const vector3<T> &v )
{
    xyz[0] += v[0];
    xyz[1] += v[1];
    xyz[2] += v[2];
    return *this;
}

//=============================================================================
// przypisanie - operator -=
//=============================================================================
template <class T> vector3<T> &vector3<T>::operator -= ( const vector3<T> &v )
{
    xyz[0] -= v[0];
    xyz[1] -= v[1];
    xyz[2] -= v[2];
    return *this;
}

//=============================================================================
// negacja sk³adowych wektora
//=============================================================================
template <class T> vector3<T> vector3<T>::operator - ()
{
    xyz[0] = -xyz[0];
    xyz[1] = -xyz[1];
    xyz[2] = -xyz[2];
    return *this;
}

//=============================================================================
// mno¿enie przez sta³¹
//=============================================================================
template <class T> vector3<T> vector3<T>::operator * ( const T f )
{
    return vector3<T>( xyz[0] *= f, xyz[1] *= f, xyz[2] *= f );
}

//=============================================================================
// dodawanie - operator +
//=============================================================================
template <class T> vector3<T> operator + ( const vector3<T> &v1, const vector3<T> &v2 )
{
    return vector3<T>( v1[0]+v2[0], v1[1]+v2[1], v1[2]+v2[2] );
}

//=============================================================================
// odejmowanie - operator -
//=============================================================================
template <class T> vector3<T> operator - ( const vector3<T> &v1, const vector3<T> &v2 )
{
    return vector3<T>( v1[0]-v2[0], v1[1]-v2[1], v1[2]-v2[2] );
}

//=============================================================================
// test równości wektorów
//=============================================================================
template <class T> bool operator == ( const vector3<T> &v1, const vector3<T> &v2 )
{
    return ( v1[0] == v2[0] ) && ( v1[1] == v2[1] ) && ( v1[2] == v2[2] );
}

//=============================================================================
// test nierówności wektorów
//=============================================================================
template <class T> bool operator != ( const vector3<T> &v1, const vector3<T> &v2 )
{
    return ( v1[0] != v2[0] ) || ( v1[1] != v2[1] ) || ( v1[2] != v2[2] );
}

//=============================================================================
// długość wektora
//=============================================================================
template <class T> const T vector3<T>::length() const
{
    return static_cast<T> ( sqrt( xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2] ) );
}

//=============================================================================
// wektor jednostkowy 3D (normalizacja)
//=============================================================================
template <class T> vector3<T> vector3<T>::normalize()
{
    T d = length();
    if( d )
    {
        xyz[0] /= d;
        xyz[1] /= d;
        xyz[2] /= d;
    }
    return *this;
}

//=============================================================================
// wektor jednostkowy 3D (normalizacja)
//=============================================================================
template <class T> vector3<T> normalize( const vector3<T> &v )
{
    vector3<T> vec( v );
    return vec.normalize();
}

//=============================================================================
// iloczyn skalarny wektorów
//=============================================================================
template <class T> const T dot3( const vector3<T> &v1, const vector3<T> &v2 )
{
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

//=============================================================================
// iloczyn wektorowy
//=============================================================================
template <class T> vector3<T> cross3( const vector3<T> &v1, const vector3<T> &v2 )
{
    return vector3<T>( v1[1]*v2[2] - v2[1]*v1[2],
                        v2[0]*v1[2] - v1[0]*v2[2],
                        v1[0]*v2[1] - v2[0]*v1[1] );
}

#endif