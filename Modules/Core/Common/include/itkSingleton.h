/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
/*=========================================================================
 *
 *  Portions of this file are subject to the VTK Toolkit Version 3 copyright.
 *
 *  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 *
 *  For complete copyright, license and disclaimer of warranty information
 *  please refer to the NOTICE file at the top of the ITK source tree.
 *
 *=========================================================================*/
#ifndef itkSingleton_h
#define itkSingleton_h

#include "itkMacro.h"
#include "itkTimeStamp.h"
#include "itkObjectFactoryBase.h"


/** \brief A function which does nothing
 *
 * This function is to be used to mark parameters as unused to suppress
 * compiler warning. It can be used when the parameter needs to be named
 * (i.e. itkNotUsed cannot be used) but is not always used. It ensures
 * that the parameter is not optimized out.
 */
template <typename T>
inline void Unused( const T &) {};

///** \brief A function which does nothing
// *
// * This function is to be used to mark parameters as unused to suppress
// * compiler warning. It can be used when the parameter needs to be named
// * (i.e. itkNotUsed cannot be used) but is not always used. It ensures
// * that the parameter is not optimized out.
// */
//template <typename T>
//void Unused( const T &) {};

namespace itk
{
/** \class Singleton
 * \brief Implementation detail.
 *
 * \ingroup ITKCommon
 */

// // Forward reference because of private implementation
//struct SingletonPrivate;

class ITKCommon_EXPORT Singleton
{
public:
  /** Standard class typedefs. */
  typedef Singleton Self;

  struct SingletonPrivate
  {
      SingletonPrivate()
      {
        m_GlobalTimeStamp = new TimeStamp::GlobalTimeStampType( 0 );
        m_ObjectFactoryBasePrivate = new ::itk::ObjectFactoryBasePrivate();
      }
      ~SingletonPrivate()
      {
        delete m_ObjectFactoryBasePrivate;
        m_ObjectFactoryBasePrivate = ITK_NULLPTR;
        delete m_GlobalTimeStamp;
        m_GlobalTimeStamp = ITK_NULLPTR;
      }
      TimeStamp::GlobalTimeStampType * m_GlobalTimeStamp;
      ObjectFactoryBasePrivate * m_ObjectFactoryBasePrivate;
  };

  /** Set/Get the pointer to GlobalSingleton.
   * Note that SetGlobalSingleton is not concurrent thread safe. */
  static SingletonPrivate * GetGlobalSingleton();
  static void SetGlobalSingleton( SingletonPrivate * singleton );

private:
  /** The static GlobalSingleton. This is initialized to NULL as the first
   * stage of static initialization. It is then populated on the first call to
   * itk::Singleton::Modified() but it can be overridden with SetGlobalSingleton().
   * */
  static SingletonPrivate * m_GlobalSingleton;
};
} // end namespace itk

#endif
