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


//
// Check if the compiler support the C++11 alignas specifier
//

struct A
{
  char a;
};


struct alignas(64) B
{
  char a;
};

// This structure will generate a compiler error if the template
// argument is false
template<bool t> struct OnlyTrue;
template<> struct OnlyTrue<true> { static ITK_CONSTEXPR bool Result = true; };


int main(void)
{
  typedef A AlignedA alignas(64);
  return OnlyTrue<alignof( AlignedA ) == 64>::Result
    && OnlyTrue<alignof( B ) == 64>::Result;
}
