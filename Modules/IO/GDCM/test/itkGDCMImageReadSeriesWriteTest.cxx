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

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkTestingMacros.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "gdcmDict.h"
#include "gdcmGlobal.h"
#include "gdcmDicts.h"
#include "gdcmStringFilter.h"

int itkGDCMImageReadSeriesWriteTest( int argc, char* argv[] )
{
  if( argc < 4 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " InputImage OutputDicomDirectory SingleOutputImage" << std::endl;
    return EXIT_FAILURE;
    }
  const char * inputImage = argv[1];
  const char * outputDirectory = argv[2];
  const char * singleOutputImage = argv[3];

  typedef signed short    PixelType;
  const unsigned int      Dimension = 3;

  typedef itk::Image< PixelType, Dimension > ImageType;
  typedef itk::ImageFileReader< ImageType  > ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( inputImage );

  TRY_EXPECT_NO_EXCEPTION( reader->Update() );

  typedef itk::GDCMImageIO            ImageIOType;
  typedef itk::NumericSeriesFileNames NamesGeneratorType;

  ImageIOType::Pointer gdcmIO = ImageIOType::New();

  itksys::SystemTools::MakeDirectory( outputDirectory );

  typedef signed short    OutputPixelType;
  const unsigned int      OutputDimension = 2;

  typedef itk::Image< OutputPixelType, OutputDimension >    Image2DType;
  typedef itk::ImageSeriesWriter< ImageType, Image2DType >  SeriesWriterType;

  NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();

  itk::MetaDataDictionary & dict = gdcmIO->GetMetaDataDictionary();
  std::string tagkey, value;
  tagkey = "0x0010, 0x0010"; //Patient name
  value = "Wes Turner";
  itk::EncapsulateMetaData<std::string>(dict, tagkey, value );
  tagkey = "0008|0060"; // Modality
  value = "RTIMAGE";
  itk::EncapsulateMetaData<std::string>(dict, tagkey, value );
  tagkey = "0008|0008"; // Image Type
  value = "DERIVED\\SECONDARY";
  itk::EncapsulateMetaData<std::string>(dict, tagkey, value);
  tagkey = "0008|0064"; // Conversion Type
  value = "DV";
  itk::EncapsulateMetaData<std::string>(dict, tagkey, value);

  reader->SetMetaDataDictionary(dict);
  SeriesWriterType::Pointer seriesWriter = SeriesWriterType::New();
  seriesWriter->SetInput( reader->GetOutput() );
  seriesWriter->SetImageIO( gdcmIO );

  ImageType::RegionType region = reader->GetOutput()->GetLargestPossibleRegion();
  ImageType::IndexType start = region.GetIndex();
  ImageType::SizeType  size  = region.GetSize();


  std::string format = outputDirectory;
  format += "/image%03d.dcm";

  namesGenerator->SetSeriesFormat( format.c_str() );
  namesGenerator->SetStartIndex( start[2] );
  namesGenerator->SetEndIndex( start[2] + size[2] - 1 );
  namesGenerator->SetIncrementIndex( 1 );

  seriesWriter->SetFileNames( namesGenerator->GetFileNames() );

  TRY_EXPECT_NO_EXCEPTION( seriesWriter->Update() );


  // Now read back in and write out as 3D image for comparison with the input.
  gdcm::Global &      g = gdcm::Global::GetInstance();
  const gdcm::Dicts & dicts = g.GetDicts();
  const gdcm::Dict &  pubdict = dicts.GetPublicDict();
  typedef itk::ImageSeriesReader< ImageType > SeriesReaderType;
  SeriesReaderType::Pointer seriesReader = SeriesReaderType::New();
  seriesReader->SetFileNames( namesGenerator->GetFileNames() );
  seriesReader->Update();
  dict = gdcmIO->GetMetaDataDictionary();
  itk::MetaDataDictionary::ConstIterator itr = dict.Begin();
  const itk::MetaDataDictionary::ConstIterator end = dict.End();
  while ( itr != end )
    {
    const std::string & key = itr->first;
    itk::ExposeMetaData< std::string >( dict, key, value );
    // Convert DICOM name to DICOM (group,element)
    gdcm::Tag            tag;
    bool b = tag.ReadFromPipeSeparatedString( key.c_str() );
              std::cerr << "key:"<<key << std::endl;
    // Anything that has been changed in the MetaData Dict will be pushed
    // into the DICOM header:
    if ( b /*tag != gdcm::Tag(0xffff,0xffff)*/ /*dictEntry*/ )
      {
      const gdcm::DictEntry & dictEntry = pubdict.GetDictEntry(tag);
      gdcm::VR::VRType        vrtype = dictEntry.GetVR();
      if ( dictEntry.GetVR() == gdcm::VR::SQ )
        {
        // How did we reach here ?
        }
      else if ( vrtype & ( gdcm::VR::OB | gdcm::VR::OF | gdcm::VR::OW | gdcm::VR::UN ) )
        {
        }
      else // VRASCII
        {
        if ( gdcm::VR::UI )
          {
          // No gdcm::VR::UI
          }
        else
          {
          gdcm::StringFilter sf;
          std::string si = sf.FromString( tag, value.c_str(), value.size() );
          for(std::string::iterator itsi = si.begin(); itsi != si.end(); ++itsi)
            {
            if(*itsi == '\0')
              {
              std::cerr << "DICOM tag should not be padded with '\0' but with spaces" << std::endl;
              std::cerr << vrtype << std::endl;
              return EXIT_FAILURE;
              }
            }
          }
        }
      }
      ++itr;
    }

  typedef itk::ImageFileWriter< ImageType > SingleWriterType;
  SingleWriterType::Pointer singleWriter = SingleWriterType::New();
  singleWriter->SetInput( seriesReader->GetOutput() );
  singleWriter->SetFileName( singleOutputImage );

  TRY_EXPECT_NO_EXCEPTION( singleWriter->Update() );

  return EXIT_SUCCESS;
}
