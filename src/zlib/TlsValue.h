#pragma once

#ifndef __TlsValue_h__
#define __TlsValue_h__

/////////////////////////////////////////////////////////////////////////////
// TlsValue.h : Declaration of the TlsValue template class.
//

#include "AutoCriticalSection.h"


/////////////////////////////////////////////////////////////////////////////
// TlsValue

template <class T>
class TlsValue
{
// Construction / Destruction
public:
  TlsValue(void)                         // Default Constructor (does nothing)
  {
  }
  TlsValue(const TlsValue& that)         // Copy Constructor
  {
    SetValue(that.GetValue());   
  }
  ~TlsValue()
  {
    TlsFree(m_dwSlot);
  }

// Attributes
public:
  T GetValue() const
  {
    return reinterpret_cast<T>(TlsGetValue(const_cast<TlsValue*> (this)->GetSlot()));
  }

  T SetValue(const T& value)
  {
    TlsSetValue(GetSlot(), reinterpret_cast<PVOID>(value));
    return value;
  }

// Operators
public:
  // Unary Operators
  T operator!()                          // Logical NOT
  {
    return !GetValue();
  }
  T operator~()                          // One's complement
  {
    return ~GetValue();
  }
  T operator+()                          // Unary plus
  {
    return +GetValue();
  }
  T operator++()                         // Pre-Increment
  {
    SetValue(GetValue() + 1);
    return *this;
  }
  T operator--()                         // Pre-Decrement
  {
    SetValue(GetValue() - 1);
    return *this;
  }
  T operator-()                          // Unary negation
  {
    return -GetValue();
  }
  T operator++(int)                      // Post-Increment
  {
    T temp(GetValue());
    SetValue(temp + 1);
    return temp;
  }
  T operator--(int)                      // Post-Decrement
  {
    T temp(GetValue());
    SetValue(temp - 1);
    return temp;
  }

  T operator%=(const T& value)           // Modulus/assignment 
  {
    SetValue(GetValue() % value);
    return *this;
  }
  T operator&=(const T& value)           // Bitwise AND/assignment
  {
    SetValue(GetValue() & value);
    return *this;
  }
  T operator*=(const T& value)           // Multiplication/assignment 
  {
    SetValue(GetValue() * value);
    return *this;
  }
  T operator+=(const T& value)           // Addition/assignment 
  {
    SetValue(GetValue() + value);
    return *this;
  }
  T operator-=(const T& value)           // Subtraction/assignment
  {
    SetValue(GetValue() - value);
    return *this;
  }
  T operator/=(const T& value)           // Division/assignment
  {
    SetValue(GetValue() / value);
    return *this;
  }
  T operator<<=(const T& value)          // Left shift/assignment 
  {
    SetValue(GetValue() << value);
    return *this;
  }
  T operator=(const TlsValue& that)      // Assignment
  {
    return SetValue(that.GetValue());
  }
  T operator=(const T& value)            // Assignment
  {
    return SetValue(value);
  }
  T operator>>=(const T& value)          // Right shift/assignment 
  {
    SetValue(GetValue() >> value);
    return *this;
  }
  T operator^=(const T& value)           // Exclusive OR/assignment
  {
    SetValue(GetValue() ^ value);
    return *this;
  }
  T operator|=(const T& value)           // Inclusive OR/assignment
  {
    SetValue(GetValue() | value);
    return *this;
  }

  // Casting Operator
  operator T() const
  {
    return GetValue();
  }


// Implementation
protected:
  static ZAutoCriticalSection& GetSyncObject()
  {
    // Shared by all instances, which is not a big deal since this is only
    // used when first allocating the TLS slot.
    static ZAutoCriticalSection s_cs;
    return s_cs;
  }
  DWORD GetSlot()
  {
    if (m_dwSlot)                // Check for initialized slot
      return m_dwSlot;

    GetSyncObject().Lock();      // Lock the sync object
    if (!m_dwSlot)               // Check (again) for uninitialized slot
    {
      m_dwSlot = TlsAlloc();     // Allocate a TLS slot
      if (!m_dwSlot)             // Zero is valid, but not for us since zero
      {                          //   is used to indicate not-initialized
        m_dwSlot = TlsAlloc();   // Next index will not be zero
        TlsFree(0);              // Free the zero index
      }
    }
    GetSyncObject().Unlock();    // Unlock the sync object
    return m_dwSlot;             // Return the (non-zero) slot
  }


// Data Members
protected:
  DWORD m_dwSlot;
};


/////////////////////////////////////////////////////////////////////////////

typedef TlsValue<DWORD > tlsDWORD;
typedef TlsValue<WORD  > tlsWORD;
typedef TlsValue<int   > tlsINT;
typedef TlsValue<UINT  > tlsUINT;
typedef TlsValue<SHORT > tlsSHORT;
typedef TlsValue<USHORT> tlsUSHORT;
typedef TlsValue<LONG  > tlsLONG;
typedef TlsValue<ULONG > tlsULONG;
typedef TlsValue<TCHAR > tlsTCHAR;
typedef TlsValue<WCHAR > tlsWCHAR;
typedef TlsValue<CHAR  > tlsCHAR;
typedef TlsValue<BOOL  > tlsBOOL;
typedef TlsValue<bool  > tlsBool;


/////////////////////////////////////////////////////////////////////////////

#endif // !__TlsValue_h__
