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
#include "itkTimeStamp.h"

#include <functional>


namespace itk
{

/**
 * Instance creation.
 */
TimeStamp *
TimeStamp
::New()
{
  return new Self;
}

/**
 * Make this timestamp to be the same as another one.
 */
const TimeStamp &
TimeStamp::operator=( const Self & other )
{
  this->m_ModifiedTime = other.m_ModifiedTime;
  return *this;
}

TimeStamp::GlobalTimeStampType *
TimeStamp
::GetGlobalTimeStamp()
{
  if( m_GlobalTimeStamp == nullptr )
    {
    static auto setLambda = [](void * a)
      {
        delete m_GlobalTimeStamp;
        m_GlobalTimeStamp = static_cast<TimeStamp::GlobalTimeStampType*>(a);
      };
    static auto deleteLambda = [](){ delete m_GlobalTimeStamp; };
//    auto* old_instance = SingletonIndex::GetInstance()->GetGlobalInstance<GlobalTimeStampType>("GlobalTimeStamp");
    m_GlobalTimeStamp = Singleton<TimeStamp::GlobalTimeStampType>( "GlobalTimeStamp" , setLambda, deleteLambda);
//  if( old_instance == nullptr)
//  {
//    *m_GlobalTimeStamp = 0;
//  }
  }
  return m_GlobalTimeStamp;
}

/**
 * Make sure the new time stamp is greater than all others so far.
 */
void
TimeStamp
::Modified()
{
  // This is called once, on-demand to ensure that m_GlobalTimeStamp is
  // initialized.
  static GlobalTimeStampType * globalTimeStamp = GetGlobalTimeStamp();
  Unused(globalTimeStamp);
  this->m_ModifiedTime = ++(*m_GlobalTimeStamp);
}

TimeStamp::GlobalTimeStampType * TimeStamp::m_GlobalTimeStamp= TimeStamp::GetGlobalTimeStamp();

} // end namespace itk
