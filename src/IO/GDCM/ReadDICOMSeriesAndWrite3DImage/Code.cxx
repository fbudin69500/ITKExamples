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

#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"

int main(int argc, char* argv[])
{
  if (argc < 2)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " [DicomDirectory  [outputFileName  [seriesName]]]";
    std::cerr << "\nIf DicomDirectory is not specified, current directory is used\n";
    }
  std::string dirName = "."; //current directory by default
  if (argc > 1)
    {
    dirName = argv[1];
    }

  typedef signed short    PixelType;
  const unsigned int      Dimension = 3;
  typedef itk::Image< PixelType, Dimension >         ImageType;

  typedef itk::GDCMSeriesFileNames NamesGeneratorType;
  NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

  nameGenerator->SetUseSeriesDetails(true);
  nameGenerator->AddSeriesRestriction("0008|0021");
  nameGenerator->SetGlobalWarningDisplay(false);
  nameGenerator->SetDirectory(dirName);

  try
    {
    typedef std::vector< std::string >    SeriesIdContainer;
    const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
    SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
    SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();

    if (seriesItr != seriesEnd)
      {
      std::cout << "The directory: ";
      std::cout << dirName << std::endl;
      std::cout << "Contains the following DICOM Series: ";
      std::cout << std::endl;
      }
    else
      {
      std::cout << "No DICOMs in: " << dirName << std::endl;
      return EXIT_SUCCESS;
      }

    while (seriesItr != seriesEnd)
      {
      std::cout << seriesItr->c_str() << std::endl;
      ++seriesItr;
      }

    seriesItr = seriesUID.begin();
    while (seriesItr != seriesUID.end())
      {
      std::string seriesIdentifier;
      if (argc > 3) // If seriesIdentifier given convert only that
        {
        seriesIdentifier = argv[3];
        seriesItr = seriesUID.end();
        }
      else //otherwise convert everything
        {
        seriesIdentifier = seriesItr->c_str();
        seriesItr++;
        }
      std::cout << "\nReading: ";
      std::cout << seriesIdentifier << std::endl;
      typedef std::vector< std::string >   FileNamesContainer;
      FileNamesContainer fileNames =
        nameGenerator->GetFileNames(seriesIdentifier);

      typedef itk::ImageSeriesReader< ImageType > ReaderType;
      ReaderType::Pointer reader = ReaderType::New();
      typedef itk::GDCMImageIO       ImageIOType;
      ImageIOType::Pointer dicomIO = ImageIOType::New();
      reader->SetImageIO(dicomIO);
      reader->SetFileNames(fileNames);

      typedef itk::ImageFileWriter< ImageType > WriterType;
      WriterType::Pointer writer = WriterType::New();
      std::string outFileName;
      if (argc > 2)
        {
        outFileName = argv[2];
        }
      else
        {
        outFileName = dirName + std::string("/") + seriesIdentifier + ".nrrd";
        }
      writer->SetFileName(outFileName);
      writer->UseCompressionOn();
      writer->SetInput(reader->GetOutput());
      std::cout << "Writing: " << outFileName << std::endl;
      try
        {
        writer->Update();
        }
      catch (itk::ExceptionObject &ex)
        {
        std::cout << ex << std::endl;
        continue;
        }
      }
    }
  catch (itk::ExceptionObject &ex)
    {
    std::cout << ex << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
