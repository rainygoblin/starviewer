#ifndef UDGCREATEDICOMPRINTSPOOL_H
#define UDGCREATEDICOMPRINTSPOOL_H

#include "dicomprinter.h"
#include "dicomprintpage.h"

class DVPSStoredPrint;
class DVPresentationState;
class DVPSAnnotationContent_PList;

namespace udg
{
class Image;

class CreateDicomPrintSpool
{
public:

    enum CreateDicomPrintSpoolError { ErrorLoadingImageToPrint, ErrorCreatingImageSpool, Ok };

    QString createPrintSpool(DicomPrinter dicomPrinter, DicomPrintPage dicomPrintPage, const QString &spoolDirectoryPath);

    CreateDicomPrintSpool::CreateDicomPrintSpoolError getLastError();

private:

    bool transformImageForPrinting(Image *image, const QString &spoolDirectoryPath);

    void setBasicFilmBoxAttributes();

    bool createHardcopyGrayscaleImage(Image *imageToPrint, const void *pixelData, unsigned long bitmapWidth, unsigned long bitmapHeight, double pixelAspectRatio, const QString &spoolDirectoryPath);

    void setImageBoxAttributes();

    ///CRea Annotation Box en funció de les anotacions que ens hagin passat
    void createAnnotationBoxes();

    /**Crea el fitxer de Dcmtk on es guarden els paràmetres amb els quals s'ha de fer la impressió i la estructura de FilmBox, que conté un FilmSession
       que conté n imatges, i els seus paràmetres d'impressió.
       Retorna el pathfile del fitxer creat*/
    QString createStoredPrintDcmtkFile(const QString &spoolDirectoryPath);

    DicomPrintPage m_dicomPrintPage;
    DicomPrinter m_dicomPrinter;
    DVPSStoredPrint* m_storedPrint;
    DVPresentationState * m_presentationState;
    DVPSAnnotationContent_PList *m_annotationBoxes;
    CreateDicomPrintSpoolError m_lastError;
    QString m_annotationDisplayFormatIDTagValue;
};
};

#endif