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
#include "itkSingleton.h"

#include "itkTimeStamp.h"

namespace
{
// This ensures that m_GlobalSingleton is has been initialized once the library
// has been loaded. In some cases, this call will perform the initialization.
// In other cases, static initializers like the IO factory initialization code
// will have done the initialization.
static ::itk::Singleton::SingletonPrivate * initializedGlobalSingleton = ::itk::Singleton::GetGlobalSingleton();

/** \class GlobalSingletonInitializer
 *
 * \brief Initialize a GlobalSingleton and delete it on program
 * completion.
 * */
class GlobalSingletonInitializer
{
public:
  typedef GlobalSingletonInitializer            Self;
  typedef ::itk::Singleton::SingletonPrivate    SingletonPrivate;

  GlobalSingletonInitializer() {}

  /** Delete the time stamp if it was created. */
  ~GlobalSingletonInitializer()
    {
    delete m_GlobalSingleton;
    m_GlobalSingleton = ITK_NULLPTR;
    }

  /** Create the GlobalSingleton if needed and return it. */
  static SingletonPrivate * GetGlobalSingleton()
    {
    if( !m_GlobalSingleton )
      {
      m_GlobalSingleton = new SingletonPrivate;

      // To avoid being optimized out. The compiler does not like this
      // statement at a higher scope.
      Unused(initializedGlobalSingleton);
      }
    return m_GlobalSingleton;
    }

private:
  static SingletonPrivate * m_GlobalSingleton;
};

// Takes care of cleaning up the GlobalSingleton
static GlobalSingletonInitializer GlobalSingletonInitializerInstance;
// Initialized by the compiler to zero
GlobalSingletonInitializer::SingletonPrivate * GlobalSingletonInitializer::m_GlobalSingleton;

} // end anonymous namespace

namespace itk
{
Singleton::SingletonPrivate *
Singleton
::GetGlobalSingleton()
{
  if( m_GlobalSingleton == ITK_NULLPTR )
    {
    std::cout<<"singleton is NULL"<<std::endl;
    m_GlobalSingleton = GlobalSingletonInitializer::GetGlobalSingleton();
    // Pointers initialization
    ::itk::TimeStamp::SetGlobalTimeStamp(m_GlobalSingleton->m_GlobalTimeStamp);
    ::itk::ObjectFactoryBase::SynchronizeObjectFactoryBase(m_GlobalSingleton->m_ObjectFactoryBasePrivate);
    }
  return m_GlobalSingleton;
}


void
Singleton
::SetGlobalSingleton( SingletonPrivate * singleton )
{
  m_GlobalSingleton = singleton;
  if(m_GlobalSingleton)
  {
    ::itk::TimeStamp::SetGlobalTimeStamp(m_GlobalSingleton->m_GlobalTimeStamp);
    ::itk::ObjectFactoryBase::SynchronizeObjectFactoryBase(m_GlobalSingleton->m_ObjectFactoryBasePrivate);
    std::cout<<"SetGlobalSingleton Singleton"<< m_GlobalSingleton->m_ObjectFactoryBasePrivate <<std::endl;
    }
}


Singleton::SingletonPrivate * Singleton::m_GlobalSingleton;


} // end namespace itk
