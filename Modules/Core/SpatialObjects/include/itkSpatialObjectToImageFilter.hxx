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
#ifndef itkSpatialObjectToImageFilter_hxx
#define itkSpatialObjectToImageFilter_hxx

#include "itkSpatialObjectToImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkProgressReporter.h"
#include "itkMath.h"

namespace itk
{
/** Constructor */
template< typename TInputSpatialObject, typename TOutputImage >
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::SpatialObjectToImageFilter()
{
}

/** Destructor */
template< typename TInputSpatialObject, typename TOutputImage >
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::~SpatialObjectToImageFilter()
{}

/** Set the Input SpatialObject */
template< typename TInputSpatialObject, typename TOutputImage >
void
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::SetInput(const InputSpatialObjectType *input)
{
}

/** Connect one of the operands  */
template< typename TInputSpatialObject, typename TOutputImage >
void
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::SetInput(unsigned int index, const TInputSpatialObject *object)
{
}

/** Get the input Spatial Object */
template< typename TInputSpatialObject, typename TOutputImage >
const typename SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >::InputSpatialObjectType *
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::GetInput(void)
{
}

/** Get the input Spatial Object */
template< typename TInputSpatialObject, typename TOutputImage >
const typename SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >::InputSpatialObjectType *
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::GetInput(unsigned int idx)
{
}

//----------------------------------------------------------------------------
template< typename TInputSpatialObject, typename TOutputImage >
void
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::SetSpacing(const SpacingType & spacing)
{
}

//----------------------------------------------------------------------------
template< typename TInputSpatialObject, typename TOutputImage >
void
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::SetSpacing(const double *spacing)
{
}

template< typename TInputSpatialObject, typename TOutputImage >
void
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::SetSpacing(const float *spacing)
{
}

template< typename TInputSpatialObject, typename TOutputImage >
const double *
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::GetSpacing() const
{
}

//----------------------------------------------------------------------------
template< typename TInputSpatialObject, typename TOutputImage >
void
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::SetOrigin(const PointType & origin)
{
}

//----------------------------------------------------------------------------
template< typename TInputSpatialObject, typename TOutputImage >
void
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::SetOrigin(const double *origin)
{
}

template< typename TInputSpatialObject, typename TOutputImage >
void
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::SetOrigin(const float *origin)
{
}

template< typename TInputSpatialObject, typename TOutputImage >
const double *
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::GetOrigin() const
{
}

//----------------------------------------------------------------------------

template< typename TInputSpatialObject, typename TOutputImage >
void
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::SetDirection(const DirectionType & dir)
{
}

template< typename TInputSpatialObject, typename TOutputImage >
const typename SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >::DirectionType &
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::GetDirection(void) const
{
}

//----------------------------------------------------------------------------

/** Update */
template< typename TInputSpatialObject, typename TOutputImage >
void
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::GenerateData(void)
{
} // end update function

template< typename TInputSpatialObject, typename TOutputImage >
void
SpatialObjectToImageFilter< TInputSpatialObject, TOutputImage >
::PrintSelf(std::ostream & os, Indent indent) const
{
}
} // end namespace itk

#endif
